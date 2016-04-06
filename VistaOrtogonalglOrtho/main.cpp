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
#include <string.h>
#include "Sound.h"
#include "SOIL/SOIL.h"

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
int ballBounceSeq = 0;
float rotationAngles = 0.0;

// game mechanics
typedef enum { START, PLAYING, PAUSED, GAME_OVER } State;
State gameState = START;
int p1Points = 0, p2Points = 0;

// sound variables
//Sound bopSound = Sound();
//Sound panSound = NULL;

// EXAM Vars
bool verticalOrientation = true, bounced = false, mouseOver = false;

const int NUM_TEXTURES = 7;
const int BG_TEX = 0;
const int GIRL_TEX = 1;
const int BOY_TEX = 2;
const int BALL_TEX = 3;
const int BALL2_TEX = 4;
const int INICIO1_TEX = 5;
const int INICIO2_TEX = 6;
string fullPath = __FILE__;
GLuint textures[NUM_TEXTURES];


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
        ballBounceSeq = 1;
		ballWidth = 0.8;
		bounced = true;
        ballDirection = directionChange[0][ballDirection];
    } else if ((ballX + ballRadius >= X_MAX) || (ballX - ballRadius <= X_MIN)) {
        printf("Left / Right Collision!!!\n");
//        bopSound.PlaySound();
        awardPoints(ballX);
        resetBallLoc();
    } else if ((ballY + ballRadius >= Y_MAX) || (ballY - ballRadius <= Y_MIN)) {
        printf("Top / Down Collision!!!\n");
//        panSound.PlaySound();
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

void drawBall() {
	//	Cuando se desconocen los vertices del objeto
	//	Selecciona la textura
	glEnable(GL_TEXTURE_2D);
	//	Cambiamos el color para que no se pinte la textura
	glColor3f(1, 1, 1);
	if (bounced)
    {
		glBindTexture(GL_TEXTURE_2D, textures[BALL2_TEX]);
		bounced = !bounced;
	} else
    {
		glBindTexture(GL_TEXTURE_2D, textures[BALL_TEX]);
	}
	//	Como se van a generar las coordenadas?
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	//	Activar la generación de coordenadas
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	
    glPushMatrix();
    glTranslated(ballX, ballY, 0);
    glScaled(ballWidth, ballHeight, ballDepth);
    glColor3d(1.0, 1.0, 1.0);
    glutSolidSphere(ballRadius, 20, 20);
    glColor3d(0.0, 0.0, 1.0);
    glLineWidth(1.0);
    glutWireSphere(ballRadius, 20, 20);
    glPopMatrix();
	
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_2D);
}

void drawScreenText () {
    glColor3f(1, 1, 1);
    if (gameState == PAUSED){
        char msg[7] = "PAUSED";
        draw3dString(GLUT_STROKE_ROMAN, msg, -2, -0.5, 0.008);
    }
    char score[100] = "";
    sprintf(score, "%d %d", p1Points, p2Points);
    draw3dString(GLUT_STROKE_ROMAN, score, -1, 1.5, 0.008);
    
    char name[50] = "Luis Lamadrid - A01191158";
    draw3dString(GLUT_STROKE_ROMAN, name, -2.8, -1.5, 0.003);
    
    char name2[50] = "Manuel Sanudo - A01192241";
    draw3dString(GLUT_STROKE_ROMAN, name2, -2.8, -2.0, 0.003);
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

void drawMenu() {
    glColor3f(1, 1, 1);
    //Habilitar el uso de texturas
    glEnable(GL_TEXTURE_2D);
    
    //Elegir la textura del Quads: angulo cambia con el timer
    glBindTexture(GL_TEXTURE_2D, textures[BG_TEX]);
    
    glBegin(GL_QUADS);
    //Asignar la coordenada de textura 0,0 al vertice
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(X_MIN, Y_MIN, 0);
    //Asignar la coordenada de textura 1,0 al vertice
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(X_MAX, Y_MIN, 0);
    //Asignar la coordenada de textura 1,1 al vertice
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(X_MAX, Y_MAX, 0);
    //Asignar la coordenada de textura 0,1 al vertice
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(X_MIN, Y_MAX, 0);
    glEnd();
    
    if (!mouseOver) {
        glBindTexture(GL_TEXTURE_2D, textures[INICIO1_TEX]);
    } else {
        glBindTexture(GL_TEXTURE_2D, textures[INICIO2_TEX]);

    }
    glBegin(GL_QUADS);
    //Asignar la coordenada de textura 0,0 al vertice
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2, -0.5, 0);
    //Asignar la coordenada de textura 1,0 al vertice
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2, -0.5, 0);
    //Asignar la coordenada de textura 1,1 al vertice
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2, 0.5, 0);
    //Asignar la coordenada de textura 0,1 al vertice
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2, 0.5, 0);
    glEnd();
    
    
}

void mouseMoved(int x, int y) {
    // printf("Motion = X: %d,  Y: %d\n", x, y);
    mouseOver =  (x >= 135 && x <= 365 && y >= 225 && y <= 275);
}

