#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include "SOIL.h"

using namespace std;

#define WCX 	800	
#define WCY		560

#define GOAL_SIZE 360
#define OUT_SIZE  (WCY - GOAL_SIZE) / 2
#define BALL_SIZE  30


#define PADDLE_WIDTH 20
#define PADDLE_LENGTH 150
#define PADDLE_SPEED 10

int BALL_SPEED = 15;

struct Ball{
  double radius = BALL_SIZE/2;
  double x      = 100;
  double y      = 250;
}ball;

struct Paddle{
  double length = PADDLE_LENGTH;
  double width  = PADDLE_WIDTH;
  double yPos   = WCY/2;
  int score     = 0;
}paddleLeft,paddleRight;

struct State{
  double x = 0.8;
  double y = 0.6;

}state;

// texture bindings:

GLuint tex_ball;
GLuint tex_field;
GLuint tex_paddle_blue;
GLuint tex_paddle_red;

// functions

void reshape(int w,int h);
void reset();

bool keyBuffer[256];

void normal_keys(unsigned char key, int x, int y) {
 if(key == 'W' || key == 'w'){
     keyBuffer['w'] = true;
 }

 if(key == 'S' || key == 's'){
    keyBuffer['s'] = true;
 }

 if(key == '+'){
   BALL_SPEED++;
 }

 if(key == '-'){
   if(BALL_SPEED > 1){
     BALL_SPEED--;
   }
 }
 
 if(key == 'n' || key == 'N'){
   paddleLeft.score  = 0;
   paddleRight.score = 0;
 }

 if(key == 'r' || key == 'R'){
   reset();
 }


}

void normal_keys_up(unsigned char key, int x, int y) {
  if(key == 'W' || key == 'w'){
     keyBuffer['w'] = false;
  }
 if(key == 'S' || key == 's'){
    keyBuffer['s'] = false;
  }

}

void special_keys(int key, int x, int y) {
  keyBuffer[key] = true;
}

void special_keys_up(int key, int x, int y) {
  keyBuffer[key] = false;
}

void drawBall(){
  double bx = ball.x;
  double by = ball.y;
  double radius = ball.radius;
  double rad, xc, ys, x,y,tx,ty;
  
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, tex_ball);
  
  glBegin(GL_POLYGON);
  for(int i = 0; i < 360; i++){
    rad = i*(M_PI/180.0);

    xc = cos(rad);
    ys = sin(rad);
    x = xc*radius + bx;
    y = ys*radius + by;

    tx = xc*0.3 + 0.3;
    ty = ys*0.3 + 0.3;

    glTexCoord2f(tx,ty);
    glVertex2f(x,y);

  }

  glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawPaddle(Paddle& paddle, bool blue){
  // Left paddle
  
  double yPos = paddle.yPos;
  double width1 = 0; 
  double width2 = PADDLE_WIDTH;

  glEnable(GL_TEXTURE_2D);
  if(blue){
    glBindTexture(GL_TEXTURE_2D, tex_paddle_blue);
  }else{
    glBindTexture(GL_TEXTURE_2D, tex_paddle_red);
    width1 = WCX - PADDLE_WIDTH;
    width2 = WCX;
   }
  
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(width1, yPos - PADDLE_LENGTH/2 );

	glTexCoord2f(1.0, 0.0);
	glVertex2f(width2,yPos - PADDLE_LENGTH/2);

	glTexCoord2f(1.0,1.0);
	glVertex2f(width2,yPos + PADDLE_LENGTH/2);

	glTexCoord2f(0.0, 1.0);
	glVertex2f(width1,yPos + PADDLE_LENGTH/2);

	glEnd();

	glDisable(GL_TEXTURE_2D);

}

void drawField() {
  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, tex_field);
  
	glBegin(GL_POLYGON);
	glTexCoord2f(0.065, 0.09);
	glVertex2f(0, 0);

	glTexCoord2f(0.935, 0.09);
	glVertex2f(WCX,0);

	glTexCoord2f(0.935, 0.915);
	glVertex2f(WCX,WCY);

	glTexCoord2f(0.065, 0.915);
	glVertex2f(0,WCY);

	glEnd();

	glDisable(GL_TEXTURE_2D);

}

void drawString(GLdouble x,GLdouble y,GLdouble scale, GLfloat width, const string& s){
  //glColor3f(1.0f,0.0f,0.0f);
  
  glPushMatrix();
  glTranslated(x,y,0.0f);
  glScaled(scale,scale,1.0f);
  glLineWidth(width);
  glColor3f(1.0f,1.0f,0.0f);
  for(int i = 0; i < s.size(); i++){
    glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);
  }

  glColor3f(1.0f,1.0f,1.0f);

  glPopMatrix();
}

