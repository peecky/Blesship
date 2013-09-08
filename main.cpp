// main.cpp

// Blesship 3D FPS

#include <GL/glut.h>
#include "blesship.h"
#include <ctime>

Blesship game;

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(iWindowWidth, iWindowHeight);
	glutCreateWindow("Blesship");

	game.Initialize();

	glutMainLoop();
	return 0;
}
