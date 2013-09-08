#include<cmath>
#include<cstdlib>
//#include<gl/glut.h>

#include"project.h"
#include "../defines.h"



void rotate(coordinate& co2,coordinate& co3,double cos1,double sin1)
{
    double d1,d2;
    coordinate temp;
    d1=co2.z*co2.z+co2.x*co2.x;
    d2=d1+co2.y*co2.y;
    d1=sqrtl(d1);
    d2=sqrtl(d2);
    if(d1!=0) {
	temp.x=(co3.x*co2.z/d1) + (co3.z*(-1)*co2.x/d1);
	temp.y=(co3.x*(-1)*co2.x*co2.y/(d1*d2)) + (co3.y*(co2.x*co2.x+co2.z*co2.z)/(d1*d2)) + (co3.z*(-1)*co2.y*co2.z/(d1*d2));
	temp.z=(co3.x*co2.x*(co2.x*co2.x+co2.z*co2.z)/(d1*d2*d1)) + (co3.y*co2.y/d2) + (co3.z*co2.z*(co2.z*co2.z+co2.x*co2.x)/(d2*d1*d1));
	co3.x=temp.x;
	co3.y=temp.y;
	co3.z=temp.z;

	temp.x=cos1*co3.x+(-1)*sin1*co3.y;
	temp.y=sin1*co3.x+co3.y*cos1;
	co3.x=temp.x;
	co3.y=temp.y;

	temp.x=(co3.x*co2.z/d1) + (co3.y*(-1)*co2.x*co2.y/(d1*d2)) + (co3.z*co2.x*(co2.z*co2.z+co2.x*co2.x)/(d1*d1*d2));
	temp.y=(co3.y*(co2.z*co2.z+co2.x*co2.x)/(d1*d2)) + (co3.z*co2.y/d2);
	temp.z=(co3.x*(-1)*co2.x/d1) + (co3.y*(-1)*co2.z*co2.y/(d1*d2)) + (co3.z*co2.z*(co2.z*co2.z+co2.x*co2.x)/(d1*d1*d2));
	co3.x=temp.x;
	co3.y=temp.y;
	co3.z=temp.z;
    }
    else {
	double c,s;
	coordinate bulletship(1,0,0);
	c=cos(-1.57);
	s=sin(-1.57);
	rotate(bulletship,co2,c,s);
	rotate(bulletship,co3,c,s);

	rotate(co2,co3,cos1,sin1);

	c=cos(1.57);
	s=sin(1.57);
	rotate(bulletship,co2,c,s);
	rotate(bulletship,co3,c,s);
    }
}
	

double distance(coordinate a,coordinate b) {
	double temp;
	temp=(a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z);
	temp=sqrt(temp);
	return temp;
}

//////////////////////////////////coordinate class////////////////////////////////
coordinate::coordinate(){
	x=0.0;
	y=0.0;
	z=0.0;
}

coordinate::coordinate(double xx,double yy,double zz){
	x=xx;
	y=yy;
	z=zz;
}

void coordinate::unital()
{
	double temp;
	temp=x*x+y*y+z*z;
	temp=sqrt(temp);
	x/=temp;
	y/=temp;
	z/=temp;
}

void coordinate::up() {
	double i;
	for(i=-1.1;i<1.1;i+=0.0001) {
		if((x-i) < 0.0001 && (x-i) > -0.0001 ) {
			x=i;
		}
		if((y-i) < 0.0001 && (y-i) > -0.0001 ) {
			y=i;
		}
		if((z-i) < 0.0001 && (z-i) > -0.0001 ) {
			z=i;
		}
	}
}

//////////////////////////////////unit class////////////////////////////////

void unit::clearfloor() {
	liv=lif;
	ttime=-1;
}

void unit::nextfloor() {
	if(liv==lif) {
		liv=tickle;
		ttime=0;
	}
	else if(liv==tickle) {
		if(ttime>60)
			liv=dead;
		else
			ttime++;
	}
}

life unit::getfloor() {
	return liv;
}

