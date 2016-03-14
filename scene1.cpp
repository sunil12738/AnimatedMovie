#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double x1=0,y111=0,nn=0;
double x_cor[6]={0,2,2,0,-2,-2};
double y_cor[6]={4,-3.464,3.464,-4,3.464,-3.464};
int global_c_c_rotate=0;
int global_counter=0;
int flag_stop_rotate=0;
double x_7_c=-10,y_7_c=-10;
int flag_names=0;

GLfloat Tx,Ty,Tz;
GLfloat Ra,Rb,Rc;
GLfloat Sx,Sy,Sz;
GLfloat x,y,z,angle;
#define PI 3.14159265f

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
typedef struct Image Image;

int light;;

/*  Create checkerboard texture  */

static GLuint texName1, texName2;

double overall_counter=50;
//to rotate the char to make him run towards cloud
double rotate_character=0;
double rotate_hand=0;
double rotate_leg=0,rotate_leg_initial=25,leg_flag=2;
double run_object=0;
double run_object_up=0;
//next three lines for the smile
int smile_counter=0;
double p_x[7]={-3,-2,-1,0,1,2,3};
double p_y[7]={-6,-3.75,-3.25,-3.0,-3.25,-3.75,-6};

//char smile

double b0(double u) { return (1-u)*(1-u)*(1-u)/6; }
double b1(double u) { return (3*u*u*u - 6*u*u + 4)/6; }
double b2(double u) { return (-3*u*u*u + 3*u*u + 3*u + 1)/6; }
double b3(double u) { return u*u*u/6; }

void create_curve(double * p_x, double * p_y, double r, double as, double points, double translate_z ){
    double x,y;
    // for (int i = 0; i < points; ++i)
    // {
    //         glPushMatrix();  
    //         glTranslatef(p_x[i]/as, p_y[i]/as, 0.0f);

    //         glutWireSphere(r,30, 30);
    //     glPopMatrix();
    // }
    for(int i = -1;i<points-2;++i){
        for(double u = 0;u<1;u+=0.005){
            x = b0(u)*p_x[i] + b1(u)*p_x[i+1] + b2(u)*p_x[i+2] + b3(u)*p_x[i+3];
            y = b0(u)*p_y[i] + b1(u)*p_y[i+1] + b2(u)*p_y[i+2] + b3(u)*p_y[i+3];
            glPushMatrix();
            // glPointSize(15);
            glTranslatef(x/as, y/as, translate_z);
              glutWireSphere(r,30, 30);
            glPopMatrix();
       }
    }
}
void character();

int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size;
    unsigned long i;
    unsigned short int plane;
    unsigned short int bpp;
    char temp;
    if ((file = fopen(filename, "rb"))==NULL)
    {
        printf("File Not Found : %s\n",filename);
        return 0;
    }
    fseek(file, 18, SEEK_CUR);
    fread(&image->sizeX, 4, 1, file);
    fread(&image->sizeY, 4, 1, file);
    size = image->sizeX * image->sizeY * 3;
    fread(&plane, 2, 1, file);
    fread(&bpp, 2, 1, file);
    fseek(file, 24, SEEK_CUR);
    image->data = (char *) malloc(size);
    fread(image->data, size, 1, file);
    for (i=0;i<size;i+=3) { 
        temp = image->data[i];
        image->data[i] = image->data[i+2];
        image->data[i+2] = temp;
    }
    return 1;
}
Image * loadTexture(){
    Image *image1;
    image1 = (Image *) malloc(sizeof(Image));
    if (!ImageLoad("title_page.bmp", image1)) {
        exit(1);
    }     
    return image1;
}


void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);


//glGenTextures(2, texture);
    Image *image1 = loadTexture();
    if(image1 == NULL){
        printf("Image was not returned from loadTexture 1\n");
        exit(0);
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, texName1);    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    glShadeModel(GL_FLAT);
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(0.0, 0.0, -5.0);
    // glRotatef(75,1.0,0.0,0.0);  
    // glColor3f(0.0, 1.0, 1.0); 
    
    
/* ---------------------------------------------------------------------- */ 

