#include "Modeller.h"
#include "Camera.h"
#include <iostream>
#include <thread>

/*******************************************************************
Scene Modelling Program
********************************************************************/
Modeller::Modeller()
{
  selector = nullptr;
  ScreenWidth = ScreenHeight = 500;
  windowName = "Scene Modeller";

}

void Modeller::setUpScene() {
  rooms.push_back(new Room(Vector3(-4.0f, 0, -4.0), Vector3(4.0f, 4.0, 4.0)));
  rooms.push_back(rooms[0]->SpawnRoom(Room::LEFT));
  rooms.push_back(rooms[1]->SpawnRoom(Room::FORWARD));
  rooms.push_back(rooms[1]->SpawnRoom(Room::BACK));
  rooms.push_back(rooms[3]->SpawnRoom(Room::BACK));

  mainCamera->pos = Vector3(0.0, 18.0, 22.0);
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
    for (auto& c : cubes) c->drawCube();
    for (auto& r : rooms) r->Draw();
    for (auto& l : lines) l.Draw();
    glutSwapBuffers();
  }

  Vector3 pos = Vector3(0, 0, 0);

  // Mouse button callback - use only if you want to 
  void Modeller::mouse(int button, int state, int xMouse, int yMouse)
  {
    currentButton = button;

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN)
      {
        

        if (currentAction == SELECT || currentAction == MULTIPLESELECT) {
          Ray ray = mainCamera->ScreenToWorldRay(xMouse, yMouse);
          lines.clear();
          for (auto& c : cubes) {
            Vector3 hit = c->Intersects(ray);
            if (hit.isValid()) {
              selectCube(c);
            }
          }
        }

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

    if (selector)selector->hovered = false;
    cube->hovered = true;
    if (CubeMesh::singleSelecting) {
      for (auto& c : selectedCubes) c->selected = false;
      selectedCubes.clear();
    } else if (cube->selected){
      cube->selected = false;
        selector = cube;
        selectedCubes.erase(std::remove(selectedCubes.begin(), selectedCubes.end(), cube), selectedCubes.end());
        return;
    }
    cube->selected = true;
    selectedCubes.push_back(cube);
    selector = cube;
  }

  void Modeller::select(Vector3 diff) {
    if (cubes.size() == 0) return;
    if (cubes.size() == 1 && selector != nullptr) return;
    Vector3 center = (selector == nullptr) ? Vector3() : selector->center;
    CubeMesh* best = nullptr;
    float bestDist = 100;
    int topPointer = 0;
    for (auto& c : cubes) {
      if (c == selector) continue;
      float dist = 0;
      if (diff.x == -1) {
        dist = center.x - c->center.x;
      }
      else if (diff.x == 1) {
        dist = c->center.x - center.x;
      }
      else if (diff.z == -1) {
        dist = center.z - c->center.z;
      }
      else if (diff.z == 1) {
        dist = c->center.z - center.z;
      }

      if (dist < 0)dist = 100 + dist;

      if (dist != 0) {
        if (dist < bestDist) {
          best = c; bestDist = dist;
        }
      }
    }
    if (best != nullptr) {
      if (currentAction == SELECT) selectCube(best);
      else if (currentAction == MULTIPLESELECT) {
        if (selector != nullptr) selector->hovered = false;
        selector = best;
        selector->hovered = true;
      }
    }

  }


  void Modeller::UpdateConsole() {
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      system("cls");
      std::cout << "Curent mode: " + ActionNames[currentAction] << std::endl;
      std::cout << "Press f1 to spawn a block." << std::endl;
      std::cout << "Scale  : S | Translate   : T | Rotate   : R | Extrude : E   | Raise : H   |" << std::endl;
      std::cout << "Select : C | MultiSelect : + | Deselect : - | Delete  : Del | Quit  : Esc |" << std::endl;

      switch (currentAction) {
      case TRANSLATE:       std::cout << "Left/Right: Translate lengthwise.    | Up/Down: Translate depthwise" << std::endl;              break;
      case SCALE:           std::cout << "Left/Right: Scale lengthwise.        | Up/Down: Scale depthwise" << std::endl;                      break;
      case ROTATE:          std::cout << "Left/Down = Rotate CounterClockwise. | Up/Right = Rotate Clockwise." << std::endl;           break;
      case EXTRUDE:         std::cout << "Left/Down = Shrink vertically.       | Up/Right = Extrude vertically." << std::endl;               break;
      case RAISE:           std::cout << "Left/Down = Lower.                   | Up/Right = Raise." << std::endl;                                        break;
      case SELECT:          std::cout << "Use ArrowKeys or Click to select     |" << std::endl;                                            break;
      case MULTIPLESELECT:  std::cout << "ArrowKeys/Mouse: Change pointer.     | Spacebar/click to Select/Deselect" << std::endl;    break;
      }
      if (selector == nullptr) std::cout << "CurrentBlock: No block selected." << std::endl;
      if (selector != nullptr) std::cout << "CurrentBlock: Pos:" + selector->center.toString() + " Size:" + selector->dim.toString() + " Rotation:" + floatToSmallString(selector->angle) << std::endl;
      for (auto& r : rooms) {std::cout << r->min.toString() +"       ";}

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
      for (auto& c : selectedCubes) c->selected = false;
      selectedCubes.clear();
      lines.clear();
      break;
    case 27: exit(0); break;
    case 127: 
      bool resetSelector;
      for (auto& c : selectedCubes) {
        cubes.erase(std::remove(cubes.begin(), cubes.end(), c), cubes.end());
        delete c;
        if (c == selector)
        {
          selector = nullptr;
          currentAction = CubeMesh::singleSelecting ? SELECT : MULTIPLESELECT;
        }
      }
      selectedCubes.clear();
      break;
    case ' ': //spacebar
      if (currentAction == MULTIPLESELECT && selector != nullptr) {
        selectCube(selector);
      }
    }
    glutPostRedisplay();
  }

  void Modeller::functionKeys(int key, int x, int y)
  {
    auto diff = Vector3();
    switch (key) {
    case GLUT_KEY_F1: {
      auto c = new CubeMesh(rooms[0]);
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
      case TRANSLATE: for (auto& c : selectedCubes) clear &= c->translate(diff); break;
      case SCALE:     for (auto& c : selectedCubes) clear &= c->scale(diff); break;
      case ROTATE:    for (auto& c : selectedCubes) clear &= c->rotate(diff); break;
      case EXTRUDE:   for (auto& c : selectedCubes) clear &= c->extrude(diff); break;
      case RAISE:     for (auto& c : selectedCubes) clear &= c->raise(diff); break;
      case SELECT:          select(diff); break;
      case MULTIPLESELECT:  select(diff); break;
      }
      if(first&& !clear)
      {
        diff *= -1;
        first = false;
        goto attempt; //MWAHAHAHA
      }
    }

    glutPostRedisplay();
  }