void drawScore(){

  drawString(310,480,0.55,5.0, to_string(paddleLeft.score));
  drawString(440,480,0.55,5.0, to_string(paddleRight.score));
}

// Draw Particles
void display( ) {
  glClearColor(1.0f,1.0f,1.0f,1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  drawField();
  drawPaddle(paddleLeft,true);
  drawPaddle(paddleRight,false);
  drawScore();
  drawBall();

  glutSwapBuffers();
}

void reshape(int w, int h) {
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  glOrtho(0,WCX-1,0,WCY-1,0,1);
}

void reset(){
  srand(time(NULL));
  
  ball.x = 400; 
  ball.y = 280;
  
  int deg = 0 + rand()%45;
  double rad = deg*M_PI/180;

  state.x = rand()%2 == 0 ? cos(rad) : -cos(rad);
  state.y = rand()%2 == 0 ? sin(rad) : -sin(rad);

}

void calculate(Ball& ball, Paddle& left, Paddle& right, State& state){
 
  // paddles

  // left paddle
  double b_rad = BALL_SIZE/2;


  if ( (ball.x-b_rad) <= PADDLE_WIDTH && (ball.y-b_rad) < left.yPos + PADDLE_LENGTH/2
                              && (ball.y+b_rad) > left.yPos - PADDLE_LENGTH/2){
   if( ball.y < left.yPos){
     double ratio = (left.yPos - ball.y)/(PADDLE_LENGTH/2+50);
     state.y = -ratio;
     state.x = sqrt(1-ratio*ratio);
   }else{
     double ratio = (ball.y - left.yPos )/(PADDLE_LENGTH/2+50);
     state.y = ratio;
     state.x = sqrt(1-ratio*ratio);
   } // right paddle
  } else if( (ball.x+b_rad) >= WCX - PADDLE_WIDTH && (ball.y-b_rad) < right.yPos + PADDLE_LENGTH/2
                              && (ball.y+b_rad) > right.yPos - PADDLE_LENGTH/2){
   if( ball.y < right.yPos){
     double ratio = (right.yPos - ball.y)/(PADDLE_LENGTH/2+50);
     state.y = -ratio;
     state.x = -sqrt(1-ratio*ratio);
   }else{
     double ratio = (ball.y - right.yPos )/(PADDLE_LENGTH/2+50);
     state.y = ratio;
     state.x = -sqrt(1-ratio*ratio);
   }
  }

  //board
  if( ball.y >= WCY - 1 || ball.y <= 0){ 
    state.y = -state.y;
  }
    
  if( ball.x <= 0 || ball.x >= WCX - 1){
    if( ball.y < WCY - OUT_SIZE && ball.y > OUT_SIZE){
        if( ball.x <= 0) right.score++;
        else             left.score++;
        reset(); 
    }else{ // bounce
      state.x = -state.x;
    }
  }
  
  ball.x = ball.x + state.x*BALL_SPEED;
  ball.y = ball.y + state.y*BALL_SPEED;
}

void animate (int value) {
  if( keyBuffer['w'] == true) 
    if(paddleLeft.yPos + PADDLE_LENGTH/2 < WCY){
      paddleLeft.yPos += PADDLE_SPEED;
    }

  if( keyBuffer['s'] == true)
    if(paddleLeft.yPos - PADDLE_LENGTH/2 > 0){
      paddleLeft.yPos -= PADDLE_SPEED;
    }

  if( keyBuffer[GLUT_KEY_UP] == true) 
    if(paddleRight.yPos + PADDLE_LENGTH/2 < WCY){
      paddleRight.yPos += PADDLE_SPEED;
    }

  if( keyBuffer[GLUT_KEY_DOWN] == true)
    if(paddleRight.yPos - PADDLE_LENGTH/2 > 0){
      paddleRight.yPos -= PADDLE_SPEED;
    }
 
  calculate(ball,paddleLeft,paddleRight,state);
  glutPostRedisplay();
  glutTimerFunc(1,animate,0);
}

void initTextureBindings(){
  tex_ball = SOIL_load_OGL_texture
	(
		"images/ball.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
  
  tex_field = SOIL_load_OGL_texture
	(
		"images/field.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	); 
  
  tex_paddle_blue = SOIL_load_OGL_texture
	(
		"images/paddleBlue.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	); 
  
  tex_paddle_red= SOIL_load_OGL_texture
	(
		"images/paddleRed.png",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
}

int main (int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowPosition(600,300);
  glutInitWindowSize(WCX, WCY);
  glutCreateWindow("PONG");
  initTextureBindings();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(normal_keys);
  glutKeyboardUpFunc(normal_keys_up);
  glutSpecialFunc(special_keys);
  glutSpecialUpFunc(special_keys_up);

  glutTimerFunc(20,animate,0);
  glutMainLoop();
  return 0;
}