/* ---------------------------------------------------------------------- */ 
    
        if(global_c_c_rotate<=6){
        for (int k = 0; k < 35000; ++k)
        {
            printf("%d %d\n",k,global_counter);
        }}
        // gluLookAt(0, 0.0, - 4, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // if(global_c_c_rotate>5) {global_c_c_rotate=0;}
    if(global_c_c_rotate<=5)
    gluLookAt(x_cor[global_c_c_rotate], 0.0, y_cor[global_c_c_rotate], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texName1);

    // tex 1
    if(global_c_c_rotate==0){
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.47); glVertex3f( -4.15f, -3.00f,  0.0f);
    glTexCoord2f(0.5, 0.47); glVertex3f(4.15f, -3.00f,  0.0f);
    glTexCoord2f(0.5, 0.67); glVertex3f(4.15f, 3.00f, 0.0f);
    glTexCoord2f(0.0, 0.67); glVertex3f( -4.15f, 3.00f, 0.0f);
    glEnd();
    glPopMatrix();
    }

    //tex 2
    else if(global_c_c_rotate==1){
    glPushMatrix();
    // glTranslatef(-3.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.5, 0.0); glVertex3f( -4.15f, -3.00f,  0.0f);
    glTexCoord2f(1.0, 0.0); glVertex3f(4.15f, -3.00f,  0.0f);
    glTexCoord2f(1.0, 0.27); glVertex3f(4.15f, 3.00f, 0.0f);
    glTexCoord2f(0.5, 0.27); glVertex3f( -4.15f, 3.00f, 0.0f);
    glEnd();
    glPopMatrix();
    }

    //tex 3
    else if(global_c_c_rotate==2){
    glPushMatrix();
    glTranslatef(-2.0,0.0,-2.5);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.23); glVertex3f( -4.15f, -3.00f,  0.0f);
    glTexCoord2f(0.5, 0.23); glVertex3f(4.15f, -3.00f,  0.0f);
    glTexCoord2f(0.5, 0.45); glVertex3f(4.15f, 3.00f, 0.0f);
    glTexCoord2f(0.0, 0.45); glVertex3f( -4.15f, 3.00f, 0.0f);
    glEnd();
    glPopMatrix();
    }

    //tex 4
    else if(global_c_c_rotate==3){
    glPushMatrix();
    glTranslatef(0.0, 0.0,4.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.5, 0.22); glVertex3f( -4.15f, -3.00f,  0.0f);
    glTexCoord2f(1.0, 0.22); glVertex3f(4.15f, -3.00f,  0.0f);
    glTexCoord2f(1.0, 0.46); glVertex3f(4.15f, 3.00f, 0.0f);
    glTexCoord2f(0.5, 0.46); glVertex3f( -4.15f, 3.00f, 0.0f);
    glEnd();
    glPopMatrix();
    }

    //tex 5
    else if(global_c_c_rotate==4){
    glPushMatrix();
    glTranslatef(3.0, 0.0,-3.5);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f( -4.15f, -3.00f,  0.0f);
    glTexCoord2f(0.5, 0.0); glVertex3f(4.15f, -3.00f,  0.0f);
    glTexCoord2f(0.5, 0.23); glVertex3f(4.15f, 3.00f, 0.0f);
    glTexCoord2f(0.0, 0.23); glVertex3f( -4.15f, 3.00f, 0.0f);
    
    glEnd();
    glPopMatrix();
    }

    //tex 6
    else if(global_c_c_rotate==5){
    glPushMatrix();
    glTranslatef(1.5,-0.00,4.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.5, 0.46); glVertex3f( -4.15f, -3.00f,  0.0f);
    glTexCoord2f(1.0, 0.46); glVertex3f(4.15f, -3.00f,  0.0f);
    glTexCoord2f(1.0, 0.65); glVertex3f(4.15f, 3.00f, 0.0f);
    glTexCoord2f(0.5, 0.65); glVertex3f( -4.15f, 3.00f, 0.0f);
    glEnd();
    glPopMatrix();
    }
    else{
    gluLookAt(0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

        glPushMatrix();
        glTranslatef(-0.5,2.0,-1.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.47); glVertex3f( -4.15f, -3.00f,  0.0f);
    glTexCoord2f(0.5, 0.47); glVertex3f(4.15f, -3.00f,  0.0f);
    glTexCoord2f(0.5, 0.67); glVertex3f(4.15f, 3.00f, 0.0f);
    glTexCoord2f(0.0, 0.67); glVertex3f( -4.15f, 3.00f, 0.0f);
    
    glEnd();
    glPopMatrix();

    
    if(flag_names==0){
    x_7_c+=0.075;
    if(x_7_c>=0) {
        for(int i=0;i<100000;++i){printf("h\n");}
        x_7_c=-10;flag_names=2;
    }
    glPushMatrix();
    glTranslatef(0.0+x_7_c,-1.5,0.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.90); glVertex3f( -4.15f, -1.00f,  0.0f);
    glTexCoord2f(0.9, 0.90); glVertex3f(4.15f, -1.00f,  0.0f);
    glTexCoord2f(0.9, 1.0); glVertex3f(4.15f, 1.00f, 0.0f);
    glTexCoord2f(0.0, 1.0); glVertex3f( -4.15f, 1.00f, 0.0f);   
    glEnd();
    glPopMatrix();
    // for(int i=0;i<5000;++i){printf("h\n");}
    }

    else if(flag_names==2){
    x_7_c+=0.075;
    if(x_7_c>=0) {
        for(int i=0;i<100000;++i){printf("h\n");}
        x_7_c=-10;flag_names=3;
    }
    glPushMatrix();
    glTranslatef(0.0+x_7_c,-2.7,0.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.05, 0.80); glVertex3f( -2.00f, -1.00f,  0.0f);
    glTexCoord2f(0.44, 0.80); glVertex3f(2.00f, -1.00f,  0.0f);
    glTexCoord2f(0.44, 0.90); glVertex3f(2.00f, 1.00f, 0.0f);
    glTexCoord2f(0.05, 0.90); glVertex3f( -2.00f, 1.00f, 0.0f);
    
    glEnd();
    glPopMatrix();

    }
    else if(flag_names==3){
    x_7_c+=0.075;
    if(x_7_c>=0) {
        for(int i=0;i<250000;++i){printf("h\n");}
        x_7_c=0;flag_names=4;
    }
    glPushMatrix();
    glTranslatef(0.0+x_7_c,-2.5,0.0);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.70); glVertex3f( -4.15f, -1.00f,  0.0f);
    glTexCoord2f(1.0, 0.70); glVertex3f(4.15f, -1.00f,  0.0f);
    glTexCoord2f(1.0, 0.80); glVertex3f(4.15f, 1.00f, 0.0f);
    glTexCoord2f(0.0, 0.80); glVertex3f( -4.15f, 1.00f, 0.0f);
    
    glEnd();
    glPopMatrix();
    }

    }
    



    glDisable(GL_TEXTURE_2D);


