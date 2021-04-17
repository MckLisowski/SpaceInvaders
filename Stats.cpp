#include "Stats.h"

Stats::Stats(std::string cplayer,  short ctankLife) : player(cplayer), tankLife(ctankLife) {
	this->points=1;
	this->level=1;
}

Stats::~Stats() {
}

void Stats::addPoints() {
	this->points+=10;
	if(points%1000==1)
	{
		this->level+=1;
	}
}

void Stats::substractPoints() {
	this->points-=100;
}
