// blesship.cpp

#include <cstdlib>
#include <iostream>
#include <GL/glut.h>
#include "blesship.h"
#include "defines.h"
#include "wrapfunc.h"

static float light0_ambient[] = {.0, .0, .0, 1.0};
static float light0_diffuse[] = {1.0, .0, .0, 1.0};
static float light0_specular[] = {1.0, 1.0, 1.0, 1.0};
static float light0_position[] = {1.0, 1.0, 1.0, 0.0};

static float light1_ambient[] = {.0, .0, .0, 1.0};
static float light1_diffuse[] = {1.0, 1.0, 1.0, 1.0};
static float light1_specular[] = {1.0, 1.0, 1.0, 1.0};
static float light1_position[] = { -1.0, -1.0, -1.0, 0.0};

static unitList List;
static unitList mlist;

Blesship::Blesship() :
pc(&List,0.2/*maxspeed*/,10/*maxmissile*/,7/*weight*/,30/*maxhp*/,1/*force*/,.03/*turnRate*/,101,50,-10,coordinate(1,0,0),coordinate(0,0,1))
{
	loading = 0;

	new NPC(&List,0.185/*maxspeed*/,10/*maxmissile*/,1/*weight*/,3000/*maxhp*/,1/*force*/,.1/*turnRate*/,101,51,-11,coordinate(1,0,0),coordinate(0,0,1));
	for(int i=0;i<10;i++)
		pc.makeMissile(0.3,0,0,15,0,0.01);	
//double ms,double r,double w,double d,double f,double tr
	new item(&List,99,49,-9,coordinate(1,0,0),coordinate(0,0,1));
}

void Blesship::Initialize()
{
	// regist functions
	glutReshapeFunc(::Reshape);
	glutDisplayFunc(::Display);
	glutIdleFunc(::Idle);

	// key interface init
	glutIgnoreKeyRepeat(1);
	isleftarrow = isrightarrow = isuparrow = isdownarrow = false;
	isepress = isqpress = iswpress = isspress = isapress = false;

	// display init setting
	glClearColor(.0, .0, .0, .0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// light 0
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	// light 1
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

	isfps = true;
	isfullscreen = false;

	gstate = LOADING;
	bmenu = CUSTOME;

	// loading screen
	if(img_loading.OpenPsi("./images/loading.psi") == false) exit(0);
}

void Blesship::Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	ViewSetting();
}