/* ---------------------------------------------------------------------- */ 

/* ---------------------------------------------------------------------- */ 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);//to enable lightening
    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);//ambient light model
    glShadeModel(GL_SMOOTH);// to select the shading model GL_FLAT or GL_SMOOTHs
    GLfloat specular[] = {1.0f, 1.0f, 1.0f , 1.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    GLfloat diffuse[]={1.0,0.8,0.8,1.0};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
    GLfloat position0[] = { 0.0f, 6.0f, 0.0f, 1.0f }; //position of light source 1
    glLightfv(GL_LIGHT0, GL_POSITION, position0);
    glEnable(GL_LIGHT0);//enable light source 1


    glEnable(GL_COLOR_MATERIAL); //to enable the color tracking. enalbe these 2 lines and then use glcolor
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    float mat_specular[4]={0.0,0.3f,0.8f,1.0f};
    float mat_diffuse[4]={0.0,1.0f,0.0f,1.0f};
    float mat_ambient[4]={0.1,0.1f,0.1f,1.0f};
    float mat_shininess=50.0;

    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);

    GLfloat emission[] = { .5,0.0,0.0,1.0};
    glMaterialfv(GL_BACK,GL_EMISSION,emission);


    //glColor3f(1.0, 0.60, 0.0);

    GLfloat position1[] = { 1.7f,-0.1f,-8.0f, 1.0f }; //position of light source 2
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; //other properties of light source 2
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT1);//enable light source 1


// character
    

    ++global_c_c_rotate;
    // global_c_c_rotate=9;
    ++global_counter;
    if(global_counter>425)     
        character();
    if(global_counter>490) exit(0);
    printf("%d\n",global_counter );
    glFlush();

    glutSwapBuffers();
}
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -3.6);
}
void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
        exit(0);
        break;
        default: 
        break;
    }
}
int main(int argc, char** argv)
{
    /* initialize GLUT, using any commandline parameters passed to the program */
    glutInit(&argc, argv);

    /* setup the size, position, and display mode for new windows */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutInitWindowPosition(100, 100);
    
    /* create and set up a window */
    glutCreateWindow("Scene 1: Introduction");
    Tx = 0.0; Ty = 0.0; Tz = -10.0;

    init();
    glutReshapeFunc (reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display);                  

    glutMainLoop();
    return 0;
}


