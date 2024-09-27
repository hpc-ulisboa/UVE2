#include "Functions.h"
// Problem Constants
#define nAtoms 256
#define maxNeighbors 16
// LJ coefficients
#define LJ1 1.5
#define LJ2 2.0


void core(DataType *force_x, DataType *force_y, DataType *force_z, DataType *position_x, DataType *position_y, DataType *position_z, int32_t *NL) {
    DataType delx, dely, delz, r2inv;
    DataType r6inv, potential, force, j_x, j_y, j_z;
    DataType i_x, i_y, i_z, fx, fy, fz;

    int32_t i, j, jidx;

    // loop_i:
    for (i = 0; i < nAtoms; i++) {
        i_x = position_x[i];
        //  i_y = position_y[i];
        //  i_z = position_z[i];
        fx = 0;
        //  fy = 0;
        //  fz = 0;
        // loop_j:
        for (j = 0; j < maxNeighbors; j++) {
            // Get neighbor
            jidx = NL[i * maxNeighbors + j];
            // Look up x,y,z positions
            j_x = position_x[jidx];
            //  j_y = position_y[jidx];
            //  j_z = position_z[jidx];
            // Calc distance
            delx = i_x - j_x;
            //  dely = i_y - j_y;
            //  delz = i_z - j_z;
            r2inv = 1.0 / (delx * delx); // + dely*dely + delz*delz );
            // Assume no cutoff and aways account for all nodes in area
            r6inv = r2inv * r2inv * r2inv;
            potential = r6inv * (LJ1 * r6inv - LJ2);
            // Sum changes in force
            force = r2inv * potential;
            fx += delx * force;
            //  fy += dely * force;
            //  fz += delz * force;
        }
        // Update forces after all neighbors accounted for.
        force_x[i] = fx;
        //  force_y[i] = fy;
        //  force_z[i] = fz;
        // printf("dF=%lf,%lf,%lf\n", fx, fy, fz);
    }
}
