#ifdef WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <GLUT/GLUT.h>

char *file_name;
GLuint texture[4];
GLubyte *pic;
GLint width;
GLint height;
GLubyte *data_out;
GLubyte *floyd_out;             //initialize an output image for the Floyd algorithm
GLubyte *halfTone_out;          //initialize an output image for the Halftone algorithm
GLubyte *sobel_out;             //initialize an output image for the Edge Detection algorithm

//Floyd-Steinberg dithering algorithm
void floydSteinberg()
{
    floyd_out = new GLubyte[width * height];
    int i ,j, quant_error;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            int oldPixel = data_out[(i * width) + j];
            int newPixel = (oldPixel / 16) * 16;
            floyd_out[(i * width) + j] = newPixel;
            quant_error = oldPixel - newPixel;
            if (i < height - 1 & j < width - 1) {
            floyd_out[(i * width) + j + 1] = data_out[(i * width) + j + 1] + quant_error * (7 / 16);
            floyd_out[i * (width + 1) + (j + 1)] = data_out[i * (width + 1) + (j + 1)] + quant_error * (1 / 16);
            floyd_out[i * (width + 1) + j] = data_out[i * (width+1) + j] + quant_error * (5 / 16);
            floyd_out[i * (width + 1)+ (j - 1)] = data_out[i * (width + 1) + (j - 1)] + quant_error * (3 / 16);
            }
            else if (i < height - 1) {
                floyd_out[i * (width + 1) + j] = data_out[i * (width+1) + j] + quant_error * (5 / 16);
                floyd_out[i * (width + 1)+ (j - 1)] = data_out[i * (width + 1) + (j - 1)] + quant_error * (3 / 16);
            }
            else if (j < width - 1) {
                floyd_out[(i * width) + j + 1] = data_out[(i * width) + j + 1] + quant_error * (7 / 16);
            }
        }
    }
}

//HalfTone Algorithm
void halfTone(){
    halfTone_out = new GLubyte[256*256*4];
    int x, y;
    int i,j;
    int currPixel;
    
    for (i = 0; i < width*2; i++) {
        for (j = 0; j < height*2; j++) {
             halfTone_out[(j*width)+i] = 255;
        }
    }
    
    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            currPixel = data_out[(y*width)+x];
            
            if(currPixel >= 0 && currPixel < 51){
                //top left
                halfTone_out[(y*width*4)+(2*x)] = 0;
                //top right
                halfTone_out[(y*width*4)+(2*x+1)] = 0;
                //bottom left
                halfTone_out[((y+1)*width*4)+(2*width + x*2)] = 0;
                //bottom right
                halfTone_out[((y+1)*width*4)+(2*width + x*2 + 1)] = 0;
            }
            else if (currPixel >= 51 && currPixel < 102){
                //top left
                halfTone_out[(y*width*4)+(2*x)] = 0;
                //top right
                halfTone_out[(y*width*4)+(2*x+1)] = 0;
                //bottom left
                halfTone_out[((y+1)*width*4)+(2*width + x*2)] = 255;
                //bottom right
                halfTone_out[((y+1)*width*4)+(2*width + x*2 + 1)] = 0;
            }
            else if (currPixel >= 102 && currPixel < 153){
                //top left
                halfTone_out[(y*width*4)+(2*x)] = 0;
                //top right
                halfTone_out[(y*width*4)+(2*x+1)] = 255;
                //bottom left
//                halfTone_out[((y+1)*width*4)+(2*width + x*2)] = 255;
                //bottom right
//                halfTone_out[((y+1)*width*4)+(2*width + x*2 + 1)] = 0;
            }
            else if (currPixel >= 153 && currPixel < 202){
                //top left
                halfTone_out[(y*width*4)+(2*x)] = 0;
                //top right
                halfTone_out[(y*width*4)+(2*x + 1)] = 255;
                //bottom left
//                halfTone_out[((y+1)*width*4)+(2*width + x*2)] = 255;
                //bottom right
//                halfTone_out[((y+1)*width*4)+(2*width + x*2 + 1)] = 255;
            }
            else if (currPixel >= 202 && currPixel < 256){
                //top left
                halfTone_out[(y*width*4)+(2*x)] = 255;
                //top right
                halfTone_out[(y*width*4)+(2*x)] = 255;
                //bottom left
//                halfTone_out[((y+1)*width*4)+(2*width + x*2)] = 255;
                //bottom right
//                halfTone_out[((y+1)*width*4)+(2*width + x*2 + 1)] = 255;
            }
            else{
                std::cout<<"Pixel Value Out Of above 255 or below 0"<<std::endl;
                }
        }
    }
}
        

