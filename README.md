# block_sort
Rejection Sampling in NEON

## How to build 

- Run `make` to build all 
- Run `make bench` to start benchmarking cycles count. Please read `papi_hl_output` to see the result. The `PAPI_TOT_CYC` field describe the total clock cycles over a number of `TESTS` iteration. 
- Run `make verify` to verify the vectorize implementation compare with reference code. 

## Test platform 

This code is developed and tested on Raspberry Pi 4 8Gb Cortex-A72. 

## Result

|                    |   C  | NEON-Full | Ratio| NEON-Half | Ratio| NEON-Mix | Ratio |
|:------------------:|:----:|:---------:|------|:---------:|:----:|:--------:|:-----:|
| Rejection Sampling | 1686 |    773    | 2.18 |    1250   | 1.34 | 765      |  2.20 |



