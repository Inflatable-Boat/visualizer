/*void load_coords_marjolein(FILE *f){
  int i;
  int n;
  float l;
  fscanf(f,"%i\n",&n);
  fscanf(f,"%f\n",&l);
  if(n > n_part){
    n_part=n;
    free(particles);
    particles=(tPart*) malloc(n_part * sizeof(tPart));
  }
  x_box_a=0;y_box_a=0;z_box_a=0;
  x_box_b=0;y_box_b=0;z_box_b=0;

  float xa,ya,za,xb,yb,zb;

  float s;

  for(i=0;i<n_part;i++){
    fscanf(f,"%e %e %e\n",&particles[i].x,&particles[i].y,&particles[i].z);
    fscanf(f,"%e %e %e\n",&particles[i].nx,&particles[i].ny,&particles[i].nz);

    if(particles[i].x>x_box_b) x_box_b=particles[i].x;
    if(particles[i].x<x_box_a) x_box_a=particles[i].x;
    if(particles[i].y>y_box_b) y_box_b=particles[i].y;
    if(particles[i].y<y_box_a) y_box_a=particles[i].y;
    if(particles[i].z>z_box_b) z_box_b=particles[i].z;
    if(particles[i].z<z_box_a) z_box_a=particles[i].z;

    particles[i].r=fabs(particles[i].nx);
    particles[i].g=fabs(particles[i].ny);
    particles[i].b=fabs(particles[i].nz);

    particles[i].size1=1;
    particles[i].size2=0.1;
  }

  xa=-(x_box_b-x_box_a)/2.0;  xb= (x_box_b-x_box_a)/2.0;
  ya=-(y_box_b-y_box_a)/2.0;  yb= (y_box_b-y_box_a)/2.0;
  za=-(z_box_b-z_box_a)/2.0;  zb= (z_box_b-z_box_a)/2.0;
  s=xb;  if(yb>s) s=yb;  if(zb>s) s=zb;

  for(i=0;i<n_part;i++){
    particles[i].x+=xa-x_box_a; particles[i].y+=ya-y_box_a; particles[i].z+=za-z_box_a;
    particles[i].x/=s;          particles[i].y/=s;          particles[i].z/=s;
    particles[i].size1 /= s;
    particles[i].size2 /= s;
    particles[i].selected=0;
    particles[i].visible=1;
    particles[i].alpha=1.0;
    particles[i].soort=2;
  }

  x_box_a=xa/s;  x_box_b=xb/s;
  y_box_a=ya/s;  y_box_b=yb/s;
  z_box_a=za/s;  z_box_b=zb/s;
}*/

/*void load_coords_rods(FILE *f){
  int i;
  int n;
  float l;
  fscanf(f,"%i\n",&n);
  fscanf(f,"%f\n",&l);
  if(n > n_part){
    n_part=n;
    free(particles);
    particles=(tPart*) malloc(n_part * sizeof(tPart));
  }
  fscanf(f,"%e %e %e\n",&x_box_b,&y_box_b,&z_box_b);
  x_box_a=0;y_box_a=0;z_box_a=0;

  float xa,ya,za,xb,yb,zb;
  xa=-(x_box_b-x_box_a)/2.0;  xb= (x_box_b-x_box_a)/2.0;
  ya=-(y_box_b-y_box_a)/2.0;  yb= (y_box_b-y_box_a)/2.0;
  za=-(z_box_b-z_box_a)/2.0;  zb= (z_box_b-z_box_a)/2.0;

  float s;
  s=xb;  if(yb>s) s=yb;  if(zb>s) s=zb;

  for(i=0;i<n_part;i++){
    fscanf(f,"%e %e %e\n",&particles[i].x,&particles[i].y,&particles[i].z);
    particles[i].x+=xa-x_box_a; particles[i].y+=ya-y_box_a; particles[i].z+=za-z_box_a;
    particles[i].x/=s;          particles[i].y/=s;          particles[i].z/=s;
    particles[i].r=0.9;         particles[i].g=0.0;         particles[i].b=0.0;
    particles[i].size1=1/s;
    particles[i].size2=5/s;
    particles[i].selected=0;
    particles[i].visible=1;
    particles[i].alpha=1.0;
    particles[i].soort=3;
  }
  for(i=0;i<n_part;i++){
    fscanf(f,"%e %e %e\n",&particles[i].nx,&particles[i].ny,&particles[i].nz);
  }
  x_box_a=xa/s;  x_box_b=xb/s;
  y_box_a=ya/s;  y_box_b=yb/s;
  z_box_a=za/s;  z_box_b=zb/s;
}

void load_coords_disks(FILE *f){
  int i;
  int n;
  fscanf(f,"%i\n",&n);
  if(n > n_part){
    n_part=n;
    free(particles);
    particles=(tPart*) malloc(n_part * sizeof(tPart));
  }
  fscanf(f,"%e %e\n",&x_box_a,&x_box_b);
  fscanf(f,"%e %e\n",&y_box_a,&y_box_b);
  fscanf(f,"%e %e\n",&z_box_a,&z_box_b);

  float xa,ya,za,xb,yb,zb;
  xa=-(x_box_b-x_box_a)/2.0;  xb= (x_box_b-x_box_a)/2.0;
  ya=-(y_box_b-y_box_a)/2.0;  yb= (y_box_b-y_box_a)/2.0;
  za=-(z_box_b-z_box_a)/2.0;  zb= (z_box_b-z_box_a)/2.0;

  float s;
  s=xb;  if(yb>s) s=yb;  if(zb>s) s=zb;

  for(i=0;i<n_part;i++){
    fscanf(f,"%e %e %e\n",&particles[i].x,&particles[i].y,&particles[i].z);
    particles[i].x+=xa-x_box_a; particles[i].y+=ya-y_box_a; particles[i].z+=za-z_box_a;
    particles[i].x/=s;          particles[i].y/=s;          particles[i].z/=s;
    particles[i].r=0.9;         particles[i].g=0.0;         particles[i].b=0.0;
    particles[i].size1=1/s;
    particles[i].size2=0.01/s;
    particles[i].selected=0;
    particles[i].visible=1;
    particles[i].alpha=1.0;
    particles[i].soort=2;
    fscanf(f,"%e %e %e\n",&particles[i].nx,&particles[i].ny,&particles[i].nz);
  }
  x_box_a=xa/s;  x_box_b=xb/s;
  y_box_a=ya/s;  y_box_b=yb/s;
  z_box_a=za/s;  z_box_b=zb/s;
}*/