void unit::setcoordinate(double x,double y,double z,coordinate direction1,coordinate direction2) {
//중심좌표
	center.x=x;
	center.y=y;
	center.z=z;
//head와, right는 주어진걸로 입력, 주어진 것은 unit vector로 가정.
	head.x=direction1.x;
	head.y=direction1.y;
	head.z=direction1.z;
	head.unital();

	right.x=direction2.x;
	right.y=direction2.y;
	right.z=direction2.z;
	right.unital();
//두 vector에서 top벡터 계산.
	top.x=direction1.y*direction2.z-direction1.z*direction2.y;
	top.y=-1*(direction1.x*direction2.z-direction1.z*direction2.x);
	top.z=direction1.x*direction2.y-direction1.y*direction2.x;
	top.unital();
}

coordinate unit::getcoordinate() {
	return center;
}

void unit::idle() {
	if(liv==tickle) {
		nextfloor();
	}
}

void unit::getitem(int c,int n) {
}

void unit::setlock(bool lock,unit *mi) {
}

void unit::go(double speed) {
	center.x+=head.x*speed;
	center.y+=head.y*speed;
	center.z+=head.z*speed;
}

coordinate radar/*top-right*/(coordinate co2/*head*/,coordinate co3) {
	double d1,d2;
	coordinate temp;
	d1=co2.z*co2.z+co2.x*co2.x;
	d2=d1+co2.y*co2.y;
	d1=sqrtl(d1);
	d2=sqrtl(d2);

	temp.x=(co3.x*co2.z/d1) + (co3.z*(-1)*co2.x/d1);
	temp.y=(co3.x*(-1)*co2.x*co2.y/(d1*d2)) + (co3.y*(co2.x*co2.x+co2.z*co2.z)/(d1*d2)) + (co3.z*(-1)*co2.y*co2.z/(d1*d2));
	temp.z=(co3.x*co2.x*(co2.x*co2.x+co2.z*co2.z)/(d1*d2*d1)) + (co3.y*co2.y/d2) + (co3.z*co2.z*(co2.z*co2.z+co2.x*co2.x)/(d2*d1*d1));
	co3.x=temp.x;
	co3.y=temp.y;
	co3.z=temp.z;
	co3.z=0;

	return co3;
}

void unit::rotate(coordinate& co2,coordinate& co3,double cos1,double sin1)
{
	double d1,d2;
	coordinate temp;
	d1=co2.z*co2.z+co2.x*co2.x;
	d2=d1+co2.y*co2.y;
	d1=sqrtl(d1);
	d2=sqrtl(d2);
	if(d1!=0) {
	//회전 중심 벡터를 z로 옮기는 매트릭스
		temp.x=(co3.x*co2.z/d1) + (co3.z*(-1)*co2.x/d1);
		temp.y=(co3.x*(-1)*co2.x*co2.y/(d1*d2)) + (co3.y*(co2.x*co2.x+co2.z*co2.z)/(d1*d2)) + (co3.z*(-1)*co2.y*co2.z/(d1*d2));
		temp.z=(co3.x*co2.x*(co2.x*co2.x+co2.z*co2.z)/(d1*d2*d1)) + (co3.y*co2.y/d2) + (co3.z*co2.z*(co2.z*co2.z+co2.x*co2.x)/(d2*d1*d1));
		co3.x=temp.x;
		co3.y=temp.y;
		co3.z=temp.z;
	//벡터 중심으로 회전
		temp.x=cos1*co3.x+(-1)*sin1*co3.y;
		temp.y=sin1*co3.x+co3.y*cos1;
		co3.x=temp.x;
		co3.y=temp.y;
	//역매트릭스
		temp.x=(co3.x*co2.z/d1) + (co3.y*(-1)*co2.x*co2.y/(d1*d2)) + (co3.z*co2.x*(co2.z*co2.z+co2.x*co2.x)/(d1*d1*d2));
		temp.y=(co3.y*(co2.z*co2.z+co2.x*co2.x)/(d1*d2)) + (co3.z*co2.y/d2);
		temp.z=(co3.x*(-1)*co2.x/d1) + (co3.y*(-1)*co2.z*co2.y/(d1*d2)) + (co3.z*co2.z*(co2.z*co2.z+co2.x*co2.x)/(d1*d1*d2));
		co3.x=temp.x;
		co3.y=temp.y;
		co3.z=temp.z;
	}
	else {
		double c,s;
		coordinate bulletship(1,0,0);
		c=cos(-1.57);
		s=sin(-1.57);
		::rotate(bulletship,co2,c,s);
		::rotate(bulletship,co3,c,s);
	
		::rotate(co2,co3,cos1,sin1);

		c=cos(1.57);
		s=sin(1.57);
		::rotate(bulletship,co2,c,s);
		::rotate(bulletship,co3,c,s);
	}

//	co2.up();
//	co3.up();
}

