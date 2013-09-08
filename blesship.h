// blesship.h

#ifndef __pi_piano_BLESSHIP_H_BLESSIP__
#define __pi_piano_BLESSHIP_H_BLESSIP__

#include "objects/project.h"
#include "objects/psitexture.h"

// window info
#define iWindowWidth	640
#define dWindowWidth	640.0
#define iWindowHeight	480
#define dWindowHeight	480.0
// Space info
#define STARNUM			3000	// three times of the number of stars
#define MAX_SIGHT		3000.0	// max sight length

class Blesship {
public:
	Blesship();
	void Initialize();

// Display
public:
	void Reshape(int w, int h);
	void LoadingScreen();
	void Display();
	void Idle();
	void TIdle(int value);
private:
	void ViewSetting();
	void DrawObject(unit *obj);
	void Boom(unit *obj, double scale);
	bool isfps;		// first person view or not
	bool isfullscreen;
	bool istidle;
	// images
	int loading;
	PsiTexture img_loading;
	PsiTexture psi_small;
	PsiTexture psi_hp;
	PsiTexture psi_msl;
	PsiTexture psi_radar;
	unsigned list_cockpit_interf;
	unsigned list_hp_gauge;
	unsigned list_fuel_gauge;
	PsiTexture psi_xbtm;
	PsiTexture psi_xtop;
	PsiTexture psi_xback;
	unsigned list_ship_xwing;
	PsiTexture psi_missile;
	PsiTexture psi_missile_back;
	unsigned list_missile;
	PsiTexture psi_boom;
	unsigned list_boom;

// key input
public:
	void KeyInput(unsigned char key, int x, int y);
	void SKeyInput(int key, int x, int y);
	void KeyUp(unsigned char key, int x, int y);
	void SKeyUp(int key, int x, int y);
private:
	bool isleftarrow;
	bool isrightarrow;
	bool isuparrow;
	bool isdownarrow;
	bool isqpress;
	bool iswpress;
	bool isapress;
	bool isspress;
	bool isdpress;
	bool isepress;
	void pcKeyInput();
	
// game info
public:
	enum GameState {LOADING, OPENING, BEGINMENU, PLAYING, GAMEOVER};
private:
	GameState gstate;

// begin menu
public:
	enum BeginMenu {INTRO, STAGE, CUSTOME, HELP, CREDIT, QUIT};
private:
	BeginMenu bmenu;

// objects
private:
	PC pc;
	double farstar[STARNUM];
};

#endif