void getline2(char* str,FILE *f){
  int comment=1;
  while(comment){
    fgets(str,400,f);
    if(str[0]!='#') comment=0;
  }
}

void set_rect_box(float xa,float xb,float ya,float yb,float za,float zb){
  VSET(bb[0],xa,ya,za);
  VSET(bb[1],xb,ya,za);
  VSET(bb[2],xb,yb,za);
  VSET(bb[3],xa,yb,za);

  VSET(bb[4],xa,ya,zb);
  VSET(bb[5],xb,ya,zb);
  VSET(bb[6],xb,yb,zb);
  VSET(bb[7],xa,yb,zb);
}

void load_coords_spheres(FILE *f){
  int i,j;
  int n;
  char str[400];
  float a,b,c;

 //Read headers
  getline2(str,f);
  if( sscanf(str,"%e %e %e\n",&a,&b,&c) == 3 ){
    float x_box_a,x_box_b;
    float y_box_a,y_box_b;
    float z_box_a,z_box_b;
    x_box_b=a;  y_box_b=b;  z_box_b=c;
    x_box_a=0;  y_box_a=0;  z_box_a=0;
    getline2(str,f);
    if( sscanf(str,"%i\n",&n) != 1) printf("Warning: failed reading header.\n");
    set_rect_box(x_box_a,x_box_b,y_box_a,y_box_b,z_box_a,z_box_b);
  } else {
    sscanf(str,"%i\n",&n);
    getline2(str,f);
    if( sscanf(str,"%e %e %e\n",&a,&b,&c) == 3 ){
      VSET(bb[0],a,b,c);
      getline2(str,f);
      if(sscanf(str,"%e %e %e\n",&a,&b,&c) !=3) printf("Warning: failed reading header.\n");
      VSET(bb[1],a,b,c);
      getline2(str,f);
      if(sscanf(str,"%e %e %e\n",&a,&b,&c) !=3) printf("Warning: failed reading header.\n");
      VSET(bb[3],a,b,c);
      getline2(str,f);
      if(sscanf(str,"%e %e %e\n",&a,&b,&c) !=3) printf("Warning: failed reading header.\n");
      VSET(bb[4],a,b,c);

      VADD(bb[2],bb[1],bb[3]);
      VADD(bb[5],bb[1],bb[4]);
      VADD(bb[6],bb[2],bb[4]);
      VADD(bb[7],bb[3],bb[4]);
      for(i=1;i<8;i++) VADD(bb[i],bb[i],bb[0]);


    } else {
      float x_box_a,x_box_b;
      float y_box_a,y_box_b;
      float z_box_a,z_box_b;
      if(sscanf(str,"%e %e\n",&x_box_a,&x_box_b) != 2) printf("Warning: failed reading header.\n");
      getline2(str,f);
      if(sscanf(str,"%e %e\n",&y_box_a,&y_box_b) != 2) printf("Warning: failed reading header.\n");
      getline2(str,f);
      if(sscanf(str,"%e %e\n",&z_box_a,&z_box_b) != 2) printf("Warning: failed reading header.\n");
      set_rect_box(x_box_a,x_box_b,y_box_a,y_box_b,z_box_a,z_box_b);
    }
  }

  particles = realloc(particles,sizeof(tPart)*n);
  n_part=n;
  // printf("Reading %i particles.\n", n);

 //Set the scale factor
  float s;
  double v[3];
  VSUB(v,bb[0],bb[1]);
  s=VLEN(v);
  VSUB(v,bb[0],bb[3]);
  if(VLEN(v)>s) s=VLEN(v);
  VSUB(v,bb[0],bb[4]);
  if(VLEN(v)>s) s=VLEN(v);
  s*=0.5;

 //Calculate center
  for(i=0;i<3;i++) {
    v[i]=0;
    for(j=0;j<8;j++) v[i]-=bb[j][i]/8.0;
  }

 //scale box
  for(i=0;i<8;i++) {
    VADD(bb[i],bb[i],v);
    VSMUL(bb[i],bb[i],1.0/s);
  }

 //Read the particle data
  double t1,t2,t3,t4;
  int ti;

  for(i=0;i<n_part;i++){
    getline2(str,f);
    int r = sscanf(str,"%le %le %le %le %i\n",&t1,&t2,&t3,&t4,&ti);

    if(r >= 3){
      particles[i].x=(float) t1;
      particles[i].y=(float) t2;
      particles[i].z=(float) t3;
    } else printf("FILE READ ERROR\n");

    if(r >= 4) {
      particles[i].size1=(float) t4;
    } else {
      particles[i].size1 = 1.0;
    }
    if(particles[i].size1 < 0.4) particles[i].lowres=1; else particles[i].lowres=0;
    if(r >= 5) {
      switch(ti % 26){
        case 1 :
          particles[i].r=0.6;         particles[i].g=0.0;         particles[i].b=0.0;
          break;
        case 2 :
          particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.0;
          break;
        case 3 :
          particles[i].r=0.0;         particles[i].g=0.0;         particles[i].b=0.6;
          break;
        case 4 :
          particles[i].r=0.6;         particles[i].g=0.6;         particles[i].b=0.0;
          break;
        case 5 :
          particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.6;
          break;
        case 6 :
          particles[i].r=0.6;         particles[i].g=0.0;         particles[i].b=0.6;
          break;
        case 7 :
          particles[i].r=0.4;         particles[i].g=0.0;         particles[i].b=0.0;
          break;
        case 8 :
          particles[i].r=0.0;         particles[i].g=0.4;         particles[i].b=0.0;
          break;
        case 9 :
          particles[i].r=0.0;         particles[i].g=0.0;         particles[i].b=0.4;
          break;
        case 10 :
          particles[i].r=0.4;         particles[i].g=0.4;         particles[i].b=0.0;
          break;
        case 11 :
          particles[i].r=0.0;         particles[i].g=0.4;         particles[i].b=0.4;
          break;
        case 12 :
          particles[i].r=0.4;         particles[i].g=0.0;         particles[i].b=0.4;
          break;
        case 13 :
          particles[i].r=0.3;         particles[i].g=0.6;         particles[i].b=0.0;
          break;
        case 14 :
          particles[i].r=0.6;         particles[i].g=0.3;         particles[i].b=0.0;
          break;
        case 15 :
          particles[i].r=0.0;         particles[i].g=0.3;         particles[i].b=0.6;
          break;
        case 16 :
          particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.3;
          break;
        case 17 :
          particles[i].r=0.6;         particles[i].g=0.0;         particles[i].b=0.3;
          break;
        case 18 :
          particles[i].r=0.3;         particles[i].g=0.0;         particles[i].b=0.6;
          break;
        case 19 :
          particles[i].r=0.6;         particles[i].g=0.6;         particles[i].b=0.6;
          break;
        case 20 :
          particles[i].r=0.3;         particles[i].g=0.6;         particles[i].b=0.6;
          break;
        case 21 :
          particles[i].r=0.6;         particles[i].g=0.3;         particles[i].b=0.6;
          break;
        case 22 :
          particles[i].r=0.6;         particles[i].g=0.6;         particles[i].b=0.3;
          break;
        case 23 :
          particles[i].r=0.6;         particles[i].g=0.3;         particles[i].b=0.3;
          break;
        case 24 :
          particles[i].r=0.3;         particles[i].g=0.3;         particles[i].b=0.6;
          break;
        case 25 :
          particles[i].r=0.3;         particles[i].g=0.6;         particles[i].b=0.3;
          break;
        default :
          particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.6;
          break;
      }
    } else {
      particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.6;
    }

    if(r >= 3) {
      particles[i].x+=v[0]; particles[i].y+=v[1]; particles[i].z+=v[2];
      particles[i].x/=s;          particles[i].y/=s;          particles[i].z/=s;
      particles[i].size1*=1/s;
      if(particles[i].selected!=1) particles[i].selected=0;
      particles[i].visible=1;
      particles[i].alpha=1.0;
      particles[i].soort=1;
    } else {
      particles[i].visible=0;
    }
  }

}