void unit::hited(double damage) {
}

bool unit::isfired() {
	return false;
}

bool unit::isitem() {
	return false;
}

bool unit::isbullet() {
	return false;
}

//////////////////////////////////flyingUnit class////////////////////////////////

void flyingUnit::accelate(bool fs) {
	if(fs==true && speed<maxspeed) {
		speed+=(force/weight);
		if(speed>maxspeed)
			speed=maxspeed;
	}
	else if(!fs && speed*(-1) < maxspeed) {
		speed-=(force/weight);
		if(speed*(-1) > maxspeed)
			speed=(-1)*maxspeed;
	}
}

void flyingUnit::rotate1(bool rl) {
	if(rl) {
		unit::rotate(top,head,cos(turnRate),sin(turnRate));
		unit::rotate(top,right,cos(turnRate),sin(turnRate));
	}
	else {
		unit::rotate(top,head,cos(-turnRate),sin(-turnRate));
		unit::rotate(top,right,cos(-turnRate),sin(-turnRate));
	}
}

void flyingUnit::rotate2(bool ud) {
	if(ud) {
		unit::rotate(right,head,cos(turnRate),sin(turnRate));
		unit::rotate(right,top,cos(turnRate),sin(turnRate));
	}
	else {
		unit::rotate(right,head,cos(-turnRate),sin(-turnRate));
		unit::rotate(right,top,cos(-turnRate),sin(-turnRate));
	}
}

void flyingUnit::rotate3(bool ss) {
	if(ss) {
		unit::rotate(head,top,cos(turnRate),sin(turnRate));
		unit::rotate(head,right,cos(turnRate),sin(turnRate));
	}
	else {
		unit::rotate(head,top,cos(-turnRate),sin(-turnRate));
		unit::rotate(head,right,cos(-turnRate),sin(-turnRate));
	}
}

flyingUnit::flyingUnit() {
	weight=1;
	force=0;
	speed=0;
	turnRate=0;
}

flyingUnit::flyingUnit(double ms,double w,double d,double f,double tr) {
	weight=w;
	if(weight<=0) weight=1;
	durability=d;
	max_hp=durability;
	force=f;
	speed=0;
	maxspeed=ms;
	turnRate=tr;
}

flyingUnit::flyingUnit(double ms,double w,double d, double f,double tr,double x,double y,double z,coordinate d1,coordinate d2) {
	weight=w;
	if(weight<=0) weight=1;
	durability=d;
	max_hp=durability;
	force=f;
	speed=0;
	maxspeed=ms;
	turnRate=tr;

	unit::setcoordinate(x,y,z,d1,d2);	
}

flyingUnit::~flyingUnit() {
}

void flyingUnit::move() {
	go(speed);
}

void flyingUnit::setspeck(double w,double d, double f,double tr) {
	weight=w;
	durability=d;
	force=f;
	turnRate=tr;
}

double flyingUnit::getf() {
	return force;
}

double flyingUnit::gett() {
	return turnRate;
}

double flyingUnit::getw() {
	return weight;
}

double flyingUnit::gets() {
	return speed;
}

void flyingUnit::sett(double t) {
	turnRate=t;
}
void flyingUnit::sets(double s) {
	speed=s;
}

void flyingUnit::hit() {
}

void flyingUnit::hited(double damage) {
	durability-=damage;
}
//////////////////////////////////missile class////////////////////////////////

missile::missile(unitList *s,flyingUnit *m,double r,double ms, double a,double b,double c,double d):flyingUnit(ms,a,b,c,d) {
	fired=false;
	range=r;
	master=m;
	set=s;
}