//Sobel edge detection algorithm
void edgeDetect()
{
    sobel_out = new GLubyte[width * height];
    int sumX, sumY;
    int newPixel;
    int i;
    int j;
    int Sx [3][3];
    int Sy [3][3];
    int col;
    int row;
    
    // Sobel Mask, Sx
    Sx[0][0]=-1;    Sx[0][1]=0;     Sx[0][2]=1;
    Sx[1][0]=-2;    Sx[1][1]=0;     Sx[1][2]=2;
    Sx[2][0]=-1;    Sx[2][1]=0;     Sx[2][2]=1;
    
    // Sobel Mask, Sy
    Sy[0][0]=1;     Sy[0][1]=2;     Sy[0][2]=1;
    Sy[1][0]=0;     Sy[1][1]=0;     Sy[1][2]=0;
    Sy[2][0]=-1;    Sy[2][1]=-2;    Sy[2][2]=-1;
    
    for (col = 0; col < width; col++) {
        for (row = 0; row < height; row++) {
            sumX = 0;
            sumY = 0;
            if (height == 0)
                newPixel = 0;
            else if (width == 0)
                newPixel = 0;
            else {
                // Gradient X
                for (i = -1; i <= 1; i++)
                    for (j = -1; j <= 1; j++)
                        sumX += data_out[(row + i) * width + (col + j)] * Sx[i + 1][j + 1];
                // Gradient Y
                for (i = -1; i <= 1; i++)
                    for (j = -1; j <= 1; j++)
                        sumY += data_out[(row + i) * width + (col + j)] * Sy[i + 1][j + 1];
                newPixel = sqrt(pow((double)(sumX / 8), 2.0) + pow((double)(sumY / 8), 2.0));
                if (newPixel > 35)
                    sobel_out[(row * width + col)] = 255;
                else if (newPixel < 0)
                    sobel_out[(row * width + col)] = 0;
            }
        }
    }
}

void init()
{
    FILE *f;
    int picSize;
    size_t rd;
    GLubyte header[54], colorTable[1024]; // where is the color table used?
    glEnable(GL_TEXTURE_2D);
    glOrtho(-1.0, 1.0, -1.0 ,1.0,-1.0,1.0); //????
    glClearColor(0,0,0,0);
    f = fopen(file_name, "rb");
//    f = fopen("/Users/bbenchaya/Documents/CG_ex1/CG_ex1/CG_ex1/lena256.bmp", "rb");
    if (f == NULL) {
        exit(1);
    }
    //image header reading
    fread(header, 54, 1, f);
    if (header[0] != 'B' || header[1] != 'M')
        exit(1);  //not a BMP file
    for (int i = 0; i < 54; i++)
        printf("%x  ", header[i]);
    picSize = (*(int*)(header + 2) - 54);
    width = *(int*)(header + 18);
    height = *(int*)(header + 22);
    printf("\nlena %d %d %d %d \n", picSize, width, height, width * height);
    
    /**********************************/
    
    pic = new GLubyte[width * height];
    data_out = new GLubyte[width * height];
    printf("***** color table *******\n");
    rd = fread(colorTable, 1, 1024, f); //read color table
    for (int i = 0;i < 256 * 4; i++)
        if (i % 4 == 1)
            printf("%x  ", colorTable[i]);
    printf("\n");
    rd = fread(pic, 1, width * height, f); //read image
    fclose(f);
    
    //scale image to 512X512 pixels image
    gluScaleImage(GL_RGB , width, height, GL_UNSIGNED_BYTE, pic, 256, 256, GL_UNSIGNED_BYTE, data_out);
    //----------------initialize Filters------------------//
    floydSteinberg();
    halfTone();
    edgeDetect();
    //----------------Set Textures------------------------//

    glGenTextures(4, texture);
    
    //bottom left - half tone
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width * 2, height * 2, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, halfTone_out);
    
    //bottom right
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, floyd_out);
    //top right - Sobel edge detection
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, sobel_out);
    //top left - grayscale
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, data_out);
    
}

void lenasWindow(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //bottom left
    glViewport(0,0,256,256);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0f, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0f, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0f, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0f, 1.0);
    glEnd();
    
    //bottom right - Floyd-Steinberg dithering
    glViewport(256, 0, 256, 256);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0f, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0f, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0f, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0f, 1.0);
    glEnd();
    
    //top right - sobel edge detection
    glViewport(256, 256, 256, 256);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0f, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0f, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0f, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0f, 1.0);
    glEnd();
    
    //top left - grayscale image
    glViewport(0, 256, 256, 256);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0f, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0f, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0f, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0f, 1.0);
    glEnd();
    
    glFlush();
}

void clearMemory()
{
    delete pic;
    delete data_out;
    delete floyd_out;
    delete halfTone_out;
    delete sobel_out;
}

void outputImageToTextFiles()
{
    FILE *f = fopen("/Users/asafchelouche/programming/CG_ex1/CG_ex1/CG_ex1/test4.txt", "w+");
    if (f == NULL)
        printf("Error: can't open file for writing: test4.txt\n");
    for (int i = 0; i < 256 * 256; i++)
        fprintf(f, "%u,", sobel_out[i] / 255);
    fclose(f);
    f = fopen("/Users/asafchelouche/programming/CG_ex1/CG_ex1/CG_ex1/test5.txt", "w+");
    if (f == NULL)
        printf("Error: can't open file for writing: test5.txt\n");
    for (int i = 0; i < 256 * 256 * 4; i++)
        fprintf(f, "%u,", halfTone_out[i] / 255);
    fclose(f);
    f = fopen("/Users/asafchelouche/programming/CG_ex1/CG_ex1/CG_ex1/test6.txt", "w+");
    if (f == NULL)
        printf("Error: can't open file for writing: test6.txt\n");
    for (int i = 0; i < 256 * 256; i++)
        fprintf(f, "%u,", floyd_out[i] / 16);
    fclose(f);
}

int main(int argc, char **argv)
{
    if (argc != 2){
        printf("Usage: assignment1 <path/to/image>\n");
        exit(1);
    }
    file_name = argv[1];
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(512,512);
    glutCreateWindow("Lena Strikes Again!");
    init();
    outputImageToTextFiles();
    glutDisplayFunc(lenasWindow);
    glutMainLoop();
    clearMemory();
    return 0;
}
