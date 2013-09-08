// wrapfunc.cpp

#include "blesship.h"
#include "wrapfunc.h"

extern Blesship game;

void Reshape(int w, int h)
{
	game.Reshape(w, h);
}

void Display()
{
	game.Display();
}

void Idle()
{
	game.Idle();
}

void Timer(int value)
{
	game.TIdle(value);
}

void KeyInput(unsigned char key, int x, int y)
{
	game.KeyInput(key, x, y);
}

void SKeyInput(int key, int x, int y)
{
	game.SKeyInput(key, x, y);
}

void KeyUp(unsigned char key, int x, int y)
{
	game.KeyUp(key, x, y);
}

void SKeyUp(int key, int x, int y)
{
	game.SKeyUp(key, x, y);
}

