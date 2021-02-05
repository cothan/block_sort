#include <papi.h>
#include <stdio.h>
#include <arm_neon.h>
#include <sys/random.h>
#include <string.h>
// Include constant file to keep this file short
#include "benchmark_sorting_const.h"

// #define TESTS 1000000
#define BUF_LEN 528

unsigned int neon_rej_uniform(int16_t *r, const uint8_t *buf)
{
#ifdef DEBUG
    print_array(buf, BUF_LEN, "neon_buf");
#endif

    uint8x16x3_t neon_buf;
    uint16x8x4_t tmp, value, sign;

    uint16_t bit_table[8] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
    uint8x16x4_t neon_table;
    uint16x8_t const_kyberq, neon_bit, const_0xfff;

    const_0xfff = vdupq_n_u16(0xfff);
    const_kyberq = vdupq_n_u16(3329 - 1);
    neon_bit = vld1q_u16(bit_table);

    unsigned int reduce_indexes[4], ctr[4];
    unsigned int i, count = 0;

    int16_t local_buf[BUF_LEN * 8 / 12];

    for (i = 0; i < BUF_LEN && count < 256; i += 16 * 3)
    {
        // 0, 3, 6, 9  - 12, 15, 18, 21
        // 1, 4, 7, 10 - 13, 16, 19, 22
        // 2, 5, 8, 11 - 14, 17, 20, 23
        neon_buf = vld3q_u8(&buf[i]);

        // Val0: 0-1 | 3-4 | 6-7| 9-10 | 12-13 | 15-16 | 18-19 | 21-22
        tmp.val[0] = (uint16x8_t)vzip1q_u8(neon_buf.val[0], neon_buf.val[1]);
        tmp.val[1] = (uint16x8_t)vzip2q_u8(neon_buf.val[0], neon_buf.val[1]);

        // Take 12 bit low
        tmp.val[0] = vandq_u16(tmp.val[0], const_0xfff);
        tmp.val[1] = vandq_u16(tmp.val[1], const_0xfff);

        // Val1: 1-2 | 4-5 | 7-8 | 10-11 | 13-14 | 16-17 | 19-20 | 22-23
        tmp.val[2] = (uint16x8_t)vzip1q_u8(neon_buf.val[1], neon_buf.val[2]);
        tmp.val[3] = (uint16x8_t)vzip2q_u8(neon_buf.val[1], neon_buf.val[2]);

        // Take 12 bit high
        tmp.val[2] = vshrq_n_u16(tmp.val[2], 4);
        tmp.val[3] = vshrq_n_u16(tmp.val[3], 4);

        // Final value
        // Index as below:
        // 0, 1, 2, 3 -- 4, 5, 6, 7
        // 8, 9, 10, 11 -- 12, 13, 14, 15
        // 16, 17, 18, 19 -- 20, 21, 22, 23
        // 24, 25, 26, 27 -- 28, 29, 30, 31
        value.val[0] = vzip1q_u16(tmp.val[0], tmp.val[2]);
        value.val[1] = vzip2q_u16(tmp.val[0], tmp.val[2]);
        value.val[2] = vzip1q_u16(tmp.val[1], tmp.val[3]);
        value.val[3] = vzip2q_u16(tmp.val[1], tmp.val[3]);

        // Compare unsigned less than equal than constant
        sign.val[0] = vcleq_u16(value.val[0], const_kyberq);
        sign.val[1] = vcleq_u16(value.val[1], const_kyberq);
        sign.val[2] = vcleq_u16(value.val[2], const_kyberq);
        sign.val[3] = vcleq_u16(value.val[3], const_kyberq);

        // Encoding: prepare indexes for table idx
        sign.val[0] = vandq_u16(sign.val[0], neon_bit);
        sign.val[1] = vandq_u16(sign.val[1], neon_bit);
        sign.val[2] = vandq_u16(sign.val[2], neon_bit);
        sign.val[3] = vandq_u16(sign.val[3], neon_bit);

        // Add across vector
        reduce_indexes[0] = vaddvq_u16(sign.val[0]);
        reduce_indexes[1] = vaddvq_u16(sign.val[1]);
        reduce_indexes[2] = vaddvq_u16(sign.val[2]);
        reduce_indexes[3] = vaddvq_u16(sign.val[3]);

        ctr[0] = __builtin_popcount(reduce_indexes[0]);
        ctr[1] = __builtin_popcount(reduce_indexes[1]);
        ctr[2] = __builtin_popcount(reduce_indexes[2]);
        ctr[3] = __builtin_popcount(reduce_indexes[3]);

        neon_table.val[0] = vld1q_u8(table_idx[reduce_indexes[0]]);
        neon_table.val[1] = vld1q_u8(table_idx[reduce_indexes[1]]);
        neon_table.val[2] = vld1q_u8(table_idx[reduce_indexes[2]]);
        neon_table.val[3] = vld1q_u8(table_idx[reduce_indexes[3]]);

        // Table-based permute for each array
        value.val[0] = (uint16x8_t)vqtbl1q_u8((uint8x16_t)value.val[0], neon_table.val[0]);
        value.val[1] = (uint16x8_t)vqtbl1q_u8((uint8x16_t)value.val[1], neon_table.val[1]);
        value.val[2] = (uint16x8_t)vqtbl1q_u8((uint8x16_t)value.val[2], neon_table.val[2]);
        value.val[3] = (uint16x8_t)vqtbl1q_u8((uint8x16_t)value.val[3], neon_table.val[3]);

        vst1q_s16(&local_buf[count], (int16x8_t)value.val[0]);
        count += ctr[0];
        vst1q_s16(&local_buf[count], (int16x8_t)value.val[1]);
        count += ctr[1];
        vst1q_s16(&local_buf[count], (int16x8_t)value.val[2]);
        count += ctr[2];
        vst1q_s16(&local_buf[count], (int16x8_t)value.val[3]);
        count += ctr[3];
    }

    count = count > 256 ? 256 : count;

    // Copy from localbuf to output, make sure no overflow happen
    for (i = 0; i < count; i++)
    {
        r[i] = local_buf[i];
    }

    return count;
}