void Blesship::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	switch(gstate) {
	case LOADING:
		glColor3f(1.0, 1.0, 1.0);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, img_loading.GetTextureNumber());
		glBegin(GL_POLYGON);
		glTexCoord2d(.0, .0);
		glVertex3d(-.1, -.1, .0);
		glTexCoord2d(img_loading.GetMaxXd(), 0);
		glVertex3d(.1, -.1, .0);
		glTexCoord2d(img_loading.GetMaxXd(), img_loading.GetMaxYd());
		glVertex3d(.1, .1, .0);
		glTexCoord2d(.0, img_loading.GetMaxYd());
		glVertex3d(-.1, .1, .0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		break;
	case PLAYING:
		// draw stars
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POINTS);
		for(int i = 0; i < STARNUM; i += 3) {
			glVertex3d(farstar[i] + pc.center.x,
				farstar[i+1] + pc.center.y,
				farstar[i+2] + pc.center.z);
		}
		glEnd();

		// rendering setting
		//glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glShadeModel(GL_SMOOTH);
		glCullFace(GL_FRONT);

		// draw objects
		unitSet *temp;
		temp = &(List.head);
		while(temp->next != &(List.tail)) {
			temp = temp->next;
			DrawObject(temp->thiss);
		}

		// rendering disable
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);

		// draw cockpit interface
		glColor3f(1.0, 1.0, 1.0);	// to blend clearly
		if(isfps) {			
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glEnable(GL_TEXTURE_2D);
			glCallList(list_cockpit_interf);
			glColor3f(1.0, 1.0, 1.0);
			glEnable(GL_TEXTURE_2D);
			// hp gauge
			glLoadIdentity();
			glTranslated(196/320.0,194/240.0, .0);
			glScaled(1.0, 12/240.0, 1.0);
			glScaled(120.0/320.0 * pc.getHP()/pc.maxHP(), 1.0, 1.0);
			glCallList(list_hp_gauge);
			// fuel gauge
			glLoadIdentity();
			glTranslated(197/320.0, 152/240.0, .0);
			glScaled(1.0, 12/240.0, 1.0);
			glScaled(117.0/320.0 * pc.getFule()/pc.maxFule(), 1.0, 1.0);
			glCallList(list_fuel_gauge);
			// radar
			glDisable(GL_TEXTURE_2D);
			temp = &(List.head);
			unit* obj;
			while(temp->next != &(List.tail)) {
				temp = temp->next;
				obj = temp->thiss;
				if(obj == &pc) continue;
				glLoadIdentity();
				// color setting
				if(obj->ismsl()) {
					if(obj->isbullet()) continue;
					else glColor3f(1.0f, 1.0f, .0f);
				}
				else if(obj->isitem())
					glColor3f(.0f, 1.0f, .0f);
				else glColor3f(1.0f, .0f, .0f);

				coordinate obj_vector, projection, direction;
				obj_vector.x = obj->center.x - pc.center.x;
				obj_vector.y = obj->center.y - pc.center.y;
				obj_vector.z = obj->center.z - pc.center.z;
				double innerprd = obj_vector.x * pc.head.x +
					obj_vector.y * pc.head.y * obj_vector.z * pc.head.z;
				projection.x = innerprd * pc.head.x;
				projection.y = innerprd * pc.head.y;
				projection.z = innerprd * pc.head.z;
				direction.x = obj_vector.x - projection.x;
				direction.y = obj_vector.y - projection.y;
				direction.z = obj_vector.z - projection.z;
				double distance = direction.x * direction.x +
					direction.y * direction.y + direction.z * direction.z;
				distance = sqrt(distance);
				double angle;
				direction.x /= distance;
				direction.y /= distance;
				direction.z /= distance;
				angle = acos(direction.x * pc.top.x + direction.y * pc.top.y + direction.z * pc.top.z);
				innerprd = pc.right.x * direction.x +
					pc.right.y * direction.y + pc.right.z * direction.z;
				if(innerprd < 0.0) angle *= -1.0;

				glTranslatef(0.51875, -0.770833333, .0);
				glScalef(.1, .1, 1.0);
				glRotatef(angle*180/3.14159, .0, .0, 1.0);
				glTranslatef(.0, .05 * distance, .0);
				if(distance < 35.0)
					glutSolidSphere(.1, 6, 6);

				glLoadIdentity();
				innerprd = obj_vector.x * pc.top.x +
					obj_vector.y * pc.top.y * obj_vector.z * pc.top.z;
				projection.x = innerprd * pc.top.x;
				projection.y = innerprd * pc.top.y;
				projection.z = innerprd * pc.top.z;
				direction.x = obj_vector.x - projection.x;
				direction.y = obj_vector.y - projection.y;
				direction.z = obj_vector.z - projection.z;
				distance = direction.x * direction.x +
					direction.y * direction.y + direction.z * direction.z;
				distance = sqrt(distance);
				
				direction.x /= distance;
				direction.y /= distance;
				direction.z /= distance;
				angle = acos(direction.x * pc.head.x + direction.y * pc.head.y + direction.z * pc.head.z);
				innerprd = pc.right.x * direction.x +
					pc.right.y * direction.y + pc.right.z * direction.z;
				if(innerprd < 0.0) angle *= -1.0;

				glTranslatef(0.834375, -0.770833333, .0);
				glScalef(.1, .1, 1.0);
				glRotatef(angle*180/3.14159, .0, .0, 1.0);
				glTranslatef(.0, .05 * distance, .0);
				if(distance < 35.0)
				glutSolidSphere(.1, 6, 6);
			}
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		break;
	}
	
	glutSwapBuffers();
}