void load_coords_poly(FILE *f){
  int i,j;
  int n;
  char str[400];
  float a,b,c;

 //Read headers
  getline2(str,f);
  if( sscanf(str,"%e %e %e\n",&a,&b,&c) == 3 ){
    float x_box_a,x_box_b;
    float y_box_a,y_box_b;
    float z_box_a,z_box_b;
    x_box_b=a;  y_box_b=b;  z_box_b=c;
    x_box_a=0;  y_box_a=0;  z_box_a=0;
    getline2(str,f);
    if( sscanf(str,"%i\n",&n) != 1) printf("Warning: failed reading header.\n");
    set_rect_box(x_box_a,x_box_b,y_box_a,y_box_b,z_box_a,z_box_b);
  } else {
    sscanf(str,"%i\n",&n);
    getline2(str,f);
    if( sscanf(str,"%e %e %e\n",&a,&b,&c) == 3 ){
      VSET(bb[0],a,b,c);
      getline2(str,f);
      if(sscanf(str,"%e %e %e\n",&a,&b,&c) !=3) printf("Warning: failed reading header.\n");
      VSET(bb[1],a,b,c);
      getline2(str,f);
      if(sscanf(str,"%e %e %e\n",&a,&b,&c) !=3) printf("Warning: failed reading header.\n");
      VSET(bb[3],a,b,c);
      getline2(str,f);
      if(sscanf(str,"%e %e %e\n",&a,&b,&c) !=3) printf("Warning: failed reading header.\n");
      VSET(bb[4],a,b,c);

      VADD(bb[2],bb[1],bb[3]);
      VADD(bb[5],bb[1],bb[4]);
      VADD(bb[6],bb[2],bb[4]);
      VADD(bb[7],bb[3],bb[4]);
      for(i=1;i<8;i++) VADD(bb[i],bb[i],bb[0]);

      // hacked in, assuming rectangular box
      volume_ruud = (bb[1][0] - bb[0][0]) * (bb[3][1] - bb[0][1]) * (bb[4][2] - bb[0][2]);

    } else {
      float x_box_a,x_box_b;
      float y_box_a,y_box_b;
      float z_box_a,z_box_b;
      if(sscanf(str,"%e %e\n",&x_box_a,&x_box_b) != 2) printf("Warning: failed reading header.\n");
      getline2(str,f);
      if(sscanf(str,"%e %e\n",&y_box_a,&y_box_b) != 2) printf("Warning: failed reading header.\n");
      getline2(str,f);
      if(sscanf(str,"%e %e\n",&z_box_a,&z_box_b) != 2) printf("Warning: failed reading header.\n");
      set_rect_box(x_box_a,x_box_b,y_box_a,y_box_b,z_box_a,z_box_b);
    }
  }
	// printf("%f %f %f\n",bb[0][0],bb[0][1],bb[0][2]);
	// printf("%f %f %f\n",bb[1][0],bb[1][1],bb[1][2]);
	// printf("%f %f %f\n",bb[2][0],bb[2][1],bb[2][2]);
	// printf("%f %f %f\n",bb[3][0],bb[3][1],bb[3][2]);
	// printf("%f %f %f\n",bb[4][0],bb[4][1],bb[4][2]);
	// printf("%f %f %f\n",bb[5][0],bb[5][1],bb[5][2]);
	// printf("%f %f %f\n",bb[6][0],bb[6][1],bb[6][2]);
	// printf("%f %f %f\n",bb[7][0],bb[7][1],bb[7][2]);

  particles = realloc(particles,sizeof(tPart)*n);
  n_part=n;
  // printf("Reading %i particles.\n", n);

 //Set the scale factor
  float s;
  double v[3];
  VSUB(v,bb[0],bb[1]);
  s=VLEN(v);
  VSUB(v,bb[0],bb[3]);
  if(VLEN(v)>s) s=VLEN(v);
  VSUB(v,bb[0],bb[4]);
  if(VLEN(v)>s) s=VLEN(v);
  s*=0.5;

 //Calculate center
  for(i=0;i<3;i++) {
    v[i]=0;
    for(j=0;j<8;j++) v[i]-=bb[j][i]/8.0;
  }

 //scale box
  for(i=0;i<8;i++) {
    VADD(bb[i],bb[i],v);
    VSMUL(bb[i],bb[i],1.0/s);
  }

 //Read the particle data
  double t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t15;
  int t14;
  int ti=0;

  for(i=0;i<n_part;i++){
    getline2(str,f);
    ti=5;
    int r = sscanf(str,"%le %le %le %le %le %le %le %le %le %le %le %le %le %i %le %i\n",&t1,&t2,&t3,&t4,&t5,&t6,&t7,&t8,&t9,&t10,&t11,&t12,&t13,&t14,&t15,&ti);
    //printf("r: %d\n", r);

    if(r >= 3){
      particles[i].x=(float) t1;
      particles[i].y=(float) t2;
      particles[i].z=(float) t3;
    } else printf("FILE READ ERROR\n");

    if(r >= 4) {
      particles[i].size1=(float) t4;
      particles[i].size2=(float) t4;
      particles[i].size3=(float) t4;
    } else {
      particles[i].size1 = 1.0;
      particles[i].size2 = 1.0;
      particles[i].size3 = 1.0;
    }

    if(r>=5) {
       particles[i].matrix[0]=t5;
       particles[i].matrix[1]=t6;
       particles[i].matrix[2]=t7;
       particles[i].matrix[4]=t8;
       particles[i].matrix[5]=t9;
       particles[i].matrix[6]=t10;
       particles[i].matrix[8]=t11;
       particles[i].matrix[9]=t12;
       particles[i].matrix[10]=t13;
       particles[i].matrix[3] = 0;
       particles[i].matrix[7] = 0;
       particles[i].matrix[11] = 0;
       particles[i].matrix[12] = 0;
       particles[i].matrix[13] = 0;
       particles[i].matrix[14] = 0;
       particles[i].matrix[15] = 1;
      } else printf("problem with file\n");

    if (r>=14) {
      particles[i].soort=t14;
     }
     if (r>=15) {
      particles[i].angle=t15;
      // printf("angle now: %lf\n", particles[i].angle);
     }
    else
     {
      particles[i].soort=7;
     }



    if(particles[i].size1 < 1.4) particles[i].lowres=0; else particles[i].lowres=0;
    if(r >= 14) {
      switch(ti % 26){
        case 1 :
          particles[i].r=0.6;         particles[i].g=0.0;         particles[i].b=0.0;
          break;
        case 2 :
          particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.0;
          break;
        case 3 :
          particles[i].r=0.0;         particles[i].g=0.0;         particles[i].b=0.6;
          break;
        case 4 :
          particles[i].r=0.6;         particles[i].g=0.6;         particles[i].b=0.0;
          break;
        case 5 :
          particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.6;
          break;
        case 6 :
          particles[i].r=0.6;         particles[i].g=0.0;         particles[i].b=0.6;
          break;
        case 7 :
          particles[i].r=0.4;         particles[i].g=0.0;         particles[i].b=0.0;
          break;
        case 8 :
          particles[i].r=0.0;         particles[i].g=0.4;         particles[i].b=0.0;
          break;
        case 9 :
          particles[i].r=0.0;         particles[i].g=0.0;         particles[i].b=0.4;
          break;
        case 10 :
          particles[i].r=0.4;         particles[i].g=0.4;         particles[i].b=0.0;
          break;
        case 11 :
          particles[i].r=0.0;         particles[i].g=0.4;         particles[i].b=0.4;
          break;
        case 12 :
          particles[i].r=0.4;         particles[i].g=0.0;         particles[i].b=0.4;
          break;
        case 13 :
          particles[i].r=0.3;         particles[i].g=0.6;         particles[i].b=0.0;
          break;
        case 14 :
          particles[i].r=0.6;         particles[i].g=0.1;         particles[i].b=0.1;
          break;
        case 15 :
          particles[i].r=0.0;         particles[i].g=0.3;         particles[i].b=0.6;
          break;
        case 16 :
          particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.3;
          break;
        case 17 :
          particles[i].r=0.6;         particles[i].g=0.0;         particles[i].b=0.3;
          break;
        case 18 :
          particles[i].r=0.3;         particles[i].g=0.0;         particles[i].b=0.6;
          break;
        case 19 :
          particles[i].r=0.6;         particles[i].g=0.6;         particles[i].b=0.6;
          break;
        case 20 :
          particles[i].r=0.3;         particles[i].g=0.6;         particles[i].b=0.6;
          break;
        case 21 :
          particles[i].r=0.6;         particles[i].g=0.3;         particles[i].b=0.6;
          break;
        case 22 :
          particles[i].r=0.6;         particles[i].g=0.6;         particles[i].b=0.3;
          break;
        case 23 :
          particles[i].r=0.6;         particles[i].g=0.3;         particles[i].b=0.3;
          break;
        case 24 :
          particles[i].r=0.3;         particles[i].g=0.3;         particles[i].b=0.6;
          break;
        case 25 :
          particles[i].r=0.3;         particles[i].g=0.6;         particles[i].b=0.3;
          break;
        default :
          particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.6;
          break;
      }
    } else {
      particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.6;
    }

    if(r >= 3) {
      particles[i].x+=3*v[0]; particles[i].y+=3*v[1]; particles[i].z+=3*v[2]; // NOTE: some bug is causing particles to be offset by twice the CM of the box
      particles[i].x/=s;          particles[i].y/=s;          particles[i].z/=s;
      particles[i].size1*=1/s;
      if(particles[i].selected!=1) particles[i].selected=0;
      particles[i].visible=1;
      particles[i].alpha=1.0;
    } else {
      particles[i].visible=0;
    }
  }

}


