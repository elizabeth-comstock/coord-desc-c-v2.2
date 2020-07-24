#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "cdvars.h"

double recalcfuncval(double *v)
{
    return 100*pow((v[1]-pow(v[0],2)),2) + pow(1-v[0],2);
}

// calculate banana function for given inputs
/*
double recalcfuncval(double x, double y)
{
    return 100*pow((y-pow(x,2)),2) + pow(1-x,2);
}
*/
int main()
{
    // calculate function value at starting point
    double fn = recalcfuncval(cn);
    // and a pointer to one of two orthogonal vectors
    // pointed to vx first to prevent weird values
    double *v = vx;
    double fk = fn;     // holder for comparison
    double ft = fn;     // holder for trial

    /* This loop contains the entire process:
       1. Determine orthogonal vectors
       2. Cycle through vectors
       3. For each vector, determine search direction
       4. Perform line search, calculating step length
       5. When all orthogonal vectors have been cycled through,
          recalculate orthogonal vectors.
       First iteration starts at Step 2.
     */
    for (int h=1; h<=5; ++h)
    {
        /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
           2. Cycle through already chosen orthogonal vectors
           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
        if (dirflag == 0){
            v = vx;     // point v to vx
            dirflag = 1;
        } else {
            v = vy;     // point v to vy
            dirflag = 0;
        }

        /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
           3. For each vector, determine search direction
           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
        // DEBUG
        printf("***ORTHOGONAL VECTOR, ITERATION %i***\n",h);
        printf("v  = %f, %f\t\n\n",v[0],v[1]);
        printf("***DETERMINE SEARCH DIRECTION, ITERATION %i***\n",h);
        printf("cn = %f, %f \tFn = %f \n",cn[0],cn[1],fn);
        // printf("v  = %f, %f \n",v[0],v[1]);
        // values declared here because they reset with each iteration
        double d = 0.001;   // move a thousandth of a unit vector to test
        bool dirfound = 0;  // direction finding success flag
        int sdn = 1;        // hard iteration limit so no going out of control
        int efactor = 1;    // either 1 or -1, multiply to e to set direction
        // actually determine search direction
        while(dirfound == 0 && sdn <= 500)
        {
            for (int i=0; i<=1; ++i){
                ctrial[i] = cn[i] + d*v[i];
            }
            // then calculate new value yielded by trial
            ft = recalcfuncval(ctrial);

            // DEBUG
            printf("ct = %f, %f \tFt = %f \n",ctrial[0],ctrial[1],ft);

            if (ft < fn) {
                // trial successful
                printf("Dir trial %i success! \tefactor = %i \td = %f \n\n",sdn,efactor,d);
                dirfound = 1;
            } else {
                // reverse direction and halve, and check other side
                printf("Dir trial %i fail! \tefactor = %i \td = %f \n\n",sdn,efactor,d);
                d *= -0.5;
                efactor *= -1;
            }
            sdn++;
        }

        /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
           4. Perform line search, calculating step length
           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
        printf("***PERFORM LINE SEARCH, ITERATION %i***\n",h);
        double e = 0.5 * efactor;   // try a step of arbitrary length e
        int lsn = 1;                // hard iteration limit so no going out of control
        // calculate actual step length
        while((succount < 1 || failcount < 5) && lsn <= 500)
        {
            // iteratively solve coordinate values for each x-y dimension
            for (int j=0; j<=1; ++j){
                ctrial[j] = cn[j] + e*v[j];
            }
            // then calculate new value yielded by trial
            ft = recalcfuncval(ctrial);
            // DEBUG
            printf("ct = %f, %f \tFt = %f   \t",ctrial[0],ctrial[1],ft);

            if (ft <= fk) {
                fk = ft;                // update function value to lowest found
                for (int k=0; k<=1; ++k){
                    ck[k] = ctrial[k];  // write new coordinates to placeholder
                }
                e = e*a;
                succount += 1;
                failcount = 0;          // we want one success and five fails in succession
            } else {
                e = e*b;
                failcount += 1;
            }
            printf("%i %i\n",succount,failcount);
            lsn++;
        }
        // reset line search flags
        succount = 0;
        failcount = 0;
        lsn = 1;
        // write new coordinates and function value
        fn = fk;
        for (int l=0; l<=1; ++l){
            cn[l] = ck[l];
        }
        // DEBUG
        printf("\n***RESULT, ITERATION %i***\n",h);
        printf("cn = %f, %f \tFn = %f \n\n",cn[0],cn[1],fn);

        /* %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
           5. When all orthogonal vectors have been cycled
              through, recalculate orthogonal vectors.
              Linear search algorithm ensures always success,
              so you can't wait for a fail flag.
           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% */
        if (dirflag == 0)
        {
            vx[0] = cn[0] - cinit[0];
            vx[1] = cn[1] - cinit[1];
            double vlength = sqrt(pow(vx[0],2) + pow(vx[1],2));
            vx[0] /= vlength;
            vx[1] /= vlength;
            // find the other orthogonal vector, rotates different ways:
            vy[0] = vx[1];
            vy[1] = -vx[0];
            // reset flags
            dirflag = 0;
            // write current coordinates to cinit for next iteration
            for (int m=0; m<=1; ++m){
                cinit[m] = cn[m];
            }
        }
    }
    return 0;
}
