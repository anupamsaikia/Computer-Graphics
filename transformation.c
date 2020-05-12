/**
 * Program written by:
 * Name: Anupam Saikia
 * Roll Number: GAU-C-17/064
 * B.Tech CSE 6th Sem
 * Central Institute of Technology, Kokrajhar
 * ___________________________________________
 * 
 * To install required libraries:
 * "sudo apt install mesa-common-dev libglu1-mesa-dev freeglut3-dev"
 * 
 * To compile: "gcc transformation.c -lGL -lGLU -lglut -lm"
 * 
 * To run program: "./a.out"
 * 
 **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define PI 3.14159265

/* Global variables are used beacause of a limitation in OpenGL*/
int vertexCount;
double vertexMatrix[3][10];
double resultMatrix[3][10];

/**************************** Helper Functions ********************************/

/**
 * Multiply two 2D matrices
 * Operates based on global variable 'vertexCount'
 * Multiply 'mat1' & 'mat2'
 * Result is stored in 'res'
 **/
void multiply(double mat1[3][3], double mat2[3][10], double res[3][10])
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < vertexCount; j++)
        {
            res[i][j] = 0;
            for (int k = 0; k < 3; k++)
                res[i][j] += mat1[i][k] * mat2[k][j];
        }
}

// Clear Infut buffer
void clearInputBuffer()
{
    while ((getchar()) != '\n')
        ;
}

void displayMatrix(double mat[3][10])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < vertexCount; j++)
            printf("%g\t", mat[i][j]);
        printf("\n");
    }
}

/**************************** Transformation Functions ********************************/

/**
 * Translate on x-axis & y-axis by given amount
 * valMatrix = Matrix on which translation will be applied
 * resMatrix = Matrix where the result will be stored
 **/
void translate(double x, double y, double valMatrix[3][10], double resMatrix[3][10])
{
    // Initialize transformation matrix for translation
    double transMatrix[3][3];
    transMatrix[0][0] = 1;
    transMatrix[0][1] = 0;
    transMatrix[0][2] = x;
    transMatrix[1][0] = 0;
    transMatrix[1][1] = 1;
    transMatrix[1][2] = y;
    transMatrix[2][0] = 0;
    transMatrix[2][1] = 0;
    transMatrix[2][2] = 1;

    multiply(transMatrix, valMatrix, resMatrix);
}

/**
 * Rotate by given degree around the first point of the object
 * valMatrix = Matrix on which rotation will be applied
 * resMatrix = Matrix where the result will be stored
 **/
void rotateAroundFirstPoint(double degree, double valMatrix[3][10], double resMatrix[3][10])
{
    double radian = degree * PI / 180.0;

    // translate the object to origin w.r.t first point
    double tempMatrix1[3][10];
    translate(-valMatrix[0][0], -valMatrix[1][0], valMatrix, tempMatrix1);

    // Initialize transformation matrix for rotation
    double transMatrix[3][3];
    transMatrix[0][0] = cos(radian);
    transMatrix[0][1] = -sin(radian);
    transMatrix[0][2] = 0;
    transMatrix[1][0] = sin(radian);
    transMatrix[1][1] = cos(radian);
    transMatrix[1][2] = 0;
    transMatrix[2][0] = 0;
    transMatrix[2][1] = 0;
    transMatrix[2][2] = 1;

    //rotate
    double tempMatrix2[3][10];
    multiply(transMatrix, tempMatrix1, tempMatrix2);

    // translate the object back to original position
    translate(valMatrix[0][0], valMatrix[1][0], tempMatrix2, resMatrix);
}

/**
 * Rotate by given degree around the origin
 * valMatrix = Matrix on which rotation will be applied
 * resMatrix = Matrix where the result will be stored
 **/
void rotateAroundOrigin(double degree, double valMatrix[3][10], double resMatrix[3][10])
{
    double radian = degree * PI / 180.0;

    // Initialize transformation matrix for rotation
    double transMatrix[3][3];
    transMatrix[0][0] = cos(radian);
    transMatrix[0][1] = -sin(radian);
    transMatrix[0][2] = 0;
    transMatrix[1][0] = sin(radian);
    transMatrix[1][1] = cos(radian);
    transMatrix[1][2] = 0;
    transMatrix[2][0] = 0;
    transMatrix[2][1] = 0;
    transMatrix[2][2] = 1;

    multiply(transMatrix, valMatrix, resMatrix);
}