void Blesship::DrawObject(unit *obj)
{
	coordinate upvector, temptop(0.0, 1.0, 0.0);
	double angle;
	bool ismissile = obj->ismsl();

	if(isfps && obj == &pc) return;
	if(ismissile) {
		if(obj->isfired() == false) return;
	}

	glLoadIdentity();
	glTranslated(obj->center.x, obj->center.y, obj->center.z);
	
	if(obj->head.x != 1.0 || obj->head.y != 0.0 || obj->head.z != 0.0) {	// 자기 자신과 외적할 수 없으므로
		upvector.x = 0;
		upvector.y = -obj->head.z;
		upvector.z = obj->head.y;

		if(obj->head.x >= 1.0) angle = 0.0;
		else if(obj->head.x <= -1.0) angle = 180.0/180*3.14159;
		else angle=acos(obj->head.x);
		glRotated(angle*180/3.14159, upvector.x, upvector.y, upvector.z);
		unit::rotate(upvector,temptop,cos(angle),sin(angle));
	}

	double dotproduct = obj->top.x*temptop.x + obj->top.y*temptop.y + obj->top.z*temptop.z;
	if(dotproduct >= 1.0) angle = 0.0;
	else if(dotproduct <= -1.0) angle = 180.0;
	else angle = acos(dotproduct)*180/3.14159;
	double upvector_len, angle_flag;
	upvector.x = obj->top.y*temptop.z - obj->top.z*temptop.y;
	upvector.y = obj->top.z*temptop.x - obj->top.x*temptop.z;
	upvector.z = obj->top.x*temptop.y - obj->top.y*temptop.x;
	upvector_len = sqrt(upvector.x*upvector.x + upvector.y*upvector.y + upvector.z*upvector.z);
	upvector.x /= upvector_len;
	upvector.y /= upvector_len;
	upvector.z /= upvector_len;
	angle_flag = upvector.x*obj->head.x + upvector.y*obj->head.y + upvector.z*obj->head.z;	// head벡터와 회전축 벡터와의 내적, 1또는 -1이 나온다.수학적으론..
	if(angle_flag < 0.0)
		glRotated(angle, 1.0, .0, .0);
	else
		glRotated(-angle, 1.0, .0, .0);
	
	glColor3f(1.0f, 1.0f, 1.0f);	
	if(ismissile) {
		if(obj->isbullet()) {
			// bullet
			if(obj->liv == lif) {
				glDisable(GL_TEXTURE_2D);
				glEnable(GL_LIGHTING);
				glutSolidSphere(.03, 16, 16);
			}
		}
		else {
			// missile
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			if(obj->liv == lif) {
				glScalef(.1, .1, .1);
				glCallList(list_missile);
			}
			else if(obj->liv == tickle) {
				Boom(obj, .3);
			}
		}
	}
	else if(obj->isitem()) {
		// item
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glutSolidCube(.1);
	}
	else {
		// ship
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		if(obj->liv == lif) {
			glScalef(.3f, .3f, .3f);
			glCallList(list_ship_xwing);
		}
		else if(obj->liv == tickle)
			Boom(obj, .8);	
	}
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}

void Blesship::Boom(unit *obj, double scale)
{
	int ttime = obj->ttime;

	glRotatef(90.0f, .0, 1.0, .0);
	glScalef(scale, scale, scale);
	if(ttime < 30)
		glScalef(ttime * 1/30.0, ttime * 1/30.0, ttime * 1/30.0);
	else
		glRotatef(rand() % 360, .0, .0, 1.0);
	glCallList(list_boom);
}

