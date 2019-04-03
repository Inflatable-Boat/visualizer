/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

/*  Number of vertices for polygon!!! */
// Cube -> 8
//#define Nvert 8
// TruncatedOctahedron -> 24
#define Ncub 8
#define Ntoct 25
#define Nprim6 12

float vcub[Ncub][3];
float vdcub[Ncub][3];
float vtoct[Ntoct][3];
float vprism6[Nprim6][3];


#define M_PI 3.14159265
#define PROGRAM_TITLE "Visual Colloids (opengl)"
#define MY_CRYSTAL_LIST 1
#define MY_SPHERE_LIST 3
#define MY_SPHERE_LIST_LR 4
#define MY_CRYSTAL_LIST_TRANS 2


#ifdef WIN32
#include <windows.h>		// Header File For Windows
#endif
#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library
#include <GL/glut.h>

//#include <GL/glext.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#include <glob.h>
#endif
#include <time.h>    // For our FPS stats.
#include <math.h>    // Math is good!
#include <png.h>     // For our screen captures


#include "defs.h"

// Some global variables.

// Window and texture IDs, window width and height.
int Texture_ID;
int Window_ID;
int Window_Width = 1200;
int Window_Height = 800;



// Our display mode settings.
//int cull = 1;
int clearcolor=1;
int boundingbox=1;
int ortho=1;
int fps=1;
int mouse_shift=0;


//int Curr_TexMode = 0;
//char *TexModesStr[] = {"GL_DECAL","GL_MODULATE","GL_BLEND","GL_REPLACE"};
//GLint TexModes[] = {GL_DECAL,GL_MODULATE,GL_BLEND,GL_REPLACE};

// Object and scene global variables.
GLUquadricObj *quadObj; //Used for the cylinders

// Cube position and rotation speed variables.
GLfloat id_matrix[16]={1.0,0.0,0.0,0.0,
                     0.0,1.0,0.0,0.0,
                     0.0,0.0,1.0,0.0,
                     0.0,0.0,0.0,1.0};

GLfloat rot_matrix[16]={1.0,0.0,0.0, 0.0,
                      0.0,1.0,0.0, 0.0,
                      0.0,0.0,1.0, 0.0,
                      0.0,0.0,0.0, 1.0};

GLfloat rot_matrix_old[16];

GLfloat X_Speed = 0.0f;
GLfloat Y_Speed = 0.0f;
GLfloat Z_Speed = 0.0f;
GLfloat X_Off   = 0.0f;
GLfloat Y_Off   = 0.0f;
GLfloat Z_Off   =-5.0f;
GLfloat step = 0.05;
GLfloat cut_dist=0;
GLfloat scale=1.0;
GLfloat fov=45.0f;

int redraw=1;
int rebuild=1;

int detail=10;
int n_part;
int filter_part;
//GLfloat x_box_a,x_box_b;
//GLfloat y_box_a,y_box_b;
//GLfloat z_box_a,z_box_b;
float bb[8][3];

int mouse1_pressed=0;
int mouse2_pressed=0;
int mouse1_px,mouse1_py;
int mouse2_px,mouse2_py;

double volume_ruud; // hacked in by Ruud, this is done because of the way I visualize clusters. initialized in readdata.c

//Stuf for the selecting
#define BUFSIZE 2048
GLuint selectBuf[BUFSIZE];

