#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
GLfloat texture[1];
int global_counter=0;
double explosion_sphere=0;

float DeltaX=0;
int ParticleCount = 1000;

GLfloat Tx,Ty,Tz;
GLfloat Ra,Rb,Rc;
GLfloat Sx,Sy,Sz;
GLfloat x,y,z,angle;
#define PI 3.14159265f
double tep=0;
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
typedef struct Image Image;

struct PARTICLES
{
double x_coord,y_coord,z_coord;
double delta_x,delta_y;
double Direction;
double PVelocity,NVelocity;
};

PARTICLES * Particle = new PARTICLES[ParticleCount];

static GLuint texName1;

//to draw building
int flag_draw_city=0;
void draw_city(double i, double j){
    glPushMatrix();
    glTranslatef(i,0,j);
    glBegin(GL_QUADS);
    // glColor3f((float)(rand()%10)/10,(float)(rand()%10)/10,(float)(rand()%10)/10);
    glVertex3f(-0.5,0,-5);
    glVertex3f(0.5,0,-5);
    glVertex3f(0.5,3,-5);
    glVertex3f(-0.5,3,-5);
    glEnd();
    glPopMatrix();

}
// PARTICLES* Particle;
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
    if (!ImageLoad("asd.bmp", image1)) {
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
    GLfloat position0[] = { 3.8f, 5.0f, -5.5f, 1.0f }; //position of light source 1
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


glTranslatef(0.0,-2.0,0.0);



/*----------------------------------------------------*/
    // gluLookAt(1.0,1.0,0.0,0.0,1.0,0.0,0.0,1.0,0.0);
// glRotatef(0.0+tep,0.0,1.0,0.0);
// tep+=1;
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

        
    glDisable (GL_BLEND);       

    // if(flag_draw_city==0){
        // for(double i=-5;i<6;++i){
        //     for(double j=-5;j<6;++j){
        //     draw_city(i,j);

        //     }
        //     printf("hi\n");
        // }

    // }
    // flag_draw_city=1;

/*----------------------------------------------------*/
    // glTranslatef(0.0,0.0,-10.0);

    if(global_counter<100){

for (int i = 1; i < ParticleCount; i++)
    {
        glColor3f (1.0, 0.8, 0.5);
        Particle[i].y_coord = Particle[i].y_coord + Particle[i].PVelocity/10 - Particle[i].NVelocity/10;
        Particle[i].NVelocity = Particle[i].NVelocity +0.00009;
        Particle[i].x_coord = Particle[i].x_coord + Particle[i].delta_x;
        Particle[i].z_coord = Particle[i].z_coord + Particle[i].delta_y;
        Particle[i].Direction=Particle[i].Direction+(int)(rand()%11+1);
        Particle[i].Direction=Particle[i].Direction+(float)((rand()%10)*(rand()%10))*0.1;

        if (Particle[i].y_coord < -5)
        {
        Particle[i].x_coord = DeltaX;
        Particle[i].y_coord = -5;
        Particle[i].z_coord = -5;
        Particle[i].PVelocity = (((rand()%50) * rand()%11) + 1) * 0.1;
        }
    }
    DeltaX+=0.001;

    for (int i = 1; i < ParticleCount; i++)
    {
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        glPushMatrix();
        glTranslatef (Particle[i].x_coord, Particle[i].y_coord, Particle[i].z_coord-5);
        // printf("%f %f %f\n",Particle[i].x_coord,Particle[i].y_coord, Particle[i].z_coord );
        glDisable (GL_DEPTH_TEST);
        glEnable (GL_BLEND);       
        glBlendFunc (GL_ONE, GL_ONE);
        glutWireSphere(1.0,15,5);
        glPopMatrix();
    }

}
else if(global_counter==100){
    ParticleCount=2500;
Particle = new PARTICLES[ParticleCount];

    for (int i = 1; i < ParticleCount; i++)
    {
        Particle[i].x_coord = DeltaX;
        Particle[i].y_coord = -5;
        Particle[i].z_coord = -5;
        Particle[i].delta_x = (float)((rand()%10-5)*(rand()%10-5))*0.002;
        Particle[i].delta_y = (float)((rand()%10-5)&rand()%10-5)*0.002;
        Particle[i].Direction = 0;
        Particle[i].PVelocity = float((rand()%10-5)*(rand()%10-5))*0.001;
        Particle[i].NVelocity = float((rand()%10-5)*(rand()%10-5))*0.001;
    }
}
else if(global_counter>100 && global_counter<350){
for (int i = 1; i < ParticleCount; i++)
    {
        glColor3f (1.0, 0.8, 0.5);
        Particle[i].y_coord = Particle[i].y_coord + Particle[i].PVelocity/10 - Particle[i].NVelocity/10;
        Particle[i].NVelocity = Particle[i].NVelocity +0.00009;
        Particle[i].x_coord = Particle[i].x_coord + Particle[i].delta_x;
        Particle[i].z_coord = Particle[i].z_coord + Particle[i].delta_y;
        Particle[i].Direction=Particle[i].Direction+(int)(rand()%11+1);
        Particle[i].Direction=Particle[i].Direction+(float)((rand()%10)*(rand()%10))*0.1;

        if (Particle[i].y_coord < -5)
        {
        Particle[i].x_coord = DeltaX;
        Particle[i].y_coord = -5;
        Particle[i].z_coord = -5;
        Particle[i].PVelocity = (((rand()%50) * rand()%11) + 1) * 0.05;
        }
    }
    DeltaX+=0.001;

    for (int i = 1; i < ParticleCount; i++)
    {
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        glPushMatrix();
        glTranslatef (Particle[i].x_coord, Particle[i].y_coord, Particle[i].z_coord-5);
        glDisable (GL_DEPTH_TEST);
        glEnable (GL_BLEND);       
        glBlendFunc (GL_ONE, GL_ONE);
        glutWireSphere(1.0,15,5);
        glPopMatrix();
    }
    glPushMatrix();
        glDisable (GL_DEPTH_TEST);
     glTranslatef(0.0,0.0,0.0 );

        // glEnable (GL_BLEND);       
        // glBlendFunc (GL_ONE, GL_ONE);
        glColor3f(1.0,1.0,1.0);
        glutWireSphere(0.0+explosion_sphere,500,500);
        glPopMatrix();
        explosion_sphere+=0.03;

}
// else if(global_counter>=350 && global_counter<500){
//      glPushMatrix();
//      glTranslatef(0.0,-10.0,-20 );
//         glDisable (GL_DEPTH_TEST);
//         // glEnable (GL_BLEND);       
//         // glBlendFunc (GL_ONE, GL_ONE);
//         glColor3f(1.0,1.0,1.0);
//         glutWireSphere(10.0+explosion_sphere,500,500);
//         glPopMatrix();
//         explosion_sphere+=0.1;
// }
else{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
++global_counter;
if(global_counter>=360) exit(0);
printf("%d\n",global_counter );
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
        Particle[i].PVelocity = float((rand()%10-5)*(rand()%10-5))*0.001;
        Particle[i].NVelocity = float((rand()%10-5)*(rand()%10-5))*0.001;
    }


    init();
    glutReshapeFunc (reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display);                  
    glutMainLoop();
    return 0;
}