void load_coords_cubes(FILE *f){
  int i,j;
  int n;
  char str[400];
  float a,b,c;

 //Read headers
  getline2(str,f);
  if( sscanf(str,"%e %e %e\n",&a,&b,&c) == 3 ){
    float x_box_a,x_box_b;
    float y_box_a,y_box_b;
    float z_box_a,z_box_b;
    x_box_b=a;  y_box_b=b;  z_box_b=c;
    x_box_a=0;  y_box_a=0;  z_box_a=0;
    getline2(str,f);
    if( sscanf(str,"%i\n",&n) != 1) printf("Warning: failed reading header.\n");
    set_rect_box(x_box_a,x_box_b,y_box_a,y_box_b,z_box_a,z_box_b);
  } else {
    sscanf(str,"%i\n",&n);
    getline2(str,f);
    if( sscanf(str,"%e %e %e\n",&a,&b,&c) == 3 ){
      VSET(bb[0],a,b,c);
      getline2(str,f);
      if(sscanf(str,"%e %e %e\n",&a,&b,&c) !=3) printf("Warning: failed reading header.\n");
      VSET(bb[1],a,b,c);
      getline2(str,f);
      if(sscanf(str,"%e %e %e\n",&a,&b,&c) !=3) printf("Warning: failed reading header.\n");
      VSET(bb[3],a,b,c);
      getline2(str,f);
      if(sscanf(str,"%e %e %e\n",&a,&b,&c) !=3) printf("Warning: failed reading header.\n");
      VSET(bb[4],a,b,c);

      VADD(bb[2],bb[1],bb[3]);
      VADD(bb[5],bb[1],bb[4]);
      VADD(bb[6],bb[2],bb[4]);
      VADD(bb[7],bb[3],bb[4]);
      for(i=1;i<8;i++) VADD(bb[i],bb[i],bb[0]);


    } else {
      float x_box_a,x_box_b;
      float y_box_a,y_box_b;
      float z_box_a,z_box_b;
      if(sscanf(str,"%e %e\n",&x_box_a,&x_box_b) != 2) printf("Warning: failed reading header.\n");
      getline2(str,f);
      if(sscanf(str,"%e %e\n",&y_box_a,&y_box_b) != 2) printf("Warning: failed reading header.\n");
      getline2(str,f);
      if(sscanf(str,"%e %e\n",&z_box_a,&z_box_b) != 2) printf("Warning: failed reading header.\n");
      set_rect_box(x_box_a,x_box_b,y_box_a,y_box_b,z_box_a,z_box_b);
    }
  }

  particles = realloc(particles,sizeof(tPart)*n);
  n_part=n;
  // printf("Reading %i particles.\n", n);

 //Set the scale factor
  float s;
  double v[3];
  VSUB(v,bb[0],bb[1]);
  s=VLEN(v);
  VSUB(v,bb[0],bb[3]);
  if(VLEN(v)>s) s=VLEN(v);
  VSUB(v,bb[0],bb[4]);
  if(VLEN(v)>s) s=VLEN(v);
  s*=0.5;

 //Calculate center
  for(i=0;i<3;i++) {
    v[i]=0;
    for(j=0;j<8;j++) v[i]-=bb[j][i]/8.0;
  }

 //scale box
  for(i=0;i<8;i++) {
    VADD(bb[i],bb[i],v);
    VSMUL(bb[i],bb[i],1.0/s);
  }

 //Read the particle data
  double t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13;
  int ti;

  for(i=0;i<n_part;i++){
    getline2(str,f);
    int r = sscanf(str,"%le %le %le %le %le %le %le %le %le %le %le %le %le %i\n",&t1,&t2,&t3,&t4,&t5,&t6,&t7,&t8,&t9,&t10,&t11,&t12,&t13,&ti);

    if(r >= 3){
      particles[i].x=(float) t1;
      particles[i].y=(float) t2;
      particles[i].z=(float) t3;
    } else printf("FILE READ ERROR\n");

    if(r >= 4) {
      particles[i].size1=(float) t4;
      particles[i].size2=(float) t4;
      particles[i].size3=(float) t4;
    } else {
      particles[i].size1 = 1.0;
      particles[i].size2 = 1.0;
      particles[i].size3 = 1.0;
    }

    if(r>=5) {
       particles[i].matrix[0]=t5;
       particles[i].matrix[1]=t6;
       particles[i].matrix[2]=t7;
       particles[i].matrix[4]=t8;
       particles[i].matrix[5]=t9;
       particles[i].matrix[6]=t10;
       particles[i].matrix[8]=t11;
       particles[i].matrix[9]=t12;
       particles[i].matrix[10]=t13;
       particles[i].matrix[3] = 0;
       particles[i].matrix[7] = 0;
       particles[i].matrix[11] = 0;
       particles[i].matrix[12] = 0;
       particles[i].matrix[13] = 0;
       particles[i].matrix[14] = 0;
       particles[i].matrix[15] = 1;
      } else printf("problem with file\n");





    if(particles[i].size1 < 0.4) particles[i].lowres=1; else particles[i].lowres=0;
    if(r >= 14) {
      switch(ti % 26){
        case 1 :
          particles[i].r=0.6;         particles[i].g=0.0;         particles[i].b=0.0;
          break;
        case 2 :
          particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.0;
          break;
        case 3 :
          particles[i].r=0.0;         particles[i].g=0.0;         particles[i].b=0.6;
          break;
        case 4 :
          particles[i].r=0.6;         particles[i].g=0.6;         particles[i].b=0.0;
          break;
        case 5 :
          particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.6;
          break;
        case 6 :
          particles[i].r=0.6;         particles[i].g=0.0;         particles[i].b=0.6;
          break;
        case 7 :
          particles[i].r=0.4;         particles[i].g=0.0;         particles[i].b=0.0;
          break;
        case 8 :
          particles[i].r=0.0;         particles[i].g=0.4;         particles[i].b=0.0;
          break;
        case 9 :
          particles[i].r=0.0;         particles[i].g=0.0;         particles[i].b=0.4;
          break;
        case 10 :
          particles[i].r=0.4;         particles[i].g=0.4;         particles[i].b=0.0;
          break;
        case 11 :
          particles[i].r=0.0;         particles[i].g=0.4;         particles[i].b=0.4;
          break;
        case 12 :
          particles[i].r=0.4;         particles[i].g=0.0;         particles[i].b=0.4;
          break;
        case 13 :
          particles[i].r=0.3;         particles[i].g=0.6;         particles[i].b=0.0;
          break;
        case 14 :
          particles[i].r=0.6;         particles[i].g=0.3;         particles[i].b=0.0;
          break;
        case 15 :
          particles[i].r=0.0;         particles[i].g=0.3;         particles[i].b=0.6;
          break;
        case 16 :
          particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.3;
          break;
        case 17 :
          particles[i].r=0.6;         particles[i].g=0.0;         particles[i].b=0.3;
          break;
        case 18 :
          particles[i].r=0.3;         particles[i].g=0.0;         particles[i].b=0.6;
          break;
        case 19 :
          particles[i].r=0.6;         particles[i].g=0.6;         particles[i].b=0.6;
          break;
        case 20 :
          particles[i].r=0.3;         particles[i].g=0.6;         particles[i].b=0.6;
          break;
        case 21 :
          particles[i].r=0.6;         particles[i].g=0.3;         particles[i].b=0.6;
          break;
        case 22 :
          particles[i].r=0.6;         particles[i].g=0.6;         particles[i].b=0.3;
          break;
        case 23 :
          particles[i].r=0.6;         particles[i].g=0.3;         particles[i].b=0.3;
          break;
        case 24 :
          particles[i].r=0.3;         particles[i].g=0.3;         particles[i].b=0.6;
          break;
        case 25 :
          particles[i].r=0.3;         particles[i].g=0.6;         particles[i].b=0.3;
          break;
        default :
          particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.6;
          break;
      }
    } else {
      particles[i].r=0.0;         particles[i].g=0.6;         particles[i].b=0.6;
    }

    if(r >= 3) {
      particles[i].x+=v[0]; particles[i].y+=v[1]; particles[i].z+=v[2];
      particles[i].x/=s;          particles[i].y/=s;          particles[i].z/=s;
      particles[i].size1*=1/s;
      if(particles[i].selected!=1) particles[i].selected=0;
      particles[i].visible=1;
      particles[i].alpha=1.0;
      particles[i].soort=7;
    } else {
      particles[i].visible=0;
    }
  }

}