missile::missile(unitList *s,flyingUnit *m,double r,double ms, double w,double d, double f,double tr,double x,double y,double z,coordinate d1,coordinate d2):flyingUnit(ms,w,0,f,tr,x,y,z,d1,d2) {
	damage=d;
	fired=false;
	range=r;
	master=m;
	maxturnrate=tr;
	set=s;
}

void missile::hited(double damage) {
	durability-=damage;
	if(durability<0) {
		set->sub(this);
	}
}

void missile::distroy() {
	delete this;
}

void missile::rotate1(bool fs) {
	fule-=(getw()*gett());
	flyingUnit::rotate1(fs);
}

void missile::rotate2(bool fs) {
	fule-=(getw()*gett());
	flyingUnit::rotate2(fs);
}

void missile::rotate3(bool fs) {
	fule-=(getw()*gett());
	flyingUnit::rotate3(fs);
}

void missile::fire(unit* tar) {
	target=tar;
	sets(maxspeed);
	center=master->center;
	head=master->head;
	top=master->top;
	right=master->right;
	fired=true;
	set->add(this);
	target->setlock(true,this);
}

bool missile::ismsl()
{
	return true;
}

bool missile::isfired()
{
	return fired;
}

void missile::idle() {
	double t1,temp,d1,d2;
	coordinate tar,ts;
	unit *st;
	unit::idle();
	if(fired && liv==lif && target->getfloor()!=lif) {
		set->sub(this);
	cerr<<"target dead"<<endl;
		return;
	}
	if(fired && liv==lif && target->getfloor()==lif) {
		st=set->find(center);
		target->setlock(true,this);
		if(st->getfloor()==lif 
		    && st!=target 
		    && ::distance(center,master->center)>1 
		    && ::distance(center,st->center)<0.1) {
			st->hited(damage);
			set->sub(this);
			target->setlock(false,this);
	cerr<<"crashed"<<endl;
			return;
		}

		tar.x=target->getcoordinate().x;
		tar.y=target->getcoordinate().y;
		tar.z=target->getcoordinate().z;

		if(target->getfloor()==lif && ::distance(tar,center)<0.1) {
			target->hited(damage);
			master->hit();
	cerr<<"hit"<<endl;
			set->sub(this);
			target->setlock(false,this);
			return;
		}
		else {
	
			tar.x-=center.x;
			tar.y-=center.y;
			tar.z-=center.z;
			tar.unital();

			t1=tar.x*head.x+tar.y*head.y+tar.z*head.z;

			if(t1>=range && ::distance(tar,center)<100000) {
				//좌,우 어느쪽으로 방향을 틀지 정함.
				//tar을 head-right평면에 정사영;
				d1=tar.x*top.x+tar.y*top.y+tar.z*top.z;
				ts.x=top.x*d1;
				ts.y=top.y*d1;
				ts.z=top.z*d1;
				ts.x=tar.x-ts.x;
				ts.y=tar.y-ts.y;
				ts.z=tar.z-ts.z;
				//ts와head사이의 각도
				ts.unital();
				d2=ts.x*head.x+ts.y*head.y+ts.z*head.z;
				if(d2>=1.0)
					d2=1.0;
				temp=d2;
				temp=acos(temp);
				if(temp>maxturnrate) {
					temp=maxturnrate;
				}
				flyingUnit::sett(temp);
				d2=tar.x*right.x+tar.y*right.y+tar.z*right.z;
				if(d2>=0)
					rotate1(true);
				else
					rotate1(false);
				//위, 아래 어느쪽으로 방향을 틀지 정함.
				temp=tar.x*head.x+tar.y*head.y+tar.x*head.z;
				
				if(temp>=1.0)
					temp=1.0;
				temp=acos(temp);
				if(temp>maxturnrate) {
					temp=maxturnrate;
				}
				flyingUnit::sett(temp);
				d2=tar.x*top.x+tar.y*top.y+tar.z*top.z;
				if(d2>=0)
					rotate2(false);
				else
					rotate2(true);
			}
			else {
				set->sub(this);
				target->setlock(false,this);
	cerr<<"target lost"<<endl;
				return;
			}
			
			move();
	
			if(fule<=0) {
			}
		}
	}
}
//////////////////////////////////bullet class////////////////////////////////