// loading function
void Blesship::Idle()
{
	int i;
	double maxx, maxy;
	switch(loading) {
	case 0:
		// star position generating
		for(i = 0; i < STARNUM; i++)
			farstar[i] = rand() - RAND_MAX/2;
		double vec_size;
		for(i = 0; i < STARNUM; i += 3) {
			vec_size = farstar[i] * farstar[i] +
				farstar[i+1] * farstar[i+1] +
				farstar[i+2] * farstar[i+2];
			vec_size = sqrt(vec_size);
			if(vec_size > 0.0) {
				farstar[i] *= MAX_SIGHT / vec_size;
				farstar[i+1] *= MAX_SIGHT / vec_size;
				farstar[i+2] *= MAX_SIGHT / vec_size;
			}
		}
		break;
	case 1:
		if(psi_small.OpenPsi("./images/small.psi") == false ) printf("error interface images"), exit(0);
		if(psi_hp.OpenPsi("./images/hpfuel.psi") == false) printf("error interface images"), exit(0);
		if(psi_msl.OpenPsi("./images/msl.psi") == false) printf("error interface images"), exit(0);
		if(psi_radar.OpenPsi("./images/radar.psi") == false) printf("error interface images"), exit(0);
		break;
	case 2:
		// cockpit interface
		list_cockpit_interf = glGenLists(1);
		// HP, Fuel gauges
		glNewList(list_cockpit_interf, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, psi_hp.GetTextureNumber());
		maxx = psi_hp.GetMaxXd();
		maxy = psi_hp.GetMaxYd();
		glBegin(GL_QUADS);
		glTexCoord2d(.0, .0);
		glVertex2d(0.60625, 0.625);
		glTexCoord2d(maxx, 0.0);
		glVertex2d(1.0, 0.625);
		glTexCoord2d(maxx, maxy);
		glVertex2d(1.0, 1.0);
		glTexCoord2d(.0, maxy);
		glVertex2d(0.60625, 1.0);
		glEnd();
		// missile panel
		glBindTexture(GL_TEXTURE_2D, psi_msl.GetTextureNumber());
		maxx = psi_msl.GetMaxXd();
		maxy = psi_msl.GetMaxYd();
		glBegin(GL_QUADS);
		glTexCoord2d(.0, .0);
		glVertex2d(-1.0, -1.0);
		glTexCoord2d(maxx, .0);
		glVertex2d(0.284375, -1.0);
		glTexCoord2d(maxx, maxy);
		glVertex2d(0.284375, -121.0/240.0);
		glTexCoord2d(.0, maxy);
		glVertex2d(-1.0, -121.0/240.0);
		glEnd();
		// radar panel
		glBindTexture(GL_TEXTURE_2D, psi_radar.GetTextureNumber());
		maxx = psi_radar.GetMaxXd();
		maxy = psi_radar.GetMaxYd();
		glBegin(GL_QUADS);
		glTexCoord2d(.0, .0);
		glVertex2d(0.284375, -1.0);
		glTexCoord2d(maxx, .0);
		glVertex2d(1.0, -1.0);
		glTexCoord2d(maxx, maxy);
		glVertex2d(1.0, -121.0/240.0);
		glTexCoord2d(.0, maxy);
		glVertex2d(0.284375, -121.0/240.0);
		glEnd();
		// bullet aim(bead)
		glBindTexture(GL_TEXTURE_2D, psi_small.GetTextureNumber());
		glBegin(GL_QUADS);
		glTexCoord2d(0.53125, 0.9921875);
		glVertex2d(-0.0625, -20.0/240.0);
		glTexCoord2d(0.84375, 0.9921875);
		glVertex2d(0.0625, -20.0/240.0);
		glTexCoord2d(0.84375, 0.6796875);
		glVertex2d(0.0625, 20.0/240.0);
		glTexCoord2d(0.53125, 0.6796875);
		glVertex2d(-0.0625, 20.0/240.0);
		glEnd();
		// rearview
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glColor3ub(42, 32, 246);
		glVertex2d(-1.0, 1.0);
		glVertex2d(-1.0, 0.9875);
		glVertex2d(-0.5, 0.9875);
		glVertex2d(-0.5, 1.0);
		
		glVertex2d(-0.5, 1.0);
		glVertex2d(-0.4875, 1.0);
		glVertex2d(-0.4875, 0.45);
		glVertex2d(-0.5, 0.45);

		glVertex2d(-1.0, 0.45);
		glVertex2d(-1.0, 0.4625);
		glVertex2d(-0.5, 0.4625);
		glVertex2d(-0.5, 0.45);

		glVertex2d(-1.0, 0.9875);
		glVertex2d(-0.990625, 0.9875);
		glVertex2d(-0.990625, 0.4625);
		glVertex2d(-1.0, 0.4625);
		glEnd();
		glEndList();
		break;
	case 3:
		// hp gauge
		list_hp_gauge = glGenLists(1);
		glNewList(list_hp_gauge, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, psi_small.GetTextureNumber());
		glBegin(GL_QUADS);
		glTexCoord2d(14/128.0, 95/128.0);
		glVertex2d(.0, .0);
		glTexCoord2d(17/128.0, 95/128.0);
		glVertex2d(1.0, .0);
		glTexCoord2d(17/128.0, 98/128.0);
		glVertex2d(1.0, 1.0);
		glTexCoord2d(14/128.0, 98/128.0);
		glVertex2d(.0, 1.0);
		glEnd();
		glEndList();
		// fuel gauge
		list_fuel_gauge = glGenLists(1);
		glNewList(list_fuel_gauge, GL_COMPILE);
		glBegin(GL_QUADS);
		glTexCoord2d(22/128.0, 95/128.0);
		glVertex2d(.0, .0);
		glTexCoord2d(25/128.0, 95/128.0);
		glVertex2d(1.0, .0);
		glTexCoord2d(25/128.0, 98/128.0);
		glVertex2d(1.0, 1.0);
		glTexCoord2d(22/128.0, 98/128.0);
		glVertex2d(.0, 1.0);
		glEnd();
		glEndList();
		break;
	case 4:
		if(psi_xbtm.OpenPsi("./images/x_btm.psi") == false) printf("error x wing images"), exit(0);
		if(psi_xtop.OpenPsi("./images/x_hd.psi") == false) printf("error x wing images"), exit(0);
		if(psi_xback.OpenPsi("./images/x_back.psi") == false) printf("error x wing images"), exit(0);
		break;
	case 5:
		// x wing
		list_ship_xwing = glGenLists(1);
		glNewList(list_ship_xwing, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, psi_xbtm.GetTextureNumber());
		glBegin(GL_TRIANGLES);
		// bottom
		glNormal3d(1/sqrt(3), -1/sqrt(3), 1/sqrt(3));
		glTexCoord2d(83.0/512.0, 104.0/512.0);
		glVertex3d(3.0, 0.0, 0.0);
		glTexCoord2d(485.0/512.0, 283.0/512.0);
		glVertex3d(-1.0, 0.0, 2.0);
		glTexCoord2d(378.0/512.0, 328.0/512.0);
		glVertex3d(-1.0, -1.0, 0.0);
		glNormal3d(1/sqrt(3), -1/sqrt(3), -1/sqrt(3));
		glTexCoord2d(69.0/512.0, 110.0/512.0);
		glVertex3d(3.0, .0, .0);
		glTexCoord2d(245.0/512.0, 339.0/512.0);
		glVertex3d(-1.0, .0, -2.0);
		glTexCoord2d(342.0/512.0, 335.0/512.0);
		glVertex3d(-1.0, -1.0, .0);
		glEnd();
		// top
		glBindTexture(GL_TEXTURE_2D, psi_xtop.GetTextureNumber());
		glBegin(GL_TRIANGLES);
		glNormal3d(1/sqrt(3), 1/sqrt(3), 1/sqrt(3));
		glTexCoord2d(504.0/512.0, 356.0/512.0);
		glVertex3d(3.0, .0, .0);
		glTexCoord2d(146.0/512.0, 69.0/512.0);
		glVertex3d(-1.0, 1.0, .0);
		glTexCoord2d(205.0/512.0, 63.0/512.0);
		glVertex3d(-1.0, .0, 2.0);
		glNormal3d(1/sqrt(3), 1/sqrt(3), -1/sqrt(3));
		glTexCoord2d(496.0/512.0, 364.0/512.0);
		glVertex3d(3.0, .0, .0);
		glTexCoord2d(133.0/512.0, 72.0/512.0);
		glVertex3d(-1.0, 1.0, .0);
		glTexCoord2d(56.0/512.0, 121.0/512.0);
		glVertex3d(-1.0, .0, -2.0);
		glEnd();
		// back
		glBindTexture(GL_TEXTURE_2D, psi_xback.GetTextureNumber());
		glBegin(GL_QUADS);
		glNormal3d(-1, 0, 0);
		glTexCoord2d(299.0/512.0, 1-88.0/128.0);
		glVertex3d(-1.0, .0, 2.0);
		glTexCoord2d(202.0/512.0, 1-106.6/128.0);
		glVertex3d(-1.0, -1.0, .0);
		glTexCoord2d(38.0/512.0, 1-71.0/128.0);
		glVertex3d(-1.0, .0, -2.0);
		glTexCoord2d(194.0/512.0, 1-19.0/128.0);
		glVertex3d(-1.0, 1.0, .0);
		glEnd();
		glEndList();
		break;
	case 6:
		if(psi_missile.OpenPsi("./images/missile.psi") == false) printf("error missile images"), exit(0);
		if(psi_missile_back.OpenPsi("./images/missi_b.psi") == false) printf("error misssile images"), exit(0);
		if(psi_boom.OpenPsi("./images/boom.psi") == false) printf("error boom image"), exit(0);
		break;
	case 7:
		list_missile = glGenLists(1);
		glNewList(list_missile, GL_COMPILE);
		// head
		glBindTexture(GL_TEXTURE_2D, psi_missile.GetTextureNumber());
		glBegin(GL_TRIANGLES);
		glTexCoord2d(317/512.0, 205/256.0);
		glVertex3d(1.0, .0, .0);
		glTexCoord2d(304/512.0, 213/256.0);
		glVertex3d(.0, .0, 1.0);
		glTexCoord2d(302/512.0, 195/256.0);
		glVertex3d(.0, -1.0, .0);

		glTexCoord2d(7/512.0, 147/256.0);
		glVertex3d(1.0, .0, .0);
		glTexCoord2d(23/512.0, 156/256.0);
		glVertex3d(.0, -1.0, .0);
		glTexCoord2d(23/512.0, 138/256.0);
		glVertex3d(.0, .0, -1.0);

		glTexCoord2d(318/512.0, 90/256.0);
		glVertex3d(1.0, .0, .0);
		glTexCoord2d(302/512.0, 98/256.0);
		glVertex3d(.0, .0, -1.0);
		glTexCoord2d(303/512.0, 81/256.0);
		glVertex3d(.0, 1.0, .0);

		glTexCoord2d(7/512.0, 39/256.0);
		glVertex3d(1.0, .0, .0);
		glTexCoord2d(23/512.0, 47/256.0);
		glVertex3d(.0, 1.0, .0);
		glTexCoord2d(24/512.0, 27/256.0);
		glVertex3d(.0, .0, 1.0);
		glEnd();
		// body
		glBegin(GL_QUADS);
		glTexCoord2d(28/512.0, 47/256.0);
		glVertex3d(.0, 1.0, .0);
		glTexCoord2d(28/512.0, 26/256.0);
		glVertex3d(.0, .0, 1.0);
		glTexCoord2d(262/512.0, 26/256.0);
		glVertex3d(-7.0, .0, 1.0);
		glTexCoord2d(262/512.0, 47/256.0);
		glVertex3d(-7.0, 1.0, .0);

		glTexCoord2d(291/512.0, 99/256.0);
		glVertex3d(.0, -1.0, .0);
		glTexCoord2d(291/512.0, 80/256.0);
		glVertex3d(.0, .0, 1.0);
		glTexCoord2d(59/512.0, 80/256.0);
		glVertex3d(-7.0, .0, 1.0);
		glTexCoord2d(59/512.0, 99/256.0);
		glVertex3d(-7.0, -1.0, .0);

		glTexCoord2d(29/512.0, 137/256.0);
		glVertex3d(.0, 1.0, .0);
		glTexCoord2d(29/512.0, 156/256.0);
		glVertex3d(.0, .0, -1.0);
		glTexCoord2d(266/512.0, 156/256.0);
		glVertex3d(-7.0, .0, -1.0);
		glTexCoord2d(266/512.0, 137/256.0);
		glVertex3d(-7.0, 1.0, .0);

		glTexCoord2d(294/512.0, 213/256.0);
		glVertex3d(.0, -1.0, .0);
		glTexCoord2d(294/512.0, 195/256.0);
		glVertex3d(.0, .0, -1.0);
		glTexCoord2d(58/512.0, 195/256.0);
		glVertex3d(-7.0, .0, -1.0);
		glTexCoord2d(58/512.0, 213/256.0);
		glVertex3d(-7.0, -1.0, .0);
		glEnd();
		// tail
		maxx = psi_missile_back.GetMaxXd();
		maxy = psi_missile_back.GetMaxYd();
		glBindTexture(GL_TEXTURE_2D, psi_missile_back.GetTextureNumber());
		glBegin(GL_QUADS);
		glTexCoord2d(.0, .0);
		glVertex3d(-7.0, 1.0, .0);
		glTexCoord2d(.0, maxy);
		glVertex3d(-7.0, .0, 1.0);
		glTexCoord2d(maxx, maxy);
		glVertex3d(-7.0, -1.0, .0);
		glTexCoord2d(maxx, .0);
		glVertex3d(-7.0, .0, -1.0);
		glEnd();
		glEndList();
		
		// boom
		maxx = psi_boom.GetMaxXd();
		maxy = psi_boom.GetMaxYd();
		list_boom = glGenLists(1);
		glNewList(list_boom, GL_COMPILE);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, psi_boom.GetTextureNumber());
		glBegin(GL_QUADS);
		glTexCoord2d(.0, .0);
		glVertex2d(-1.0, -1.0);
		glTexCoord2d(.0, maxy);
		glVertex2d(-1.0, 1.0);
		glTexCoord2d(maxx, maxy);
		glVertex2d(1.0, 1.0);
		glTexCoord2d(maxx, .0);
		glVertex2d(1.0, -1.0);
		glEnd();
		glEndList();
		break;
	default:
		istidle = true;
		glutIdleFunc(NULL);
		glutKeyboardFunc(::KeyInput);
		glutSpecialFunc(::SKeyInput);
		glutKeyboardUpFunc(::KeyUp);
		glutSpecialUpFunc(::SKeyUp);

		glutTimerFunc(1000/FRAME_PER_SEC, ::Timer, 0);
		gstate = PLAYING;
	}
	loading++;
}