// Settings for our light.  Try playing with these (or add more lights).
int lighton=12;
GLfloat Light_Ambient[]=  { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat Light_Diffuse[]=  { 1.4f, 1.4f, 1.4f, 1.0f };
GLfloat Light_Position[]= { 0.0f, 0.0f, 4.0f, 1.0f };
GLfloat Light_Specular[]= { 1.7f, 1.7f, 1.7f, 1.0f };

typedef struct {
  int soort; GLfloat x; GLfloat y; GLfloat z;
  GLfloat nx; GLfloat ny; GLfloat nz;
  GLfloat r; GLfloat g; GLfloat b;
  GLfloat matrix[16];
  GLfloat size1;GLfloat size2;GLfloat size3;
  int visible;
  int selected; GLfloat alpha;
  int lowres;
  GLfloat angle;
} tPart;

tPart *particles;

//typedef struct {
//}tBox

char *filename="coords_opengl.dat";
char datakind='s';
glob_t globbuf;
int globcur=0;
int cmdl_n=0;
char **cmdl_s;

#include "readdata.c"

// ------
// Frames per second (FPS) statistic variables and routine.

#define FRAME_RATE_SAMPLES 50
int FrameCount=0;
GLfloat FrameRate=0;

static void ourDoFPS() {
   static clock_t last=0;
   clock_t now;
   GLfloat delta;

   if (++FrameCount >= FRAME_RATE_SAMPLES) {
      now  = clock();
      delta= (now - last) / (GLfloat) CLOCKS_PER_SEC;
      last = now;

      FrameRate = FRAME_RATE_SAMPLES / delta;
      FrameCount = 0;
  }
}


// ------
// String rendering routine; leverages on GLUT routine.

static void ourPrintString(
   void *font,
   char *str
)
{
   int i,l=strlen(str);

   for(i=0;i<l;i++)
      glutBitmapCharacter(font,*str++);
}

void draw_bounding_box (){
  glColor4f(0.2,0.2,0.2,1.0);     // Color of the box
  glBegin(GL_LINES);

  GLfloat normal[3];  //to fix the normal vector of the lines
  int i;
  for(i=0;i<3;i++) normal[i]=rot_matrix[4*i+2];
  glNormal3fv(normal);

  glVertex3fv(bb[0]); glVertex3fv(bb[1]);
  glVertex3fv(bb[1]); glVertex3fv(bb[2]);
  glVertex3fv(bb[2]); glVertex3fv(bb[3]);
  glVertex3fv(bb[3]); glVertex3fv(bb[0]);

  glVertex3fv(bb[4]); glVertex3fv(bb[5]);
  glVertex3fv(bb[5]); glVertex3fv(bb[6]);
  glVertex3fv(bb[6]); glVertex3fv(bb[7]);
  glVertex3fv(bb[7]); glVertex3fv(bb[4]);

  glVertex3fv(bb[0]); glVertex3fv(bb[4]);
  glVertex3fv(bb[1]); glVertex3fv(bb[5]);
  glVertex3fv(bb[2]); glVertex3fv(bb[6]);
  glVertex3fv(bb[3]); glVertex3fv(bb[7]);

  glEnd();
}

void draw_sphere_list(GLfloat x, GLfloat y, GLfloat z, GLfloat d,int lr){
  glPushMatrix();
  glTranslatef(x, y, z);
  glScalef(d,d,d);
  if(lr)
    glCallList(MY_SPHERE_LIST_LR);
  else
    glCallList(MY_SPHERE_LIST);
  glPopMatrix();
}

void draw_sphere(GLfloat x, GLfloat y, GLfloat z, GLfloat d){
  glPushMatrix();
  glTranslatef(x, y, z);
  glutSolidSphere(d/2.0, detail, detail);
  glPopMatrix();
}

void draw_cylinder2(GLfloat x, GLfloat y, GLfloat z, GLfloat d ,GLfloat l, GLfloat xn,GLfloat yn,GLfloat zn){
   GLfloat xrot,yrot;
   yrot = acos(zn);
   if (xn*xn+yn*yn==0) xrot=0; else if (0<xn) xrot=asin(yn/sqrt(xn*xn+yn*yn)); else
     xrot=M_PI-asin(yn/sqrt(xn*xn+yn*yn));
   xrot=xrot*180.0f/M_PI;
   yrot=yrot*180.0f/M_PI;
   if(!finitef(xrot)) xrot=M_PI/2.0f;
   if(!finitef(yrot)) yrot=M_PI/2.0f;

   glPushMatrix();
   glTranslatef(x,y,z);
   glRotatef(xrot,0.0f,0.0f,1.0f);
   glRotatef(yrot,0.0f,1.0f,0.0f);
   glTranslatef(0.0f,0.0f,-l/2.0f);

   double clip_plane1[]={0.0f,0.0f,-1.0f,0.0f};
   double clip_plane2[]={0.0f,0.0f,1.0f,0.0f};
   glClipPlane(GL_CLIP_PLANE1,clip_plane1);
   glEnable(GL_CLIP_PLANE1);

   glutSolidSphere(d/2.0,detail,detail);

   glDisable(GL_CLIP_PLANE1);

 //  glRotatef(-90,0.0f,0.0f,1.0f);
   gluCylinder(quadObj,d/2.0f,d/2.0f,l,detail,1);
 //  glRotatef(90,0.0f,0.0f,1.0f);
   glTranslatef(0.0f,0.0f,l);

   glClipPlane(GL_CLIP_PLANE1,clip_plane2);
   glEnable(GL_CLIP_PLANE1);

   glutSolidSphere(d/2.0f,detail,detail);

   glDisable(GL_CLIP_PLANE1);
   glPopMatrix();
}



/**************************************************************
 * **                        DRAW_CUBE
 * ***************************************************************/
void draw_cube(GLfloat x, GLfloat y, GLfloat z, GLfloat mat[16], GLfloat s1)
{
   glPushMatrix();
   glTranslatef(x,y,z);
   glMultMatrixf(mat);
   glutSolidCube(s1);
   glPopMatrix();
}

void draw_poly_prim6(GLfloat x, GLfloat y, GLfloat z, GLfloat mat[16], GLfloat s)
{
  int i;
  float vert2[Nprim6][3];

  for(i=0;i<Nprim6;i++)
   {
     vert2[i][0]=vprism6[i][0]*s;
     vert2[i][1]=vprism6[i][1]*s;
     vert2[i][2]=vprism6[i][2]*s;

   }

	glPushMatrix();
	glTranslatef(x,y,z);
	glMultMatrixf(mat);



	glBegin(GL_POLYGON);
	glNormal3f(0.,0.,-1.);
	glVertex3f(vert2[6][0],vert2[6][1],vert2[6][2]);
	glVertex3f(vert2[2][0],vert2[2][1],vert2[2][2]);
	glVertex3f(vert2[0][0],vert2[0][1],vert2[0][2]);
	glVertex3f(vert2[4][0],vert2[4][1],vert2[4][2]);
	glVertex3f(vert2[8][0],vert2[8][1],vert2[8][2]);
	glVertex3f(vert2[10][0],vert2[10][1],vert2[10][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.,0.,1.);
	glVertex3f(vert2[11][0],vert2[11][1],vert2[11][2]);
	glVertex3f(vert2[9][0],vert2[9][1],vert2[9][2]);
	glVertex3f(vert2[5][0],vert2[5][1],vert2[5][2]);
	glVertex3f(vert2[1][0],vert2[1][1],vert2[1][2]);
	glVertex3f(vert2[3][0],vert2[3][1],vert2[3][2]);
	glVertex3f(vert2[7][0],vert2[7][1],vert2[7][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.866025,0.5,0.);
	glVertex3f(vert2[10][0],vert2[10][1],vert2[10][2]);
	glVertex3f(vert2[8][0],vert2[8][1],vert2[8][2]);
	glVertex3f(vert2[9][0],vert2[9][1],vert2[9][2]);
	glVertex3f(vert2[11][0],vert2[11][1],vert2[11][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.,1.,0.);
	glVertex3f(vert2[8][0],vert2[8][1],vert2[8][2]);
	glVertex3f(vert2[4][0],vert2[4][1],vert2[4][2]);
	glVertex3f(vert2[5][0],vert2[5][1],vert2[5][2]);
	glVertex3f(vert2[9][0],vert2[9][1],vert2[9][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(-0.866025,0.5,0.);
	glVertex3f(vert2[4][0],vert2[4][1],vert2[4][2]);
	glVertex3f(vert2[0][0],vert2[0][1],vert2[0][2]);
	glVertex3f(vert2[1][0],vert2[1][1],vert2[1][2]);
	glVertex3f(vert2[5][0],vert2[5][1],vert2[5][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(-0.866025,-0.5,0.);
	glVertex3f(vert2[0][0],vert2[0][1],vert2[0][2]);
	glVertex3f(vert2[2][0],vert2[2][1],vert2[2][2]);
	glVertex3f(vert2[3][0],vert2[3][1],vert2[3][2]);
	glVertex3f(vert2[1][0],vert2[1][1],vert2[1][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.,-1.,0.);
	glVertex3f(vert2[2][0],vert2[2][1],vert2[2][2]);
	glVertex3f(vert2[6][0],vert2[6][1],vert2[6][2]);
	glVertex3f(vert2[7][0],vert2[7][1],vert2[7][2]);
	glVertex3f(vert2[3][0],vert2[3][1],vert2[3][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.866025,-0.5,0.);
	glVertex3f(vert2[6][0],vert2[6][1],vert2[6][2]);
	glVertex3f(vert2[10][0],vert2[10][1],vert2[10][2]);
	glVertex3f(vert2[11][0],vert2[11][1],vert2[11][2]);
	glVertex3f(vert2[7][0],vert2[7][1],vert2[7][2]);
	glEnd();

	glPopMatrix();


}



void draw_poly_toct(GLfloat x, GLfloat y, GLfloat z, GLfloat mat[16], GLfloat s)
 {
   int i;
   float vert2[Ntoct][3];

  for(i=1;i<Ntoct;i++)
   {
     vert2[i][0]=vtoct[i][0]*s;
     vert2[i][1]=vtoct[i][1]*s;
     vert2[i][2]=vtoct[i][2]*s;
 //    printf ("%d) %f\n", i, dot_product(vtoct[i]));
   }

	glPushMatrix();
	glTranslatef(x,y,z);
	glMultMatrixf(mat);

	glBegin(GL_POLYGON);
	glNormal3f(0.,0.,1.);
	glVertex3f(vert2[17][0],vert2[17][1],vert2[17][2]);
	glVertex3f(vert2[11][0],vert2[11][1],vert2[11][2]);
	glVertex3f(vert2[9][0],vert2[9][1],vert2[9][2]);
	glVertex3f(vert2[15][0],vert2[15][1],vert2[15][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.,0.,-1.);
	glVertex3f(vert2[14][0],vert2[14][1],vert2[14][2]);
	glVertex3f(vert2[8][0],vert2[8][1],vert2[8][2]);
	glVertex3f(vert2[10][0],vert2[10][1],vert2[10][2]);
	glVertex3f(vert2[16][0],vert2[16][1],vert2[16][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.7071067811865475,0.7071067811865475,0.);
	glVertex3f(vert2[22][0],vert2[22][1],vert2[22][2]);
	glVertex3f(vert2[24][0],vert2[24][1],vert2[24][2]);
	glVertex3f(vert2[21][0],vert2[21][1],vert2[21][2]);
	glVertex3f(vert2[18][0],vert2[18][1],vert2[18][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(-0.7071067811865475,0.7071067811865475,0.);
	glVertex3f(vert2[12][0],vert2[12][1],vert2[12][2]);
	glVertex3f(vert2[5][0],vert2[5][1],vert2[5][2]);
	glVertex3f(vert2[2][0],vert2[2][1],vert2[2][2]);
	glVertex3f(vert2[6][0],vert2[6][1],vert2[6][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.7071067811865475,-0.7071067811865475,0.);
	glVertex3f(vert2[13][0],vert2[13][1],vert2[13][2]);
	glVertex3f(vert2[19][0],vert2[19][1],vert2[19][2]);
	glVertex3f(vert2[23][0],vert2[23][1],vert2[23][2]);
	glVertex3f(vert2[20][0],vert2[20][1],vert2[20][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(-0.7071067811865475,-0.7071067811865475,0.);
	glVertex3f(vert2[4][0],vert2[4][1],vert2[4][2]);
	glVertex3f(vert2[1][0],vert2[1][1],vert2[1][2]);
	glVertex3f(vert2[3][0],vert2[3][1],vert2[3][2]);
	glVertex3f(vert2[7][0],vert2[7][1],vert2[7][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.,-0.816496580927726,-0.5773502691896258);
	glVertex3f(vert2[19][0],vert2[19][1],vert2[19][2]);
	glVertex3f(vert2[13][0],vert2[13][1],vert2[13][2]);
	glVertex3f(vert2[7][0],vert2[7][1],vert2[7][2]);
	glVertex3f(vert2[3][0],vert2[3][1],vert2[3][2]);
	glVertex3f(vert2[8][0],vert2[8][1],vert2[8][2]);
	glVertex3f(vert2[14][0],vert2[14][1],vert2[14][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.,-0.816496580927726,0.5773502691896256);
	glVertex3f(vert2[15][0],vert2[15][1],vert2[15][2]);
	glVertex3f(vert2[9][0],vert2[9][1],vert2[9][2]);
	glVertex3f(vert2[4][0],vert2[4][1],vert2[4][2]);
	glVertex3f(vert2[7][0],vert2[7][1],vert2[7][2]);
	glVertex3f(vert2[13][0],vert2[13][1],vert2[13][2]);
	glVertex3f(vert2[20][0],vert2[20][1],vert2[20][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.,0.816496580927726,-0.5773502691896256);
	glVertex3f(vert2[16][0],vert2[16][1],vert2[16][2]);
	glVertex3f(vert2[10][0],vert2[10][1],vert2[10][2]);
	glVertex3f(vert2[5][0],vert2[5][1],vert2[5][2]);
	glVertex3f(vert2[12][0],vert2[12][1],vert2[12][2]);
	glVertex3f(vert2[18][0],vert2[18][1],vert2[18][2]);
	glVertex3f(vert2[21][0],vert2[21][1],vert2[21][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.,0.816496580927726,0.5773502691896258);
	glVertex3f(vert2[22][0],vert2[22][1],vert2[22][2]);
	glVertex3f(vert2[18][0],vert2[18][1],vert2[18][2]);
	glVertex3f(vert2[12][0],vert2[12][1],vert2[12][2]);
	glVertex3f(vert2[6][0],vert2[6][1],vert2[6][2]);
	glVertex3f(vert2[11][0],vert2[11][1],vert2[11][2]);
	glVertex3f(vert2[17][0],vert2[17][1],vert2[17][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.816496580927726,0.,0.5773502691896258);
	glVertex3f(vert2[20][0],vert2[20][1],vert2[20][2]);
	glVertex3f(vert2[23][0],vert2[23][1],vert2[23][2]);
	glVertex3f(vert2[24][0],vert2[24][1],vert2[24][2]);
	glVertex3f(vert2[22][0],vert2[22][1],vert2[22][2]);
	glVertex3f(vert2[17][0],vert2[17][1],vert2[17][2]);
	glVertex3f(vert2[15][0],vert2[15][1],vert2[15][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.816496580927726,0.,-0.5773502691896256);
	glVertex3f(vert2[14][0],vert2[14][1],vert2[14][2]);
	glVertex3f(vert2[16][0],vert2[16][1],vert2[16][2]);
	glVertex3f(vert2[21][0],vert2[21][1],vert2[21][2]);
	glVertex3f(vert2[24][0],vert2[24][1],vert2[24][2]);
	glVertex3f(vert2[23][0],vert2[23][1],vert2[23][2]);
	glVertex3f(vert2[19][0],vert2[19][1],vert2[19][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(-0.816496580927726,0.,0.5773502691896256);
	glVertex3f(vert2[9][0],vert2[9][1],vert2[9][2]);
	glVertex3f(vert2[11][0],vert2[11][1],vert2[11][2]);
	glVertex3f(vert2[6][0],vert2[6][1],vert2[6][2]);
	glVertex3f(vert2[2][0],vert2[2][1],vert2[2][2]);
	glVertex3f(vert2[1][0],vert2[1][1],vert2[1][2]);
	glVertex3f(vert2[4][0],vert2[4][1],vert2[4][2]);
	glEnd();

	glBegin(GL_POLYGON);

	glNormal3f(-0.816497, 0., -0.57735);
	glVertex3f(vert2[3][0],vert2[3][1],vert2[3][2]);
	glVertex3f(vert2[1][0],vert2[1][1],vert2[1][2]);
	glVertex3f(vert2[2][0],vert2[2][1],vert2[2][2]);
	glVertex3f(vert2[5][0],vert2[5][1],vert2[5][2]);
	glVertex3f(vert2[10][0],vert2[10][1],vert2[10][2]);
	glVertex3f(vert2[8][0],vert2[8][1],vert2[8][2]);
	glEnd();




	glPopMatrix();


}

void draw_poly_cube(GLfloat x, GLfloat y, GLfloat z, GLfloat mat[16], GLfloat s)
{
	int i;
	float vert2[Ncub][3];

	// printf("%f %f %f", x,y,z);
	// s*=2;

	for(i=0;i<Ncub;i++)
	{
	  vert2[i][0]=vcub[i][0]*s;
	  vert2[i][1]=vcub[i][1]*s;
	  vert2[i][2]=vcub[i][2]*s;
	}


	glPushMatrix();
	glTranslatef(x,y,z);
	glMultMatrixf(mat);

	glBegin(GL_POLYGON);
	glNormal3f(0.0f,0.0f,1.0f);
	glVertex3f(vert2[7][0],vert2[7][1],vert2[7][2]);
	glVertex3f(vert2[3][0],vert2[3][1],vert2[3][2]);
	glVertex3f(vert2[1][0],vert2[1][1],vert2[1][2]);
	glVertex3f(vert2[5][0],vert2[5][1],vert2[5][2]);
	glEnd();


	glBegin(GL_POLYGON);
	glNormal3f(1.0f,0.0f,0.0f);
	glVertex3f(vert2[7][0],vert2[7][1],vert2[7][2]);
	glVertex3f(vert2[5][0],vert2[5][1],vert2[5][2]);
	glVertex3f(vert2[4][0],vert2[4][1],vert2[4][2]);
	glVertex3f(vert2[6][0],vert2[6][1],vert2[6][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f(vert2[7][0],vert2[7][1],vert2[7][2]);
	glVertex3f(vert2[6][0],vert2[6][1],vert2[6][2]);
	glVertex3f(vert2[2][0],vert2[2][1],vert2[2][2]);
	glVertex3f(vert2[3][0],vert2[3][1],vert2[3][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(-1.0f,0.0f,0.0f);
	glVertex3f(vert2[3][0],vert2[3][1],vert2[3][2]);
	glVertex3f(vert2[2][0],vert2[2][1],vert2[2][2]);
	glVertex3f(vert2[0][0],vert2[0][1],vert2[0][2]);
	glVertex3f(vert2[1][0],vert2[1][1],vert2[1][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0f,0.0f,-1.0f);
	glVertex3f(vert2[0][0],vert2[0][1],vert2[0][2]);
	glVertex3f(vert2[2][0],vert2[2][1],vert2[2][2]);
	glVertex3f(vert2[6][0],vert2[6][1],vert2[6][2]);
	glVertex3f(vert2[4][0],vert2[4][1],vert2[4][2]);
	glEnd();

	glBegin(GL_POLYGON);
	glNormal3f(0.0f,-1.0f,0.0f);
	glVertex3f(vert2[1][0],vert2[1][1],vert2[1][2]);
	glVertex3f(vert2[0][0],vert2[0][1],vert2[0][2]);
	glVertex3f(vert2[4][0],vert2[4][1],vert2[4][2]);
	glVertex3f(vert2[5][0],vert2[5][1],vert2[5][2]);
	glEnd();

	glPopMatrix();


}


void draw_poly_discube(GLfloat x, GLfloat y, GLfloat z, GLfloat mat[16], GLfloat s)
{
	int i;
	float vert2[Ncub][3];

	// printf("%f %f %f", x,y,z);
	// s*=2;

	for(i=0;i<Ncub;i++)
	 {
	   vert2[i][0]=vdcub[i][0]*s;
	   vert2[i][1]=vdcub[i][1]*s;
	   vert2[i][2]=vdcub[i][2]*s;
	   // printf("Test %d: %lf, %lf, %lf\n", i, vert2[i][0], vert2[i][1], vert2[i][2]);
	 }


		glPushMatrix();
		glTranslatef(x,y,z);
		glMultMatrixf(mat);

		glBegin(GL_POLYGON);
		glNormal3f(0.0f,0.0f,1.0f);
		glVertex3f(vert2[7][0],vert2[7][1],vert2[7][2]);
		glVertex3f(vert2[3][0],vert2[3][1],vert2[3][2]);
		glVertex3f(vert2[1][0],vert2[1][1],vert2[1][2]);
		glVertex3f(vert2[5][0],vert2[5][1],vert2[5][2]);
		glEnd();


		glBegin(GL_POLYGON);
		glNormal3f(1.0f,0.0f,0.0f);
		glVertex3f(vert2[7][0],vert2[7][1],vert2[7][2]);
		glVertex3f(vert2[5][0],vert2[5][1],vert2[5][2]);
		glVertex3f(vert2[4][0],vert2[4][1],vert2[4][2]);
		glVertex3f(vert2[6][0],vert2[6][1],vert2[6][2]);
		glEnd();

		glBegin(GL_POLYGON);
		glNormal3f(0.0f,1.0f,0.0f);
		glVertex3f(vert2[7][0],vert2[7][1],vert2[7][2]);
		glVertex3f(vert2[6][0],vert2[6][1],vert2[6][2]);
		glVertex3f(vert2[2][0],vert2[2][1],vert2[2][2]);
		glVertex3f(vert2[3][0],vert2[3][1],vert2[3][2]);
		glEnd();

		glBegin(GL_POLYGON);
		glNormal3f(-1.0f,0.0f,0.0f);
		glVertex3f(vert2[3][0],vert2[3][1],vert2[3][2]);
		glVertex3f(vert2[2][0],vert2[2][1],vert2[2][2]);
		glVertex3f(vert2[0][0],vert2[0][1],vert2[0][2]);
		glVertex3f(vert2[1][0],vert2[1][1],vert2[1][2]);
		glEnd();

		glBegin(GL_POLYGON);
		glNormal3f(0.0f,0.0f,-1.0f);
		glVertex3f(vert2[0][0],vert2[0][1],vert2[0][2]);
		glVertex3f(vert2[2][0],vert2[2][1],vert2[2][2]);
		glVertex3f(vert2[6][0],vert2[6][1],vert2[6][2]);
		glVertex3f(vert2[4][0],vert2[4][1],vert2[4][2]);
		glEnd();

		glBegin(GL_POLYGON);
		glNormal3f(0.0f,-1.0f,0.0f);
		glVertex3f(vert2[1][0],vert2[1][1],vert2[1][2]);
		glVertex3f(vert2[0][0],vert2[0][1],vert2[0][2]);
		glVertex3f(vert2[4][0],vert2[4][1],vert2[4][2]);
		glVertex3f(vert2[5][0],vert2[5][1],vert2[5][2]);
		glEnd();

		glPopMatrix();


}






void draw_disk2(GLfloat x, GLfloat y, GLfloat z, GLfloat d ,GLfloat l, GLfloat xn, GLfloat yn, GLfloat zn){
   GLfloat xrot,yrot;
   yrot = acos(zn);
   if (xn*xn+yn*yn==0) xrot=0; else if (0<xn) xrot=asin(yn/sqrt(xn*xn+yn*yn)); else
     xrot=M_PI-asin(yn/sqrt(xn*xn+yn*yn));
   xrot=xrot*180/M_PI;
   yrot=yrot*180/M_PI;

   glPushMatrix();
   glTranslatef(x,y,z);
   glRotatef(xrot,0.0f,0.0f,1.0f);
   glRotatef(yrot,0.0f,1.0f,0.0f);
   glTranslatef(0,0,-l/2.0);
   glRotatef(180,0.0f,1.0f,0.0f);
   gluDisk(quadObj,0,d/2.0,detail,1);
   glRotatef(180,0.0f,1.0f,0.0f);
   gluCylinder(quadObj,d/2.0,d/2.0,l,detail,1);
   glTranslatef(0,0,l);
   gluDisk(quadObj,0,d/2.0,detail,1);
   glPopMatrix();
}

void draw_line(GLfloat x,GLfloat y,GLfloat z, GLfloat l, GLfloat xn, GLfloat yn, GLfloat zn){
  glBegin(GL_LINES);
  glVertex3f(x+xn*l/2.0,y+yn*l/2.0,z+zn*l/2.0);
  glVertex3f(x-xn*l/2.0,y-yn*l/2.0,z-zn*l/2.0);
  glEnd();
}

void draw_brick_old(GLfloat x,GLfloat y,GLfloat z, GLfloat dx,GLfloat dy,GLfloat dz){
  glPushMatrix();
  glTranslatef(x-dx*0.5,y-dy*0.5,z-dz*0.5);

  glBegin(GL_QUADS);          // Start Drawing The Cube

  glNormal3f(0.0f,1.0f,0.0f);
  glVertex3f( dx  , dy  ,-0.0f);      // Top Right Of The Quad (Top)
  glVertex3f(-0.0f, dy  ,-0.0f);      // Top Left Of The Quad (Top)
  glVertex3f(-0.0f, dy  , dz  );      // Bottom Left Of The Quad (Top)
  glVertex3f( dx  , dy  , dz  );      // Bottom Right Of The Quad (Top)

  glNormal3f(0.0f,-1.0f,0.0f);
  glVertex3f( dx  ,-0.0f, dz  );      // Top Right Of The Quad (Bottom)
  glVertex3f(-0.0f,-0.0f, dz  );      // Top Left Of The Quad (Bottom)
  glVertex3f(-0.0f,-0.0f,-0.0f);      // Bottom Left Of The Quad (Bottom)
  glVertex3f( dx  ,-0.0f,-0.0f);      // Bottom Right Of The Quad (Bottom)

  glNormal3f(0.0f,0.0f,1.0f);
  glVertex3f( dx  , dy  , dz  );      // Top Right Of The Quad (Front)
  glVertex3f(-0.0f, dy  , dz  );      // Top Left Of The Quad (Front)
  glVertex3f(-0.0f,-0.0f, dz  );      // Bottom Left Of The Quad (Front)
  glVertex3f( dx  ,-0.0f, dz  );      // Bottom Right Of The Quad (Front)

  glNormal3f(0.0f,0.0f,-1.0f);
  glVertex3f( dx  ,-0.0f,-0.0f);      // Bottom Left Of The Quad (Back)
  glVertex3f(-0.0f,-0.0f,-0.0f);      // Bottom Right Of The Quad (Back)
  glVertex3f(-0.0f, dy  ,-0.0f);      // Top Right Of The Quad (Back)
  glVertex3f( dx  , dy  ,-0.0f);      // Top Left Of The Quad (Back)

  glNormal3f(-1.0f,0.0f,0.0f);
  glVertex3f(-0.0f, dy  , dz  );      // Top Right Of The Quad (Left)
  glVertex3f(-0.0f, dy  ,-0.0f);      // Top Left Of The Quad (Left)
  glVertex3f(-0.0f,-0.0f,-0.0f);      // Bottom Left Of The Quad (Left)
  glVertex3f(-0.0f,-0.0f, dz  );      // Bottom Right Of The Quad (Left)

  glNormal3f(1.0f,0.0f,0.0f);
  glVertex3f( dx  , dy  ,-0.0f);      // Top Right Of The Quad (Right)
  glVertex3f( dx  , dy  , dz  );      // Top Left Of The Quad (Right)
  glVertex3f( dx  ,-0.0f, dz  );      // Bottom Left Of The Quad (Right)
  glVertex3f( dx  ,-0.0f,-0.0f);

  glEnd();
  glPopMatrix();
}

void draw_brick(GLfloat x,GLfloat y,GLfloat z, GLfloat nx, GLfloat ny, GLfloat nz,GLfloat dx,GLfloat dy,GLfloat dz){
  glPushMatrix();
  glTranslatef(x,y,z);

  glRotatef(nx,0.0f,0.0f,1.0f); //Z
  glRotatef(ny,1.0f,0.0f,0.0f); //X There does not seem to be a convention :-(
  glRotatef(nz,0.0f,0.0f,1.0f); //Z

  glBegin(GL_QUADS);          // Start Drawing The Cube
  dx*=0.5;
  dy*=0.5;
  dz*=0.5;

  glNormal3f(0.0f,1.0f,0.0f);
  glVertex3f( dx  , dy  ,-dz  );      // Top Right Of The Quad (Top)
  glVertex3f(-dx  , dy  ,-dz  );      // Top Left Of The Quad (Top)
  glVertex3f(-dx  , dy  , dz  );      // Bottom Left Of The Quad (Top)
  glVertex3f( dx  , dy  , dz  );      // Bottom Right Of The Quad (Top)

  glNormal3f(0.0f,-1.0f,0.0f);
  glVertex3f( dx  ,-dy  , dz  );      // Top Right Of The Quad (Bottom)
  glVertex3f(-dx  ,-dy  , dz  );      // Top Left Of The Quad (Bottom)
  glVertex3f(-dx  ,-dy  ,-dz  );      // Bottom Left Of The Quad (Bottom)
  glVertex3f( dx  ,-dy  ,-dz  );      // Bottom Right Of The Quad (Bottom)

  glNormal3f(0.0f,0.0f,1.0f);
  glVertex3f( dx  , dy  , dz  );      // Top Right Of The Quad (Front)
  glVertex3f(-dx  , dy  , dz  );      // Top Left Of The Quad (Front)
  glVertex3f(-dx  ,-dy  , dz  );      // Bottom Left Of The Quad (Front)
  glVertex3f( dx  ,-dy  , dz  );      // Bottom Right Of The Quad (Front)

  glNormal3f(0.0f,0.0f,-1.0f);
  glVertex3f( dx  ,-dy  ,-dz);      // Bottom Left Of The Quad (Back)
  glVertex3f(-dx  ,-dy  ,-dz);      // Bottom Right Of The Quad (Back)
  glVertex3f(-dx  , dy  ,-dz);      // Top Right Of The Quad (Back)
  glVertex3f( dx  , dy  ,-dz);      // Top Left Of The Quad (Back)

  glNormal3f(-1.0f,0.0f,0.0f);
  glVertex3f(-dx, dy  , dz  );      // Top Right Of The Quad (Left)
  glVertex3f(-dx, dy  ,-dz  );      // Top Left Of The Quad (Left)
  glVertex3f(-dx,-dy  ,-dz  );      // Bottom Left Of The Quad (Left)
  glVertex3f(-dx,-dy  , dz  );      // Bottom Right Of The Quad (Left)

  glNormal3f(1.0f,0.0f,0.0f);
  glVertex3f( dx  , dy  ,-dz  );      // Top Right Of The Quad (Right)
  glVertex3f( dx  , dy  , dz  );      // Top Left Of The Quad (Right)
  glVertex3f( dx  ,-dy  , dz  );      // Bottom Left Of The Quad (Right)
  glVertex3f( dx  ,-dy  ,-dz  );

  glEnd();
  glPopMatrix();
}

void draw_particle(int i){
	glLoadName(i);
	if(particles[i].visible){
	  // int x,y,z;

	  double ax[3],ay[3],az[3];

	  VSET(ax,(bb[1][0]-bb[0][0]), (bb[1][1]-bb[0][1]), (bb[1][2]-bb[0][2]));
	  VSET(ay,(bb[3][0]-bb[0][0]), (bb[3][1]-bb[0][1]), (bb[3][2]-bb[0][2]));
	  VSET(az,(bb[4][0]-bb[0][0]), (bb[4][1]-bb[0][1]), (bb[4][2]-bb[0][2]));

	  glPushMatrix();

	  // glTranslatef(perwrap[0],perwrap[1],perwrap[2]);

	  glTranslatef(ax[0],ax[1],ax[2]);
	  glTranslatef(ay[0],ay[1],ay[2]);
	  glTranslatef(az[0],az[1],az[2]);

      if(particles[i].selected) glColor4f((1.0-particles[i].r),(1.0-particles[i].g),(1.0-particles[i].b),particles[i].alpha);
      else glColor4f(particles[i].r,particles[i].g,particles[i].b,particles[i].alpha);
      switch(particles[i].soort) {
        case 1:
          draw_sphere_list(particles[i].x,particles[i].y,particles[i].z,particles[i].size1*scale,particles[i].lowres);
          // draw_sphere(particles[i].x,particles[i].y,particles[i].z,particles[i].size1*scale);
          break;
        case 2:
          draw_disk2(particles[i].x,particles[i].y,particles[i].z,
              particles[i].size1*scale,particles[i].size2,particles[i].nx,particles[i].ny,particles[i].nz);
          break;
        case 3:
          draw_cylinder2(particles[i].x,particles[i].y,particles[i].z,
              particles[i].size1*scale,particles[i].size2*scale,particles[i].nx,particles[i].ny,particles[i].nz);
          break;
        case 4:
          draw_line(particles[i].x,particles[i].y,particles[i].z,
              particles[i].size2*scale,particles[i].nx,particles[i].ny,particles[i].nz);
          break;
        case 5:
          draw_brick(particles[i].x,particles[i].y,particles[i].z,
              particles[i].nx,particles[i].ny,particles[i].nz,
              particles[i].size1*scale,particles[i].size2*scale,particles[i].size3*scale);
          break;
         case 6:
          draw_cube(particles[i].x,particles[i].y,particles[i].z,
              particles[i].matrix,
              particles[i].size1*scale);
          break;
         case 7:
          draw_poly_cube(particles[i].x,particles[i].y,particles[i].z, particles[i].matrix,particles[i].size1*scale);
          break;
         case 8:
          draw_poly_toct(particles[i].x,particles[i].y,particles[i].z, particles[i].matrix,particles[i].size1*scale);
          break;
         case 9:
          draw_poly_prim6(particles[i].x,particles[i].y,particles[i].z, particles[i].matrix,particles[i].size1*scale);
          break;
	case 10:
          draw_poly_discube(particles[i].x,particles[i].y,particles[i].z, particles[i].matrix,particles[i].size1*scale);
          break;

      }

      //glPopMatrix();


    //}
  }
}

void build_crystal(){
  glNewList(MY_SPHERE_LIST,GL_COMPILE);
  if(detail*detail*0.5 > 20) {
    glutSolidSphere(0.5, detail, detail*0.5);
  } else if(detail*detail*0.5 > 12){
    glPushMatrix();
    glScalef(0.5,0.5,0.5);
    glutSolidIcosahedron();
    glPopMatrix();
  } else {
    glPushMatrix();
    glScalef(0.5/sqrt(3.0),0.5/sqrt(3.0),0.5/sqrt(3.0));
    glutSolidDodecahedron();
    glPopMatrix();
  }
  glEndList();

  glNewList(MY_SPHERE_LIST_LR,GL_COMPILE);
  if(detail*detail*0.125 > 20) {
    glutSolidSphere(0.5, detail*0.5, detail*0.25);
  } else if(detail*detail*0.125 > 12){
    glPushMatrix();
    glScalef(0.5,0.5,0.5);
    glutSolidIcosahedron();
    glPopMatrix();
  } else {
    glPushMatrix();
    glScalef(0.5/sqrt(3.0),0.5/sqrt(3.0),0.5/sqrt(3.0));
    glutSolidDodecahedron();
    glPopMatrix();
  }
  glEndList();

  glNewList(MY_CRYSTAL_LIST,GL_COMPILE);
  glNormal3f( 0.0f, -1.0f, 0.0f); // Needed for lighting
    int i;
    for(i=0;i<n_part;i++){
      if(particles[i].visible && particles[i].alpha>=1.0){
        draw_particle(i);
      }
    }
  glEndList();
  glNewList(MY_CRYSTAL_LIST_TRANS,GL_COMPILE);
    for(i=0;i<n_part;i++){
      if(particles[i].visible && particles[i].alpha<1.0){
        draw_particle(i);
      }
    }
  glEndList();
}

// ------
// Routine which actually does the drawing

void drawfps(){
   char buf[80]; // For our strings.
   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   glOrtho(0,Window_Width,0,Window_Height,-1.0,1.0);
   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);

   glTranslatef(6.0f,Window_Height - 14,0.0f);
   glColor4f(0.9,0.2,0.2,.75);
   sprintf(buf,"FPS: %f F: %2d", FrameRate, FrameCount);
   glRasterPos2i(6,0);
   ourPrintString(GLUT_BITMAP_HELVETICA_12,buf);

   // Done with this special projection matrix.  Throw it away.
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);

}

double volfrac(void);
int Xparticles(void);

void drawinfo(){
   char buf[255]; // For our strings.
   glLoadIdentity();
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   glOrtho(0,Window_Width,0,Window_Height,-1.0,1.0);
   glDisable(GL_LIGHTING);
   glDisable(GL_DEPTH_TEST);

   glColor4f(0.1,0.1,0.1,.75);
  //  sprintf(buf,"Volume Fraction: %lf", volfrac());
  int Xtalline = Xparticles();
  double prctX = 100.0 * Xtalline / n_part;
   sprintf(buf, "Frame %d / %ld,    filename %s,    volume fraction %5.3lf,     %d/%d Xtalline (%5.2lf%%)",
           globcur, globbuf.gl_pathc-1, globbuf.gl_pathv[globcur], volfrac(), Xtalline, n_part, prctX);
   glRasterPos2i(2,14); ourPrintString(GLUT_BITMAP_HELVETICA_12,buf);
  // sprintf(buf,"X: %f", X_Off);
  // glRasterPos2i(2,50); ourPrintString(GLUT_BITMAP_HELVETICA_12,buf);
  // sprintf(buf,"Y: %f", Y_Off);
  // glRasterPos2i(2,38); ourPrintString(GLUT_BITMAP_HELVETICA_12,buf);
  // sprintf(buf,"Z: %f", Z_Off);
  // glRasterPos2i(2,26); ourPrintString(GLUT_BITMAP_HELVETICA_12,buf);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);

}

void rotate_xy(GLfloat dx, GLfloat dy, GLfloat dz){
  int i;
  for(i=0;i<16;i++) rot_matrix_old[i]=rot_matrix[i];
  rot_matrix[0] = rot_matrix_old[0]*cos(dx)-rot_matrix_old[2]*sin(dx);
  rot_matrix[4] = rot_matrix_old[4]*cos(dx)-rot_matrix_old[6]*sin(dx);
  rot_matrix[8] = rot_matrix_old[8]*cos(dx)-rot_matrix_old[10]*sin(dx);
  rot_matrix[2] = rot_matrix_old[2]*cos(dx)+rot_matrix_old[0]*sin(dx);
  rot_matrix[6] = rot_matrix_old[6]*cos(dx)+rot_matrix_old[4]*sin(dx);
  rot_matrix[10]= rot_matrix_old[10]*cos(dx)+rot_matrix_old[8]*sin(dx);

  for(i=0;i<16;i++) rot_matrix_old[i]=rot_matrix[i];
  rot_matrix[1] = rot_matrix_old[1]*cos(dy)-rot_matrix_old[2]*sin(dy);
  rot_matrix[5] = rot_matrix_old[5]*cos(dy)-rot_matrix_old[6]*sin(dy);
  rot_matrix[9] = rot_matrix_old[9]*cos(dy)-rot_matrix_old[10]*sin(dy);
  rot_matrix[2] = rot_matrix_old[2]*cos(dy)+rot_matrix_old[1]*sin(dy);
  rot_matrix[6] = rot_matrix_old[6]*cos(dy)+rot_matrix_old[5]*sin(dy);
  rot_matrix[10]= rot_matrix_old[10]*cos(dy)+rot_matrix_old[9]*sin(dy);

  for(i=0;i<16;i++) rot_matrix_old[i]=rot_matrix[i];
  rot_matrix[0] = rot_matrix_old[0]*cos(dz)-rot_matrix_old[1]*sin(dz);
  rot_matrix[4] = rot_matrix_old[4]*cos(dz)-rot_matrix_old[5]*sin(dz);
  rot_matrix[8] = rot_matrix_old[8]*cos(dz)-rot_matrix_old[9]*sin(dz);
  rot_matrix[1] = rot_matrix_old[1]*cos(dz)+rot_matrix_old[0]*sin(dz);
  rot_matrix[5] = rot_matrix_old[5]*cos(dz)+rot_matrix_old[4]*sin(dz);
  rot_matrix[9] = rot_matrix_old[9]*cos(dz)+rot_matrix_old[8]*sin(dz);
}

void move_xy(GLfloat dx, GLfloat dy){
  float x,y,z;
  dx*=300.0/Window_Width;
  dy*=300.0/Window_Height;
  x=rot_matrix[0]*dx+rot_matrix[1]*dy;
  y=rot_matrix[4]*dx+rot_matrix[5]*dy;
  z=rot_matrix[8]*dx+rot_matrix[9]*dy;
  int i;
  float h[3][3];
  float h_inv[3][3];
  float a,b,c;

  VSET(h[0],bb[1][0]-bb[0][0], bb[1][1]-bb[0][1], bb[1][2]-bb[0][2]);
  VSET(h[1],bb[3][0]-bb[0][0], bb[3][1]-bb[0][1], bb[3][2]-bb[0][2]);
  VSET(h[2],bb[4][0]-bb[0][0], bb[4][1]-bb[0][1], bb[4][2]-bb[0][2]);

  matrix_inverse(h,h_inv);

  for(i=0;i<n_part;i++){
    particles[i].x-=x;
    particles[i].y-=y;
    particles[i].z-=z;

    //periodic boundary
    a=1+particles[i].x*h_inv[0][0]+particles[i].y*h_inv[1][0]+particles[i].z*h_inv[2][0];
    b=1+particles[i].x*h_inv[0][1]+particles[i].y*h_inv[1][1]+particles[i].z*h_inv[2][1];
    c=1+particles[i].x*h_inv[0][2]+particles[i].y*h_inv[1][2]+particles[i].z*h_inv[2][2];

    if(a>0.5) {
      particles[i].x -= h[0][0];
      particles[i].y -= h[0][1];
      particles[i].z -= h[0][2];
    }
    if(a<-0.5) {
      particles[i].x += h[0][0];
      particles[i].y += h[0][1];
      particles[i].z += h[0][2];
    }

    if(b>0.5) {
      particles[i].x -= h[1][0];
      particles[i].y -= h[1][1];
      particles[i].z -= h[1][2];
    }
    if(b<-0.5) {
      particles[i].x += h[1][0];
      particles[i].y += h[1][1];
      particles[i].z += h[1][2];
    }

    if(c>0.5) {
      particles[i].x -= h[2][0];
      particles[i].y -= h[2][1];
      particles[i].z -= h[2][2];
    }
    if(c<-0.5) {
      particles[i].x += h[2][0];
      particles[i].y += h[2][1];
      particles[i].z += h[2][2];
    }

  }

  rebuild=1;
  redraw=1;
}

void clip_spheres_2(void){
  // Do not cut through spheres; redraws things that have been cut off
  double l,r[3],dist;
  GLdouble plane[4];
  GLfloat m[16];
  int i;
  for(i=0;i<n_part;i++){
    if (particles[i].soort==1){
      glPushMatrix();
      glTranslatef(particles[i].x,particles[i].y,particles[i].z);
      glGetFloatv(GL_MODELVIEW_MATRIX,m);
      glGetClipPlane(GL_CLIP_PLANE3,plane);
      r[0]=-plane[0]*plane[3]-m[12];
      r[1]=-plane[1]*plane[3]-m[13];
      r[2]=-plane[2]*plane[3]-m[14];
      l = sqrt(plane[0]*plane[0]+plane[1]*plane[1]+plane[2]*plane[2]);
      dist=(r[0]*plane[0]+r[1]*plane[1]+r[2]*plane[2])/l;
      glPopMatrix();
      if(fabs(dist) < scale*particles[i].size1/2.0) {
        draw_particle(i);
      }
    }
  }
}

void filter(){
  double a=0;
  int i;
  for(i=0;i<n_part;i++) a+=particles[i].size1;
  a/=n_part;
  for(i=0;i<n_part;i++) if(particles[i].size1 <a) particles[i].visible=0;
  /*for(i=0;i<n_part;i++) if(particles[i].size1 <a) {
    particles[i].r=0.7;
    particles[i].g=0.0;
    particles[i].b=0.0;
  } else {
    particles[i].r=0.0;
    particles[i].g=0.7;
    particles[i].b=0.0;
  }*/
}

void cbRenderScene(void){
  if(redraw){
 // if(1){

   //turn on the depth test
   glDepthFunc(GL_LEQUAL);  glEnable(GL_DEPTH_TEST);

   //Take a nice projection matrix
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if(ortho) {//orthogonal or a perspective projection
     GLfloat tmp=-tan(M_PI*fov/180)*Z_Off/3.0;
     GLfloat asp=(GLfloat)Window_Width/(GLfloat)Window_Height;
     if(asp > 1) glOrtho(-tmp*asp,tmp*asp,-tmp,tmp,-10.0f,300.0f);
            else glOrtho(-tmp,tmp,-tmp/asp,tmp/asp,-10.0f,300.0f);
   } else {
     gluPerspective(fov,(GLfloat)Window_Width/(GLfloat)Window_Height,0.01f,300.0f);
   }
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   //To fix the amount of light when scaling
   glEnable(GL_NORMALIZE);

   //Set correct blending parameters for transparant things
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   //glEnable(GL_BLEND);

   //Setup a clipping plane
   GLdouble plane[]={0.0f,0.0f,-1.0f,-cut_dist};
   glClipPlane(GL_CLIP_PLANE3,plane);
   if (cut_dist >0) glEnable(GL_CLIP_PLANE3); else glDisable(GL_CLIP_PLANE3);

   // Rotate the calculated amount.
   rot_matrix[12]=X_Off;
   rot_matrix[13]=Y_Off;
   rot_matrix[14]=Z_Off;
   glMultMatrixf(rot_matrix);

   // Clear the color and depth buffers.
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   //Turn on the lights
   glEnable(GL_LIGHTING);

   //set material specularity
   GLfloat  specular[] = {0.1, 0.1, 0.1, 1.0};
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
   //set material shininess
   GLfloat  shininess  = 20.0;
   glMaterialf(GL_FRONT, GL_SHININESS, shininess);

   //if(lighton){
     GLfloat ld[]=  { 0.1f*lighton, 0.1f*lighton, 0.1f*lighton, 1.0f};
     glLightfv(GL_LIGHT1, GL_DIFFUSE,  ld);
   //}else{
   //  glLightfv(GL_LIGHT1, GL_DIFFUSE,  Light_Diffuse);
   //}

   //rebuild crystal list if necessary
   if(rebuild) {
     rebuild=0;
     if(filter_part) filter();
     build_crystal();
   }

   //Color the material
   glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);

   glEnable(GL_CULL_FACE);
   //glDisable(GL_BLEND);

   //Draw the scene
   glCallList(MY_CRYSTAL_LIST);

   glDepthMask(0); //turn off depth buffer for transparent things.
   glEnable(GL_BLEND);
   //glCallList(MY_CRYSTAL_LIST_TRANS);

   //turn it off if it was on
   glDisable(GL_CLIP_PLANE3);

   //Turn the depth buffer back on
   glDepthMask(1);

   //draw the spheres that have been cut by the clipping plane
   clip_spheres_2();

   //draw the box if needed
   if(boundingbox) draw_bounding_box();

   //drawinfo();

   if(fps) {
     //drawfps();
     drawinfo();
   }

   glutSwapBuffers();
  }
#ifndef WIN32
  else usleep(100);
#endif

   // Now let's do the motion calculations.
   rotate_xy(X_Speed,Y_Speed,Z_Speed);

   if(X_Speed!=0||Y_Speed!=0||Z_Speed!=0) redraw=1; else redraw=0;

   // And collect our statistics.
   if (redraw) ourDoFPS();
}

void cbReRenderScene(void){
  redraw=1;
  cbRenderScene();
}

void quit(){
  save_povray();
  gluDeleteQuadric(quadObj);
  free(particles);
  glutDestroyWindow(Window_ID);
  exit(1);
}

void hide(){
  int i;
  for(i=0;i<n_part;i++){
    if(particles[i].selected) {
      particles[i].visible=0;
      particles[i].selected=0;
    }
  }
}

void unhide(){
  int i;
  for(i=0;i<n_part;i++){
    if(!particles[i].visible) {
      particles[i].visible=1;
    }
  }
}

void transperant(){
  int i;
  for(i=0;i<n_part;i++){
    if(particles[i].selected) {
      particles[i].alpha=1.5-particles[i].alpha;
      particles[i].selected=0;
    }
  }
}

void untransperant(){
  int i;
  for(i=0;i<n_part;i++){
    if(particles[i].alpha<1.0) {
      particles[i].alpha=1.5-particles[i].alpha;
    }
  }
}

void unselect(){
  int i;
  for(i=0;i<n_part;i++){
    if(particles[i].selected) {
      particles[i].selected=0;
    }
  }
}

void green(){
  int i;
  for(i=0;i<n_part;i++){
    if(particles[i].selected) {
      //particles[i].r=0.0;
      //particles[i].g=0.0;
      GLfloat r=particles[i].r;
      particles[i].r = particles[i].g;
      particles[i].g = particles[i].b;
      particles[i].b = r;
      particles[i].selected=0;
    }
  }
}

void PngRead(png_structp png_ptr, png_bytep data, png_size_t length){
  #if PNG_LIBPNG_VER >= 10504
  fread(data,1,length,(png_FILE_p) png_get_io_ptr(png_ptr));
  #else
  fread(data,1,length,(png_FILE_p) png_ptr->io_ptr);
  #endif
}

void PngWrite(png_structp png_ptr, png_bytep data, png_size_t length){
  #if PNG_LIBPNG_VER >= 10504
  fwrite(data,1,length,(png_FILE_p) png_get_io_ptr(png_ptr));
  #else
  fwrite(data,1,length,(png_FILE_p) png_ptr->io_ptr);
  #endif 
}

void PngFlush(png_structp png_ptr){
  #if PNG_LIBPNG_VER >= 10504
  fflush((png_FILE_p) png_get_io_ptr(png_ptr));
  #else
  fflush((png_FILE_p) png_ptr->io_ptr);
  #endif
}

void capture_screen(char *pngfilename){
  png_byte color_type=PNG_COLOR_TYPE_RGB;
  png_byte bit_depth=8;

  png_structp png_ptr;
  png_infop info_ptr;
  unsigned char **row_pointers;

  FILE *fp=fopen(pngfilename,"wb");
  if(!fp) {printf("Error opening png file.\n"); exit(1);}

  /* initialize stuff */
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  info_ptr = png_create_info_struct(png_ptr);

 // png_init_io(png_ptr, fp);  //gives an error in windows due to clib incompatiblity between the dll and this file
  png_set_write_fn(png_ptr, (png_voidp) fp, PngWrite, PngFlush);

  /* write header */
  png_set_IHDR(png_ptr, info_ptr, Window_Width, Window_Height,
               bit_depth, color_type, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  /* write info header */
  png_write_info(png_ptr, info_ptr);

  /* set row pointers to lines*/
  row_pointers = (unsigned char**) malloc(sizeof(png_bytep*) * Window_Height);
  int y;
  for (y=0; y<Window_Height; y++){
    row_pointers[y] = (unsigned char*) malloc(Window_Width * sizeof(unsigned char)*3);
    glReadPixels(0,Window_Height-y-1,Window_Width,1,GL_RGB,GL_UNSIGNED_BYTE,row_pointers[y]);
  }

  /* write bytes */
  png_write_image(png_ptr,(unsigned char**) row_pointers);

  /* end write */
  png_write_end(png_ptr, NULL);

  /* cleanup heap allocation */
  for (y=0; y<Window_Height; y++) free(row_pointers[y]);
  free(row_pointers);

  fclose(fp);
  png_destroy_write_struct(&png_ptr, &info_ptr); //nodig of nutteloos?
  printf("%s written!\n",pngfilename);
}

void invert_selection(){
  int i;
  for(i=0;i<n_part;i++){
    if (particles[i].visible)
      particles[i].selected = !particles[i].selected;
  }
}

void temp_movie_script(void){
  int i;
  char pngfilename[255];
  for(i=0;i < globbuf.gl_pathc;i++){
    globcur=i;
    sprintf(pngfilename,"openglseries_%.04i.png",i);
    Y_Speed=0;
    X_Speed=0;
    Z_Speed=0;
    load();
    rebuild=1;
    redraw=1;
    cbRenderScene();
    capture_screen(pngfilename);
  }
}

void temp_movie_rot(void){
  int i;
  char pngfilename[255];
  if(X_Speed==0&&Y_Speed==0) {X_Speed=-0.025f;Y_Speed=-0.025f;}
  double aa=sqrt(X_Speed*X_Speed+Y_Speed*Y_Speed);
  for(i=0;i<2*M_PI/aa;i++){
    sprintf(pngfilename,"rot_series_%.04i.png",i);
    cbRenderScene();
    printf("%i %le \n",i,aa);
    capture_screen(pngfilename);
  }
  X_Speed=0;
  Y_Speed=0;
  Z_Speed=0;
}

// please don't ask how this works I have no idea
void temp_movie_time(int i) {
  int j;
  char pngfilename[255];
  for (j = 0; globcur + i < globbuf.gl_pathc; j++) {
    globcur += i;
    sprintf(pngfilename,"time_series_%.04i.png",j);
    load();
    rebuild = 1;
    redraw = 1;
    cbRenderScene();
    capture_screen(pngfilename);
    // printf("%i\n",j);
  }
    sprintf(pngfilename,"time_series_%.04i.png",j);
    load();
    rebuild=1;
    redraw=1;
    cbRenderScene();
    capture_screen(pngfilename);

}

void printhelp(void) {
printf("\n" PROGRAM_TITLE "\n\n\
    F1  Prints this info\n\
    0   Changes the field of view.\n\
    9   Changes the field of view.\n\
    f   Draws information on the screen.\n\
    o   Switches between orthogonal en perspective projections.\n\
    b   Toggles drawing of the bounding box.\n\
    i   Inverts the selection.\n\
    a   Makes a series of screen shots of the rotating scene.\n\
    y   Makes a series of screen shots of all snapshots that have been given at the command line.\n\
    p   Makes a .png screen shot.\n\
    s   Changes the amount of light.\n\
    r   Reverses the rotation.\n\
    +   Increase number of polygons per sphere.\n\
    -   Decrease number of polygons per sphere.\n\
    8   Moves up.\n\
    2   Moves down.\n\
    6   Moves right.\n\
    4   Moves left.\n\
    H   Shows all hidden objects.\n\
    h   hide selected objects.\n\
    t   Makes selected objects transparent.\n\
    T   Makes all objects solid.\n\
    u   Deselects all.\n\
    g   Gives selected particles a different colour.\n\
    w   Saves the current view to a file (does not work yet).\n\
    l   Reread current file.\n\
    <   Moves a cutting plane back.\n\
    >   Moves a cutting plane forward.\n\
    c   Changes the background color.\n\
    /   Makes everything slightly bigger.\n\
    *   makes everything slightly smaller.\n\
    [   Loads previous file.\n\
    ]   Loads next file.\n\
    \\   Jump 10 frames\n\
    |   Jump back 10 frames\n\
    1   Jump 100 frames\n\
    3   Jump back 100 frames \n\
    j   makes movie jumping 1 frames each step\n\
    J   makes movie jumping 10 frames each step\n\
    m   makes movie jumping 100 frames each step\n\
    d   nothing interesting.\n\
    v   Calculated the volume fraction of the spheres (not the other objects).\n\
    e   Tests for overlap between spheres. Over lapping spheres are selected.\n\
    q or Esc: Quits.\n\
    F or Space: Stops the rotation.\n\
    Enter: Puts the object at rest at the starting position.\n\
    Home: Moves the objects further away small step.\n\
    End:  Moves the objects closer small step.\n\
    Pg Up: Moves the objects further away large step.\n\
    Pg Dn: Moves the objects closer large step.\n\
    Up,Down,Left,Right arrows: Change rotation speed.\n");
}

double sqr(double x){
  return x*x;
}

void overlaptest(void){
  int i,j;
  double drsq;
  double dr[3];
  double ax[3],ay[3],az[3];
  VSET(ax,bb[1][0]-bb[0][0], bb[1][1]-bb[0][1], bb[1][2]-bb[0][2]);
  VSET(ay,bb[3][0]-bb[0][0], bb[3][1]-bb[0][1], bb[3][2]-bb[0][2]);
  VSET(az,bb[4][0]-bb[0][0], bb[4][1]-bb[0][1], bb[4][2]-bb[0][2]);

  //spheres only for now.
  for(i=0;i<n_part-1;i++) if(particles[i].soort==1 && particles[i].selected !=1)
    for(j=i+1;j<n_part;j++) if(particles[j].soort==1 && particles[j].selected !=1){
      dr[0]=particles[i].x-particles[j].x;
      dr[1]=particles[i].y-particles[j].y;
      dr[2]=particles[i].z-particles[j].z;

      if( VDOT(dr,ax) > VLENSQ(ax) * 0.5 ) VSUB(dr,dr,ax);
      if( VDOT(dr,ax) <-VLENSQ(ax) * 0.5 ) VSUB(dr,ax,dr);
      if( VDOT(dr,ay) > VLENSQ(ay) * 0.5 ) VSUB(dr,dr,ay);
      if( VDOT(dr,ay) <-VLENSQ(ay) * 0.5 ) VSUB(dr,ay,dr);
      if( VDOT(dr,az) > VLENSQ(az) * 0.5 ) VSUB(dr,dr,az);
      if( VDOT(dr,az) <-VLENSQ(az) * 0.5 ) VSUB(dr,az,dr);

      drsq=VLENSQ(dr);
      if(drsq < sqr(scale*(particles[i].size1+particles[j].size1)*0.5)) {
        particles[i].selected=1;
        particles[j].selected=1;
      }
  }
}


void toggle_filter(void){
  filter_part=1-filter_part;
}


void printprut(void){
  printf("(%f %f %f)\n",X_Off,Y_Off,Z_Off);
  printf("  | %f | %f | %f |\n"  ,rot_matrix[0],rot_matrix[1],rot_matrix[2]);
  printf("  | %f | %f | %f |\n"  ,rot_matrix[4],rot_matrix[5],rot_matrix[6]);
  printf("  | %f | %f | %f |\n\n",rot_matrix[8],rot_matrix[9],rot_matrix[10]);
}

int Xparticles(void){
  int result = 0;
  for (int i = 0; i < n_part; i++) {
    if (particles[i].size2 == 1.0)
      result++;
  }
  return result;
}

double volfrac(void){
  // printf("%lf\n", sin(particles[0].angle));
  // printf("scale %lf\n", scale);
  /* double m[9];
  MSET(m,
    bb[3][0]-bb[0][0], bb[3][1]-bb[0][1], bb[3][2]-bb[0][2],
    bb[1][0]-bb[0][0], bb[1][1]-bb[0][1], bb[1][2]-bb[0][2],
    bb[4][0]-bb[0][0], bb[4][1]-bb[0][1], bb[4][2]-bb[0][2]);

  double volume = fabs(MDET(m));
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 3; j++) {
      printf("bb[%d][%d] = %lf ", i, j, bb[i][j]);
    }
    printf("\n");
  } */
  
  // printf("volume %lf\n", volume);
  // double volume = (bb[1][0] - bb[0][0]) * (bb[3][1] - bb[0][1]) * (bb[4][2] - bb[0][2]);
  double partvol=0;
  int i;
  for(i=0;i<n_part;i++){
    switch (particles[i].soort){
      case 1: // sphere
        partvol+=pow(particles[i].size1/2.0,3)*M_PI*4.0/3.0;
      break;
      case 2: // cylinder
        partvol+=pow(particles[i].size1/2.0,2)*M_PI*particles[i].size2;
      break;
      case 3: // spherocylinder
        partvol+=pow(particles[i].size1/2.0,3)*M_PI*4.0/3.0+pow(particles[i].size1/2.0,2)*M_PI*particles[i].size2;
      break;
      case 6: // cube
       partvol+=pow(particles[i].size1,3);
      break;
      case 7: // poly
       partvol+=0.0;
      break;
      case 8: // poly
       partvol+=0.0;
      break;
      case 9: // poly
       partvol+=0.0;
      break;
      case 10: // poly // assumes edge length 1 and every particle the same
       partvol = sin(particles[i].angle) * n_part;
       return partvol / volume_ruud;

    }
  }
  // return scale*scale*scale*partvol/volume;
  return scale*scale*scale*partvol / volume_ruud;
}

void printvolfrac(void){
  printf("The volume fraction is: %f\n",volfrac());
}

void readcmdl(){
  int i;
  for(i=0;i<cmdl_n;i++){
    if(i==0) {
      if(glob(cmdl_s[i],GLOB_TILDE,NULL,&globbuf) != 0) printf("Error reading something\n");
    } else {
      if(glob(cmdl_s[i],GLOB_TILDE | GLOB_APPEND,NULL,&globbuf) != 0) printf("Error reading something\n");
    }
  }
  if(globcur >= globbuf.gl_pathc) globcur= globbuf.gl_pathc-1;
}

// ------
// Callback function called when a normal key is pressed.

void cbKeyPressed( unsigned char key, int x, int y ) {
   char pngfile[255];
   int i;
   switch (key) {
   case 114: // r
     globfree(&globbuf);
     readcmdl();
     break;
   case 118: // v
      printvolfrac();
      break;
   case 101: // e
      unselect();
      overlaptest();
      rebuild=1;
      redraw=1;
      break;
   case 57: // 0
      if(fov > 5.0){
        Z_Off=Z_Off/((fov-1.0f)/(fov));
        cut_dist=cut_dist/((fov-1.0f)/(fov));
        fov--;
        redraw=1;
      }
      break;
   case 48: // 9
      if(fov < 120){
        Z_Off=Z_Off/((fov+1.0f)/(fov));
        cut_dist=cut_dist/((fov+1.0f)/(fov));
        fov++;
        redraw=1;
      }
      break;
   case 102: // f
      fps=!fps;
      redraw=1;
      break;
   case 111: // o orthogonal projection
      ortho=!ortho;
      redraw=1;
      break;
   case 98: // b removes the bounding box
      boundingbox=!boundingbox;
      redraw=1;
      break;
   case 105: // i
      invert_selection();
      rebuild=1;
      redraw=1;
      break;
   case 106: // j
      temp_movie_time(1);
      break;
   case 74: // J
      temp_movie_time(10);
      break;
   case 109: // m
      temp_movie_time(100);
      break;

   case 97: // a
      temp_movie_rot();
      break;

   case 121:  // Y temp to make a movie should be removed!
       temp_movie_script();
       break;

   case 112:  // p outputs a screenshot
       sprintf(pngfile,"screenshot_%li.png",time(NULL)-1146216211);
       capture_screen(pngfile);
       break;

   case 113: case 81: case 27: // Q (Escape) - We're outta here.
      quit();
      break; // exit doesn't return, but anyway...

   case 115: // s
      lighton++;
      if(lighton > 30) lighton =1;
      redraw=1;
      break;

   case 32:  // F (Space) - Freeze!
      X_Speed=Y_Speed=0;Z_Speed=0;
      break;

   case 43:  // + more polygons per sphere
      detail++;
      rebuild=1;
      redraw=1;
      break;

   case 45: // - less polygons per sphere
      if(detail>3)detail--;
      rebuild=1;
      redraw=1;
      break;

   case 56: // 8
      Y_Off += step;
      redraw=1;
      break;

   case 54: // 6
      X_Off += step;
      redraw=1;
      break;

   case 52: // 4
      X_Off -= step;
      redraw=1;
      break;

   case 50: // 2
      Y_Off -= step;
      redraw=1;
      break;

    case 49: // 1
      if(globcur > 99) {
        globcur-=100;
        load();
        rebuild=1;
        redraw=1;
      }
      break;

    case 51: // 3
      if(globcur < (int)globbuf.gl_pathc-100) {
        globcur+=100;
        load();
        rebuild=1;
        redraw=1;
      }
      break;

   case 72: // H
      unhide();
      rebuild=1;
      redraw=1;
      break;

   case 104: // h
      hide();
      rebuild=1;
      redraw=1;
      break;

   case 116: // t
      transperant();
      rebuild=1;
      redraw=1;
      break;

   case 84: // T
      untransperant();
      rebuild=1;
      redraw=1;
      break;

   case 117: // u
      unselect();
      rebuild=1;
      redraw=1;
      break;

   case 103: // g
      green();
      rebuild=1;
      redraw=1;
      break;

   case 119: // w
      save_povray();
      // printf("Saved!\n");
      break;

   case 108: // l
      load();
      rebuild=1;
      redraw=1;
      break;

   case 44: // <
      if (cut_dist > 0) cut_dist-=0.05;// else cull=1;
      redraw=1;
      break;
   case 46: // >
      cut_dist+=0.05;
      //cull=0;
      redraw=1;
      break;
   case 83: // S
      toggle_filter();
      redraw=1;
      rebuild=1;
      break;
   case 99: // c
      if(clearcolor==1) {glClearColor(1.0f,1.0f,1.0f,0.0f); clearcolor++;} else
        if(clearcolor==2) {glClearColor(0.1f,0.1f,0.1f,0.0f); clearcolor++;} else
          if(clearcolor==3) {glClearColor(0.0f,0.0f,0.0f,0.0f); clearcolor=1;}
      redraw=1;
      break;
   case 47: // /
      scale *=1.0/1.01;
      rebuild=1;
      redraw=1;
      break;
   case 42: // *
      scale *=1.01;
      rebuild=1;
      redraw=1;
      break;

   case 91: // [
      if(globcur > 0) {
        globcur--;
        load();
        rebuild=1;
        redraw=1;
      }
      break;
   case 92: // Backslash 
      if(globcur < (int)globbuf.gl_pathc-10) {
        globcur+=10;
        load();
        rebuild=1;
        redraw=1;
      }
      break;
   case 124: // | 
      if(globcur > 9) {
        globcur-=10;
        load();
        rebuild=1;
        redraw=1;
      }
      break;

   case 93: // ]
      if(globcur < globbuf.gl_pathc-1) {
        globcur++;
        load();
        rebuild=1;
        redraw=1;
      }
      break;
   case 123: // {
      globcur=0;
      load();
      rebuild=1;
      redraw=1;
      break;
   case 125: // }
      globcur = globbuf.gl_pathc-1;
      load();
      rebuild=1;
      redraw=1;
      break;
   case 100: // d
      printprut();
      break;

   case 13: // Enter
     for(i=0;i<16;i++) rot_matrix[i]=id_matrix[i];
     X_Speed = 0.0f;
     Y_Speed = 0.0f;
     Z_Speed = 0.0f;
     rebuild=1;
     redraw=1;
     break;

   default:
      printf ("KP: No action for %d.\n", key);
      break;
    }
}


// ------
// Callback Function called when a special key is pressed.

void cbSpecialKeyPressed(
   int key,
   int x,
   int y
)
{
   switch (key) {
   case 1: // F1
      printhelp();
      break;

   case 106: // move the cube into the distance. home
      Z_Off -= 0.005f;
      redraw=1;
      break;

   case 107: // move the cube closer. end
      Z_Off += 0.005f;
      redraw=1;
      break;

    case GLUT_KEY_PAGE_UP: // move the cube into the distance.
      Z_Off -= 0.05f;
      redraw=1;
      break;

   case GLUT_KEY_PAGE_DOWN: // move the cube closer.
      Z_Off += 0.05f;
      redraw=1;
      break;

   case GLUT_KEY_UP: // decrease x rotation speed;
      if(glutGetModifiers()==GLUT_ACTIVE_SHIFT) {
        rotate_xy(0.0,-M_PI/2.0,0.0);
        redraw=1;
      } else {
        Y_Speed -= 0.01f;
      }
      break;

   case GLUT_KEY_DOWN: // increase x rotation speed;
      if(glutGetModifiers()==GLUT_ACTIVE_SHIFT) {
        rotate_xy(0.0,M_PI/2.0,0.0);
        redraw=1;
      } else {
        Y_Speed += 0.01f;
      }
      break;

   case GLUT_KEY_LEFT: // decrease y rotation speed;
      if(glutGetModifiers()==GLUT_ACTIVE_SHIFT) {
        rotate_xy(M_PI/2.0,0.0,0.0);
        redraw=1;
      } else {
        X_Speed += 0.01f;
      }
      break;

   case GLUT_KEY_RIGHT: // increase y rotation speed;
      if(glutGetModifiers()==GLUT_ACTIVE_SHIFT) {
        rotate_xy(-M_PI/2.0,0.0,0.0);
        redraw=1;
      } else {
        X_Speed -= 0.01f;
      }
      break;

   default:
      printf ("SKP: No action for %d.\n", key);
      break;
    }
}


void locate(int x, int y){
  GLint viewport[4];
  GLint hits;
  glInitNames(); //clears name stack
  glRenderMode(GL_SELECT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
   viewport[0] = 0;
   viewport[1] = 0;
   viewport[2] = Window_Width;
   viewport[3] = Window_Height;

 // glMatrixMode(GL_MODELVIEW);
 // glLoadIdentity();

  gluPickMatrix(x, Window_Height - y, 0.5f, 0.5f, viewport);
  if(ortho) {
    GLfloat tmp=-tan(M_PI*fov/180)*Z_Off*0.3333;
    GLfloat asp=(GLfloat)Window_Width/(GLfloat)Window_Height;
    if(asp > 1) glOrtho(-tmp*asp,tmp*asp,-tmp,tmp,-10.0f,300.0f);
           else glOrtho(-tmp,tmp,-tmp/asp,tmp/asp,-10.0f,300.0f);
  } else {
    gluPerspective(fov,(GLfloat)Window_Width/(GLfloat)Window_Height,0.01f,300.0f);
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  GLdouble clip_plane[]={0.0,0.0,-1.0,-cut_dist};
  glClipPlane(GL_CLIP_PLANE3,clip_plane);
  glEnable(GL_CLIP_PLANE3);

  glMultMatrixf(rot_matrix);

  glPushName(0);//Puts -1 in name stack to overload later

  glCallList(MY_CRYSTAL_LIST);
  //glCallList(MY_CRYSTAL_LIST_TRANS);
  glDisable(GL_CLIP_PLANE3);
  clip_spheres_2();

  glutSwapBuffers();
  hits = glRenderMode(GL_RENDER);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //gluPerspective(45.0f,(GLfloat)Window_Width/(GLfloat)Window_Height,0.1f,100.0f);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   int i;

   GLuint depth = ~0;
   GLuint *ptr = (GLuint*) selectBuf;
   int name=-1;
   for(i=0;i<hits;i++) {
     //if(*(ptr+i*4+1) < depth && *(ptr+i*4+1) > 0) {
     if(*(ptr+i*4+1) < depth ) {
       depth=*(ptr+i*4+1);
       name=*(ptr+i*4+3);
     }
   }

   if(name>=0){
     particles[name].selected=1-particles[name].selected;
     rebuild=1;
     redraw=1;
   }
}

void mouse_pressed(int button, int state, int x, int y){
  static int px,py;
  switch (button) {
    case GLUT_LEFT_BUTTON:
      if (!state) {
        int specialKey = glutGetModifiers();
        if (specialKey == GLUT_ACTIVE_SHIFT) {
          mouse_shift=1;
        } else {
          mouse_shift=0;
        }
        X_Speed = 0;
        Y_Speed = 0;
        mouse1_pressed=1;
        mouse1_px=x;
        mouse1_py=y;
        px=x;
        py=y;
      }
      else {
        mouse1_pressed=0;
        if(x==px && y==py){
          locate(x,y);
        }
      }
      redraw=1;
      break;
    case GLUT_MIDDLE_BUTTON:
      if(!state) {
        locate(x,y);
      }else mouse2_pressed=0;
      redraw=1;
      break;
    case GLUT_RIGHT_BUTTON: // menu is bound to third button
      if (!state) {
        X_Speed = 0;
        Y_Speed = 0;
        mouse2_pressed=1;
        mouse2_px=x;
        mouse2_py=y;
      }
      else {
        mouse2_pressed=0;
      }
      redraw=1;
      break;
      break;
    case 3:
      if(state) Z_Off+=step;
      redraw=1;
      break;
    case 4:
      if(state) Z_Off-=step;
      redraw=1;
      break;
    default:
      printf("No action bound to mouse button: %d \n",button);

  }
}

void mouse_move(int x, int y){
  if(mouse1_pressed) {
    GLfloat dx=-((GLfloat)x-(GLfloat)mouse1_px)/100.0;
    GLfloat dy=((GLfloat)y-(GLfloat)mouse1_py)/100.0;

    if(!mouse_shift){
      rotate_xy(dx,dy,0.0);
    } else {
      move_xy(dx,dy);
    }

    X_Speed=0;
    Y_Speed=0;

    mouse1_px=x;
    mouse1_py=y;
    redraw=1;
  }
  if(mouse2_pressed) {
    GLfloat dx=-((GLfloat)x-(GLfloat)mouse2_px)/100.0;
    GLfloat dy=((GLfloat)y-(GLfloat)mouse2_py)/100.0;

    rotate_xy(dx,0.0,dy);

    X_Speed=0;
    Y_Speed=0;

    mouse2_px=x;
    mouse2_py=y;
    redraw=1;
  }
}

// ------
// Callback routine executed whenever our window is resized.  Lets us
// request the newly appropriate perspective projection matrix for
// our needs.  Try removing the gluPerspective() call to see what happens.

void cbResizeScene(
   int Width,
   int Height
)
{
   // Let's not core dump, no matter what.
   if (Height == 0)
      Height = 1;

   glViewport(0, 0, Width, Height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

   glMatrixMode(GL_MODELVIEW);

   Window_Width  = Width;
   Window_Height = Height;
   redraw=1;
}


void vis(int visible){
  if (visible == GLUT_VISIBLE) {
    //if (X_Speed==0 && Y_Speed==0)
      glutIdleFunc(&cbRenderScene);
  } else {
    glutIdleFunc(NULL);
  }
}

void menu(int value){
  switch (value) {
    case 1:
      quit();
      break;
  }
}


// ------
// Does everything needed before losing control to the main
// OpenGL event loop.

void ourInit(
  int Width,
  int Height
)
{
  //free mem for coordinates. If not enouph program will free more later.
   n_part=1;
   particles=(tPart*) malloc(n_part*sizeof(tPart));
   load();

  // if there is a previous pov file, read it in
  FILE* fp = fopen("pov.txt", "r");
  if (fp) {
    for (int i = 0; i < 16; i++) {
      fscanf(fp, "%f ", &rot_matrix[i]);
    }
  }

  //For the cylinders
   quadObj = gluNewQuadric();
   gluQuadricOrientation(quadObj, GLU_OUTSIDE);

   build_crystal();

   // Color to clear color buffer to.
   if(clearcolor==1) {glClearColor(1.0f,1.0f,1.0f,0.0f); clearcolor++;} else
     if(clearcolor==2) {glClearColor(0.1f,0.1f,0.1f,0.0f); clearcolor++;} else
       if(clearcolor==3) {glClearColor(0.0f,0.0f,0.0f,0.0f); clearcolor=1;}

   // Depth to clear depth buffer to; type of test.
   glClearDepth(1.0);
   //glDepthFunc(GL_LESS);
   glSelectBuffer(BUFSIZE, selectBuf);

   // Enables Smooth Color Shading; try GL_FLAT for (lack of) fun.
   glShadeModel(GL_SMOOTH);

   // Load up the correct perspective matrix; using a callback directly.
   cbResizeScene(Width,Height);

   // Set up a light, turn it on.
   glLightfv(GL_LIGHT1, GL_POSITION, Light_Position);
   glLightfv(GL_LIGHT1, GL_AMBIENT,  Light_Ambient);
   glLightfv(GL_LIGHT1, GL_DIFFUSE,  Light_Diffuse);
   glLightfv(GL_LIGHT1, GL_SPECULAR,  Light_Specular);
   glEnable (GL_LIGHT1);

   //GLubyte *str;
   //str=glGetString(GL_EXTENSIONS);
   //printf("%s\n",str);

   // A handy trick -- have surface material mirror the color.
   //glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
   //glEnable(GL_COLOR_MATERIAL);
}

// ------
// The main() function.  Inits OpenGL.  Calls our own init function,
// then passes control onto OpenGL.


int main(int argc, char ** argv)
{
   //printf("\tPress F1 for a list of keyboard commands\t");
   int is=1;
   glutInit(&argc,argv);
   if(argc == 1) {
     fprintf(stderr,"ERROR: No filename given!\n");
     exit(666);
   }
  // if(argc > 1) filename=argv[argc-1];
   if(argc > 1) {
     if( argv[1][0]=='-' ){
       datakind=argv[1][1];
       printf("%c\n",datakind);
       is=2;
     } else {
      //  printf("No data kind given hoping for default data!\n");
     }
     cmdl_n=argc-is;
     cmdl_s=&argv[is];
     readcmdl();
   }
   if(globbuf.gl_pathc > 0){
     // To see OpenGL drawing, take out the GLUT_DOUBLE request.
     glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
     glutInitWindowSize(Window_Width, Window_Height);

     // Open a window
     Window_ID = glutCreateWindow( PROGRAM_TITLE );

     // Register the callback function to do the drawing.
     glutDisplayFunc(&cbReRenderScene);

     // If there's nothing to do, draw.
     glutIdleFunc(&cbRenderScene);

     //menu
     glutCreateMenu(menu);
     glutAddMenuEntry("Quit",1);
     //glutAttachMenu(GLUT_RIGHT_BUTTON);

     // It's a good idea to know when our window's resized.
     glutReshapeFunc(&cbResizeScene);

     glutVisibilityFunc(&vis);

     // And let's get some keyboard input.
     glutKeyboardFunc(&cbKeyPressed);
     glutSpecialFunc(&cbSpecialKeyPressed);

     glutMouseFunc(&mouse_pressed);
     glutMotionFunc(&mouse_move);

     // OK, OpenGL's ready to go.  Let's call our own init function.
     ourInit(Window_Width, Window_Height);
    //  load();
    //  rebuild = 1;
    //  redraw = 1;

     // Print out a bit of help dialog.
/*      printf("\n" PROGRAM_TITLE "\n\n\
          F1  Prints this info\n\
          0   Changes the field of view.\n\
          9   Changes the field of view.\n\
          f   Draws information on the screen.\n\
          o   Switches between orthogonal en perspective projections.\n\
          b   Toggles drawing of the bounding box.\n\
          i   Inverts the selection.\n\
          a   Makes a series of screen shots of the rotating scene.\n\
          y   Makes a series of screen shots of all snapshots that have been given at the command line.\n\
          p   Makes a .png screen shot.\n\
          s   Changes the amount of light.\n\
          r   Reverses the rotation.\n\
          +   Increase number of polygons per sphere.\n\
          -   Decrease number of polygons per sphere.\n\
          8   Moves up.\n\
          2   Moves down.\n\
          6   Moves right.\n\
          4   Moves left.\n\
          H   Shows all hidden objects.\n\
          h   hide selected objects.\n\
          t   Makes selected objects transparent.\n\
          T   Makes all objects solid.\n\
          u   Deselects all.\n\
          g   Gives selected particles a different colour.\n\
          w   Saves the current view to a file (does not work yet).\n\
          l   Reread current file.\n\
          <   Moves a cutting plane back.\n\
          >   Moves a cutting plane forward.\n\
          c   Changes the background color.\n\
          /   Makes everything slightly bigger.\n\
          *   makes everything slightly smaller.\n\
          [   Loads previous file.\n\
          ]   Loads next file.\n\
          \\   Jump 10 frames\n\
          |   Jump back 10 frames\n\
          1   Jump 100 frames\n\
          3   Jump back 100 frames \n\
          j   makes movie jumping 1 frames each step\n\
          J   makes movie jumping 10 frames each step\n\
          m   makes movie jumping 100 frames each step\n\
          d   nothing interesting.\n\
          v   Calculated the volume fraction of the spheres (not the other objects).\n\
          e   Tests for overlap between spheres. Over lapping spheres are selected.\n\
          q or Esc: Quits.\n\
          F or Space: Stops the rotation.\n\
          Enter: Puts the object at rest at the starting position.\n\
          Home: Moves the objects further away small step.\n\
          End:  Moves the objects closer small step.\n\
          Pg Up: Moves the objects further away large step.\n\
          Pg Dn: Moves the objects closer large step.\n\
          Up,Down,Left,Right arrows: Change rotation speed.\n"); */

     // Pass off control to OpenGL.
     // Above functions are called as appropriate.
     glutMainLoop();

     globfree(&globbuf);

     return 0;
   } else return 1;
}