void character(){
    // glScalef(0.2,0.2,0.2);
    glTranslatef(0.0,0.0,-20.0);
++overall_counter;

    // glRotatef(-90,0.0,1.0,0.0);

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0); 
    glTranslatef(0.0,0.0,0.0);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    GLUquadric *body = gluNewQuadric();           
    gluCylinder(body,1.5,1.5,3,20,20);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0); 
    glTranslatef(0.0,0.0f,-2.0f);
    glTranslatef(0.0f, -0.1f, 2.0f);
    glutSolidSphere(1.5f,30, 30);
    glPopMatrix();


    glPushMatrix();
    glColor3f(0.0, 0.5, 0.14); 
    glTranslatef(-1.0f,-3.0f,0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    GLUquadric *thigh_1 = gluNewQuadric();           
    gluCylinder(thigh_1,0.5,0.5,3.0,10,10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.5, 0.14); 
    glTranslatef(1.0f,-3.0f,0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    GLUquadric *thigh_2 = gluNewQuadric();           
    gluCylinder(thigh_2,0.5,0.5,3.0,10,10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.5, 0.14); 
    glTranslatef(1.8,-0.5,0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    GLUquadric *hand_1 = gluNewQuadric();           
    gluCylinder(hand_1,0.3,0.3,3.0,10,10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.5, 0.14); 
    glTranslatef(-1.8,-0.5,0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    GLUquadric *hand_2 = gluNewQuadric();           
    gluCylinder(hand_2,0.3,0.3,3.0,10,10);
    glPopMatrix();

    //eye ball 1
    glPushMatrix();
    glColor3f(1.0, 0.0, 1.0); 
    glTranslatef(0.7,0.0f,-0.4f);
    glTranslatef(0.0f, -0.1f, 2.0f);
    glutSolidSphere(0.2f,30, 30);
    glPopMatrix();
    //eye ball 2
    glPushMatrix();
    glColor3f(1.0, 0.0, 1.0); 
    glTranslatef(-0.7,0.0f,-0.4f);
    glTranslatef(0.0f, -0.1f, 2.0f);
    glutSolidSphere(0.2f,30, 30);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0); 
    glTranslatef(0.7,0.0,1.3f);
    GLUquadric *eye_1 = gluNewQuadric();           
        gluCylinder(eye_1,0.2,0.4,0.5,10,10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0); 
    glTranslatef(-0.7,0.0,1.3f);
    GLUquadric *eye_2 = gluNewQuadric();           
        gluCylinder(eye_2,0.2,0.4,0.5,10,10);
    glPopMatrix();

    double point_hair = 7;
    double as_hair=8, r_hair= 0.02;
    double p_x_hair[7]={-3-0.25,-2,0,1,2,2.2,2.3};
    double p_y_hair[7]={4+16,3+16,1+16,0+16,-3.3+16,-4.5+16,-6+16};;
    create_curve(p_x_hair,p_y_hair,r_hair,as_hair,point_hair,0);

    if(smile_counter%6<3){

        p_x[0]=-3;
        p_x[1]=-1;
        p_x[2]=1;
        p_x[3]=0;
        p_x[4]=1;
        p_x[5]=2;
        p_x[6]=-3.1;

        p_y[0]=-4;
        p_y[1]=-4;
        p_y[2]=-4;
        p_y[3]=-4;
        p_y[4]=-4;
        p_y[5]=-4;
        p_y[6]=-4;

    }
    else if(smile_counter%6<6){

        p_x[0]=-3.1;
        p_x[1]=-1;
        p_x[2]=1;
        p_x[3]=3.1;
        p_x[4]=1.0;
        p_x[5]=-1.0;
        p_x[6]=-3.0;

        p_y[0]=-4;
        p_y[1]=-3;
        p_y[2]=-3;
        p_y[3]=-4;
        p_y[4]=-5;
        p_y[5]=-5;
        p_y[6]=-4;

    }
    ++smile_counter;
    double points = 7;
    double as=3.3, r= 0.05;
    create_curve(p_x,p_y,r,as,points,1.5);

}