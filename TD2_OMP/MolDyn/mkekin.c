#include <stdio.h>

extern double sum;

/*
 *  Scale forces, update velocities and compute K.E.
 */
double mkekin(int npart, double f[], double vh[], double hsq2, double hsq) {
  int i;
  double ekin;

#pragma omp single
  {
    sum = 0;
  }

#pragma omp for reduction(+:sum)
  for (i=0; i<3*npart; i++) {
    f[i]*=hsq2;
    vh[i]+=f[i];
    sum+=vh[i]*vh[i];
  } /* barrier implicite */

  ekin=sum/hsq;

  return(ekin);
}