/*
This function is similar to neon_rej_uniform, 
but only operate on half of SIMD register

The purpose is to use smaller table.
*/

unsigned int neon_rej_uniform_half(int16_t *r, const uint8_t *buf)
{
#ifdef DEBUG
    print_array(buf, BUF_LEN, "neon_buf_half");
#endif
    uint8x8x3_t neon_buf8;
    uint16x4x4_t tmp8, value8, sign8;
    uint8x8x4_t neon_table8;
    uint16x4_t const8_kyberq, neon_bit8, const8_0xfff;

    uint16_t bit_table[8] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};
    unsigned int reduce_indexes[4], ctr[4];

    const8_0xfff = vdup_n_u16(0xfff);
    const8_kyberq = vdup_n_u16(3329 - 1);
    neon_bit8 = vld1_u16(bit_table);

    int16_t local_buf[BUF_LEN * 8 / 12];
    int i, count = 0;

    for (i = 0; i < BUF_LEN && count < 256; i += 8 * 3)
    {
        neon_buf8 = vld3_u8(&buf[i]);

        // Val0: 0-1 | 3-4 | 6-7| 9-10
        tmp8.val[0] = (uint16x4_t)vzip1_u8(neon_buf8.val[0], neon_buf8.val[1]);
        tmp8.val[1] = (uint16x4_t)vzip2_u8(neon_buf8.val[0], neon_buf8.val[1]);

        tmp8.val[0] = vand_u16(tmp8.val[0], const8_0xfff);
        tmp8.val[1] = vand_u16(tmp8.val[1], const8_0xfff);

        // Val1: 1-2 | 4-5 | 7-8 | 10-11
        tmp8.val[2] = (uint16x4_t)vzip1_u8(neon_buf8.val[1], neon_buf8.val[2]);
        tmp8.val[3] = (uint16x4_t)vzip2_u8(neon_buf8.val[1], neon_buf8.val[2]);

        tmp8.val[2] = vshr_n_u16(tmp8.val[2], 4);
        tmp8.val[3] = vshr_n_u16(tmp8.val[3], 4);

        // Final value
        value8.val[0] = vzip1_u16(tmp8.val[0], tmp8.val[2]);
        value8.val[1] = vzip2_u16(tmp8.val[0], tmp8.val[2]);
        value8.val[2] = vzip1_u16(tmp8.val[1], tmp8.val[3]);
        value8.val[3] = vzip2_u16(tmp8.val[1], tmp8.val[3]);

        // Compare unsigned less than equal
        sign8.val[0] = vcle_u16(value8.val[0], const8_kyberq);
        sign8.val[1] = vcle_u16(value8.val[1], const8_kyberq);
        sign8.val[2] = vcle_u16(value8.val[2], const8_kyberq);
        sign8.val[3] = vcle_u16(value8.val[3], const8_kyberq);

        // Prepare indexes for table idx
        sign8.val[0] = vand_u16(sign8.val[0], neon_bit8);
        sign8.val[1] = vand_u16(sign8.val[1], neon_bit8);
        sign8.val[2] = vand_u16(sign8.val[2], neon_bit8);
        sign8.val[3] = vand_u16(sign8.val[3], neon_bit8);

        // Add across vector
        reduce_indexes[0] = vaddv_u16(sign8.val[0]);
        reduce_indexes[1] = vaddv_u16(sign8.val[1]);
        reduce_indexes[2] = vaddv_u16(sign8.val[2]);
        reduce_indexes[3] = vaddv_u16(sign8.val[3]);

        // Popcount
        ctr[0] = __builtin_popcount(reduce_indexes[0]);
        ctr[1] = __builtin_popcount(reduce_indexes[1]);
        ctr[2] = __builtin_popcount(reduce_indexes[2]);
        ctr[3] = __builtin_popcount(reduce_indexes[3]);

        // Load entry for permutation
        neon_table8.val[0] = vld1_u8(table_idx_small[reduce_indexes[0]]);
        neon_table8.val[1] = vld1_u8(table_idx_small[reduce_indexes[1]]);
        neon_table8.val[2] = vld1_u8(table_idx_small[reduce_indexes[2]]);
        neon_table8.val[3] = vld1_u8(table_idx_small[reduce_indexes[3]]);

        // Table-based permute
        value8.val[0] = (uint16x4_t)vtbl1_u8((uint8x8_t)value8.val[0], neon_table8.val[0]);
        value8.val[1] = (uint16x4_t)vtbl1_u8((uint8x8_t)value8.val[1], neon_table8.val[1]);
        value8.val[2] = (uint16x4_t)vtbl1_u8((uint8x8_t)value8.val[2], neon_table8.val[2]);
        value8.val[3] = (uint16x4_t)vtbl1_u8((uint8x8_t)value8.val[3], neon_table8.val[3]);

        vst1_s16(&local_buf[count], (int16x4_t)value8.val[0]);
        count += ctr[0];
        vst1_s16(&local_buf[count], (int16x4_t)value8.val[1]);
        count += ctr[1];
        vst1_s16(&local_buf[count], (int16x4_t)value8.val[2]);
        count += ctr[2];
        vst1_s16(&local_buf[count], (int16x4_t)value8.val[3]);
        count += ctr[3];
    }

    count = count > 256 ? 256 : count;

    for (i = 0; i < count; i++)
    {
        r[i] = local_buf[i];
    }
    return count;
}

