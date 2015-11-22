#include "Modeller.h"
#include "Camera.h"

/*******************************************************************
Scene Modelling Program
********************************************************************/
Modeller::Modeller()
{
  selector = nullptr;
  ScreenWidth = ScreenHeight = 500;
  windowName = "Scene Modeller";

}

void Modeller:: setUpScene(){
  // Set up meshes
  Vector3 origin = Vector3(-8.0f, 0.0f, 8.0f);
  Vector3 dir1v = Vector3(1.0f, 0.0f, 0.0f);
  Vector3 dir2v = Vector3(0.0f, 0.0f, -1.0f);
  floorMesh = new QuadMesh(meshSize, 16.0);
  floorMesh->InitMesh(meshSize, origin, 16.0, 16.0, dir1v, dir2v);
  floorMesh->mat_diffuse = Vector3(0.9f, 0.5f, 0.0f);


  auto diffuse = Vector3(0, 1, 0.0f);
  origin = Vector3(-8.0f, 0.0f, 8.0f);
  dir1v = Vector3(0.0f, 0.0f, -1.0f);
  dir2v = Vector3(0.0f, .25f, 0.0f);
  wallMeshes[0] = new QuadMesh(meshSize, 16.0);
  wallMeshes[0]->InitMesh(meshSize, origin, 16.0, 16.0, dir1v, dir2v);
  wallMeshes[0]->mat_diffuse = diffuse;
  
  origin = Vector3(8.0f, 0.0f, -8.0f);
  dir1v = Vector3(0.0f, 0.0f, 1.0f);
  dir2v = Vector3(0.0f, .25f, 0.0f);
  wallMeshes[1] = new QuadMesh(meshSize, 16.0);
  wallMeshes[1]->InitMesh(meshSize, origin, 16.0, 16.0, dir1v, dir2v);
  wallMeshes[1]->mat_diffuse = diffuse;
  
  origin = Vector3(-8.0f, 0.0f, -8.0f);
  dir1v = Vector3(1.0f, 0.0f, 0.0f);
  dir2v = Vector3(0.0f, .25f, 0.0f);
  wallMeshes[2] = new QuadMesh(meshSize, 16.0);
  wallMeshes[2]->InitMesh(meshSize, origin, 16.0, 16.0, dir1v, dir2v);
  wallMeshes[2]->mat_diffuse = diffuse;

  roomBox = new BBox;
  roomBox->min.Set(-8.0f, 0.0, -8.0);
  roomBox->max.Set(8.0f, 6.0, 8.0);

  mainCamera->pos = Vector3(0.0, 6.0, 22.0);
  mainCamera->target = Vector3(0.0, 0.0, 0.0);
  mainCamera->up = Vector3(0.0, 1.0, 0.0);
  mainCamera->fovY = 60.0;
  mainCamera->aspect = 1.0;
  mainCamera->nearZ = 0.2;
  mainCamera->farZ = 40.0;
}


  void Modeller::display(void)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // Set up the camera
    mainCamera->display();
    // Draw all cubes (see CubeMesh.h)
    for (auto c : cubes) {
      c->drawCube();
    }
    // Draw floor and wall meshes
    floorMesh->DrawMesh(meshSize);
    for (auto w : wallMeshes) {
      w->DrawMesh(meshSize);
    }
    wallMeshes[0]->DrawMesh(meshSize);

    glDisable(GL_LIGHTING);
    glColor3f(1, 0, 0);

    glBegin(GL_LINES);
    for (auto l : lines) {
      glVertex3f(l->from.x, l->from.y, l->from.z);
      glVertex3f(l->to.x, l->to.y, l->to.z);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glutSwapBuffers();
  }

  Vector3 pos = Vector3(0, 0, 0);

  // Mouse button callback - use only if you want to 
  void Modeller::mouse(int button, int state, int x, int y)
  {
    currentButton = button;

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN)
      {

        Vector3 dir2 = mainCamera->ScreenToWorldDir(x, y);

        for (int i = 10; i < 44; i++){
          auto l = new Line();
          l->from = Vector3();// mainCamera->pos;
          l->to = mainCamera->pos+ dir2*i;
          lines.push_back(l);
        }
        return;

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
  void Modeller::mouseMotionHandler(int xMouse, int yMouse)
  {
    if (currentButton == GLUT_LEFT_BUTTON)
    {
      ;
    }
    glutPostRedisplay();
  }

  void Modeller::selectCube(CubeMesh* cube) {
    if (CubeMesh::singleSelecting) {
      for (auto c : selectedCubes) c->selected = false;
      selectedCubes.clear();
    }
    if (selector)selector->hovered = false;
    cube->selected = true;
    cube->hovered = true;
    selectedCubes.push_back(cube);
    selector = cube;
  }

  void Modeller::select(Vector3 diff) {
    if (cubes.size() == 0) return;
    if (cubes.size() == 1 && selector != nullptr) return;
    Vector3 center = (selector == nullptr) ? Vector3() : selector->center;
    CubeMesh* best = nullptr;
    float bestDist = 100;
    float altDist = -100;
    int topPointer = 0;
    for (auto c : cubes) {
      if (c == selector) continue;
      float dist = 0;
      float dist2 = 0;
      if (diff.x == -1) {
        dist = center.x - c->center.x;
        dist2 = center.z - c->center.z;
      }
      else if (diff.x == 1) {
        dist = c->center.x - center.x;
        dist2 = c->center.z - center.z;
      }
      else if (diff.z == -1) {
        dist2 = center.x - c->center.x;
        dist = center.z - c->center.z;
      }
      else if (diff.z == 1) {
        dist2 = c->center.x - center.x;
        dist = c->center.z - center.z;
      }

      if (dist < 0)dist = 100 + dist;
      //if (dist2 < 0)dist2 = 100 + dist;

      if (dist != 0) {
        if (dist < bestDist) {
          best = c; bestDist = dist;
          altDist = dist2;
        }
        else if (dist == bestDist && dist2 < altDist) {
          best = c; bestDist = dist;
          altDist = dist2;
        }
      }
      else if (dist2 > 0) {
        best = c; bestDist = 0; altDist = dist2;
      }
      else if (dist2 == 0 && int(c) > topPointer && c < selector) {
        best = c; bestDist = 0; altDist = dist2;
      }
    }

    if (currentAction == SELECT) selectCube(best);
    else if (currentAction == MULTIPLESELECT) {
      if (selector != nullptr) selector->hovered = false;
      selector = best;
      selector->hovered = true;
    }


  }


  /* Handles input from the keyboard, non-arrow keys */
  void Modeller::keyboard(unsigned char key, int x, int y)
  {
    switch (key)
    {
    case 't': currentAction = TRANSLATE;   break;
    case 's': currentAction = SCALE;       break;
    case 'r': currentAction = ROTATE;      break;
    case 'e': currentAction = EXTRUDE;     break;
    case 'h': currentAction = RAISE;       break;
    case 'c':
      currentAction = SELECT;
      CubeMesh::singleSelecting = true;
      break;
    case '+':
      currentAction = MULTIPLESELECT;
      CubeMesh::singleSelecting = false;
      break;
    case '-':
      for (auto c : selectedCubes) c->selected = false;
      selectedCubes.clear();
      break;
    case 27: exit(0); break;
    case ' ': //spacebar
      if (currentAction == MULTIPLESELECT && selector != nullptr) {
        if (!selector->selected)selectCube(selector);
        else {
          selector->selected = false;
          selectedCubes.erase(std::remove(selectedCubes.begin(), selectedCubes.end(), selector), selectedCubes.end());
        }
      }
    }
    glutPostRedisplay();
  }

  void Modeller::functionKeys(int key, int x, int y)
  {
    auto diff = Vector3();
    switch (key) {
    case GLUT_KEY_F1: {
      auto c = new CubeMesh();
      c->center.SetY(1);
      cubes.push_back(c);
      selectCube(c);
      break;
    }
    case GLUT_KEY_DOWN:   diff.SetZ(1);   break;
    case GLUT_KEY_UP:     diff.SetZ(-1);  break;
    case GLUT_KEY_RIGHT:  diff.SetX(1);   break;
    case GLUT_KEY_LEFT:   diff.SetX(-1);  break;
    }
    if (diff.GetLength() > 0) {
      bool first = true;
      bool clear = true;
      attempt:
      switch (currentAction) {
      case TRANSLATE: for (auto c : selectedCubes) clear &= c->translate(diff, roomBox); break;
      case SCALE:     for (auto c : selectedCubes) clear &= c->scale(diff, roomBox); break;
      case ROTATE:    for (auto c : selectedCubes) clear &= c->rotate(diff, roomBox); break;
      case EXTRUDE:   for (auto c : selectedCubes) clear &= c->extrude(diff, roomBox); break;
      case RAISE:     for (auto c : selectedCubes) clear &= c->raise(diff, roomBox); break;
      case SELECT:          select(diff); break;
      case MULTIPLESELECT:  select(diff); break;
      }
      if(first&& !clear)
      {
        diff *= -1;
        first = false;
        goto attempt;
      }
    }

    glutPostRedisplay();
  }