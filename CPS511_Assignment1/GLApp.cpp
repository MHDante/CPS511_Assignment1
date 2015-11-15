#include "GLApp.h"
#include "Camera.h"

GLApp* instance__;
GLApp::GLApp()
{
  instance__ = this;
  windowName = "OpenGL App";
  ScreenWidth = ScreenHeight = 100;
  mainCamera = new Camera(this);
}
// Setup openGL
void GLApp::init(int argc, char **argv)
{

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(ScreenWidth, ScreenHeight);
  glutInitWindowPosition(ScreenPosX, ScreenPosY);
  WinID = glutCreateWindow(windowName);
  glutDisplayFunc([]() {instance__->display(); });
  glutReshapeFunc([](int w, int h) {instance__->reshape(w, h); });
  glutMouseFunc([](int b, int i, int x, int y) {instance__->mouse(b, i, x, y); });
  glutMotionFunc([](int x, int y) {instance__->mouseMotionHandler(x, y); });
  glutKeyboardFunc([](unsigned char k, int x, int y) {instance__->keyboard(k, x, y); });
  glutSpecialFunc([](int k, int x, int y) {instance__->functionKeys(k, x, y); });

  // Set up viewport, projection, then change to modelview matrix mode - 
  // display function will then set up camera and modeling transforms
  glViewport(0, 0, GLsizei(ScreenWidth), GLsizei(ScreenHeight));
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, 1.0, 0.2, 80.0);
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
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  setUpScene();
  glutMainLoop();

}
// Called at initialization and whenever user resizes the window */
void GLApp::reshape(int w, int h) const
{
  glViewport(0, 0, GLsizei(w), GLsizei(h));
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  mainCamera->perspective();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}