/**
 * Scale by given amount
 * valMatrix = Matrix on which scaling will be applied
 * resMatrix = Matrix where the result will be stored
 **/
void scale(double x, double y, double valMatrix[3][10], double resMatrix[3][10])
{
    // Initialize transformation matrix
    double transMatrix[3][3];
    transMatrix[0][0] = x;
    transMatrix[0][1] = 0;
    transMatrix[0][2] = 0;
    transMatrix[1][0] = 0;
    transMatrix[1][1] = y;
    transMatrix[1][2] = 0;
    transMatrix[2][0] = 0;
    transMatrix[2][1] = 0;
    transMatrix[2][2] = 1;

    multiply(transMatrix, valMatrix, resMatrix);
}

/**
 * Shear by given amount
 * valMatrix = Matrix on which shear will be applied
 * resMatrix = Matrix where the result will be stored
 **/
void shear(double x, double y, double valMatrix[3][10], double resMatrix[3][10])
{
    // Initialize transformation matrix
    double transMatrix[3][3];
    transMatrix[0][0] = 1;
    transMatrix[0][1] = x;
    transMatrix[0][2] = 0;
    transMatrix[1][0] = y;
    transMatrix[1][1] = 1;
    transMatrix[1][2] = 0;
    transMatrix[2][0] = 0;
    transMatrix[2][1] = 0;
    transMatrix[2][2] = 1;

    multiply(transMatrix, valMatrix, resMatrix);
}

/**
 * Reflect across a given line with points (x1,y1) & (x2,y2)
 * valMatrix = Matrix on which reflection will be applied
 * resMatrix = Matrix where the result will be stored
 **/
void reflect(double x1, double y1, double x2, double y2, double valMatrix[3][10], double resMatrix[3][10])
{
    // calculate degree
    double slope, degree;
    if (x2 - x1 != 0)
    {
        slope = (y2 - y1) / (x2 - x1);
        degree = (atan(slope) * 180) / PI;
    }
    else
    {
        degree = 90.0;
    }

    // translate to origin
    double tempMatrix1[3][10];
    translate(-x1, -y1, valMatrix, tempMatrix1);

    // rotate to align w.r.t x-axis
    double tempMatrix2[3][10];
    rotateAroundOrigin(-degree, tempMatrix1, tempMatrix2);

    // reflect along x-axis
    double transMatrix[3][3];
    transMatrix[0][0] = 1;
    transMatrix[0][1] = 0;
    transMatrix[0][2] = 0;
    transMatrix[1][0] = 0;
    transMatrix[1][1] = -1;
    transMatrix[1][2] = 0;
    transMatrix[2][0] = 0;
    transMatrix[2][1] = 0;
    transMatrix[2][2] = 1;
    multiply(transMatrix, tempMatrix2, tempMatrix1);

    // finally rotate and translate back
    rotateAroundOrigin(degree, tempMatrix1, tempMatrix2);
    translate(x1, y1, tempMatrix2, resMatrix);
}
/*************************************************************************************/
/** Graphics Helper functions ********************************************************/

/** Function to draw the cartesian plane */
void drawCartesianPlane()
{
    glBegin(GL_LINES);
    glColor3f(0.1f, 0.1f, 0.1f);
    for (double i = -1; i < 1; i += 0.1)
    {
        glVertex2d(-1.0f, i);
        glVertex2d(1.0f, i);
        glVertex2d(i, -1.0f);
        glVertex2d(i, 1.0f);
    }
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.8f);
    glVertex2d(0.0f, 1.0f);
    glVertex2d(0.0f, -1.0f);
    glVertex2d(-1.0f, 0.0f);
    glVertex2d(1.0f, 0.0f);
    glEnd();
}

