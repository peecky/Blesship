#include<cmath>
#include<iostream>

enum life { lif, tickle, dead };

using namespace std;

struct coordinate
{
	double x, y, z;
	coordinate();
	coordinate(double,double,double);
	void up();
	void unital();
};

coordinate radar/*top-right*/(coordinate co2/*head*/,coordinate co3);

struct unit
{
	life liv;int ttime;
	coordinate center,head,right,top;
	void setcoordinate(double,double,double,coordinate,coordinate);
	void go(double);
	void nextfloor();
	void clearfloor();
	life getfloor();
	virtual void setlock(bool,unit*);
	virtual void distroy()=0;
	static void rotate(coordinate&,coordinate&,double,double);
	virtual void hited(double);
	virtual void idle();
	virtual bool ismsl() = 0;
	virtual bool isitem();
	virtual bool isfired();
	virtual bool isbullet();
	virtual void getitem(int,int);
	coordinate getcoordinate();
};

struct unitSet
{
	unit *thiss;
	unitSet *next;
	unitSet();
	~unitSet();
};

struct unitList
{
	unitSet head,tail;
	int count;
	unitList();
	void add(unit*);
	void sub(unit*);
	void idle();
	unit* search(unit*);
	unit* lock(const coordinate&,const coordinate&);
	unit* find(const coordinate&);
};

class item:public unit
{
	int Case,num;
	unitList *set;
//void airplane::makeMissile(double ms,double r,double w,double d,double f,double tr) {
public:
	void distroy();
	void idle();
	bool ismsl();
	bool isitem();
	item(unitList*,double,double,double,coordinate,coordinate);
};

class flyingUnit:public unit
{
protected:
	double weight, durability,max_hp, force,turnRate,speed,maxspeed;
public:
	flyingUnit();
	flyingUnit(double,double,double,double,double);
	flyingUnit(double,double,double,double,double,double,double,double,coordinate,coordinate);
	~flyingUnit();

	void move();
	void rotate1(bool);
	void rotate2(bool);
	void rotate3(bool);
	void accelate(bool);
	double getf();
	double getw();
	double gett();
	double gets();
	virtual void hit();
	virtual void hited(double);
	void sets(double);
	void sett(double);
	void setspeck(double,double,double,double);
};

class missile:public flyingUnit
{
protected:
	missile();
	unit *target;
	unitList *set;
	bool fired;
	flyingUnit *master;
	double range,fule,damage,maxturnrate;
public:
	missile(unitList *,flyingUnit*,double,double,double,double,double,double);
	missile(unitList *,flyingUnit*,double,double,double,double,double,double,double,double,double,coordinate,coordinate);

	bool ismsl();
	bool isfired();
	void idle();
	void distroy();
	void hited(double);
	virtual void fire(unit*);
	void rotate1(bool);
	void rotate2(bool);
	void rotate3(bool);
};

class bullet:public missile
{
protected:
	int lifetime;
public:
	bullet(unitList *,flyingUnit*,double,double,double,double,double,double,double,double,double,coordinate,coordinate);
	void idle();
	bool isbullet();
	void distroy();
	void fire(unit*);
};

class airplane:public flyingUnit
{
protected:
	missile **sidewinder;
	double missilespeck[4][6];
	int *sidew;
	bool locked;
	int side,select,score;
	int cooltime,cttime;
	double fule,maxfule;
	unit *target;
	unit *mi;
	unitList *Set;
public:
	airplane(unitList*,double,int,double,double,double,double,double,double,double,coordinate,coordinate);
	~airplane();
	bool ismsl();
	void rotate1(bool);
	void rotate2(bool);
	void rotate3(bool);
	void accelate(bool);
	void stop();
	void setlock(bool,unit*);
	void setmissile(int);
	void lockOn();
	double maxHP();
	double getHP();
	double maxFule();
	double getFule();
	missile* getmissileaddress(int select);
	void fire();
	void getitem(int,int);
	void makeMissile(double,double,double,double,double,double);
	void hit();
	void hited(double);
	void shot();
	void idle();
};

class PC:public airplane
{
public:
	PC(unitList*,double,int,double,double,double,double,double,double,double,coordinate,coordinate);
	void hited(double);
	void idle();
	void distroy();
};
class NPC:public airplane
{
public:
	NPC(unitList*,double,int,double,double,double,double,double,double,double,coordinate,coordinate);
	void distroy();
	void idle();
	void hited(double);
};

double distance(coordinate,coordinate);