void arrowKeysPressed (int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_DOWN:
            if (!verticalOrientation) break;
            updateRightPaddleLoc(-paddleSpeed);
            glutPostRedisplay();
            break;
            
        case GLUT_KEY_UP:
            if (!verticalOrientation) break;
            updateRightPaddleLoc(paddleSpeed);
            glutPostRedisplay();
            break;
            
        case GLUT_KEY_LEFT:
            if (verticalOrientation) break;
            updateRightPaddleLoc(paddleSpeed);
            glutPostRedisplay();
            break;
            
        case GLUT_KEY_RIGHT:
            if (verticalOrientation) break;
            updateRightPaddleLoc(-paddleSpeed);
            glutPostRedisplay();
            break;
            
        default: break;
    }
}

void keyboardPressed (unsigned char key, int mouseX, int mouseY) {
    switch (key) {
        case 'w':
        case 'W':
            if (!verticalOrientation) break;
            updateLeftPaddleLoc(paddleSpeed);
            glutPostRedisplay();
            break;
            
        case 's':
        case 'S':
            if (!verticalOrientation) break;
            updateLeftPaddleLoc(-paddleSpeed);
            glutPostRedisplay();
            break;
            
        case 'a':
        case 'A':
            if (verticalOrientation) break;
            updateLeftPaddleLoc(paddleSpeed);
            glutPostRedisplay();
            break;
            
        case 'd':
        case 'D':
            if (verticalOrientation) break;
            updateLeftPaddleLoc(-paddleSpeed);
            glutPostRedisplay();
            break;
            
        case 'i':
        case 'I':
            if (gameState == START) break;
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
            
        case 'o':
        case 'O':
            verticalOrientation = !verticalOrientation;
            break;
            
        case 27: exit(0);
            
        default: break;
    }
}

void loadTextures() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(NUM_TEXTURES, textures);
	
//	remove "main.cpp" from path
	for (int i = (int)fullPath.length()-1; i>=0 && fullPath[i] != '/'; i--) {
		fullPath.erase(i,1);
	}
	char  path[200];
//	sprintf(path,"%s%s", fullPath.c_str() , "bop.wav");
//	bopSound = Sound(path);
//	sprintf(path,"%s%s", fullPath.c_str() , "pan.wav");
//	panSound = Sound(path);
	
	sprintf(path,"%s%s", fullPath.c_str() , "Texturas/Fondo.bmp");
	textures[BG_TEX] = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	
	sprintf(path,"%s%s", fullPath.c_str() , "Texturas/nina.bmp");
	textures[GIRL_TEX] = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	
	sprintf(path,"%s%s", fullPath.c_str() , "Texturas/nino.bmp");
	textures[BOY_TEX] = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	
	sprintf(path,"%s%s", fullPath.c_str() , "Texturas/burger.bmp");
	textures[BALL_TEX] = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	
	sprintf(path,"%s%s", fullPath.c_str() , "Texturas/burger2.bmp");
	textures[BALL2_TEX] = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    
    sprintf(path,"%s%s", fullPath.c_str() , "Texturas/inicio1.bmp");
    textures[INICIO1_TEX] = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    
    sprintf(path,"%s%s", fullPath.c_str() , "Texturas/inicio2.bmp");
    textures[INICIO2_TEX] = SOIL_load_OGL_texture(path, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

void gameTimer(int value){
    if (gameState == PLAYING) {
        if (ballBounceSeq <= 2 && ballBounceSeq != 0) {
            ballWidth = 0.7;
            ballBounceSeq++;
        } else {
            ballWidth = 1.0;
            ballBounceSeq = 0;
        }
        updateBallLoc(ballSpeed);
    } else if (gameState == GAME_OVER) {
        resetBallLoc();
    }
    
    rotationAngles = (ballX / 2);
    
    glutPostRedisplay();
    glutTimerFunc(50, gameTimer, 1);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (gameState == START){
        drawMenu();
    }
    else {
        glPushMatrix();
        if (!verticalOrientation) { glRotated(90, 0, 0, 1); }
        glRotatef(rotationAngles, 0, 1, 0);
        
        drawGuidlines();
        drawPaddles();
        
        glPushMatrix();
        if (!verticalOrientation) { glRotated(90, 0, 0, -1); }
        drawScreenText(); // score or pause, etc...
        glPopMatrix();
        
        drawBall();
        
        glPopMatrix();
    }
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    screenWidth = w;
    screenHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // glOrtho(-4.0, 4.0, -4.0, 4.0, 1.0, 4.0);
    glFrustum(X_MIN, X_MAX, Y_MIN, Y_MAX, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 1.1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void init() {
    glClearColor (0.2039, 0.6588, 0.3254, 1.0);
    glColor3f(0.0, 0.0, 0.0);
	loadTextures();
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
    glutPassiveMotionFunc(mouseMoved);
    glutSpecialFunc(arrowKeysPressed);
    init();
    glutTimerFunc(50, gameTimer, 1);
    glutMainLoop();
}







