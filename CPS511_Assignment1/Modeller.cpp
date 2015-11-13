/*******************************************************************
	       Scene Modelling Program
********************************************************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <utility>
#include <vector>
#include <list>
#include "VECTOR3D.h"
#include "CubeMesh.h"
#include "QuadMesh.h"

void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
Vector3 ScreenToWorld(int x, int y);

static int currentButton;
static unsigned char currentKey;

GLfloat light_position0[] = {-6.0,  12.0, 0.0,1.0};
GLfloat light_position1[] = { 6.0,  12.0, 0.0,1.0};
GLfloat light_diffuse[]   = {1.0, 1.0, 1.0, 1.0};
GLfloat light_specular[]  = {1.0, 1.0, 1.0, 1.0};
GLfloat light_ambient[]   = {0.2, 0.2, 0.2, 1.0};


// Cube Mesh Array variables and initialization
// ...
// also add a variable to keep track of current cube mesh
// Interaction State Variable
std::list<CubeMesh*> cubes;
std::vector<CubeMesh*> selectedCubes;
std::list<CubeMesh*>::iterator selector;


enum Action {TRANSLATE, ROTATE, SCALE, EXTRUDE, RAISE, SELECT, MULTIPLESELECT, DESELECT_ALL};
enum Action currentAction = TRANSLATE;

QuadMesh *floorMesh = NULL;
// Wall Mesh variables here
QuadMesh *wallMeshes[3];
BBox *roomBox;
// Default Mesh Size
int meshSize = 16;
int WinID = 0;

int main(int argc, char **argv)
{
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  WinID = glutCreateWindow("Scene Modeller");

  initOpenGL(500,500);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMotionHandler);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(functionKeys);
  glutMainLoop();
  return 0;
}



// Setup openGL */
void initOpenGL(int w, int h)
{
  // Set up viewport, projection, then change to modelview matrix mode - 
  // display function will then set up camera and modeling transforms
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0,1.0,0.2,80.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Set up and enable lighting
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  
  // Other OpenGL setup
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glClearColor(0.6, 0.6, 0.6, 0.0);  
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  // This one is important - renormalize normal vectors 
  glEnable(GL_NORMALIZE);
  
  //Nice perspective.
  glHint(GL_PERSPECTIVE_CORRECTION_HINT , GL_NICEST);
  
  // Set up meshes
  Vector3 origin  = Vector3(-8.0f,0.0f,8.0f);
  Vector3 dir1v   = Vector3(1.0f, 0.0f, 0.0f);
  Vector3 dir2v   = Vector3(0.0f, 0.0f,-1.0f);
  floorMesh = new QuadMesh(meshSize, 16.0);
  floorMesh->InitMesh(meshSize, origin, 16.0, 16.0, dir1v, dir2v);
  
  Vector3 ambient = Vector3(0.0f,0.0f,0.0f);
  Vector3 specular= Vector3(0.0f,0.0f,0.0f);
  Vector3 diffuse= Vector3(0.9f, 0.5f, 0.0f);
  float shininess = 0.0;
  floorMesh->SetMaterial(ambient,diffuse,specular,shininess);
  

  // Set up wall meshes
  // Make sure direction vectors are such that the normals are pointing into the room
  // Use the right-hand-rule (cross product) 
  // If you are confused about this, ask in class
  diffuse = Vector3(0, 1, 0.0f);

  origin = Vector3(-8.0f, 0.0f, 8.0f);
  dir1v = Vector3(0.0f, 0.0f, -1.0f);
  dir2v = Vector3(0.0f, .25f, 0.0f);
  wallMeshes[0] = new QuadMesh(meshSize, 16.0);
  wallMeshes[0]->InitMesh(meshSize, origin, 16.0, 16.0, dir1v, dir2v);
  wallMeshes[0]->SetMaterial(ambient, diffuse, specular, shininess);

  origin = Vector3(8.0f, 0.0f, -8.0f);
  dir1v = Vector3(0.0f, 0.0f, 1.0f);
  dir2v = Vector3(0.0f, .25f, 0.0f);
  wallMeshes[1] = new QuadMesh(meshSize, 16.0);
  wallMeshes[1]->InitMesh(meshSize, origin, 16.0, 16.0, dir1v, dir2v);
  wallMeshes[1]->SetMaterial(ambient, diffuse, specular, shininess);

  origin = Vector3(-8.0f, 0.0f, -8.0f);
  dir1v = Vector3(1.0f, 0.0f, 0.0f);
  dir2v = Vector3(0.0f, .25f, 0.0f);
  wallMeshes[2] = new QuadMesh(meshSize, 16.0);
  wallMeshes[2]->InitMesh(meshSize, origin, 16.0, 16.0, dir1v, dir2v);
  wallMeshes[2]->SetMaterial(ambient, diffuse, specular, shininess);
  



  // Set up the bounding box of the room
  // Change this if you change your floor/wall dimensions
  roomBox = new BBox;
  roomBox->min.Set(-8.0f, 0.0, -8.0);
  roomBox->max.Set( 8.0f, 6.0,  8.0);
}



