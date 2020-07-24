#ifndef CDVARS_H_INCLUDED
#define CDVARS_H_INCLUDED

// begin with an arbitrary point, hold first point coordinates for comparison
double cinit[2] = {-2,0};
double cn[2] = {-2,0};
double ck[2] = {-2,0};
double ctrial[2] = {-2,0};

// define orthogonal vectors
double vx[2] = {1,0};
double vy[2] = {0,1};

// define factors
double a = 3;
double b = 0.5;
// for cycling through orthogonal vectors and setting new vectors
bool dirflag = 0;
// for line search
int succount, failcount = 0;

#endif // CDVARS_H_INCLUDED
