#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <GLUT/GLUT.h>

GLuint texture[4];
GLuint bitTex;
GLubyte *pic;
GLint width;
GLint height;
GLubyte *data_out;


//char* filename;

/*void floydSteinberg(GLuint texture){
 GLdouble fls_a = 7/16;
 GLdouble fls_b = 1/16;
 GLdouble fls_g = 5/16;
 GLdouble fls_d = 3/16;
 int x;
 int y;
 gl_pixel
 for (x = 0; x < width; x++) {
 for (y = 0; y < height; y++) {
 P(x,y) = trunc(I(x,y) + 0.5)
 e = I(x,y) - P(x,y)
 I(x,y+1) += fls_a*e;
 I(x+1,y-1) += fls_b*e;
 I(x+1,y) += fls_g*e;
 I(x+1,y+1) += fls_d*e;
 
 }*/


void init()
{
    FILE *f;
    int picSize;
    size_t rd;
    GLubyte header[54],colorTable[1024];
    glEnable(GL_TEXTURE_2D);
    glOrtho(-1.0, 1.0, -1.0 ,1.0,-1.0,1.0); //????
    
    glClearColor(0,0,0,0);
    
    //f=fopen(filename,"rb");
    f=fopen("/Users/bbenchaya/Downloads/lena256.bmp","rb");
    
    //image header reading
    fread(header,54,1,f);
    if(header[0]!='B' || header[1]!='M')
        exit(1);  //not a BMP file
    for(int i=0;i<54;i++)
        printf("%x  ",header[i]);
    
    picSize=(*(int*)(header+2)-54);
    width=*(int*)(header+18);
    height=*(int*)(header+22);
    printf("\nlena %d %d %d %d \n",picSize,width,height,width*height);
    
    /**********************************/
    
    pic=new GLubyte[width*height];
    data_out=new GLubyte[width*height];
    printf("***** color table *******\n");
    rd=fread(colorTable,1,1024,f); //read color table
    for(int i=0;i<256*4;i++)
        if(i%4==1)
            printf("%x  ",colorTable[i]);
    printf("\n");
    
    rd=fread(pic,1,width*height,f); //read image
    
    printf("***** %zu *******\n",rd);
    
    fclose(f);
    
    //scale image to 512X512 pixels image
    gluScaleImage(GL_RGB , width, height,GL_UNSIGNED_BYTE, pic, 256*3, 256*3,GL_UNSIGNED_BYTE, data_out);
    //**************************************
    printf("texture\n");
    
    
    glGenTextures(4, texture);
    
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, pic);
    
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, pic);
    
    
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, pic);
    
    
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, pic);
    
}

void mydisplay(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //bottom left
    glViewport(0,0,256,256);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0 ,-1.0f,1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0 , -1.0f,1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0 , 1.0f,1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0 , 1.0f,1.0);
    glEnd();
    
    //bottom right
    glViewport(256,0,256,256);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0 ,-1.0f,1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0 , -1.0f,1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0 , 1.0f,1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0 , 1.0f,1.0);
    glEnd();
    
    
    //top right
    glViewport(256,256,256,256);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0 ,-1.0f,1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0 , -1.0f,1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0 , 1.0f,1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0 , 1.0f,1.0);
    glEnd();
    
    //top left
    glViewport(0,256,256,256);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0 ,-1.0f,1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0 , -1.0f,1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0 , 1.0f,1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0 , 1.0f,1.0);
    glEnd();
    
    
    glFlush();
}


int main(int  argc,  char** argv)
{
    //filename = argv[1];
    glutInit (& argc, argv) ;
    glutInitDisplayMode (GLUT_SINGLE) ;
    glutInitWindowSize (512,512) ;
    glutCreateWindow("Lena times four") ;
    init();
    
    glutDisplayFunc(mydisplay) ;
    
    glutMainLoop () ;
    
    delete(pic);
}
