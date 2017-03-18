CFLAGS = -msse2 --std gnu99 -O0 -Wall -Wextra -mavx2

GIT_HOOKS := .git/hooks/applied

all: $(GIT_HOOKS) main.c
	$(CC) $(CFLAGS) -o main main.c

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

cache-miss-naive: main
	$(CC) $(CFLAGS) -DNAIVE -DNAME=\"cache-miss-naive\" -o cache-miss-naive main.c
	perf stat --repeat 100 \
		-e cache-misses,cache-references,L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses \
		./cache-miss-naive

cache-miss-sse: main
	$(CC) $(CFLAGS) -DSSE -DNAME=\"cache-miss-sse\" -o cache-miss-sse main.c
	perf stat --repeat 100 \
		-e cache-misses,cache-references,L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses \
		./cache-miss-sse

cache-miss-ssep: main
	$(CC) $(CFLAGS) -DSSE_P -DNAME=\"cache-miss-ssep\" -o cache-miss-ssep main.c
	perf stat --repeat 100 \
		-e cache-misses,cache-references,L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses \
		./cache-miss-ssep

cache-miss-avx: main
	$(CC) $(CFLAGS) -DAVX -DNAME=\"cache-miss-avx\" -o cache-miss-avx main.c
	perf stat --repeat 100 \
		-e cache-misses,cache-references,L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses \
		./cache-miss-avx
test-avx: main
	$(CC) $(CFLAGS) -DAVX -DTEST -DVERITY -DNAME=\"test-avx\" -o test-avx main.c
		./test-avx
clean:
	$(RM) main
