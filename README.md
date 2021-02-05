# block_sort
Rejection Sampling in NEON

## How to build 

- Run `make` to build all 
- Run `make bench` to start benchmarking cycles count. Please read `papi_hl_output` to see the result. The `PAPI_TOT_CYC` field describe the total clock cycles over a number of `TESTS` iteration. 
- Run `make verify` to verify the vectorize implementation compare with reference code. 

## Test platform 

This code is developed and tested on Raspberry Pi 4 8Gb Cortex-A72. 

## Result

|                    |   C  | NEON-Full | Ratio| NEON-Half | Ratio|
|:------------------:|:----:|:---------:|------|:---------:|:----:|
| Rejection Sampling | 1887 |    771    | 2.44 |    1251   | 1.50 |