int main(void)
{
    int16_t r_test[256], r_test_half[256], r_gold[256];
    uint8_t input[BUF_LEN];
    ;
    int cnt_gold = 0, cnt_test = 0, cnt_test_half = 0;
    int i;

    for (i = 0; i < TESTS; i++)
    {
        // Quick sampling input, in our implementation genradom is SHAKE128
        getrandom(input, sizeof(input), 0);

#ifdef DEBUG
        print_array(input, sizeof(input), "input");
#endif

        cnt_gold = rej_uniform(r_gold, 256, input, sizeof(input));
        
        cnt_test = neon_rej_uniform(r_test, input);
        cnt_test_half = neon_rej_uniform_half(r_test_half, input);

        if (cnt_gold != cnt_test)
        {
            printf("Error: cnt_gold != cnt_test: %d != %d\n", cnt_gold, cnt_test);
            return 1;
        }

        if (compare(r_gold, r_test, cnt_gold))
        {
            printf("Error: r_gold != r_test\n");
            return 1;
        }

        if (cnt_gold != cnt_test_half)
        {
            printf("Error: cnt_gold != cnt_test_half: %d != %d\n", cnt_gold, cnt_test_half);
            return 1;
        }

        if (compare(r_gold, r_test_half, cnt_gold))
        {
            printf("Error: r_gold != r_test_half \n");
            return 1;
        }
    }

    return 0;
}
