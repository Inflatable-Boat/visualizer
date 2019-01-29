#include "colors.inc"

#include "coords_opengl.pov"

background { color White }

#declare i=0;

#while ( i < npart)

  #switch (Data[i][0])
    #case (1.0)
    sphere{
      <Data[i][1],Data[i][2],Data[i][3]>
      Data[i][10]/2.0
      texture {
        pigment { color red Data[i][4] green Data[i][5] blue Data[i][6] }
      }
      no_shadow
      no_reflection
    }
  #end 

  #declare i=i+1;
#end