void Blesship::TIdle(int value)
{
	if(istidle == false) return;
	glutTimerFunc(1000/FRAME_PER_SEC, ::Timer, 0);
	
	switch(gstate) {
	case OPENING:
		break;
	case BEGINMENU:
		break;
	case PLAYING:
		pcKeyInput();
		List.idle();
		ViewSetting();
		Display();
		break;
	case GAMEOVER:
		break;
	}
}

void Blesship::KeyInput(unsigned char key, int x, int y)
{
	switch(key) {
	case 'w':
	case 'W':
		iswpress = true;
		break;
	case 'q':
	case 'Q':
		isqpress = true;
		break;
	case 'a':
	case 'A':
		isapress = true;
		break;
	case 's':
	case 'S':
		isspress = true;
		break;
	case 'd':
	case 'D':
		isdpress = true;
		break;
	case 'e':
	case 'E':
		isepress = true;
		break;
	case ' ':
		pc.stop();
	}
	if(key >= '0' && key <= '9') {
		pc.setmissile(key-'0');
	}
}

void Blesship::KeyUp(unsigned char key, int x, int y)
{
	switch(key) {
	case 'q':
	case 'Q':
		isqpress = false;
		break;
	case 'w':
	case 'W':
		iswpress = false;
		break;
	case 'a':
	case 'A':
		isapress = false;
		break;
	case 's':
	case 'S':
		isspress = false;
		break;
	case 'd':
	case 'D':
		isdpress = false;
		break;
	case 'e':
	case 'E':
		isepress = false;
		break;
	}
}
void Blesship::SKeyInput(int key, int x, int y)
{
	switch(key) {
	case GLUT_KEY_F4:
		// exit program
		if(glutGetModifiers() & GLUT_ACTIVE_ALT)	// ALT + F4
			exit(0);
		// full screen mode
		if(isfullscreen) {
			glutReshapeWindow(iWindowWidth, iWindowHeight);
			glutPositionWindow(10, 40);
			glutSetCursor(GLUT_CURSOR_INHERIT);
			isfullscreen = false;
		}
		else  {
			glutFullScreen();
			glutSetCursor(GLUT_CURSOR_NONE);
			isfullscreen = true;
		}
		break;
	case GLUT_KEY_F5:
		isfps = !isfps;
		break;
	case GLUT_KEY_UP:
		isuparrow = true;
		break;
	case GLUT_KEY_DOWN:
		isdownarrow = true;
		break;
	case GLUT_KEY_LEFT:
		isleftarrow = true;
		break;
	case GLUT_KEY_RIGHT:
		isrightarrow = true;
		break;
	}
}

