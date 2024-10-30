// https://github.com/breagen/MachSuite/tree/master/fft/strided
#include "Functions.h"

long int start = 0, end = 0;

void core(DataType *real, DataType *img, DataType *real_twid, DataType *img_twid, DataType *temp, int FFT_SIZE) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));

    int even, odd, span, log, rootindex;
    log = 0;

    for (span = FFT_SIZE >> 1; span; span >>= 1, log++) {
        for (odd = span; odd < FFT_SIZE; odd++) {
            odd |= span;
            even = odd ^ span;

            temp[odd] = real[even] + real[odd];
            real[odd] = real[even] - real[odd];
            real[even] = temp[odd];

            temp[odd] = img[even] + img[odd];
            img[odd] = img[even] - img[odd];
            img[even] = temp[odd];

            rootindex = (even << log) & (FFT_SIZE - 1);
            if (rootindex) {
                temp[odd] = real_twid[rootindex] * real[odd] -
                       img_twid[rootindex] * img[odd];
                img[odd] = real_twid[rootindex] * img[odd] +
                           img_twid[rootindex] * real[odd];
                real[odd] = temp[odd];
            }
        }
    }

    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
