#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>

#include <xmmintrin.h>

#include <immintrin.h>

#ifndef TEST
#define TEST_W 4096
#define TEST_H 4096
#endif

#ifdef TEST
#define TEST_W 4
#define TEST_H 4
#endif

#include "impl.c"

static long diff_in_us(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec * 1000000.0 + diff.tv_nsec / 1000.0);
}

int main()
{

    struct timespec start, end;
    int *src  = (int *) malloc(sizeof(int) * TEST_W * TEST_H);

    srand(time(NULL));
    for (int y = 0; y < TEST_H; y++)
        for (int x = 0; x < TEST_W; x++)
            *(src + y * TEST_W + x) = rand();

    int *out = (int *) malloc(sizeof(int) * TEST_W * TEST_H);

    int *testtest = (int *) malloc(sizeof(int) * TEST_W * TEST_H);
    sse_prefetch_transpose(src, testtest, TEST_W, TEST_H);


#ifdef SSE_P
    clock_gettime(CLOCK_REALTIME, &start);
    sse_prefetch_transpose(src, out, TEST_W, TEST_H);
    clock_gettime(CLOCK_REALTIME, &end);
    //printf("sse prefetch: \t %ld us\n", diff_in_us(start, end));
#endif

#ifdef SSE
    clock_gettime(CLOCK_REALTIME, &start);
    sse_transpose(src, out, TEST_W, TEST_H);
    clock_gettime(CLOCK_REALTIME, &end);
    //printf("sse: \t\t %ld us\n", diff_in_us(start, end));
#endif

#ifdef NAIVE
    clock_gettime(CLOCK_REALTIME, &start);
    naive_transpose(src, out, TEST_W, TEST_H);
    clock_gettime(CLOCK_REALTIME, &end);
    //printf("naive: \t\t %ld us\n", diff_in_us(start, end));
#endif

#ifdef AVX
    clock_gettime(CLOCK_REALTIME, &start);
    avx_transpose(src, out, TEST_W, TEST_H);
    clock_gettime(CLOCK_REALTIME, &end);
    //printf("avx: \t\t %ld us\n", diff_in_us(start, end));
#endif

#ifdef VERITY
    assert(0 == memcmp(testtest, out, TEST_W * TEST_H * sizeof(int)) && "Verification Fails");
#endif

    free(src);
    free(out);
    return 0;
}