bullet::bullet(unitList *s,flyingUnit *m,double r,double ms, double w,double d, double f,double tr,double x,double y,double z,coordinate d1,coordinate d2):missile(s,m,r,ms,w,d,f,tr,x,y,z,d1,d2) {
	fired=true;
	target=NULL;
	lifetime=60;
	fire(NULL);
}

void bullet::fire(unit *tar) {
	target=tar;
	sets(maxspeed);
	center=master->center;
	head=master->head;
	top=master->top;
	right=master->right;
	fired=true;
	set->add(this);
}

bool bullet::isbullet() {
	return true;
}

void bullet::distroy() {
	delete this;
}

void bullet::idle() {
	unit *temp;
	unit::idle();
	if(liv==lif) {
		temp=set->find(center);
		if(temp->getfloor()==lif && ::distance(master->center,center)>1 && ::distance(center,temp->center)<0.1) {
			temp->hited(damage);
			master->hit();
			set->sub(this);
			liv=dead;
		}
		else {
			move();
		}
		lifetime--;
	}
	if(lifetime==0 && liv==lif) {
		set->sub(this);
	}
}


//////////////////////////////////unitSet class////////////////////////////////

unitSet::unitSet() {
	thiss=NULL;
	next=NULL;
}

unitSet::~unitSet() {
	if(thiss != NULL)
		this->thiss->distroy();
}

unitList::unitList() {
	count=0;
	head.next=&tail;
}

void unitList::add(unit *feel) {
	unitSet *temp;

	temp=new unitSet;
	temp->thiss=feel;

	temp->next=head.next;
	head.next=temp;

	feel->clearfloor();

	count++;
}

void unitList::sub(unit *feel) {
	if(count>0) {
		feel->nextfloor();
		count--;
	}
}

void unitList::idle() {
	unitSet *temp = NULL,*ptemp = NULL;
	int coun=0;
	if(count>0) {
		while(head.next->thiss->getfloor()==dead) {
			ptemp=head.next;
			head.next=head.next->next;
			delete ptemp;
		}
		temp=head.next;
		while(temp!=&tail) {
			coun++;
			if(temp->next != &tail && temp->next->thiss->getfloor()==dead) {
				ptemp=temp->next;
				temp->next = temp->next->next;
				temp=temp->next;
	
				delete ptemp;
			}
			else if(temp->thiss->getfloor()!=dead) {
				temp->thiss->idle();
				temp=temp->next;
			}
		}
	}
}

unit* unitList::search(unit *feel) {
	unitSet *temp;
	temp=head.next;
	while(temp->thiss!=feel && temp!=&tail) {
		temp=temp->next;
	}
	if(temp->thiss==feel)
		return feel;
	else
		return NULL;
}

unit* unitList::lock(const coordinate &center,const coordinate &had) {
	unit *target;
	unitSet *temp;
	double distances,angle,dist;
	coordinate tcent;
	target=NULL;
	temp=head.next;
	distances=1000000000;
	while(temp!=&tail) {
		tcent.x=temp->thiss->center.x;
		tcent.y=temp->thiss->center.y;
		tcent.z=temp->thiss->center.z;

		tcent.x-=center.x;
		tcent.y-=center.y;
		tcent.z-=center.z;
		tcent.unital();

		angle=tcent.x*had.x+tcent.y*had.y+tcent.z*had.z;

		if(angle>(sqrtl(2)/2)) {
			dist=::distance(tcent,center);
			if(temp->thiss->getfloor()==lif && dist<distances && dist!=0  && temp->thiss->isitem()==false) {
				distances=dist;
				target=temp->thiss;
			}
		}
		temp=temp->next;
	}
	return target;
}

unit* unitList::find(const coordinate &center) {
	unit *target;
	unitSet *temp;
	double distances,dist;
	target=NULL;
	temp=head.next;
	distances=1000000000;
	while(temp!=&tail) {
		dist=::distance(temp->thiss->center,center);
		if(dist<distances && dist!=0) {
			distances=dist;
			target=temp->thiss;
		}
		temp=temp->next;
	}
	return target;
}

