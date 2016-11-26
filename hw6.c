/*
 *  Textures Lighting
 *
 *  Demonstrates textures and lighting on 3d scene
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F2         Toggle local viewer mode
 *  F3         Toggle light distance 
 *  F8         Change ball increment
 *  F9         Invert bottom normal
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
int    sky[2];   //  Sky textures
int axes=1;       //  Display axes
int mode=2;//1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=15.0;   //  Size of world //this was 3 before
// Light values
int one       =   1;  // Unit value
int distance  =   12;  // Light distance //this was 5 before
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   3;  // Elevation of light //this was 0 before

// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f,ly = 0.0f;
// XZ position of the camera
float xx=0.0f,z= 25.0f, yy = 1.0f;
float angle=0.0,angleVert = 0.0;

//bounding sphere parameters
float bounding_centre_x[30];
float bounding_centre_y[30];
float bounding_centre_z[30];
float bounding_radius[30];
int bounding_count = 0;
int done = 0;

//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))


unsigned int texture[7]; // Texture names

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}
static void Sky(double D)
{
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslated(0,5,0);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,sky[0]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glEnd();

   //  Top and bottom
   glBindTexture(GL_TEXTURE_2D,sky[1]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
   glEnd();
glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void cylinder(float x, float y, float z)
{
   float textureFactor = .3;
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
    int dim = 1;
    glPushMatrix();
    glTranslated(x,y,z);
    glRotated(0,1,0,0);
    glScaled(0.3,0.3,0.3);
	//Enable Textures
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glColor3f(1,1,1);
    glBegin( GL_TRIANGLE_FAN );
    glNormal3f(0.0f,1.0f,0.0f);//normal for top points up (this wont be visible though)
    glColor3f(.8,0.2,0.2);
    glVertex3f(0.0f,6/dim,0.0f); int i = 0, j =0;
    for( i=0;i<=360;i++)
    {
	//glNormal3f(0, 1, 0);
        glVertex3f(2*Cos(-i)/dim,6/dim,2*Sin(-i)/dim);
    }
    glEnd();
    // bottom end of cylinder
    glBegin( GL_TRIANGLE_FAN );
    glNormal3f(0.0f,-1.0f,0.0f);//normal for bottom points down (this wont be visible though)
    glVertex3f(0.0f,-0,0.0f);
    for( i=0;i<=360;i++)
    {
	//glNormal3f(0, -1, 0);
        glVertex3f(2*Cos(i)/dim,0,2*Sin(i)/dim);
    }
    glEnd();  


	double x1 = 0,  z1 = 0;//y1 = 0
	double x2 = 0, z2 = 0; //, y2 = 0
	double x3 = 0,  z3 = 0;//y3 = 0,
	double x4 = 0, z4 = 0;//, y4 = 0
	double n1 = 0,n2= 0,n3=0;
    //side of cylinder, y-values are used for the length of the cylinder
    //x and z values are used for the radius.
  glBindTexture(GL_TEXTURE_2D,texture[4]); 
  glBegin(GL_QUAD_STRIP);
    for( j=0;j<=45;j++){

                        for(i=0;i<360;i++ ){
                        //glNormal3f(2*cos(i), 0.0f, 2*sin(i));
			if(i>0)
			{
			x1 = 2*Cos(i-1)/dim; 
			//y1= (j)/7/dim;
			z1 = 2*Sin(i-1)/dim;
			x2 = 2*Cos(i-1)/dim; 
			//y2 = (j+1)/7/dim; 
			z2= 2*Sin(i-1)/dim;
			x3 = 2*Cos(i)/dim;
			//y3= (j)/7/dim;
			z3 = 2*Sin(i)/dim;
			x4 = 2*Cos(i)/dim; 
			//y4 = (j+1)/7/dim; 
			z4= 2*Sin(i)/dim;
			n1 = (x1+x2+x3+x4)/4;
			n3 = (z1+z2+z3+z4)/4;
			glNormal3f(n1,n2,n3); //normal for cylinder
			}
		        glTexCoord2f((i%2)*textureFactor,0);
                        glVertex3f(2*Cos(i)/dim, (j)/7/dim, 2*Sin(i)/dim);
                        glTexCoord2f(1*textureFactor,textureFactor*(i%2));
                        glVertex3f(2*Cos(i)/dim, (j+1)/7/dim, 2*Sin(i)/dim);
                        }
    }
    glEnd();
    glPopMatrix();
  glDisable(GL_TEXTURE_2D);


}
static void cone(float x, float y, float z, float radius, float height)
{
   int inc = 7;
   int th;
   float r = radius, h = height;
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
	glPushMatrix();
  	// Offset
	glTranslated(x,y+1.8,z);
	glRotated(-90,1,0,0); //since by default this cone points in positive z
        glScaled(r*0.3,r*0.3,h*0.3);
	//Enable Textures
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_TRIANGLES);
   	glColor3f(0.7,1,0.7);
	for (th = 0;th<=360;th += 2*inc)
	{
	//Base-1
	glNormal3d(Cos(th-inc),Sin(th-inc), 1);
	glTexCoord2f(0.25,0);
	glVertex3d(Cos(th-inc),Sin(th-inc), 0);
        //Tip
	glNormal3d(Cos(th),Sin(th), 1);
	glTexCoord2f(.5,4);
	glVertex3d(0,0,1);
        //Base+1
	glNormal3d(Cos(th+inc),Sin(th+inc), 1);
	glTexCoord2f(0.75,0);
	glVertex3d(Cos(th+inc),Sin(th+inc), 0);        	
	}
	glEnd();

	
	int angle;
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_TRIANGLE_FAN);


	glNormal3f(0, -1, 0); //Normal points down

	glVertex3f(0, 0, 0);
	for (angle = 0; angle < 360; angle++) {
	    // normal is  pointing down
            glTexCoord2f(0.5*Cos(angle)+0.5, 0.5*Sin(angle)+0.5);
	    glVertex3f(Sin(angle), Cos(angle), 0);
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

}
static void trapezoid(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{

   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
// Textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,0.4,0.4);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glBindTexture(GL_TEXTURE_2D,texture[1]);
   int rep = 4;
   //  Cube
   glBegin(GL_QUADS);
   //  Front

   glNormal3f( 0, 0.5, 2);
   glTexCoord2f(0,0);glVertex3f(-1,-1, 1);
   glTexCoord2f(rep,0);glVertex3f(+1,-1, 1);
   glTexCoord2f(rep*0.5,rep*0.5);glVertex3f(+0.5,+1, 0.5);
   glTexCoord2f(0,rep*0.5);glVertex3f(-0.5,+1, 0.5);
   //  Back

   glNormal3f( 0, 0.5, -2);
   glTexCoord2f(0,0);glVertex3f(+1,-1,-1);
   glTexCoord2f(rep,0);glVertex3f(-1,-1,-1);
   glTexCoord2f(rep*0.5,rep*0.5);glVertex3f(-0.5,+1,-0.5);
   glTexCoord2f(0,rep*0.5);glVertex3f(+0.5,+1,-0.5);
   //  Right

   glNormal3f( 2, 0.5, 0);
   glTexCoord2f(0,0);glVertex3f(+1,-1,+1);
   glTexCoord2f(rep,0);glVertex3f(+1,-1,-1);
   glTexCoord2f(rep*0.5,rep*0.5);glVertex3f(+0.5,+1,-0.5);
   glTexCoord2f(0,rep*0.5);glVertex3f(+0.5,+1,+0.5);
   //  Left

   glNormal3f( -2, 0.5, 0);
   glTexCoord2f(0,0);glVertex3f(-1,-1,-1);
   glTexCoord2f(rep,0);glVertex3f(-1,-1,+1);
   glTexCoord2f(rep*0.5,rep*0.5);glVertex3f(-0.5,+1,+0.5);
   glTexCoord2f(0,rep*0.5);glVertex3f(-0.5,+1,-0.5);
   //  Top

   glNormal3f( 0, 1, 0);
   glTexCoord2f(0,0);glVertex3f(-0.5,+1,+0.5);
   glTexCoord2f(rep/2,0);glVertex3f(+0.5,+1,+0.5);
   glTexCoord2f(rep/2,rep/2);glVertex3f(+0.5,+1,-0.5);
   glTexCoord2f(0,rep/2);glVertex3f(-0.5,+1,-0.5);
   //  Bottom

   glNormal3f( 0, -1, 0);
   glTexCoord2f(0,0);glVertex3f(-1,-1,-1);
   glTexCoord2f(rep,0);glVertex3f(+1,-1,-1);
   glTexCoord2f(rep,rep);glVertex3f(+1,-1,+1);
   glTexCoord2f(0,rep);glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}
/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
	if(bounding_count <= 4 && done ==0)
	{bounding_centre_x[bounding_count] = x;
	bounding_centre_y[bounding_count] = y;
	bounding_centre_z[bounding_count] = z;

   bounding_radius[bounding_count] =(dx < dz)?dz:dx ;//+ dz/2;
glTranslatef(x, y, z);
glutWireSphere(bounding_radius[bounding_count] ,50,50); 	
	printf("original = %f %f %f\n",dx,dy,dz);
   printf("%f %f %f %f \n",bounding_centre_x[bounding_count], bounding_centre_y[bounding_count], bounding_centre_z[bounding_count], bounding_radius[bounding_count]);
	bounding_count++;


	}
	//printf("\n%d  = \n",bounding_count);
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   // Textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

   //  Cube

   //  Front

   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);

   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   //  Back

   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);

   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right

   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);

   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left

   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   glBegin(GL_QUADS);

   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top

   glColor3f(.8,.8,.8);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);

   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(15,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(15,15); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,15); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glColor3f(1,0,1);
   glBegin(GL_QUADS);
   glNormal3f( 0,-one, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}


/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}
//Draw a tree at x,y,z with radius and height of upper part cone of tree given in parameters
static void tree(float x,float y, float z,float radius, float height)
{
cone(x,y,z,radius,height);
cylinder(x,y,z);
}

//Draw a house at x,y,z with dimensions dx,dy,dz and rotation th

static void house(float x,float y, float z, float dx, float dy, float dz,double th)
{
trapezoid(x,y+3*dy,z,dx,dy,dz,th);
cube(x,y+dy,z,0.8*dx,dy,0.8*dz,th);
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */

