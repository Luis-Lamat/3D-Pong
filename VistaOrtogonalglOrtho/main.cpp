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

#include <stdlib.h>
#include <stdio.h>

/* --------------------------- Global Variables ----------------------------- */

int screenWidth = 500, screenHeight = 500;
float paddleWidth = 1, paddleHeight = 2.0, paddleDepth = 0.1;

// left paddle position
float leftPaddleX = -3.5, leftPaddleY = 0.0;
float leftPaddleTopY = 1.0, leftPaddleBotY = -1.0;

// right paddle postiion
float rightPaddleX = 3.5, rightPaddleY = 0.0;
float rightPaddleTopY = 1.0, rightPaddleBotY = -1.0;

// Frustum value ranges
const float X_MAX = 4.0, X_MIN = -4.0;
const float Y_MAX = 4.0, Y_MIN = -4.0;

// movement
float paddleSpeed = 0.5;


/* ------------------------------- Functions -------------------------------- */

void updateLeftPaddleLoc (float delta) {
    if (leftPaddleTopY + delta <= Y_MAX && leftPaddleBotY + delta >= Y_MIN) {
        leftPaddleTopY += delta;
        leftPaddleBotY += delta;
        leftPaddleY += delta;
    }
}

void updateRightPaddleLoc (float delta) {
    if (rightPaddleTopY + delta <= Y_MAX && rightPaddleBotY + delta >= Y_MIN) {
        rightPaddleTopY += delta;
        rightPaddleBotY += delta;
        rightPaddleY += delta;
    }
}

void drawPaddles () {
    // left paddle
    glPushMatrix();
    glTranslated(leftPaddleX, leftPaddleY, 0);
    glScaled(paddleWidth, paddleHeight, paddleDepth);
    glColor3d(0, 0, 0);
    glutSolidCube(1);
    glColor3ub(255, 0, 0);
    glLineWidth(2.0);
    glutWireCube(1);
    glPopMatrix();
    
    // right paddle
    glPushMatrix();
    glTranslated(rightPaddleX, rightPaddleY, 0);
    glScaled(paddleWidth, paddleHeight, paddleDepth);
    glColor3d(0, 0, 0);
    glutSolidCube(1);
    glColor3ub(255, 0, 0);
    glLineWidth(2.0);
    glutWireCube(1);
    glPopMatrix();
}

void drawGuidlines () {
    // guidelines
    glPushMatrix();
    glScaled(8, 8, paddleDepth + 0.08);
    glColor3d(0, 0, 0);
    glLineWidth(4.0);
    glutWireCube(1);
    glPopMatrix();
    
    glColor3ub(0, 0, 0); // added black
    glBegin(GL_LINES);
    // Eje Y
    glVertex2d(0, -4);
    glVertex2d(0, 4);
    // Eje X
    glVertex2d(-4, 0);
    glVertex2d(4, 0);
    glEnd();
}

void drawBall () {
    glPushMatrix();
    glColor3d(1.0, 1.0, 1.0);
    glLineWidth(1.0);
    glutWireSphere(0.5, 20, 20);
    glPopMatrix();
}

void keyboardPressed (unsigned char theKey, int mouseX, int mouseY) {
    GLint x = mouseX;
    GLint y = screenHeight - mouseY;
    switch (theKey) {
        case 'w':
        case 'W':
            updateLeftPaddleLoc(paddleSpeed);
            glutPostRedisplay();
            break;
            
        case 's':
        case 'S':
            updateLeftPaddleLoc(-paddleSpeed);
            glutPostRedisplay();
            break;
        
        case 'o':
        case 'O':
            updateRightPaddleLoc(paddleSpeed);
            glutPostRedisplay();
            break;
            
        case 'l':
        case 'L':
            updateRightPaddleLoc(-paddleSpeed);
            glutPostRedisplay();
            break;
            
        case 27:
            exit(0);
            
        default:
            break;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawGuidlines();
    drawBall();
    drawPaddles();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    screenWidth = w;
    screenHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho(-4.0, 4.0, -4.0, 4.0, 1.0, 4.0);
    glFrustum(X_MIN, X_MAX, Y_MIN, Y_MAX, 1.0, 4.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 1.1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void init() {
    glClearColor (0.2039, 0.6588, 0.3254, 1.0);
    glColor3f(0.0, 0.0, 0.0);
}

int main(int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(0,0);
    glutCreateWindow("PONG - Luis Lamadrid");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardPressed);
    init();
    //glutTimerFunc(1000, myTimer,   1);
    glutMainLoop();
}