void load2(FILE *f){
  int i;
  int n;
  float x_box_a,x_box_b;
  float y_box_a,y_box_b;
  float z_box_a,z_box_b;
  char str[400];
  getline2(str,f);
  if(sscanf(str,"%i\n",&n) != 1) {printf("Error reading file\n"); return;}
  if(n > n_part){
    n_part=n;
    free(particles);
    particles=(tPart*) malloc(n_part * sizeof(tPart));
  }
  getline2(str,f);
  if(sscanf(str,"%e %e\n",&x_box_a,&x_box_b) != 2) {printf("Error reading file\n"); return;}
  getline2(str,f);
  if(sscanf(str,"%e %e\n",&y_box_a,&y_box_b) != 2) {printf("Error reading file\n"); return;}
  getline2(str,f);
  if(sscanf(str,"%e %e\n",&z_box_a,&z_box_b) != 2) {printf("Error reading file\n"); return;}

  float xa,ya,za,xb,yb,zb;
  xa=-(x_box_b-x_box_a)/2.0;  xb= (x_box_b-x_box_a)/2.0;
  ya=-(y_box_b-y_box_a)/2.0;  yb= (y_box_b-y_box_a)/2.0;
  za=-(z_box_b-z_box_a)/2.0;  zb= (z_box_b-z_box_a)/2.0;

  float s;
  s=xb;  if(yb>s) s=yb;  if(zb>s) s=zb;

  for(i=0;i<n_part;i++){
    getline2(str,f);
    sscanf(str,"%d %e %e %e\n",&particles[i].soort,&particles[i].x,&particles[i].y,&particles[i].z);
    particles[i].x+=xa-x_box_a; particles[i].y+=ya-y_box_a; particles[i].z+=za-z_box_a;
    particles[i].x/=s;          particles[i].y/=s;          particles[i].z/=s;

    getline2(str,f);
    sscanf(str,"%e %e %e %e\n",&particles[i].r,&particles[i].g,&particles[i].b,&particles[i].alpha);
    getline2(str,f);
    sscanf(str,"%d %d\n",&particles[i].visible,&particles[i].selected);
    getline2(str,f);
    sscanf(str,"%e %e %e\n",&particles[i].size1,&particles[i].size2,&particles[i].size3);
    getline2(str,f);
    sscanf(str,"%e %e %e\n",&particles[i].nx,&particles[i].ny,&particles[i].nz);

    if(particles[i].size1 < 0.4) particles[i].lowres=1; else particles[i].lowres=0;
    particles[i].size1*=1.0/s;
    particles[i].size2*=1.0/s;
    particles[i].size3*=1.0/s;
  }
  x_box_a=xa/s;  x_box_b=xb/s;
  y_box_a=ya/s;  y_box_b=yb/s;
  z_box_a=za/s;  z_box_b=zb/s;
  set_rect_box(x_box_a,x_box_b,y_box_a,y_box_b,z_box_a,z_box_b);
}

