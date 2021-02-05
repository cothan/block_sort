#ifndef CONST_H
#define CONST_H

extern const uint8_t table_idx[256][16];
extern const uint8_t table_idx_small[16][8];

int compare(int16_t *a, int16_t *b, int len);

void print_array(const uint8_t *buf, int buflen, const char *string);

unsigned int rej_uniform(int16_t *r,
                         unsigned int len,
                         const uint8_t *buf,
                         unsigned int buflen);

#endif