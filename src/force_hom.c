#include <R.h>
#include <Rmath.h>
#include <Rdefines.h>
#include <Rinternals.h>
#include "Rmissing.h"
#include "uncertain.h"

/* Set certain homozygous genotypes in X.snp.matrix to missing values */

SEXP force_hom(const SEXP Xsnps, const SEXP Female) {

  int *female = LOGICAL(Female);
  int N, M;
  int *dim = INTEGER(getAttrib(Xsnps, R_DimSymbol));
  N = dim[0];
  M = dim[1];

  SEXP Result;
  PROTECT(Result = duplicate(Xsnps));
  unsigned char *result = RAW(Result);
  for (int i=0; i<N; i++) {
    int male = !female[i];
    for (int j=0, ij=i; j<M; j++, ij+=N) {
      unsigned char rij = result[ij];
      if (rij && male) {
	if (rij>3) {
	  double p0, p1, p2;
	  g2post(rij, &p0, &p1, &p2);
	  if (p1>0.0)
	    result[ij] = 0;
	}
	else if (rij==2) 
	  result[ij] = 0;
      }	
    }
  }
  UNPROTECT(1);
  return(Result);
}
  
