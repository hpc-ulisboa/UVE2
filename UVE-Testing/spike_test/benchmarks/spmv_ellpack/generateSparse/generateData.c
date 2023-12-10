/* Adapted from: MachSuite
   Using Sparse Matrix 494 Bus Interconnect Matrix from SuiteSparse (http://sparse.tamu.edu/HB/494_bus)
 Compile with:
gcc -DTYPE=5 -O3 ../../../../Functions.c -c
gcc -I../../../.. -O3 generateData.c -c
gcc -O3 Functions.o generateData.o -o generateData
*/
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "Functions.h"

// These constants are valid for the IEEE 494 bus interconnect matrix
#define NNZ 1666
#define N 494
#define L 10

#define ROW 0
#define COL 1

// Row first, then column comparison
int compar(const void *v_lhs, const void *v_rhs) {
    int row_lhs = ((int *)v_lhs)[ROW];
    int row_rhs = ((int *)v_rhs)[ROW];
    int col_lhs = ((int *)v_lhs)[COL];
    int col_rhs = ((int *)v_rhs)[COL];

    if (row_lhs == row_rhs) {
        if (col_lhs == col_rhs)
            return 0;
        else if (col_lhs < col_rhs)
            return -1;
        else
            return 1;
    } else if (row_lhs < row_rhs) {
        return -1;
    } else {
        return 1;
    }
}

int main(int argc, char **argv) {
    char *current, *next, *buffer;
    int status, i, fd, nbytes;
    int coords[NNZ][2]; // row, col
    double vals[NNZ];
    int row_fill[N];
    double nzval[N * L];
    int cols[N * L];
    double vec[N];
    int row, index;
    ssize_t state;
    struct stat file_info;

    // Load matrix file
    fd = open("494_bus_full.mtx", O_RDONLY);
    assert(fd >= 0 && "couldn't open matrix");
    status = fstat(fd, &file_info);
    assert(status == 0 && "couldn't get filesize of matrix");
    buffer = malloc(file_info.st_size + 1);
    buffer[file_info.st_size] = (char)0;
    nbytes = 0;
    do {
        status = read(fd, buffer, file_info.st_size - nbytes);
        assert(status >= 0 && "Couldn't read from matrix file");
        nbytes += status;
    } while (nbytes < file_info.st_size);
    close(fd);

    // Parse matrix file
    current = buffer;
    next = strchr(current, '\n'); // skip first two lines
    *next = (char)0;
    current = next + 1;
    for (i = 0; i < NNZ; i++) {
        next = strchr(current, '\n');
        *next = (char)0;
        current = next + 1;
        status = sscanf(current, "%d %d %lf", &coords[i][ROW], &coords[i][COL], &vals[i]);
        assert(status == 3 && "Parse error in matrix file");
    }

    // Sort by row
    qsort(coords, NNZ, 2 * sizeof(int), &compar);

    // Fill data structure
    memset(row_fill, 0, N * sizeof(int));   // fill helper array
    memset(cols, 0, N * L * sizeof(int));   // value doesn't really matter
    memset(nzval, 0, N * L * sizeof(double)); // value DOES matter (must be 0)
    for (i = 0; i < NNZ; i++) {
        row = coords[i][ROW] - 1; // matrix is 1-indexed
        index = L * row + row_fill[row];
        nzval[index] = vals[i];
        cols[index] = coords[i][COL] - 1; // matrix is 1-indexed
        ++row_fill[row];
    }

    // Set vector
    srand48(8650341L);
    for (i = 0; i < N; i++)
        vec[i] = ((double)rand()) / ((double)RAND_MAX);

    // Open and write
    fd = open("input.data", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    assert(fd > 0 && "Couldn't open input data file");

    // Write 'nzval:\n' to file
    state = write(fd, "#define nzvalSpmvEllpack { ", 27);
    assert(state == 27 && "Couldn't write nzval to input data file");
    for (int i = 0; i < N * L; i++) {
        char buffer[50];
        if (i == N * L - 1)
            sprintf(buffer, "%lf };\n\n", nzval[i]);  // Convert the float to a string
        else
            sprintf(buffer, "%lf, ", nzval[i]);  // Convert the float to a string
        ssize_t status = write(fd, buffer, strlen(buffer));  // Write the string to the file
        assert(status == strlen(buffer) && "Couldn't write nzval to input data file");
    }

    // Write 'cols:\n' to file
    state = write(fd, "#define colsSpmvEllpack { ", 26);
    assert(state == 26 && "Couldn't write cols to input data file");
    for (int i = 0; i < N * L; i++) {
        char buffer[50];
        if (i == N * L - 1)
            sprintf(buffer, "%d };\n\n", cols[i]);  // Convert the int to a string
        else
            sprintf(buffer, "%d, ", cols[i]);  // Convert the int to a string
        ssize_t status = write(fd, buffer, strlen(buffer));  // Write the string to the file
        assert(status == strlen(buffer) && "Couldn't write cols to input data file");
    }

    // Write 'vec:\n' to file
    state = write(fd, "#define vecSpmvEllpack { ", 25);
    assert(state == 25 && "Couldn't write vec to input data file");
    for (int i = 0; i < N; i++) {
        char buffer[50];
        if (i == N - 1)
            sprintf(buffer, "%lf };\n\n", vec[i]);  // Convert the float to a string
        else
            sprintf(buffer, "%lf, ", vec[i]);  // Convert the float to a string
        ssize_t status = write(fd, buffer, strlen(buffer));  // Write the string to the file
        assert(status == strlen(buffer) && "Couldn't write vec to input data file");
    }

    // Write 'rowDelimitersSpmvEllpack:\n' to file
    state = write(fd, "#define rowDelimitersSpmvEllpack { ", 34);
    assert(state == 34 && "Couldn't write rowDelimiters to input data file");
    for (int i = 0; i < N; i++) {
        char buffer[50];
        if (i == N - 1)
            sprintf(buffer, "%d };\n\n", row_fill[i]);  // Convert the int to a string
        else
            sprintf(buffer, "%d, ", row_fill[i]);  // Convert the int to a string
        ssize_t status = write(fd, buffer, strlen(buffer));  // Write the string to the file
        assert(status == strlen(buffer) && "Couldn't write rowDelimiters to input data file");
    }

    close(fd);

    return 0;
}