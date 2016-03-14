#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int ParticleCount = 500;
float DeltaX=0;
int overall_counter=0;
float move_x=0,move_z=0;
float move_cx=0,move_cy=0,move_cz=0;
double rotate_hand=0;
double rotate_leg=0,rotate_leg_initial=25,leg_flag=0;
struct PARTICLES
{
double x_coord,y_coord,z_coord;
double delta_x,delta_y;
double Direction;
double PVelocity,NVelocity;
};

PARTICLES * Particle = new PARTICLES[ParticleCount];


GLfloat Tx,Ty,Tz;
GLfloat Ra,Rb,Rc;
GLfloat Sx,Sy,Sz;
GLfloat x,y,z,angle;
#define PI 3.14159265f

double X=0,Y=0;
//next three lines for the smile
int smile_counter=0;
double p_x[7]={-3,-2,-1,0,1,2,3};
double p_y[7]={-6,-3.75,-3.25,-3.0,-3.25,-3.75,-6};

//char smile

double b0(double u) { return (1-u)*(1-u)*(1-u)/6; }
double b1(double u) { return (3*u*u*u - 6*u*u + 4)/6; }
double b2(double u) { return (-3*u*u*u + 3*u*u + 3*u + 1)/6; }
double b3(double u) { return u*u*u/6; }

double tep=0;
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
typedef struct Image Image;

static GLuint texName1, texName2;

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
    if (!ImageLoad("desert_skybox.bmp", image1)) {
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
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0);   
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
    GLfloat position0[] = { 3.8f, 12.0f, 3.5f, 1.0f }; //position of light source 1
    glLightfv(GL_LIGHT0, GL_POSITION, position0);
    glEnable(GL_LIGHT0);//enable light source 1
// 
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


/*----------------------------------------------------*/
    gluLookAt(0.0,1.0,1.0,0.0,1.0,0.0,0.0,1.0,0.0);
    if (overall_counter<=130)
    {
	glRotatef(0.0-tep,0.0,1.0,0.0);
        if(tep>=50)
            {tep=50;}
        tep+=0.5;
    }
    else if(overall_counter>130)
    {
        glRotatef(-50.0,0.0,1.0,0.0);
    }
        overall_counter++;
// edit here
// glTranslatef(0.0,0.0,-tep);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    
    //bot
    glBindTexture(GL_TEXTURE_2D, texName1);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0.35, 0.0); glVertex3f( 6.0f, -0.0f,  6.0f);
    glTexCoord2f(0.5, 0.0); glVertex3f(-6.0f, -0.0f,  6.0f);
    glTexCoord2f(0.5, 0.33); glVertex3f(-6.0f, -0.0f, -6.0f);
    glTexCoord2f(0.35, 0.33); glVertex3f( 6.0f, -0.0f, -6.0f);
    glEnd();
    glPopMatrix();


    //right
    glBindTexture(GL_TEXTURE_2D, texName1);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0.75, 0.43); glVertex3f(6.0f, -0.0f, 6.0f);
    glTexCoord2f(1.0, 0.43); glVertex3f( 6.0f, -0.0f, -6.0f);
    glTexCoord2f(1.0, 0.66); glVertex3f( 6.0f, 6.0f,  -6.0f);
    glTexCoord2f(0.75, 0.66); glVertex3f(6.0f, 6.0f,  6.0f);
    glEnd();
    glPopMatrix();


    //left
    glBindTexture(GL_TEXTURE_2D, texName1);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0.35, 0.33); glVertex3f(-6.0f, -0.0f, -6.0f);
    glTexCoord2f(0.5, 0.33); glVertex3f(-6.0f, -0.0f, 6.0f);
    glTexCoord2f(0.5, 0.66); glVertex3f(-6.0f, 6.0f,  6.0f);
    glTexCoord2f(0.35, 0.66); glVertex3f(-6.0f, 6.0f,  -6.0f);
    glEnd();
    glPopMatrix();


    //back
    glBindTexture(GL_TEXTURE_2D, texName1);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.43); glVertex3f( -6.0f, -0.0f, 6.0f);
    glTexCoord2f(0.25, 0.43); glVertex3f( 6.0f, -0.0f,  6.0f);
    glTexCoord2f(0.25, 0.66); glVertex3f(6.0f, 6.0f,  6.0f);
    glTexCoord2f(0.0, 0.66); glVertex3f(-6.0f, 6.0f, 6.0f);
    glEnd();
    glPopMatrix();

    //top
    glBindTexture(GL_TEXTURE_2D, texName1);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0.35, 0.6); glVertex3f(-6.0f, 6.0f,  -6.0f);
    glTexCoord2f(0.45, 0.6); glVertex3f( 6.0f, 6.0f,  -6.0f);
    glTexCoord2f(0.45, 1.0); glVertex3f( 6.0f, 6.0f, 6.0f);
    glTexCoord2f(0.35, 1.0); glVertex3f(-6.0f, 6.0f, 6.0f);
    glEnd();
    glPopMatrix();

    //front
    glBindTexture(GL_TEXTURE_2D, texName1);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0.5, 0.43); glVertex3f( -6.0f, -0.0f, -6.0f);
    glTexCoord2f(0.75, 0.43); glVertex3f( 6.0f, -0.0f,  -6.0f);
    glTexCoord2f(0.75, 0.66); glVertex3f(6.0f, 6.0f,  -6.0f);
    glTexCoord2f(0.5, 0.66); glVertex3f(-6.0f, 6.0f, -6.0f);
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

