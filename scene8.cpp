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

GLfloat Tx,Ty,Tz;
GLfloat Ra,Rb,Rc;
GLfloat Sx,Sy,Sz;
GLfloat x,y,z,angle;
#define PI 3.14159265f

double z_a=0;
double z_b=0;
double z_c=0;
double z_d=0;
double ccx=0.1;
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
    if (!ImageLoad("12200.bmp", image1)) {
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
    
/* ---------------------------------------------------------------------- */ 

/* ---------------------------------------------------------------------- */ 

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    
    glBindTexture(GL_TEXTURE_2D, texName1);
    glPushMatrix();
    glTranslatef(Tx, Ty, -18.0f);  
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f( -4.15f, -4.15f,  z_a);
    glTexCoord2f(1.0, 0.0); glVertex3f(4.15f, -4.15f,  z_b);
    glTexCoord2f(1.0, 1.0); glVertex3f(4.15f, 4.15f, z_c);
    glTexCoord2f(0.0, 1.0); glVertex3f( -4.15f, 4.15f, z_d);
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);


    if(z_a>=10){
        z_a=10;
        if(z_b>=10){
            z_b=10;
            if(z_c>=10){
                z_c=10;
                if(z_d>=10){
                    z_d=10;
                }
                z_d+=ccx;
            }
            z_c+=ccx;
        }
        z_b+=ccx;
    }
    z_a+=ccx;

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
    GLfloat position0[] = { 3.8f, 2.0f, -7.5f, 1.0f }; //position of light source 1
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
    glutCreateWindow("Scene 8: Credits");
    Tx = 0.0; Ty = 0.0; Tz = -10.0;

    init();
    glutReshapeFunc (reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display);                  

    glutMainLoop();
    return 0;
}