#include "Game.h"
#include "Camera.h"
#include "GLUtils.h"
#include <iostream>
#include <thread>
#include "VarMesh.h"

Game* Game::instance = nullptr;
//=******************************************************************
//Scene Modelling Program
//=********************************************************************
Game::Game()
{
  instance = this;
  ScreenWidth = 1000;
  ScreenHeight = 800;
	windowName = "Scene Game";
	//centerX = glutGet(GLUT_WINDOW_X) + glutGet(GLUT_WINDOW_WIDTH) / 2;
	//centerY = glutGet(GLUT_WINDOW_Y) + glutGet(GLUT_WINDOW_HEIGHT) / 2;
	centerX = ScreenWidth / 2;
	centerY = ScreenHeight / 2;
}

void Game::setUpScene() {
  rooms.push_back(new Room(Vector3(0,0,0), Vector3(16,8,16)));
  rooms.push_back(rooms[0]->SpawnRoom(Room::LEFT));
  rooms.push_back(rooms[1]->SpawnRoom(Room::FORWARD));
  rooms.push_back(rooms[1]->SpawnRoom(Room::BACK));
  rooms.push_back(rooms[3]->SpawnRoom(Room::BACK));

	mainCamera->pos = Vector3(0.0, 12.0, 22.0);
	mainCamera->target = Vector3(0.0, 0.0, 0.0);
	mainCamera->up = Vector3(0.0, 1.0, 0.0);
	mainCamera->fovY = 60.0;
	mainCamera->aspect = 1.0;
	mainCamera->nearZ = 1.0f;
	mainCamera->farZ = 100;

	player = new Player(this);
	
	player->position = Vector3(0.0, 0, 0.0);
  cube = new VarMesh("megaman.obj");

	recenterMouse();
	player->turnPlayer(0);
	mainCamera->Follow(player);
	glutSetCursor(GLUT_CURSOR_NONE);
	glEnable(GL_TEXTURE_2D);
	loadTextures();
}

void Game::display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// Set up the camera
	mainCamera->display();
	// Draw all cubes (see CubeMesh.h)
  for (auto& c : cubes) c->draw();
  for (auto& r : rooms) r->Draw();
  for (auto& l : lines) l.Draw();
  for (auto& b : bullets)b->draw();
  for (auto& b : robots)b->draw();

	player->draw();
  cube->Draw();

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
			player->spawnBullet();
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
	int newTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = newTime - previousTime;
	previousTime = newTime;
	//printf("%f %f\n", rightLeft.x + rightLeft.y, upDown.x + upDown.y);
	player->movePlayer(rightLeft.x + rightLeft.y, upDown.x + upDown.y, deltaTime);
	for(auto& b : bullets)
	{
		b->update(deltaTime);
	}
	for (auto& r : robots)
	{
		r->update(deltaTime);
	}
  bullets.erase(
    std::remove_if(bullets.begin(), bullets.end(),
      [](Bullet* element) -> bool {
      //return true if element should be removed.
    bool remove = element->flaggedForRemoval;
    if (remove) free(element);
        return remove;
      } ), bullets.end() );
  robots.erase(
    std::remove_if(robots.begin(), robots.end(),
      [](Robot* element) -> bool {
    //return true if element should be removed.
    bool remove = element->flaggedForRemoval;
    if (remove) free(element);
    return remove;
  }), robots.end());

	spawnTimer += deltaTime;
	if (spawnTimer >= spawnTimerMaxRand)
	{
		spawnTimer = 0;
		int fourthMax = (spawnTimerMax / 4);
		int range = rand() % fourthMax - fourthMax / 2;
		spawnTimerMaxRand = spawnTimerMax + range;
		spawnEnemy();
	}

	
	glutPostRedisplay();
}
void Game::spawnEnemy()
{
	int roomIndex = rand() % rooms.size();
	Room * r = rooms[roomIndex];
	Robot * robot = new Robot(this);
	robot->position = (r->max - r->min) * (randZeroToOne() * 0.6 + 0.2) + r->min;
	robot->position.y = (r->min.y + r->max.y) / 2;
	robot->setRandDirection();
	robots.push_back(robot);

}
void Game::recenterMouse()
{
	glutWarpPointer(centerX, centerY);
}

void Game::loadTextures()
{
	loadTexture("tiles01.bmp", Textures::TILES01);
	loadTexture("professor.bmp", Textures::PROFESSOR);
  loadTexture("megaman.bmp", Textures::MEGAMAN);
	loadTexture("robot.bmp", Textures::BOT);
}
void Game::loadTexture(const char * filename, Textures tex)
{
	GLuint textureId;
	glGenTextures(1, &textureId);

	pixmaps[tex] = RGBpixmap();
	textureMap[tex] = textureId;

	pixmaps[tex].readBMPFile(filename);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // store pixels by byte	
	glBindTexture(GL_TEXTURE_2D, textureId); // select current texture (0)
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D( // initialize texture
		GL_TEXTURE_2D, // texture is 2-d
		0, // resolution level 0
		GL_RGB, // internal format
		pixmaps[tex].nCols, // image width
		pixmaps[tex].nRows, // image height
		0, // no border
		GL_RGB, // my format
		GL_UNSIGNED_BYTE, // my type
		pixmaps[tex].pixel); // the pixels


}

Room* Game::roomAt(Vector3 center) {
	for (auto& r : rooms) {
		if (r != nullptr && r->Contains(center)) return r;
	}
	return nullptr;
}
