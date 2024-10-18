#include "Functions.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Problem Constants
#define nAtoms 256
#define maxNeighbours 16

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

inline void insertInOrder(DataType currDist[maxNeighbours],
                          int currList[maxNeighbours],
                          int j,
                          DataType distIJ) {
    int dist, pos;
    DataType currMax;
    pos = maxNeighbours - 1;
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

int populateNeighbourList(DataType currDist[maxNeighbours],
                         int currList[maxNeighbours],
                         const int i,
                         int NL[nAtoms*maxNeighbours]) {
    int validPairs, neighbourIter;
    validPairs = 0;
    for (neighbourIter = 0; neighbourIter < maxNeighbours; neighbourIter++) {
        NL[i*maxNeighbours + neighbourIter] = currList[neighbourIter];
        validPairs++;
    }
    return validPairs;
}

int buildNeighbourList(DataType position_x[nAtoms],
                      DataType position_y[nAtoms],
                      DataType position_z[nAtoms],
                      int NL[nAtoms*maxNeighbours]) {
    int totalPairs, i, j, k;
    totalPairs = 0;
    DataType distIJ;
    for (i = 0; i < nAtoms; i++) {
        int currList[maxNeighbours];
        DataType currDist[maxNeighbours];
        for (k = 0; k < maxNeighbours; k++) {
            currList[k] = 0;
            currDist[k] = 999999999;
        }
        for (j = 0; j < maxNeighbours; j++) {
            if (i == j) {
                continue;
            }
            distIJ = distance(position_x, position_y, position_z, i, j);
            currList[j] = j;
            currDist[j] = distIJ;
        }
        totalPairs += populateNeighbourList(currDist, currList, i, NL);
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
    int NL[nAtoms*maxNeighbours];
    int *neighbourList = (int *)malloc(nAtoms * maxNeighbours * sizeof(int));

	initArray(position_x, nAtoms);
	initArray(position_y, nAtoms);
	initArray(position_z, nAtoms);
	initZero(force_x, nAtoms);
	initZero(force_y, nAtoms);
	initZero(force_z, nAtoms);

	for(i=0; i<nAtoms*maxNeighbours; ++i)
        NL[i] = 0;

    totalPairs = buildNeighbourList(position_x, position_y, position_z, NL);

    for (i = 0; i < nAtoms; i++) {
        for (j = 0; j < maxNeighbours; ++j)
            neighbourList[i * maxNeighbours + j] = NL[i*maxNeighbours+j];
    }

    core(force_x, force_y, force_z, position_x, position_y, position_z, neighbourList);

	for (int i = 0; i < nAtoms; ++i)
    	printf(DataFormat("", "\n"), force_x[i]);

    /*for (int i = 0; i < nAtoms; ++i)
    	printf(DataFormat("", "\n"), force_y[i]);

    for (int i = 0; i < nAtoms; ++i)
        printf(DataFormat("", "\n"), force_z[i]);*/                                    

    return 0;
}