void Blesship::SKeyUp(int key, int x, int y)
{
	switch(key) {
	case GLUT_KEY_UP:
		isuparrow = false;
		break;
	case GLUT_KEY_DOWN:
		isdownarrow = false;
		break;
	case GLUT_KEY_RIGHT:
		isrightarrow = false;
		break;
	case GLUT_KEY_LEFT:
		isleftarrow = false;
		break;
	}
}


void Blesship::pcKeyInput()
{
	if(isleftarrow != isrightarrow) {
		if(isleftarrow) pc.rotate1(true);
		else if(isrightarrow) pc.rotate1(false);
	}
	if(isuparrow != isdownarrow) {
		if(isuparrow) pc.rotate2(true);
		else if(isdownarrow) pc.rotate2(false);
	}
	if(isqpress != iswpress) {
		if(isqpress) pc.rotate3(false);
		else if(iswpress) pc.rotate3(true);
	}
	if(isapress != isspress) {
		if(isapress) pc.accelate(true);
		else if(isspress) pc.accelate(false);
	}
	if(isdpress) pc.fire();
	if(isepress) pc.shot();
}

void Blesship::ViewSetting()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, dWindowWidth/dWindowHeight, 0.1, MAX_SIGHT);
	if(isfps) {
		gluLookAt(.0, .0, .0,
			pc.head.x, pc.head.y, pc.head.z,
			pc.top.x, pc.top.y, pc.top.z);
		glTranslated(-pc.center.x, -pc.center.y, -pc.center.z);
	}
	else {
		glTranslated(-pc.center.x, -pc.center.y, -pc.center.z -10.0);
	}
}