void loadold(){

  FILE *f=fopen(globbuf.gl_pathv[globcur],"r");

  if(f!=NULL){
    // printf("File: %s opened.\n",globbuf.gl_pathv[globcur]);
      switch(datakind){
        case 'r':
          //load_coords_rods(f);
          break;
        case 'd':
          //load_coords_disks(f);
          break;
        case 's':
          load_coords_spheres(f);
          break;
        case 'm':
          //load_coords_mixtures(f);
          break;
        case 'v':
          //load_coords_vor(f);
          break;
        case 'p':
          //load_coords_marjolein(f);
          break;
        default:
          load2(f);
          break;
      }
    fclose(f);
  }else printf("File not found!\n");
}

float dot_product(float r[3])
  {
  return r[0]*r[0]+r[1]*r[1]+r[2]*r[2];
}

void load(){

  FILE *f=fopen(globbuf.gl_pathv[globcur],"r");
  // int i;
	// CUBE

  vcub[0][0]=-0.5; vcub[0][1]=-0.5; vcub[0][2]=-0.5;
  vcub[1][0]=-0.5; vcub[1][1]=-0.5; vcub[1][2]=0.5;
  vcub[2][0]=-0.5; vcub[2][1]=0.5;  vcub[2][2]=-0.5;
  vcub[3][0]=-0.5; vcub[3][1]=0.5;  vcub[3][2]=0.5;
  vcub[4][0]=0.5;  vcub[4][1]=-0.5; vcub[4][2]=-0.5;
  vcub[5][0]=0.5;  vcub[5][1]=-0.5; vcub[5][2]=0.5;
  vcub[6][0]=0.5;  vcub[6][1]=0.5;  vcub[6][2]=-0.5;
  vcub[7][0]=0.5;  vcub[7][1]=0.5;  vcub[7][2]=0.5;

  /*
  printf("v1 = %f,%f,%f \n",vdcub[0][0], vdcub[0][1], vdcub[0][2]);
  printf("v2 = %f,%f,%f \n",vdcub[1][0], vdcub[1][1], vdcub[1][2]);
  printf("v3 = %f,%f,%f \n",vdcub[2][0], vdcub[2][1], vdcub[2][2]);
  printf("v4 = %f,%f,%f \n",vdcub[3][0], vdcub[3][1], vdcub[3][2]);
  printf("v5 = %f,%f,%f \n",vdcub[4][0], vdcub[4][1], vdcub[4][2]);
  printf("v6 = %f,%f,%f \n",vdcub[5][0], vdcub[5][1], vdcub[5][2]);
  printf("v7 = %f,%f,%f \n",vdcub[6][0], vdcub[6][1], vdcub[6][2]);
  printf("v8 = %f,%f,%f \n",vdcub[7][0], vdcub[7][1], vdcub[7][2]);
  */



	// Truncated Octagedron
	vtoct[0][0]=0.0;   vtoct[0][1]=0.0;     vtoct[0][2]=0.0;
	vtoct[1][0]= -1.5; vtoct[1][1]= -0.5;   vtoct[1][2]= 0.;
	vtoct[2][0]= -1.5; vtoct[2][1]= 0.5;    vtoct[2][2]= 0.;
	vtoct[3][0]= -1.;  vtoct[3][1]= -1.;    vtoct[3][2]= -0.7071067811865475;
	vtoct[4][0]= -1.;  vtoct[4][1]= -1.;    vtoct[4][2]= 0.7071067811865475;
	vtoct[5][0]= -1.;  vtoct[5][1]= 1.;     vtoct[5][2]= -0.7071067811865475;
	vtoct[6][0]= -1.;  vtoct[6][1]= 1.;     vtoct[6][2]= 0.7071067811865475;
	vtoct[7][0]= -0.5; vtoct[7][1]= -1.5;   vtoct[7][2]= 0.;
	vtoct[8][0]= -0.5; vtoct[8][1]= -0.5;   vtoct[8][2]= -1.4142135623730951;
	vtoct[9][0]= -0.5; vtoct[9][1]= -0.5;   vtoct[9][2]= 1.4142135623730951;
	vtoct[10][0]= -0.5; vtoct[10][1]= 0.5;    vtoct[10][2]= -1.4142135623730951;
	vtoct[11][0]= -0.5; vtoct[11][1]= 0.5;  vtoct[11][2]= 1.4142135623730951;
	vtoct[12][0]= -0.5; vtoct[12][1]= 1.5;  vtoct[12][2]= 0.;
	vtoct[13][0]= 0.5; vtoct[13][1]= -1.5;  vtoct[13][2]= 0.;
	vtoct[14][0]= 0.5; vtoct[14][1]= -0.5;  vtoct[14][2]= -1.4142135623730951;
	vtoct[15][0]= 0.5; vtoct[15][1]= -0.5;  vtoct[15][2]= 1.4142135623730951;
	vtoct[16][0]= 0.5; vtoct[16][1]= 0.5;   vtoct[16][2]= -1.4142135623730951;
	vtoct[17][0]= 0.5; vtoct[17][1]= 0.5;   vtoct[17][2]= 1.4142135623730951;
	vtoct[18][0]= 0.5; vtoct[18][1]= 1.5;   vtoct[18][2]= 0.;
	vtoct[19][0]= 1.;  vtoct[19][1]= -1.;   vtoct[19][2]= -0.7071067811865475;
	vtoct[20][0]= 1.;  vtoct[20][1]= -1.;   vtoct[20][2]= 0.7071067811865475;
	vtoct[21][0]= 1.;  vtoct[21][1]= 1.;    vtoct[21][2]= -0.7071067811865475;
	vtoct[22][0]= 1.;  vtoct[22][1]= 1.;    vtoct[22][2]= 0.7071067811865475;
	vtoct[23][0]= 1.5; vtoct[23][1]= -0.5;  vtoct[23][2]= 0.;
	vtoct[24][0]= 1.5; vtoct[24][1]= 0.5;   vtoct[24][2]= 0.;


	// Hexagonal Prism
	vprism6[0][0]=-1.;vprism6[0][1]=0.;vprism6[0][2]=-0.5;
	vprism6[1][0]=-1.;vprism6[1][1]=0.;vprism6[1][2]=0.5;
	vprism6[2][0]=-0.5;vprism6[2][1]=-0.866025;vprism6[2][2]=-0.5;
	vprism6[3][0]=-0.5;vprism6[3][1]=-0.866025;vprism6[3][2]=0.5;
	vprism6[4][0]=-0.5;vprism6[4][1]=0.866025;vprism6[4][2]=-0.5;
	vprism6[5][0]=-0.5;vprism6[5][1]=0.866025;vprism6[5][2]=0.5;
	vprism6[6][0]=0.5;vprism6[6][1]=-0.866025;vprism6[6][2]=-0.5;
	vprism6[7][0]=0.5;vprism6[7][1]=-0.866025;vprism6[7][2]=0.5;
	vprism6[8][0]=0.5;vprism6[8][1]=0.866025;vprism6[8][2]=-0.5;
	vprism6[9][0]=0.5;vprism6[9][1]=0.866025;vprism6[9][2]=0.5;
	vprism6[10][0]=1.;vprism6[10][1]=0.;vprism6[10][2]=-0.5;
	vprism6[11][0]=1.;vprism6[11][1]=0.;vprism6[11][2]=0.5;


  if(f!=NULL){
    // printf("File: %s opened.\n",globbuf.gl_pathv[globcur]);
    if (strstr(globbuf.gl_pathv[globcur],".dat\0"))   load_coords_spheres(f);
   // if (strstr(globbuf.gl_pathv[globcur],".cub\0"))   load_coords_cubes(f);
    if (strstr(globbuf.gl_pathv[globcur],".poly\0"))
     {
        load_coords_poly(f);
     }

  // DISTORTED CUBE


  float dcphi=particles[0].angle;

  // printf("Angle: %lf\n", dcphi);

  vdcub[0][0]=-0.5 *(1+cos(dcphi));             vdcub[0][1]=-0.5 ; vdcub[0][2]=-0.5 * (sin(dcphi));
  vdcub[1][0]= cos(dcphi) -0.5 *(1+cos(dcphi)); vdcub[1][1]=-0.5 ; vdcub[1][2]=0.5* (sin(dcphi));
  vdcub[2][0]=-0.5*(1+cos(dcphi));              vdcub[2][1]= 0.5;  vdcub[2][2]=-0.5* (sin(dcphi));

  vdcub[3][0]= cos(dcphi) -0.5 *(1+cos(dcphi)); vdcub[3][1]= 0.5 ; vdcub[3][2]=0.5* (sin(dcphi));
  vdcub[4][0]=1-0.5 *(1+cos(dcphi));            vdcub[4][1]=-0.5;  vdcub[4][2]=-0.5* (sin(dcphi));

  vdcub[5][0]=1+cos(dcphi)-0.5*(1+cos(dcphi));  vdcub[5][1]=-0.5;  vdcub[5][2]=0.5* (sin(dcphi));
  vdcub[6][0]=1-0.5*(1+cos(dcphi));             vdcub[6][1]=0.5;   vdcub[6][2]=-0.5* (sin(dcphi));
  vdcub[7][0]=1+cos(dcphi)-0.5*(1+cos(dcphi));  vdcub[7][1]=0.5;   vdcub[7][2]=0.5* (sin(dcphi));


    fclose(f);
  } else printf("File not found!\n");
}