int isCollision()
{
		float x2 = xx, y2 = yy, z2 = z; // this is position of user
		float userRadius = 1;
	float x1 = 0, y1 = 0, z1 = 0, object_radius = 0;; //say the thing to check is placed at origin https://www.opengl.org/discussion_boards/showthread.php/142913-Collision-Detection-easy-implementation link for this.
int i = 0;
for(i = 0 ; i < bounding_count; i ++)
{
x1 = bounding_centre_x[i];
y1 = bounding_centre_y[i];
z1 = bounding_centre_z[i];
object_radius = bounding_radius[i];
	//printf(" %f %f %f \n",xx,yy,z); 
float dx = x2 - x1 ;//or x1 - x2 ! 
float dy = y2 - y1 ;// or y1 - y2!
float dz = z2 - z1 ; 

float distance = sqrt(dx*dx + dy*dy + dz*dz);
//printf("for %d distance is %f\n",i,distance);

if( distance <= (userRadius + object_radius)) return 1 ; // this condition is sum of two radii.

}	
return 0;	

}
void display()
{
   const double len=2.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode ==1)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   else 
	if(mode==2) // if FPS
		gluLookAt(xx, yy, z,
			  xx+lx, yy+ly,  z+lz,
			  0.0f, 1,  0.0f);
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
 Sky(1.5*dim);
   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.5); //r was 0.1 before
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);  

