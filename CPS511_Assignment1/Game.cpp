#include "Game.h"
#include "Camera.h"
#include <iostream>
#include <thread>


//=******************************************************************
//Scene Modelling Program
//=********************************************************************
Game::Game()
{
	ScreenWidth = ScreenHeight = 500;
	windowName = "Scene Game";
	//centerX = glutGet(GLUT_WINDOW_X) + glutGet(GLUT_WINDOW_WIDTH) / 2;
	//centerY = glutGet(GLUT_WINDOW_Y) + glutGet(GLUT_WINDOW_HEIGHT) / 2;
	centerX = ScreenWidth / 2;
	centerY = ScreenHeight / 2;
}

void Game::setUpScene() {
  rooms.push_back(new Room(Vector3(-4.0f, 0, -4.0), Vector3(4.0f, 3.0, 4.0)));
  rooms.push_back(rooms[0]->SpawnRoom(Room::LEFT));
  rooms.push_back(rooms[1]->SpawnRoom(Room::FORWARD));
  rooms.push_back(rooms[1]->SpawnRoom(Room::BACK));
  rooms.push_back(rooms[3]->SpawnRoom(Room::BACK));

	mainCamera->pos = Vector3(0.0, 12.0, 22.0);
	mainCamera->target = Vector3(0.0, 0.0, 0.0);
	mainCamera->up = Vector3(0.0, 1.0, 0.0);
	mainCamera->fovY = 60.0;
	mainCamera->aspect = 1.0;
	mainCamera->nearZ = 2.0;
	mainCamera->farZ = 40.0;

	player = new Player(rooms[0]);
	
	player->center = Vector3(0.0, 2.0, 0.0);

	recenterMouse();
	player->turnPlayer(0);
  mainCamera->Follow(player);
	glutSetCursor(GLUT_CURSOR_NONE);
}

void Game::display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// Set up the camera
	mainCamera->display();
	// Draw all cubes (see CubeMesh.h)
    for (auto& c : cubes) c->drawCube();
    for (auto& r : rooms) r->Draw();
    for (auto& l : lines) l.Draw();

	player->drawCube();

	glutSwapBuffers();
}

Vector3 pos = Vector3(0, 0, 0);

// Mouse button callback - use only if you want to 
void Game::mouse(int button, int state, int xMouse, int yMouse)
{
	currentButton = button;

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{


			//if (currentAction == SELECT || currentAction == MULTIPLESELECT) {
				Ray ray = mainCamera->ScreenToWorldRay(xMouse, yMouse);
				lines.clear();
				for (auto& c : cubes) {
					Vector3 hit = c->Intersects(ray);
					if (hit.isValid()) {
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
void Game::mouseMotionHandler(int xMouse, int yMouse)
{
	//printf("%d :: %d\n", xMouse, yMouse);
	int xdiff = centerX - xMouse;
  if (xdiff != 0) {
    player->turnPlayer(xdiff);
    recenterMouse();
  }
	if (currentButton == GLUT_LEFT_BUTTON)
	{
		;
	}
	glutPostRedisplay();
}



void Game::UpdateConsole() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		system("cls");
		std::cout << "Press f1 to spawn a block." << std::endl;
		std::cout << "Scale  : S | Translate   : T | Rotate   : R | Extrude : E   | Raise : H   |" << std::endl;
		std::cout << "Select : C | MultiSelect : + | Deselect : - | Delete  : Del | Quit  : Esc |" << std::endl;
	}
}

// Handles input from the keyboard, non-arrow keys 
void Game::keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':     upDown.x = 1;  break;
	case 's':   upDown.y = -1;   break;
	case 'd':  rightLeft.x = 1;   break;
	case 'a':   rightLeft.y = -1;  break;
	case 't':
		break;
	case 27: exit(0); break; // esc
	case 127://del
		break;
	case ' ':break; //spacebar
	}
	glutPostRedisplay();
}

void Game::functionKeys(int key, int x, int y)
{
	/*switch (key) {
	case GLUT_KEY_F1: {
	}
	case GLUT_KEY_UP:     upDown.x = 1;  break;
	case GLUT_KEY_DOWN:   upDown.y = -1;   break;
	case GLUT_KEY_RIGHT:  rightLeft.x = 1;   break;
	case GLUT_KEY_LEFT:   rightLeft.y = -1;  break;
	}
	
	glutPostRedisplay();
	*/
}
void Game::keyboardRelease(unsigned char key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_F1: {
	}
	case 'w':  upDown.x = 0;  break;
	case 's':  upDown.y = 0;   break;
	case 'd':  rightLeft.x = 0;   break;
	case 'a':  rightLeft.y = 0;  break;
	}

	glutPostRedisplay();
}

void Game::idleFunc()
{
	//printf("%f %f\n", rightLeft.x + rightLeft.y, upDown.x + upDown.y);
	player->movePlayer(rightLeft.x + rightLeft.y, upDown.x + upDown.y);

	glutPostRedisplay();
}

void Game::recenterMouse()
{
	glutWarpPointer(centerX, centerY);
}