/*
void save(){
  int i;
  FILE *f=fopen("coords_opengl.dat","w");
  fprintf(f,"%d \n",n_part);
  fprintf(f,"%f %f\n",x_box_a,x_box_b);
  fprintf(f,"%f %f\n",y_box_a,y_box_b);
  fprintf(f,"%f %f\n",z_box_a,z_box_b);
  for(i=0;i<n_part;i++){
    fprintf(f,"%d %e %e %e\n",particles[i].soort,particles[i].x,particles[i].y,particles[i].z);
    fprintf(f,"%e %e %e %e\n",particles[i].r,particles[i].g,particles[i].b,particles[i].alpha);
    fprintf(f,"%d %d\n",particles[i].visible,particles[i].selected);
    fprintf(f,"%e %e\n",particles[i].size1,particles[i].size2);
    fprintf(f,"%e %e %e\n",particles[i].nx,particles[i].ny,particles[i].nz);
  }
  fclose(f);
}*/

void rotv(GLfloat m[16], GLfloat v[3]){
  GLfloat vn[3];
  vn[0]=v[0]*m[0]+v[1]*m[1]+v[2]*m[2];
  vn[1]=v[0]*m[4]+v[1]*m[5]+v[2]*m[6];
  vn[2]=v[0]*m[8]+v[1]*m[9]+v[2]*m[10];
  v[0]=vn[0];
  v[1]=vn[1];
  v[2]=vn[2];
}

