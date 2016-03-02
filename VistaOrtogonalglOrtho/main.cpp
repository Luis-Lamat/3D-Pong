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

// screen and object sizes
int screenWidth = 500, screenHeight = 500;
float paddleWidth = 1, paddleHeight = 2.0, paddleDepth = 0.1;
float ballRadius = 0.5, ballWidth = 1.0, ballHeight = 1.0, ballDepth = 1.0;

// left paddle position
float leftPaddleX = -3.5, leftPaddleY = 0.0, leftPaddleFaceX = -3;
float leftPaddleTopY = 1.0, leftPaddleBotY = -1.0;

// right paddle postiion
float rightPaddleX = 3.5, rightPaddleY = 0.0, rightPaddleFaceX = 3;
float rightPaddleTopY = 1.0, rightPaddleBotY = -1.0;

// ball position and direction
float ballX = 1.0, ballY = 0.0;
int ballDirection = 1; // a number for each cartesian quadrant
// row 0 is for x coord. direction changes and row 1 for y-coord. changes
int directionVector[2][5] = {{0, 1, -1, -1, 1}, {0, 1, 1, -1, -1}};
// row 0 is for x-changes (y collision) and row 1 for y-changes (x-collision)
int directionChange[2][5] = {{0, 2,  1,  4, 3}, {0, 4, 3,  2,  1}};

// Frustum value ranges
const float X_MAX = 4.0, X_MIN = -4.0;
const float Y_MAX = 4.0, Y_MIN = -4.0;

// movement
float paddleSpeed = 0.5, ballSpeed = 0.1;

// game state definition
typedef enum { PLAYING, PAUSED, GAME_OVER } State;
State gameState = PAUSED;


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

void updateBallLoc (float delta){
    // checking for left or right paddle collisions
    if ((ballY <= leftPaddleTopY && ballY >= leftPaddleBotY &&
         ballX - ballRadius <= leftPaddleFaceX) ||
        (ballY <= rightPaddleTopY && ballY >= rightPaddleBotY &&
         ballX + ballRadius >= rightPaddleFaceX)) {
        printf("Paddle Collision!!!\n");
        ballDirection = directionChange[0][ballDirection];
    } else if ((ballX + ballRadius >= X_MAX) || (ballX - ballRadius <= X_MIN)) {
        printf("Left / Right Collision!!!\n");
        ballDirection = directionChange[0][ballDirection];
    } else if ((ballY + ballRadius >= Y_MAX) || (ballY - ballRadius <= Y_MIN)) {
        printf("Top / Down Collision!!!\n");
        ballDirection = directionChange[1][ballDirection];
    }

    // updating the paddle's position
    ballX += (delta * directionVector[0][ballDirection]);
    ballY += (delta * directionVector[1][ballDirection]);
}

void resetBallLoc() {
    ballX = 0.0; ballY = 0.0;
}

void startGame (int startPosition) {
    gameState = PLAYING;
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
    glTranslated(ballX, ballY, 0);
    glScaled(ballWidth, ballHeight, ballDepth);
    glColor3d(1.0, 1.0, 1.0);
    glutSolidSphere(ballRadius, 20, 20);
    glColor3d(0.0, 0.0, 1.0);
    glLineWidth(1.0);
    glutWireSphere(ballRadius, 20, 20);
    glPopMatrix();
}

void arrowKeysPressed (int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_DOWN:
            updateRightPaddleLoc(-paddleSpeed);
            glutPostRedisplay();
            break;
            
        case GLUT_KEY_UP:
            updateRightPaddleLoc(paddleSpeed);
            glutPostRedisplay();
            break;
        
        default: break;
    }
}

void keyboardPressed (unsigned char key, int mouseX, int mouseY) {
    GLint x = mouseX;
    GLint y = screenHeight - mouseY;
    switch (key) {
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
            
        case 'i':
        case 'I':
            startGame(0);
            break;
            
        case 27: exit(0);
            
        default: break;
    }
}

void gameTimer (int value){
    if (gameState == PLAYING) {
        updateBallLoc(ballSpeed);
    } else if (gameState == GAME_OVER) {
        resetBallLoc();
    }
    glutPostRedisplay();
    glutTimerFunc(100, gameTimer, 1);
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
    glOrtho(-4.0, 4.0, -4.0, 4.0, 1.0, 4.0);
    // glFrustum(X_MIN, X_MAX, Y_MIN, Y_MAX, 1.0, 4.0);
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
    glutSpecialFunc(arrowKeysPressed);
    init();
    glutTimerFunc(100, gameTimer, 1);
    glutMainLoop();
}







