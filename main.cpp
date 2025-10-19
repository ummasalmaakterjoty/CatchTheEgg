#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define PI 3.14159265358979323846

int WIN_W = 700, WIN_H = 700;
int basket_x = 320;
int BASKET_W = 80, BASKET_H = 30;

float egg_x = -200, egg_y = -200;
int egg_type = 0; // 0 normal,1 blue,2 gold,3 poop

float hen_x[2] = {120.0f, 480.0f};
int hen_dir[2] = {1, -1};
float hen_speed[2] = {2.0f, 2.0f};

int wind = 0, wind_timer = 0;

int mode = 0, paused = 0;
int points = 0, high_score = 0;
int time_left = 60, dropped = 0, caught = 0;
float fall_speed = 3.0f;
static int frameCounter = 0;

void drawText(float x, float y, const char *s)
{
  glRasterPos2f(x, y);
  for (size_t i = 0; i < strlen(s); ++i)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
}

void drawBackground()
{
  glColor3f(0.03f, 0.05f, 0.25f);
  glBegin(GL_QUADS);
  glVertex2f(0, 0);
  glVertex2f(WIN_W, 0);
  glVertex2f(WIN_W, WIN_H);
  glVertex2f(0, WIN_H);
  glEnd();
  glColor3f(0.05f, 0.35f, 0.06f);
  glBegin(GL_QUADS);
  glVertex2f(0, 0);
  glVertex2f(WIN_W, 0);
  glVertex2f(WIN_W, 120);
  glVertex2f(0, 120);
  glEnd();
  glColor3f(0.95f, 0.95f, 0.85f);
  float mx = WIN_W - 110, my = WIN_H - 110, mr = 40;
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(mx, my);
  for (int i = 0; i <= 360; i += 10)
    glVertex2f(mx + mr * cos(i * PI / 180.0), my + mr * sin(i * PI / 180.0));
  glEnd();
  glColor3f(1, 1, 1);
  for (int i = 0; i < 30; i++)
  {
    float sx = (i * 47 % WIN_W) + 10, sy = (i * 73 % WIN_H) + 150;
    glBegin(GL_POINTS);
    glVertex2f(sx, sy);
    glEnd();
  }
}

void drawHen(float cx)
{
  float cy = WIN_H - 120.0f;
  glColor3f(0.42f, 0.26f, 0.12f);
  glBegin(GL_QUADS);
  glVertex2f(40, cy - 10);
  glVertex2f(WIN_W - 40, cy - 10);
  glVertex2f(WIN_W - 40, cy);
  glVertex2f(40, cy);
  glEnd();
  glColor3f(1.0f, 0.95f, 0.85f);
  glBegin(GL_POLYGON);
  for (int ang = 0; ang <= 360; ang += 8)
  {
    float rad = ang * PI / 180.0;
    float x = cx + 26 * cos(rad), y = cy - 18 + 14 * sin(rad);
    glVertex2f(x, y);
  }
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(cx + 30, cy + 4);
  for (int ang = 0; ang <= 360; ang += 8)
    glVertex2f(cx + 30 + 10 * cos(ang * PI / 180.0), cy + 4 + 10 * sin(ang * PI / 180.0));
  glEnd();
  glColor3f(1.0f, 0.6f, 0.0f);
  glBegin(GL_TRIANGLES);
  glVertex2f(cx + 40, cy + 2);
  glVertex2f(cx + 48, cy + 6);
  glVertex2f(cx + 40, cy + 8);
  glEnd();
  glColor3f(0, 0, 0);
  glBegin(GL_POINTS);
  glVertex2f(cx + 28, cy + 8);
  glEnd();
}

void drawBasketVisual()
{
  float cx = basket_x + BASKET_W / 2.0f, cy = 30.0f + 10.0f;
  glColor3f(0.6f, 0.25f, 0.05f);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(cx, cy + 10);
  for (int a = 180; a <= 360; a += 6)
    glVertex2f(cx + (BASKET_W / 2.0f) * cos(a * PI / 180.0), cy + (BASKET_H / 1.2f) * sin(a * PI / 180.0));
  glEnd();
}

void drawEggVisual()
{
  if (egg_y < 0)
    return;
  if (egg_type == 0)
    glColor3f(1.0f, 1.0f, 1.0f);
  else if (egg_type == 1)
    glColor3f(0.1f, 0.1f, 0.9f);
  else if (egg_type == 2)
    glColor3f(1.0f, 0.9f, 0.2f);
  else
    glColor3f(0.45f, 0.28f, 0.15f);
  glBegin(GL_POLYGON);
  for (int a = 0; a <= 360; a += 8)
    glVertex2f(egg_x + 9 * cos(a * PI / 180.0), egg_y + 12 * sin(a * PI / 180.0));
  glEnd();
}

void spawnEgg()
{
  egg_x = rand() % 2 == 0 ? hen_x[0] : hen_x[1];
  egg_y = WIN_H - 140.0f;
  egg_type = rand() % 4; // 0=normal,1=blue,2=gold,3=poop
  dropped++;
}

