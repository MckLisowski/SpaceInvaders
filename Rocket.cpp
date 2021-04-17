#include "Rocket.h"



	Rocket::~Rocket() {
	}


	Rocket::Rocket(const short cwidth, const short clength, short cXpos, short cYpos, const short clife, short cspeed) :
		width(cwidth), length(clength), Xpos(cXpos), Ypos(cYpos),  life(clife), speed(cspeed)
	{
		this->actualLife=clife;
	}

	void Rocket::moveRocket() {
		this->Ypos+=this->speed;
	}

void Rocket::hit() {
	this->actualLife-=1;
	changeColour();
}

void Rocket::changeColour() {
	short life=this->life, actualLife=this->actualLife;
	unsigned long colors[6] = {0x00FF00, 0x99ff00, 0xccff66, 0xffcccc, 0xff6666, 0xFF0000};
	if(actualLife>((life*5)/6))
		{
			this->colour=colors[0];
		}
	else if(actualLife>((life*4)/6))
		{
			this->colour=colors[1];
		}
	else if(actualLife>((life*3)/6))
		{
			this->colour=colors[2];
		}
	else if(actualLife>((life*2)/6))
		{
			this->colour=colors[3];
		}
	else if(actualLife>((life*1)/6))
		{
			this->colour=colors[4];
		}
	else
		{
			this->colour=colors[5];
		}

}
//: width(width), length(length), life(life, speed(speed)
