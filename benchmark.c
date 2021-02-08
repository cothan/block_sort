#include <papi.h>
#include <stdio.h>
#include <arm_neon.h>
#include <sys/random.h>
#include <string.h>
// Include constant file to keep this file short
#include "benchmark_sorting_const.h"

// #define TESTS 1000000
#define BUF_LEN 528

int main()
{
    int16_t r_test[256], r_test_half[256], r_gold[256], r_test_mix[256];
    uint8_t input[BUF_LEN];
    int cnt_gold = 0, cnt_test = 0, cnt_test_half = 0, cnt_test_mix = 0;
    int i;

    // Quick sampling input, in our implementation genradom is SHAKE128
    getrandom(input, sizeof(input), 0);

    PAPI_hl_region_begin("rej_uniform");
    for (i = 0; i < TESTS; i++)
    {
        cnt_gold = rej_uniform(r_gold, 256, input, sizeof(input));
    }
    PAPI_hl_region_end("rej_uniform");

    PAPI_hl_region_begin("neon_rej_uniform");
    for (i = 0; i < TESTS; i++)
    {
        cnt_test = neon_rej_uniform(r_test, input);
    }
    PAPI_hl_region_end("neon_rej_uniform");

    PAPI_hl_region_begin("neon_rej_uniform_half");
    for (i = 0; i < TESTS; i++)
    {
        cnt_test_half = neon_rej_uniform_half(r_test_half, input);
    }
    PAPI_hl_region_end("neon_rej_uniform_half");

    PAPI_hl_region_begin("neon_rej_uniform_mix");
    for (i = 0; i < TESTS; i++)
    {
        cnt_test_mix = neon_rej_uniform_mix(r_test_mix, input);
    }
    PAPI_hl_region_end("neon_rej_uniform_mix");

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

    if (cnt_gold != cnt_test_mix)
    {
        printf("Error: cnt_gold != cnt_test_mix: %d != %d\n", cnt_gold, cnt_test_mix);
        return 1;
    }

    if (compare(r_gold, r_test_mix, cnt_gold))
    {
        printf("Error: r_gold != r_test_mix\n");
        return 1;
    }

    return 0;
}