void updateFrame(int unused)
{
  if (!paused && mode == 1)
  {
    
    for (int i = 0; i < 2; i++)
    {
      hen_x[i] += hen_dir[i] * hen_speed[i];
      if (hen_x[i] < 60 || hen_x[i] > WIN_W - 120)
        hen_dir[i] *= -1;
    }
    
    if (egg_y >= 0)
    {
      egg_y -= fall_speed;
      egg_x += wind * 1.2f;
      if (egg_x < 10)
        egg_x = 10;
      if (egg_x > WIN_W - 10)
        egg_x = WIN_W - 10;
      if (egg_y <= 120)
        egg_y = -200;
    }
    else if (rand() % 100 < 6)
      spawnEgg();

    
    wind_timer++;
    if (wind_timer > 60)
    {
      wind = (rand() % 3) - 1;
      wind_timer = 0;
    }

    
    frameCounter++;
    if (frameCounter >= 60)
    { 
      time_left--;
      frameCounter = 0;
      if (time_left <= 0)
        mode = 0; 
    }
  }
  glutPostRedisplay();
  glutTimerFunc(16, updateFrame, 0);
}

void checkCatches()
{
  if (egg_y >= 0 && egg_y <= 140 && egg_x >= basket_x && egg_x <= basket_x + BASKET_W)
  {
    caught++;
    if (egg_type == 0)
      points += 1;
    else if (egg_type == 1)
      points += 5;
    else if (egg_type == 2)
      points += 10;
    else if (egg_type == 3)
      points -= 10;
    if (points > high_score)
      high_score = points;
    egg_y = -200;
  }
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  drawBackground();
  if (mode != 1)
  {
    drawHen(hen_x[0]);
    drawHen(hen_x[1]);
    drawBasketVisual();
    drawEggVisual();
  }

  if (mode == 0)
  {
    glColor3f(1, 0.9f, 0.6f);
    drawText(WIN_W / 2 - 80, WIN_H / 2 + 60, "CATCH THE EGG");
    glColor3f(1, 1, 1);
    drawText(WIN_W / 2 - 90, WIN_H / 2 + 20, "Press 'S' to Start  |  Right-click for Menu");
    drawText(WIN_W / 2 - 130, WIN_H / 2 - 10, "Controls: Arrow keys / Mouse to move basket, P to Pause, Q to Quit");
    char pts[64];
    sprintf(pts, "High Score: %d", high_score);
    drawText(WIN_W / 2 - 40, WIN_H / 2 - 40, pts);
  }
  else if (mode == 1)
  {
    drawHen(hen_x[0]);
    drawHen(hen_x[1]);
    drawEggVisual();
    drawBasketVisual();

    char hud[128];
    sprintf(hud, "Points: %d   Time: %d   High: %d", points, time_left, high_score);
    glColor3f(1, 1, 1);
    drawText(12, WIN_H - 18, hud);

    if (paused)
    {
      glColor3f(1.0f, 0.5f, 0.3f);
      drawText(WIN_W / 2 - 40, WIN_H / 2, "PAUSED");
    }

    checkCatches();
  }
  else if (mode == 2)
  {
    glColor3f(0.98f, 0.98f, 0.88f);
    drawText(120, 520, "HELP / INSTRUCTIONS:");
    drawText(120, 480, "- Move basket with Arrow keys or mouse.");
    drawText(120, 450, "- Catch eggs: Normal=+1, Blue=+5, Gold=+10, Poop=-10.");
    drawText(120, 420, "- Wind may push eggs left/right.");
    drawText(120, 390, "- Press 'P' to pause/resume, 'Q' to quit, 'S' to start.");
    drawText(120, 360, "- Right-click menu: Start/Resume/Help/HighScore/Exit.");
  }
  else if (mode == 3)
  {
    glColor3f(0.95f, 0.95f, 0.85f);
    drawText(WIN_W / 2 - 60, WIN_H / 2 + 30, "HIGH SCORE");
    char hs[80];
    sprintf(hs, "Best Points: %d", high_score);
    drawText(WIN_W / 2 - 70, WIN_H / 2, hs);
    drawText(WIN_W / 2 - 120, WIN_H / 2 - 40, "Press 'S' to Start or Right-click for Menu");
  }

  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 's':
  case 'S':
    mode = 1;
    points = 0;
    time_left = 60;
    egg_y = -200;
    frameCounter = 0;
    break;
  case 'p':
  case 'P':
    paused = !paused;
    break;
  case 'q':
  case 'Q':
    exit(0);
    break;
  }
}

void specialKeys(int key, int x, int y)
{
  int step = 15;
  if (key == GLUT_KEY_LEFT)
  {
    basket_x -= step;
    if (basket_x < 0)
      basket_x = 0;
  }
  else if (key == GLUT_KEY_RIGHT)
  {
    basket_x += step;
    if (basket_x > WIN_W - BASKET_W)
      basket_x = WIN_W - BASKET_W;
  }
}

void mouseMove(int x, int y)
{
  basket_x = x - BASKET_W / 2;
  if (basket_x < 0)
    basket_x = 0;
  if (basket_x > WIN_W - BASKET_W)
    basket_x = WIN_W - BASKET_W;
}

void menu(int option)
{
  switch (option)
  {
  case 1:
    mode = 1;
    paused = 0;
    break;
  case 2:
    mode = 2;
    break;
  case 3:
    mode = 3;
    break;
  case 4:
    exit(0);
    break;
  }
}

void createMenu()
{
  glutCreateMenu(menu);
  glutAddMenuEntry("Start/Resume", 1);
  glutAddMenuEntry("Help", 2);
  glutAddMenuEntry("High Score", 3);
  glutAddMenuEntry("Exit", 4);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv)
{
  srand((unsigned int)time(NULL));
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIN_W, WIN_H);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Catch The Egg");
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, WIN_W, 0, WIN_H);

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeys);
  glutPassiveMotionFunc(mouseMove);
  createMenu();
  glutTimerFunc(16, updateFrame, 0);

  glutMainLoop();
  return 0;
}