//  Draw axes - no lighting from here on
//Draw 3d scene
	tree(0,0,0,7,20); 
	tree(7,0,3,7,15); 
	tree(8,0,-8,6,7);
	tree(-6,0,3,7,16); 
	tree(-5.5,0,-3,5,10);

	house(5,0,-7,.8,0.8,0.8,0);
	house(5,0,-5,1.8,0.8,0.8,0);
	house(2,0,6,1.5,1,0.8,45);
	house(-5,0,-6,2,0.8,0.8,0);
	house(-3,0,5,1,0.8,0.8,0);
done = 1;

   // glClear(GL_COLOR_BUFFER_BIT);


//glTranslatef(0,0,0.0f);
   //glColor4f(0.8,0.8,0.8,0); 
//glutSolidSphere( 5.0,50,50); 

//glTranslatef(xx, yy, z);
//glutWireSphere( 5.0,50,50); 

//glTranslatef(5, 0.8, -7);
//glutSolidSphere( .64,50,50); 

	//draw base
	cube(0,0,0, 22.5,0.001,22.5, 0);	
 
	//  White
	glColor3f(1,1,1);
   
glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
     float collision = 0;
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
      if((collision = isCollision()) == 1)
      Print(" THERE IS COLLISION! = %f",collision);
      else Print(" No COllision yet = %f",collision);
   }

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(80*t,360.0);//this was 90*t before
   //  Tell GLUT it is necessary to redisplay the scene
  // printf("%d zh = ",zh);
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      			angle -= 0.1f;//th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      			angle += 0.1f;//th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
     			angleVert -= 0.05f;// ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      			angleVert += 0.05f;//ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   else if (key == GLUT_KEY_F3)
      distance = (distance==12) ? 15 : 12;
   //  Toggle ball increment
   else if (key == GLUT_KEY_F8)
      inc = (inc==10)?3:10;
   //  Flip sign
   else if (key == GLUT_KEY_F9)
      one = -one;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   	ly = -sin(angleVert); //rotate camera up and down
	lx = -sin(angle)*cos(angleVert); //rotate camera left
	lz = -cos(angle)*cos(angleVert); //rotate camera right

   //  Update projection
   Project(mode?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      {
      	if(mode == 2) 
			{
			float xx1 = xx; float yy1 = yy; float z1 = z;
			xx += 0.5*lx;
			z += 0.5*lz;
			yy += 0.5*ly;
			
			if(isCollision() == 1) //if there is collision retreive to previous values;
			{
				xx = xx1 ; yy = yy1; z = z1;
			}
			} 
		else
      	fov--;
      	}
   else if (ch == '+' && ch<179)
      {if(mode == 2) 
			{
							float xx1 = xx; float yy1 = yy; float z1 = z;
			xx -= 0.5*lx;
			z -= 0.5*lz;
			yy -= 0.5*ly;
			if(isCollision() == 1) //if there is collision retreive to previous values;
			{
				xx = xx1 ; yy = yy1; z = z1;
			}
			}
		else
		fov++;}
   //  Light elevation
   else if (ch=='[') //these were .1 before
      ylight -= 1;
   else if (ch==']')
      ylight += 1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
   //  Translate shininess power to value (-1 => 0)
   shiny = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(mode?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //transparency
    glEnable( GL_BLEND ); //transparency
    
   glutInitWindowSize(400,400);
   glutCreateWindow("Sneha Parmar | HW6");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);

   glutIdleFunc(idle);
   //Load Textures
   texture[0] = LoadTexBMP("lawn.bmp");
   texture[1] = LoadTexBMP("roof.bmp");
   texture[2] = LoadTexBMP("wall.bmp");
   texture[3] = LoadTexBMP("treefoliage.bmp");
   texture[4] = LoadTexBMP("bark.bmp");
sky[0] = LoadTexBMP("skygrass0.bmp");//"sky0.bmp");
   sky[1] = LoadTexBMP("sky1.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
