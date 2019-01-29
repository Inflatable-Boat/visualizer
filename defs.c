#include "defs.h"

void matrix_inverse(float a[3][3],float ainv[3][3]){

  int i,j,l;
  static float row[3],col[3];

  float invdeta=1.0/DET(a);

  for(i=0;i<3;i++){
    for(j=0;j<3;j++){

      for(l=0;l<3;l++){
        row[l]=a[i][l];
        col[l]=a[l][j];

        if(l!=j)a[i][l]=0.0;
        if(l!=i)a[l][j]=0.0;
      }
      a[i][j]=1.0;
      ainv[j][i]=DET(a)*invdeta;

      for(l=0;l<3;l++){
        a[i][l]=row[l];
        a[l][j]=col[l];
      }

    }
  }
}
