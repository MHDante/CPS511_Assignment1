#include "Game.h"
#include "Camera.h"
#include <iostream>
#include <thread>


//=******************************************************************
//Scene Modelling Program
//=********************************************************************
Game::Game()
{
	selector = nullptr;
	ScreenWidth = ScreenHeight = 500;
	windowName = "Scene Game";

}

void Game::setUpScene() {
	// Set up meshes
	Vector3 origin = Vector3(-8.0f, 0.0f, 8.0f);
	Vector3 dir1v = Vector3(1.0f, 0.0f, 0.0f) * 16;
	Vector3 dir2v = Vector3(0.0f, 0.0f, -1.0f) * 16;
	Vector4 diffuse = Vector4(0.9f, 0.5f, 0.0f, 1);
	floorMesh = new QuadMesh(meshSize, origin, dir1v, dir2v);
	floorMesh->material.diffuse = diffuse;

	diffuse = Vector4(0, 1, 0.0f, 4);
	origin = Vector3(-8.0f, 0.0f, 8.0f);
	dir1v = Vector3(0.0f, 0.0f, -1.0f) * 16;
	dir2v = Vector3(0.0f, .25f, 0.0f) * 16;
	wallMeshes[0] = new QuadMesh(meshSize, origin, dir1v, dir2v);
	wallMeshes[0]->material.diffuse = diffuse;

	origin = Vector3(8.0f, 0.0f, -8.0f);
	dir1v = Vector3(0.0f, 0.0f, 1.0f) * 16;
	dir2v = Vector3(0.0f, .25f, 0.0f) * 16;
	wallMeshes[1] = new QuadMesh(meshSize, origin, dir1v, dir2v);
	wallMeshes[1]->material.diffuse = diffuse;

	origin = Vector3(-8.0f, 0.0f, -8.0f);
	dir1v = Vector3(1.0f, 0.0f, 0.0f) * 16;
	dir2v = Vector3(0.0f, .25f, 0.0f) * 16;
	wallMeshes[2] = new QuadMesh(meshSize, origin, dir1v, dir2v);
	wallMeshes[2]->material.diffuse = diffuse;

	roomBox = new BBox(Vector3(-8.0f, 0, -8.0), Vector3(8.0f, 6.0, 8.0));

	mainCamera->pos = Vector3(0.0, 6.0, 22.0);
	mainCamera->target = Vector3(0.0, 0.0, 0.0);
	mainCamera->up = Vector3(0.0, 1.0, 0.0);
	mainCamera->fovY = 60.0;
	mainCamera->aspect = 1.0;
	mainCamera->nearZ = 0.2;
	mainCamera->farZ = 40.0;

}

void Game::display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// Set up the camera
	mainCamera->display();
	// Draw all cubes (see CubeMesh.h)
	for (auto& c : cubes) {
		c->drawCube();
	}
	// Draw floor and wall meshes
	floorMesh->DrawMesh();
	for (auto& w : wallMeshes) {
		w->DrawMesh();
	}
	wallMeshes[0]->DrawMesh();

	for (auto& l : lines) {
		l.Draw();
	}
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
	auto diff = Vector3();
	switch (key) {
	case GLUT_KEY_F1: {
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
		if (first&& !clear)
		{
			diff *= -1;
			first = false;
			goto attempt; //MWAHAHAHA
		}
	}

	glutPostRedisplay();
}