/** Function to draw the original & transformed figure*/
void drawFigure()
{
    // prepare the x & y values of original & new figure for plotting
    double xValuesOrg[vertexCount], yValuesOrg[vertexCount], xValuesNew[vertexCount], yValuesNew[vertexCount];
    for (int i = 0; i < vertexCount; i++)
    {
        xValuesOrg[i] = vertexMatrix[0][i] / 200;
        yValuesOrg[i] = vertexMatrix[1][i] / 200;
        xValuesNew[i] = resultMatrix[0][i] / 200;
        yValuesNew[i] = resultMatrix[1][i] / 200;
    }

    int code;
    switch (vertexCount)
    {
    case 1:
        code = GL_POINTS;
        break;
    case 2:
        code = GL_LINES;
        break;
    case 3:
        code = GL_TRIANGLES;
        break;
    case 4:
        code = GL_QUADS;
        break;
    default:
        code = GL_POLYGON;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendColor(1.0f, 1.0f, 0.0f, 1.0f);

    glBegin(code);
    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < vertexCount; i++)
        glVertex2d(xValuesOrg[i], yValuesOrg[i]);
    glEnd();

    glBegin(code);
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int i = 0; i < vertexCount; i++)
        glVertex2d(xValuesNew[i], yValuesNew[i]);
    glEnd();
}

/* OpenGL display function */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawCartesianPlane();
    drawFigure();

    //render
    glFlush();
}

/* Initialize OpenGL Graphics */
void initGL()
{
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
}

int main(int argc, char *argv[])
{
    printf("\nEnter number of vertices (1-10): ");
    scanf("%d", &vertexCount);
    clearInputBuffer();
    if (vertexCount < 1 || vertexCount > 10)
    {
        printf("Allowed values: (1-10)\n");
        exit(0);
    }

    // Get the vertices from user
    double xValues[10], yValues[10];
    printf("Enter coordinate values below:\n");
    for (int i = 0; i < vertexCount; i++)
    {
        printf("x value of point %d: ", i + 1);
        scanf("%lf", &xValues[i]);
        clearInputBuffer();
        printf("y value of point %d: ", i + 1);
        scanf("%lf", &yValues[i]);
        clearInputBuffer();
    }

    // initialize the vertex matrix
    for (int i = 0; i < vertexCount; i++)
    {
        vertexMatrix[0][i] = xValues[i];
        vertexMatrix[1][i] = yValues[i];
        vertexMatrix[2][i] = 1;
    }

    char ch;
    printf("1.Translate\n2.Rotate\n3.Scale\n4.Shear\n5.Reflect\n");
    printf("Select an option (1-5): ");
    ch = getchar();
    clearInputBuffer();

    switch (ch)
    {
    case '1':
    {
        double x, y;
        printf("Enter translation amount on x-axis: ");
        scanf("%lf", &x);
        clearInputBuffer();
        printf("Enter translation amount on y-axis: ");
        scanf("%lf", &y);
        clearInputBuffer();
        translate(x, y, vertexMatrix, resultMatrix);
        break;
    }
    case '2':
    {
        double degree;
        printf("Enter rotation amount in degree: ");
        scanf("%lf", &degree);
        clearInputBuffer();
        rotateAroundFirstPoint(degree, vertexMatrix, resultMatrix);
        break;
    }
    case '3':
    {
        double x, y;
        printf("Enter scale amount on x-axis: ");
        scanf("%lf", &x);
        clearInputBuffer();
        printf("Enter scale amount on y-axis: ");
        scanf("%lf", &y);
        clearInputBuffer();
        scale(x, y, vertexMatrix, resultMatrix);
        break;
    }
    case '4':
    {
        double x, y;
        printf("Enter shear amount on x-axis: ");
        scanf("%lf", &x);
        clearInputBuffer();
        printf("Enter shear amount on y-axis: ");
        scanf("%lf", &y);
        clearInputBuffer();
        shear(x, y, vertexMatrix, resultMatrix);
        break;
    }
    case '5':
    {
        double x1, y1, x2, y2;
        printf("\nEnter the line of reflection :\n");
        printf("x1 : ");
        scanf("%lf", &x1);
        clearInputBuffer();
        printf("y1 : ");
        scanf("%lf", &y1);
        clearInputBuffer();
        printf("x2 : ");
        scanf("%lf", &x2);
        clearInputBuffer();
        printf("y2 : ");
        scanf("%lf", &y2);
        clearInputBuffer();
        reflect(x1, y1, x2, y2, vertexMatrix, resultMatrix);
        break;
    }
    default:
        break;
    }

    printf("\nOriginal Matrix:\n");
    displayMatrix(vertexMatrix);
    printf("\nResult Matrix:\n");
    displayMatrix(resultMatrix);

    /************************** Graphics stuff *********************************/
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("1 unit = 20 | Original:Red | New:Green | Overlap: Yellow");
    glutDisplayFunc(display);
    initGL();
    glutMainLoop();
    return 0;
}
