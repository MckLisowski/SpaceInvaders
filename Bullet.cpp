#include "Bullet.h"

Bullet::Bullet(const short csize, short cposX, short cposY) : size(csize), posX(cposX), posY(cposY){}

Bullet::~Bullet() {
	// TODO Auto-generated destructor stub
}

void Bullet::moveBullet(short distance) {
	this->posY-=distance;
}
