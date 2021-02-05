CC = /usr/bin/cc
# CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -Wredundant-decls -Wshadow -Wpointer-arith -Wno-unused-result -fomit-frame-pointer -O3 -mtune=native 
LIBPAPI = libpapi.a

SOURCES = benchmark_sorting_const.c

.PHONY: all shared speed clean

all: \
  test_speed \
  test_result

test_speed: $(SOURCES) benchmark.c
	$(CC) $(CFLAGS) -DTESTS=1000000 $(SOURCES) benchmark.c $(LIBPAPI) -o test_speed

test_result: $(SOURCES) benchmark_test.c
	$(CC) $(CFLAGS) -DTESTS=1000000 $(SOURCES) benchmark_test.c -o test_result


clean:
	-$(RM) -rf test_speed
	-$(RM) -rf test_result

bench:
	./test_speed

verify:
	./test_result

