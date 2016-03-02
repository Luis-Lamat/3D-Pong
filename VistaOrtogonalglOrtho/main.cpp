//
//  main.cpp
//  Pong
//
//  Created by Luis Lamadrid on 03/01/16.
//  Copyright (c) 2016 Lamadrid. All rights reserved.
//

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
float ballRadius = 0.5, ballWidth = 1.0, ballHeight = 1.0, ballDepth = 0.1;

// left paddle position
float leftPaddleX = -3.5, leftPaddleY = 0.0;
float leftPaddleFaceX = leftPaddleX + (paddleWidth/2); // special
float leftPaddleTopY = 1.0, leftPaddleBotY = -1.0;

// right paddle postiion
float rightPaddleX = 3.5, rightPaddleY = 0.0;
float rightPaddleFaceX = rightPaddleX - (paddleWidth/2); // special
float rightPaddleTopY = 1.0, rightPaddleBotY = -1.0;

// ball position and direction
float ballX = 0.0, ballY = 0.0;
int ballDirection = 1; // a number for each cartesian quadrant
// row 0 is for x coord. direction changes and row 1 for y-coord. changes
int directionVector[2][5] = {{0, 1, -1, -1, 1}, {0, 1, 1, -1, -1}};
// row 0 is for x-changes (y collision) and row 1 for y-changes (x-collision)
int directionChange[2][5] = {{0, 2,  1,  4, 3}, {0, 4, 3,  2,  1}};

// Frustum value ranges
const float X_MAX = 4.0, X_MIN = -4.0;
const float Y_MAX = 4.0, Y_MIN = -4.0;

// movement
float paddleSpeed = 0.4, ballSpeed = 0.1;

// game mechanics
typedef enum { START, PLAYING, PAUSED, GAME_OVER } State;
State gameState = START;
int p1Points = 0, p2Points = 0;


/* ------------------------------- Functions -------------------------------- */

void updateLeftPaddleLoc (float delta) {
    if (leftPaddleTopY + delta <= Y_MAX && leftPaddleBotY + delta >= Y_MIN &&
        gameState == PLAYING) {
        leftPaddleTopY += delta;
        leftPaddleBotY += delta;
        leftPaddleY += delta;
    }
}

void updateRightPaddleLoc (float delta) {
    if (rightPaddleTopY + delta <= Y_MAX && rightPaddleBotY + delta >= Y_MIN &&
        gameState == PLAYING) {
        rightPaddleTopY += delta;
        rightPaddleBotY += delta;
        rightPaddleY += delta;
    }
}

void resetBallLoc() {
    ballX = 0.0; ballY = 0.0;
}

void resetPaddlesLoc() {
    leftPaddleY = 0.0; rightPaddleY = 0.0;
    leftPaddleTopY = 1.0, leftPaddleBotY = -1.0;
    rightPaddleTopY = 1.0, rightPaddleBotY = -1.0;
}

void awardPoints (float ballX){
    if (ballX > 0) {
        ballDirection = 2;
        p1Points++;
    } else {
        ballDirection = 1;
        p2Points++;
    }
}

void resetGame () {
    gameState = START;
    ballDirection = 1;
    p1Points = p2Points = 0;
    resetBallLoc();
    resetPaddlesLoc();
    glutPostRedisplay();
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
        awardPoints(ballX);
        resetBallLoc();
    } else if ((ballY + ballRadius >= Y_MAX) || (ballY - ballRadius <= Y_MIN)) {
        printf("Top / Down Collision!!!\n");
        ballDirection = directionChange[1][ballDirection];
    }

    // updating the paddle's position
    ballX += (delta * directionVector[0][ballDirection]);
    ballY += (delta * directionVector[1][ballDirection]);
}

void draw3dString (void *font, char *str, float x, float y, float scale) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScaled(scale, scale, scale);
    
    for (int i = 0; str[i] != '\0'; i++) {
        glutStrokeCharacter(font, str[i]);
    }
    glPopMatrix();
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

void drawScreenText () {
    glColor3f(1, 1, 1);
    if (gameState == PAUSED){
        char msg[7] = "PAUSED";
        draw3dString(GLUT_STROKE_ROMAN, msg, -2, -0.5, 0.008);
    }
    char score[100] = "";
    sprintf(score, "%d %d", p1Points, p2Points);
    draw3dString(GLUT_STROKE_ROMAN, score, -1, 3, 0.008);
    
    char name[50] = "Luis Lamadrid - A01191158";
    draw3dString(GLUT_STROKE_ROMAN, name, -2.8, -3.5, 0.003);
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
            gameState = PLAYING;;
            break;
            
        case 'p':
        case 'P':
            gameState = PAUSED;
            break;
            
        case 'r':
        case 'R':
            gameState = START;
            resetGame();
            glutPostRedisplay();
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
    glutTimerFunc(50, gameTimer, 1);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawGuidlines();
    drawPaddles();
    drawScreenText(); // score or pause, etc...
    drawBall();
    
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
    glutSpecialFunc(arrowKeysPressed);
    init();
    glutTimerFunc(50, gameTimer, 1);
    glutMainLoop();
}







