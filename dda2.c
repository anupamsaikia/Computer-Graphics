// $ gcc cgDda.c -lGL -lGLU -lglut
#include<stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>

void takeInput(void);
void drawCoordinateLine(void); 
void createDisplayWindow(void);
int ADJ = 250;
double X1, Y1, X2, Y2;

void LineDDA(void) {
  double dx=(X2-X1);
  double dy=(Y2-Y1);
  double steps;
  float xInc,yInc,x=X1,y=Y1;

  /* Find out whether to increment x or y */
  steps=(abs(dx)>abs(dy))?(abs(dx)):(abs(dy));
  xInc=dx/(float)steps;
  yInc=dy/(float)steps;

  /* Clears buffers to preset values */
  glClear(GL_COLOR_BUFFER_BIT);
  
  /* Plot the points */
  glBegin(GL_POINTS);

  /* Plot the first point */
  glVertex2d(ADJ+x, ADJ+y);

  int k;
  /* For every step, find an intermediate vertex */
  for(k=0;k<steps;k++){
       x+=xInc;
       y+=yInc;
       /* printf("%0.6lf %0.6lf\n",floor(x), floor(y)); */
       glVertex2d(ADJ+x, ADJ+y);
  }
  glEnd();

  drawCoordinateLine(); 

  glFlush();
}

void createDisplayWindow(void){
    /* Set the initial display mode */
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    /* Set the initial window position and size */
    glutInitWindowPosition(0,0);
    glutInitWindowSize(640,480);

    /* Create the window with title "DDA_Line" */
    glutCreateWindow("DDA_Line");

    
    // /* Initialize drawing colors */
    /* Set clear color to white */
    glClearColor(1.0,1.0,1.0,0);
    /* Set fill color to black */
    glColor3f(0.0,0.0,0.0);
    gluOrtho2D(0 , 640 , 0 , 480);
}

int main(int argc, char *argv[]) {

    takeInput();

    /* Initialise GLUT library */
    glutInit(&argc, argv); 

    
    /* Create the display window */
    createDisplayWindow();
    
    /* Call the displaying function */
    glutDisplayFunc(LineDDA);

    glutMainLoop();
    
    return 0;
}

void takeInput(void){
     printf("Enter two end points of the line to be drawn:\n");
     printf("\n************************************");
     printf("\nEnter Point1( X1 , Y1):\n");
     scanf("%lf%lf",&X1,&Y1);
     printf("\n************************************");
     printf("\nEnter Point1( X2 , Y2):\n");
     scanf("%lf%lf",&X2,&Y2);
}

void drawCoordinateLine(void){
    // next code will draw a line at starting and ending coordinates specified by glVertex3f 
  glBegin(GL_LINES); 
    glVertex3f(ADJ, 50, 0.0f);      // origin of the line 
    glVertex3f(ADJ, ADJ+200, 0.0f); // ending point of the line 
    glVertex3f(50, ADJ, 0.0f);      // origin of the line 
    glVertex3f(ADJ+200, ADJ, 0.0f); // ending point of the line 
  glEnd();
}
