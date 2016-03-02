//
//  main.cpp
//  VistaOrtogonalglOrtho
//
//  Created by Ma. Guadalupe Roque on 2/18/15.
//  Copyright (c) 2015 em15-2. All rights reserved.
//
//
//  main.cpp
//  VistaOrtogonalGC
//
//  Created by Maria  Roque on 05/09/12.
//  Copyright (c) 2012 Tecnológico de Monterrey. All rights reserved.
//
/* draws the wire frame glut cube */


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>         glut.h includes gl.h and glu.h
#endif
float x=1,y=1,z=1;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3ub(0, 0, 0); // added black
    glBegin(GL_LINES);
    // Eje Y
    glVertex2d(0, -4);
    glVertex2d(0, 4);
    // Eje X
    glVertex2d(-4, 0);
    glVertex2d(4, 0);
    glEnd();
    
    glPushMatrix();
        glColor3d(1.0, 1.0, 1.0);
        glutWireSphere(0.5, 20, 20);
    glPopMatrix();
    
    // raqueta izquierda
    glPushMatrix();
        glTranslated(-3.75, 0, 0);
        glScaled(0.5, 2.0, 0.2);
        glColor3d(0, 0, 0);
        glutSolidCube(1);
        glColor3ub(255, 0, 0); // added red
        glLineWidth(2.0);
        glutWireCube(1);
    glPopMatrix();
    
    // raqueta izquierda
    glPushMatrix();
        glTranslated(3.75, 0, 0);
        glScaled(0.5, 2.0, 0.2);
        glColor3d(0, 0, 0);
        glutSolidCube(1);
        glColor3ub(255, 0, 0); // added red
        glLineWidth(2.0);
        glutWireCube(1);
    glPopMatrix();
    
    glutSwapBuffers();
    
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho(-4.0, 4.0, -4.0, 4.0, 1.0, 4.0);
    glFrustum(-4.0, 4.0, -4.0, 4.0, 1.0, 4.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 1.1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}

void init()
{
    glClearColor (0.2039, 0.6588, 0.3254, 1.0);
    glColor3f(0.0, 0.0, 0.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(0,0);
    glutCreateWindow("cube");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    init();
    //glutTimerFunc(1000, myTimer,   1);
    glutMainLoop();
}