/*----------------------------------------------------*/
//Smiling Character
    if(overall_counter>130)
    {
        glRotatef(20,0.0,1.0,0.0);
        glTranslatef(0.0,0.0,0.0+move_z*move_z);
        move_x+=0.03;
        move_z+=0.02;
    }

    glScaled(0.2,0.2,0.2);
    glTranslatef(0.0,6.0,-10.0);

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
    if(overall_counter>130){
        if(leg_flag==0){
            glRotatef(-rotate_leg_initial+rotate_leg,1.0,0.0,0.0);
            rotate_leg+=5/2;
            if(rotate_leg>=50) {leg_flag=1;rotate_leg=0;}
        }
        else{
            glRotatef(rotate_leg_initial-rotate_leg,1.0,0.0,0.0);
            rotate_leg+=5/2;
            if(rotate_leg>=50) {leg_flag=0; rotate_leg=0;}
        }
    }
    GLUquadric *thigh_1 = gluNewQuadric();           
    gluCylinder(thigh_1,0.5,0.5,3.0,10,10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.5, 0.14); 
    glTranslatef(1.0f,-3.0f,0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    if(overall_counter>130){
        if(leg_flag==0){
            glRotatef(rotate_leg_initial-rotate_leg,1.0,0.0,0.0);
            rotate_leg+=5/2;
            if(rotate_leg>=50) {leg_flag=1;rotate_leg=0;}
        }
        else{
            glRotatef(-rotate_leg_initial+rotate_leg,1.0,0.0,0.0);
            rotate_leg+=5/2;
            if(rotate_leg>=50) {leg_flag=0; rotate_leg=0;}
        }
    }
    GLUquadric *thigh_2 = gluNewQuadric();           
    gluCylinder(thigh_2,0.5,0.5,3.0,10,10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.5, 0.14); 
    glTranslatef(1.8,-0.5,0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    if(overall_counter>130){
        glRotatef(-rotate_hand,1.0,0.0,0.0);
        rotate_hand+=10;
        if(rotate_hand>90){rotate_hand=90;}
    }
    GLUquadric *hand_1 = gluNewQuadric();           
    gluCylinder(hand_1,0.3,0.3,3.0,10,10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.5, 0.14); 
    glTranslatef(-1.8,-0.5,0.0f);
    glRotatef(90, 1.0f, 0.0f, 0.0f);
    if(overall_counter>130){
        glRotatef(-rotate_hand,1.0,0.0,0.0);
        rotate_hand+=10;
        if(rotate_hand>90){rotate_hand=90;}
    }
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
    if(overall_counter>125 && overall_counter < 150) gluCylinder(eye_1,0.2,0.4,2.0,10,10);
    else gluCylinder(eye_1,0.2,0.4,0.5,10,10);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0); 
    glTranslatef(-0.7,0.0,1.3f);
    GLUquadric *eye_2 = gluNewQuadric(); 
    if(overall_counter>125 && overall_counter <150) gluCylinder(eye_2,0.2,0.4,2.0,10,10);
    else gluCylinder(eye_2,0.2,0.4,0.5,10,10);
    glPopMatrix();


    //for the smile of the character
    
    if(smile_counter>130){
        p_y[0]=p_y[0]+0.3;
        p_y[1]=p_y[1]-0.125;
        p_y[2]=p_y[2]-0.175;
        p_y[3]=p_y[3]+0.0;
        p_y[4]=p_y[4]-0.375;
        p_y[5]=p_y[5]-0.325;
        p_y[6]=p_y[6]+0.3;

        p_x[0]=p_x[0]+0.0;
        p_x[1]=p_x[1]+0.05;
        p_x[2]=p_x[2]+0.25;
        p_x[3]=p_x[3]+0.3;
        p_x[4]=p_x[4]+0.05;
        p_x[5]=p_x[5]-0.35;
        p_x[6]=p_x[6]-0.6;

    }
    if(smile_counter>140){

        p_x[0]=-3;
        p_x[1]=-1.5;
        p_x[2]=1.5;
        p_x[3]=3;
        p_x[4]=1.5;
        p_x[5]=-1.5;
        p_x[6]=-3.1;

        p_y[0]=-3;
        p_y[1]=-5;
        p_y[2]=-5;
        p_y[3]=-3;
        p_y[4]=-7;
        p_y[5]=-7;
        p_y[6]=-3.1;

    }

    // p_x[7]={-3,-2,-1,0,1,2,3};
    // p_y[7]={-6,-3.75,-3.25,-3.0,-3.25,-3.75,-6};

    // p_x[7]={-3,-1.5,1.5,3,1.5,-1.5,-3.1};
    // p_y[7]={-3,-5,-5,-3,-7,-7,-3.1};
    double points = 7;
    double x,y,as=3.3, r= 0.05;
    for (int i = 0; i < points; ++i)
    {
        glPushMatrix();  
        glTranslatef(p_x[i]/as, p_y[i]/as, 0.0f);

        glutWireSphere(r,30, 30);
        glPopMatrix();
    }
    for(int i = -1;i<points-2;++i){
        for(double u = 0;u<1;u+=0.005){
            X = b0(u)*p_x[i] + b1(u)*p_x[i+1] + b2(u)*p_x[i+2] + b3(u)*p_x[i+3];
            Y = b0(u)*p_y[i] + b1(u)*p_y[i+1] + b2(u)*p_y[i+2] + b3(u)*p_y[i+3];
            glPushMatrix();
            // glPointSize(15);
            glTranslatef(X/as, Y/as, 1.5f);
              glutWireSphere(r,30, 30);
            glPopMatrix();
       }
    }
    ++smile_counter;

/*----------------------------------------------------*/

    glPopMatrix();
    glScalef(3.0,3.0,3.0);
    glTranslatef(-12.0,10,10);
    if(overall_counter>130)
    {
        // glRotatef(-50,0.0,1.0,0.0);
        glTranslatef(0.0+move_x,0.0,0.0-move_cz*move_cz);
        move_cx+=0.05;
        move_cz+=0.02;
    }
    glPopMatrix();
    for (int i = 1; i < ParticleCount; i++)
    {
        glColor3f (1.0, 1.0,1.0);
        Particle[i].y_coord = Particle[i].y_coord + Particle[i].PVelocity/10 - Particle[i].NVelocity/10;
        Particle[i].NVelocity = Particle[i].NVelocity +0.0075;
        Particle[i].x_coord = Particle[i].x_coord + Particle[i].delta_x;
        Particle[i].z_coord = Particle[i].z_coord + Particle[i].delta_y;
        Particle[i].Direction=Particle[i].Direction+(int)(rand()%11+1);
        Particle[i].Direction=Particle[i].Direction+(float)((rand()%10)*(rand()%10))*0.1;

        if (Particle[i].y_coord < -5)
        {
        Particle[i].x_coord = DeltaX;
        Particle[i].y_coord = -5;
        Particle[i].z_coord = -5;
        Particle[i].PVelocity = (((rand()%50) * rand()%11) + 1) * 0.01;
        Particle[i].NVelocity = (((rand()%50) * rand()%11) + 1) * 0.01;
        }
    }
    DeltaX+=0.05;
    for (int i = 1; i < ParticleCount; i++)
    {
        glPushMatrix();
        glTranslatef (Particle[i].x_coord, Particle[i].y_coord, Particle[i].z_coord);
        glutSolidSphere(0.6,10,10);            
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
    }
    //-----------------------------------

if(overall_counter>260) exit(0);
// printf("%d\n",overall_counter );
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();   
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
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutInitWindowPosition(100, 100);
    
    glutCreateWindow("Assignment 2 : Lightening, Shading, Illumination and Shading");
    // Particle=new PARTICLES[ParticleCount];
    srand(time(NULL));
    for (int i = 1; i < ParticleCount; i++)
    {
        Particle[i].x_coord = DeltaX;
        Particle[i].y_coord = -5;
        Particle[i].z_coord = -5;
        Particle[i].delta_x = (float)((rand()%10-5)*(rand()%10-5))*0.002;
        Particle[i].delta_y = (float)((rand()%10-5)&rand()%10-5)*0.002;
        Particle[i].Direction = 0;
        Particle[i].PVelocity = float((rand()%1-0.5)*(rand()%1-0.5))*0.001;
        Particle[i].NVelocity = float((rand()%1-0.5)*(rand()%1-0.5))*0.001;
    }
    init();
    glutReshapeFunc (reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display);                  
    glutMainLoop();
    return 0;
}