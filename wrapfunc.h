// wrapfunc.h

#ifndef __pi_piano_WRAPFUNC_H_BLESSHIP__
#define __pi_piano_WRAPFUNC_H_BLESSHIP__

void Reshape(int w, int h);
void Display();
void Idle();
void Timer(int value);
void KeyInput(unsigned char key, int x, int y);
void SKeyInput(int key, int x, int y);
void KeyUp(unsigned char key, int x, int y);
void SKeyUp(int key, int x, int y);

#endif

