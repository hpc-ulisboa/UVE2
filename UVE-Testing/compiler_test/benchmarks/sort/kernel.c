#include "Functions.h"

long int start = 0, end = 0;

static inline void merge(DataType *a, int start_sort, int m, int stop_sort, int N){
    DataType temp[N];
    int i, j, k;

    for(i=start_sort; i<=m; i++){
        temp[i] = a[i];
    }

    for(j=m+1; j<=stop_sort; j++){
        temp[m+1+stop_sort-j] = a[j];
    }

    i = start_sort;
    j = stop_sort;

    for(k=start_sort; k<=stop_sort; k++){
        DataType tmp_j = temp[j];
        DataType tmp_i = temp[i];
        if(tmp_j < tmp_i) {
            a[k] = tmp_j;
            j--;
        } else {
            a[k] = tmp_i;
            i++;
        }
    }
}

void core(DataType *a, int N) {
    asm volatile("rdinstret %[s] \t\n" : [s] "=&r"(start));

    int start_sort, stop_sort;
    int i, m, from, mid, to;

    start_sort = 0;
    stop_sort = N;

    for(m=1; m<stop_sort-start_sort; m+=m) {
        for(i=start_sort; i<stop_sort; i+=m+m) {
            from = i;
            mid = i+m-1;
            to = i+m+m-1;
            if(to < stop_sort){
                merge(a, from, mid, to, N);
            }
            else{
                merge(a, from, mid, stop_sort, N);
            }
        }
    }

    asm volatile("rdinstret %[e] \t\n" : [e] "=&r"(end));
    printf("%ld\n%ld\n", start, end);
}


