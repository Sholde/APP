#include <math.h>

/*
 *  Compute average velocity
 */
  double
  velavg(int npart, double vh[], double vaver, double h){
    int i;
    double vaverh=vaver*h;
    extern double vel;
    double sq;
    extern double count;

#pragma omp single
    {
      vel = 0.0;
      count=0.0;
    }

#pragma omp for reduction(+:count, vel)
    for (i=0; i<npart*3; i+=3){
      sq=sqrt(vh[i]*vh[i]+vh[i+1]*vh[i+1]+vh[i+2]*vh[i+2]);
      if (sq>vaverh) count++;
      vel+=sq;
    }

#pragma omp single
    {
      vel/=h;
    }

    return(vel);
  }