//////////////////////////////////item class////////////////////////////////
item::item(unitList *s,double x,double y,double z,coordinate d1,coordinate d2) {
	set=s;
	setcoordinate(x,y,z,d1,d2);

	set->add(this);

	Case=rand()%3;
	if(Case==0) {
		num=rand()%4;
	}
	else if(Case==1) {
		num=rand()%100;
	}
	else if(Case==2) {
		num=rand()%1000;
	}
}
void item::distroy() {
	delete this;
}
void item::idle() {
	unit *temp;
	unit::idle();
	temp=set->find(center);
	if(temp->isitem()==false && temp->ismsl()==false) {
		if(::distance(center,temp->center)<0.1) {
			temp->getitem(Case,num);
			set->sub(this);
			liv=dead;
		}
	}
}
bool item::ismsl() {
	return false;
}
bool item::isitem() {
	return true;
}

//////////////////////////////////airplane class////////////////////////////////
airplane::airplane(unitList *set,double ms,int Ns,double w,double d, double f,double tr,double x,double y,double z,coordinate d1,coordinate d2):flyingUnit(ms,w,d,f,tr,x,y,z,d1,d2) {
	int ii;
	fule=1000;
	maxfule=fule;
	score=0;
	side=Ns;
	cooltime=10;
	cttime=0;
	locked=false;
	sidewinder=new missile*[Ns];
	for(ii=0;ii<side;ii++)
		sidewinder[ii]=NULL;
	sidew=new int[Ns];
	for(ii=0;ii<side;ii++)
		sidew[ii]=-1;
	Set=set;
	Set->add(this);
}

airplane::~airplane() {
	int i;
	for(i=0;i<side;i++) {
		delete sidewinder[i];
	}
	delete sidewinder;
	delete sidew;
}

void airplane::setlock(bool lock,unit *mis) {
	locked=lock;
	mi=mis;
}

bool airplane::ismsl()
{
	return false;
}

void airplane::rotate1(bool fs) {
	if(fule>0) {
		fule-=(getw()*gett());
		flyingUnit::rotate1(fs);
	}
}

void airplane::rotate2(bool fs) {
	if(fule>0) {
		fule-=(getw()*gett());	
		flyingUnit::rotate2(fs);
	}
}

void airplane::rotate3(bool fs) {
	if(fule>0) {
		fule-=(getw()*gett());
		flyingUnit::rotate3(fs);
	}
}

void airplane::accelate(bool fs) {
	if(fule>0) {
		fule-=getf();
		flyingUnit::accelate(fs);
	}
}

void airplane::stop()
{
	speed = 0;
}

void airplane::setmissile(int a) {
	select=a;
}
void airplane::lockOn() {
	target=Set->lock(center,head);
}

void airplane::getitem(int c,int n) {
	if(c==0) {
		makeMissile(missilespeck[n][0],missilespeck[n][1],missilespeck[n][2],missilespeck[n][3],missilespeck[n][4],missilespeck[n][5]);
	}
	else if(c==1) {
		durability+=n;
	}
	else if(c==2) {
		fule+=n;
	}
}

void airplane::fire() {
	if(target!=NULL && sidewinder[select]!=NULL) {
		sidewinder[select]->fire(target);
		sidewinder[select]=NULL;
	}
}

void airplane::shot() {
	bullet *b;
	if(cttime==0) {
		b=new bullet(Set,this,0,speed+0.4,1,5,1,0,center.x,center.y,center.z,head,right);
//list,master,range,maxspeed,weight,damage,force,turnRate,x,y,z,d1,d2
		cttime++;
	}
}

void airplane::makeMissile(double ms,double r,double w,double d,double f,double tr) {
	int i;
	for(i=0;i<side;i++)
		if(sidew[i]==-1) {
			break;
		}
	sidewinder[i]= new missile(Set,this,r,ms,w,d,f,tr,center.x,center.y,center.z,head,right);
	sidew[i]=i;
}
void airplane::hit() {
	score++;
}

void airplane::hited(double damage) {
	durability-=damage;
	if(durability<0) {
		Set->sub(this);
	}
}