void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glLoadIdentity();
  
  // Set up the camera
  gluLookAt(0.0,6.0,22.0,0.0,0.0,0.0,0.0,1.0,0.0);
  
  // Draw all cubes (see CubeMesh.h)
  for (auto c : cubes) {
    c->drawCube();
  }
  
  // Draw floor and wall meshes
  floorMesh->DrawMesh(meshSize);
  for (auto w : wallMeshes){
    w->DrawMesh(meshSize);
  }
  

  glutSwapBuffers();
}


// Called at initialization and whenever user resizes the window */
void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0,1.0,0.2,40.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

Vector3 pos = Vector3(0,0,0);

// Mouse button callback - use only if you want to 
void mouse(int button, int state, int x, int y)
{
  currentButton = button;

  switch(button)
  {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN)
	{
	  ;
	  
	}
	break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN)
	{
	  ; 
	}
	break;
  default:
	break;
  }
  glutPostRedisplay();
}

// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
  if (currentButton == GLUT_LEFT_BUTTON)
  {
	  ;
  }
  glutPostRedisplay();
}


Vector3 ScreenToWorld(int x, int y)
{
	// you will need this if you use the mouse
	return Vector3(0);
}// ScreenToWorld()

/* Handles input from the keyboard, non-arrow keys */
void keyboard(unsigned char key, int x, int y)
{
  switch (key) 
  {
  case 't':
    currentAction = TRANSLATE;
	  break;
  case 's':
	  currentAction = SCALE;
	  break;
  case 'r':
	  currentAction = ROTATE;
	  break;
  case 'e':
	  currentAction = EXTRUDE;
	  break;
  case 'h':
	  currentAction = RAISE;
	  break;
  case 'c':
	  currentAction = SELECT;
	  break;
  case '+':
	  currentAction = MULTIPLESELECT;
	  break;
  case '-':
	  currentAction = DESELECT_ALL;
	  break;
  case 27:// esc
    glutDestroyWindow(WinID);
    exit(0);
    break;
  }
  glutPostRedisplay();
}

void translate(Vector3 diff) {
  bool clear = true;
  for (auto c : selectedCubes) {
    c->center += diff;
    clear &= c->isWithin(roomBox);
  }
  if (!clear)
    for (auto c : selectedCubes) {
      c->center -= diff;
    }
}
void rotate(Vector3 diff) {
  bool clear = true;
  for (auto c : selectedCubes) {
    c->angle = ((int)(c->angle + diff.x + diff.z)) % 360;
    clear &= c->isWithin(roomBox);
  }
  if (!clear)
    for (auto c : selectedCubes) {
      c->angle = ((int)(c->angle - diff.x - diff.z)) % 360;
    }
}
void extrude(Vector3 diff) {
  bool clear = true;
  for (auto c : selectedCubes) {
    c->angle = ((int)(c->angle + diff.x + diff.z)) % 360;
    clear &= c->isWithin(roomBox);
  }
  if (!clear)
    for (auto c : selectedCubes) {
      c->angle = ((int)(c->angle - diff.x - diff.z)) % 360;
    }
}

void scale(Vector3 diff) {
  float x = diff.x == 1 ? 1.25f : diff.x == -1 ? 0.8f : 1;
  float z = diff.z == 1 ? 1.25f : diff.z == -1 ? 0.8f : 1;

  bool clear = true;
  for (auto c : selectedCubes) {
    c->dim.SetX(c->dim.x * x);
    c->dim.SetZ(c->dim.z * z);
    clear &= c->isWithin(roomBox) && c->dim.x > 0.1 && c->dim.z > 0.1;
  }
  if (!clear) {
    float x2 = diff.x == 1 ? 0.8f : diff.x == -1 ? 1.25f : 1;
    float z2 = diff.z == 1 ? 0.8f : diff.z == -1 ? 1.25f : 1;
    for (auto c : selectedCubes) {
      c->dim.SetX(c->dim.x * x2);
      c->dim.SetZ(c->dim.z * z2);
    }
  }
}

void functionKeys(int key, int x, int y)
{
  auto diff = Vector3();
  switch (key) {
  case GLUT_KEY_F1: {
    auto c = new CubeMesh();
    c->center.SetY(2);
    if (selectedCubes.size() == 1) { 
      selectedCubes.at(0)->selected = false; 
      selectedCubes.clear();
    }
    cubes.push_back(c);
    selectedCubes.push_back(c);
    selector = cubes.end();
    break;
  }
  case GLUT_KEY_DOWN:
    diff.SetZ(1);
    break;
  case GLUT_KEY_UP:
    diff.SetZ(-1);
    break;
  case GLUT_KEY_RIGHT:
    diff.SetX(1);
    break;
  case GLUT_KEY_LEFT:
    diff.SetX(-1);
    break;
  }
  if (diff.GetLength() > 0) {
    switch (currentAction) {
    case TRANSLATE: translate(diff); break;
    case SCALE: scale(diff); break;
    case ROTATE: rotate(diff); break;
    case EXTRUDE: extrude(diff);
      break;
    case RAISE:
      break;
    case SELECT:
      break;
    case MULTIPLESELECT:
      break;
    case DESELECT_ALL:
      break;
    }
  }
  
  glutPostRedisplay();
}





