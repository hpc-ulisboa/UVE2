#include "Functions.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Problem Constants
#define nAtoms 256
#define maxNeighbors 16

extern void core(void *force_x, void *force_y, void *force_z, void *position_x, void *position_y, void *position_z, void *NL);

DataType distance(
    DataType position_x[nAtoms],
    DataType position_y[nAtoms],
    DataType position_z[nAtoms],
    int i,
    int j) {
    DataType delx, dely, delz, r2inv;
    delx = position_x[i] - position_x[j];
    dely = position_y[i] - position_y[j];
    delz = position_z[i] - position_z[j];
    r2inv = delx * delx + dely * dely + delz * delz;
    return r2inv;
}

inline void insertInOrder(DataType currDist[maxNeighbors],
                          int currList[maxNeighbors],
                          int j,
                          DataType distIJ) {
    int dist, pos;
    DataType currMax;
    pos = maxNeighbors - 1;
    currMax = currDist[pos];
    if (distIJ > currMax) {
        return;
    }
    for (dist = pos; dist > 0; dist--) {
        if (distIJ < currDist[dist]) {
            currDist[dist] = currDist[dist - 1];
            currList[dist] = currList[pos - 1];
        } else {
            break;
        }
        pos--;
    }
    currDist[dist] = distIJ;
    currList[dist] = j;
}

int populateNeighborList(DataType currDist[maxNeighbors],
                         int currList[maxNeighbors],
                         const int i,
                         int NL[nAtoms*maxNeighbors]) {
    int validPairs, neighborIter;
    validPairs = 0;
    for (neighborIter = 0; neighborIter < maxNeighbors; neighborIter++) {
        NL[i*maxNeighbors + neighborIter] = currList[neighborIter];
        validPairs++;
    }
    return validPairs;
}

int buildNeighborList(DataType position_x[nAtoms],
                      DataType position_y[nAtoms],
                      DataType position_z[nAtoms],
                      int NL[nAtoms*maxNeighbors]) {
    int totalPairs, i, j, k;
    totalPairs = 0;
    DataType distIJ;
    for (i = 0; i < nAtoms; i++) {
        int currList[maxNeighbors];
        DataType currDist[maxNeighbors];
        for (k = 0; k < maxNeighbors; k++) {
            currList[k] = 0;
            currDist[k] = 999999999;
        }
        for (j = 0; j < maxNeighbors; j++) {
            if (i == j) {
                continue;
            }
            distIJ = distance(position_x, position_y, position_z, i, j);
            currList[j] = j;
            currDist[j] = distIJ;
        }
        totalPairs += populateNeighborList(currDist, currList, i, NL);
    }
    return totalPairs;
}

int main() {
    int i, iter, j, totalPairs;
    iter = 0;

    // Allocate arrays
    DataType *position_x = (DataType *)malloc(nAtoms * sizeof(DataType));
    DataType *position_y = (DataType *)malloc(nAtoms * sizeof(DataType));
    DataType *position_z = (DataType *)malloc(nAtoms * sizeof(DataType));
    DataType *force_x = (DataType *)malloc(nAtoms * sizeof(DataType));
    DataType *force_y = (DataType *)malloc(nAtoms * sizeof(DataType));
    DataType *force_z = (DataType *)malloc(nAtoms * sizeof(DataType));
    int NL[nAtoms*maxNeighbors];
    int *neighborList = (int *)malloc(nAtoms * maxNeighbors * sizeof(int));

	initArray(position_x, nAtoms);
	initArray(position_y, nAtoms);
	initArray(position_z, nAtoms);
	initArray(force_x, nAtoms);
	initArray(force_y, nAtoms);
	initArray(force_z, nAtoms);

	for(i=0; i<nAtoms*maxNeighbors; ++i)
        NL[i] = 0;

    totalPairs = buildNeighborList(position_x, position_y, position_z, NL);

    for (i = 0; i < nAtoms; i++) {
        for (j = 0; j < maxNeighbors; ++j)
            neighborList[i * maxNeighbors + j] = NL[i*maxNeighbors+j];
    }

    core(force_x, force_y, force_z, position_x, position_y, position_z, neighborList);

	for (int i = 0; i < nAtoms; ++i)
    	printf(DataFormat("", "\n"), force_x[i]);

    return 0;
}