void airplane::idle() {
	if(cttime!=0)
		cttime=(cttime+1)%cooltime;
}

missile* airplane::getmissileaddress(int select2) {
	return sidewinder[select2];
}


double airplane::maxHP() {
	return max_hp;
}
double airplane::getHP() {
	return durability;
}
double airplane::maxFule() {
	return maxfule;
}
double airplane::getFule() {
	return fule;
}

//////////////////////////////////PC class////////////////////////////////
PC::PC(unitList *set,double ms,int Ns,double w,double d, double f,double tr,double x,double y,double z,coordinate d1,coordinate d2):airplane(set,ms,Ns,w,d,f,tr,x,y,z,d1,d2) {
}
void PC::idle() {
	unit::idle();
	airplane::idle();
	//char input;
	//조작 키 입력부분 집어넣기
	/*if(input=='h' || input=='H') {
		rotate1(true);
	}
	else if(input=='k' || input=='K') {
		rotate1(false);
	}
	else if(input=='u' || input=='U') {
		rotate2(false);
	}
	else if(input=='m' || input=='M') {
		rotate2(true);
	}
	else if(input=='q' || input == 'Q') {
		rotate3(false);
	}
	else if(input=='w' || input == 'W') {
		rotate3(true);
	}
	else if(input>='0' && input<='9') {
		int in;
		in=input-48;
		if(in<side) {
			select=in;
			cout<<select<<"is selected"<<endl;
		}
	}
	else if(input=='d' || input=='D') {
		sidewinder[select]->fire(target);
		while(1)
			sidewinder[select]->idle();
	}*/
	target=Set->lock(center,head);
//	if(target!=NULL)
//		cout<<"lock! "<<"("<<target->getcoordinate().x<<","<<target->getcoordinate().y<<","<<target->getcoordinate().z<<")"<<endl;
	move();
//	if(fule<=0) {
//	}*/
}

void PC::distroy() {
	delete this;
}

void PC::hited(double damage) {
	airplane::hited(damage);
}

//////////////////////////////////NPC class////////////////////////////////
NPC::NPC(unitList *set,double ms,int Ns,double w,double d, double f,double tr,double x,double y,double z,coordinate d1,coordinate d2):airplane(set,ms,Ns,w,d,f,tr,x,y,z,d1,d2) {
}

void NPC::distroy() {
	delete this;
}

void NPC::idle() {
	int temp;
	double i1,i2;
	unit::idle();
	airplane::idle();
	accelate(true);
	coordinate mc;

	if(locked==true) {
		mc.x=mi->center.x;
		mc.y=mi->center.y;
		mc.z=mi->center.z;

		mc.x-=center.x;
		mc.y-=center.y;
		mc.z-=center.z;

		mc.unital();


		if((mc.x*head.x+mc.y*head.y+mc.z*head.z)!=0) {
			if((mc.x*head.x+mc.y*head.y+mc.z*head.z)>0.95 || (mc.x*head.x+mc.y*head.y+mc.z*head.z)<-0.95) {
				rotate2(true);
			}
			else {
				i1=top.x*mc.x+top.y*mc.y+top.z*mc.z;
				i2=right.x*mc.x+right.y*mc.y+right.z*mc.z;
				if(i1<0)
					i1*=-1;
				if(i2<0)
					i2*=-1;
				if(i1>i2) {
					i1=top.x*mc.x+top.y*mc.y+top.z*mc.z;
					if(i1>0) {
						rotate2(false);
					}
					else {
						rotate2(true);
					}
				}
				else {
					i2=right.x*mc.x+right.y*mc.y+right.z*mc.z;
					if(i2>0) {
						rotate1(true);
					}
					else {
						rotate1(false);
					}
				}
			}
		}
	}
	else {
		
	}
	temp=rand();
/*	if(temp%100==0)
		rotate1(true);
	else if(temp%100==1)
		rotate2(true);
	else if(temp%100==2)
		rotate1(false);
	else if(temp%100==3)
		rotate2(false);
*/	move();
	fule=1000;
	if(fule<=0) {
		stop();
	}
}

void NPC::hited(double damage) {
	airplane::hited(damage);
}