void rotv2(GLfloat m[16], GLfloat v[3]){
  GLfloat vn[3];
  vn[0]=v[0]*m[0]+v[1]*m[4]+v[2]*m[8];
  vn[1]=v[0]*m[1]+v[1]*m[5]+v[2]*m[9];
  vn[2]=v[0]*m[2]+v[1]*m[6]+v[2]*m[10];
  v[0]=vn[0];
  v[1]=vn[1];
  v[2]=vn[2];
}

void save_povray(){
  int i;
  FILE *f=fopen("coords_opengl.pov","w");

  GLfloat loc[3]={0.0f,0.0f,5.0f};
  GLfloat sky[3]={0.0f,1.0f,0.0f};
  GLfloat bgcolor[3]={0.0f,0.0f,0.0f};

  rotv(rot_matrix,loc);
  fprintf(f,"#include \"colors.inc\"");
  fprintf(f,"light_source { <%f, %f, %f> color <2.0,2.0,2.0>}\n",loc[0],loc[1],loc[2]);
  if(clearcolor==1) {bgcolor[0]=1.0;bgcolor[1]=1.0;bgcolor[2]=1.0; } else
    if(clearcolor==2) {bgcolor[0]=0.1;bgcolor[1]=0.1;bgcolor[2]=0.1; } else
      if(clearcolor==2) {bgcolor[0]=0.0;bgcolor[1]=0.0;bgcolor[2]=0.0; }
  fprintf(f,"background { color red %f green %f blue %f }\n",bgcolor[0],bgcolor[1],bgcolor[2]);

  rotv(rot_matrix,sky);

  if(ortho) {
    GLfloat up[3]={3.0f,0.0f,0.0f};
    GLfloat right[3]={0.0f,3.0f,0.0f};
    fprintf(f,"\
    camera {\n\
      orthographic\n\
      location <%f, %f, %f>\n\
      look_at  <0, 0,  0>\n\
      up <%f, %f, %f>\n\
      right <%f, %f, %f>\n\
      sky <%f, %f, %f>\n\
    }\n\n",loc[0],loc[1],loc[2],   up[0],up[1],up[2],  right[0],right[1],right[2],  sky[0],sky[1],sky[2]);
  } else {
    GLfloat up[3]={0.7f,0.0f,0.0f};
    GLfloat right[3]={0.0f,0.7f,0.0f};
    fprintf(f,"\
    camera {\n\
      perspective\n\
      location <%f, %f, %f>\n\
      look_at  <0, 0,  0>\n\
      up <%f, %f, %f>\n\
      right <%f, %f, %f>\n\
      sky <%f, %f, %f>\n\
    }\n\n",loc[0],loc[1],loc[2],   up[0],up[1],up[2],  right[0],right[1],right[2],  sky[0],sky[1],sky[2]);
  }

  fprintf(f,"#declare npart=%i;\n",n_part);
  fprintf(f,"#declare Data = array[%i][15]\n{\n",n_part);

  for(i=0;i<n_part;i++){
    fprintf(f,"  {%d, %e, %e, %e ",particles[i].soort,particles[i].x,particles[i].y,particles[i].z);
    fprintf(f,"%e, %e, %e, %e, ",particles[i].r,particles[i].g,particles[i].b,particles[i].alpha);
    fprintf(f,"%d, %d, ",particles[i].visible,particles[i].selected);
    fprintf(f,"%e, %e, ",particles[i].size1,particles[i].size2);
    fprintf(f,"%e, %e, %e ",particles[i].nx,particles[i].ny,particles[i].nz);
    fprintf(f,"} \n");
  }
  fprintf(f,"}\n");

  fprintf(f,"\
    #declare i=0;\n\
    #while ( i < npart)\n\
      #switch (Data[i][0])\n\
        #case (1.0)\n\
        sphere{\n\
          <Data[i][1],Data[i][2],Data[i][3]>\n\
          Data[i][10]/2.0\n\
          texture {\n\
            pigment { color red Data[i][4] green Data[i][5] blue Data[i][6] }\n\
          }\n\
          no_shadow\n\
          no_reflection\n\
        }\n\
      #end\n\
      #declare i=i+1;\n\
    #end\n\n");
  fclose(f);
}
