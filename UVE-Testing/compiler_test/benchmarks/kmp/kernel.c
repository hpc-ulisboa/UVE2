#include "Functions.h"

long int start = 0, end = 0;

void core(char *pattern, char *input, int32_t *kmpNext, int32_t *n_matches, int PATTERN_SIZE, int STRING_SIZE) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));

    int32_t k, i, q;
    n_matches[0] = 0;

    k = 0;
    kmpNext[0] = 0;

    for(q = 1; q < PATTERN_SIZE; q++){
        while(k > 0 && pattern[k] != pattern[q]){
            k = kmpNext[q];
        }
        if(pattern[k] == pattern[q]){
            k++;
        }
        kmpNext[q] = k;
    }

    q = 0;
    for(i = 0; i < STRING_SIZE; i++){
        while (q > 0 && pattern[q] != input[i]){
            q = kmpNext[q];
        }
        if (pattern[q] == input[i]){
            q++;
        }
        if (q >= PATTERN_SIZE){
            n_matches[0]++;
            q = kmpNext[q - 1];
        }
    }

    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}
