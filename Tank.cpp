#include "Tank.h"

Tank::Tank(short clife) : life(clife)
{
	this->posX=0;
	this->posY=440;
	this->inertP=0.7;
	this->speed=0;
}

Tank::~Tank() {}

void Tank::hit() {
	this->life-=1;

}


