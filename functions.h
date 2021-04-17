#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "utilities.h"
#include "peripherals.h"
#include <iostream>
#include "Tank.h"
#include "Game.h"
#include "Stats.h"
#include "Settings.h"
#include "SmartPtr.h"
#include <unistd.h>
#include <vector>
#include "bmmenu.h"
#include "bmtank.h"
#include "bmgame.h"
#include "bmrocket.h"
#include "bmgameover.h"
#include <linux/input.h>

extern unsigned int GRAPH[640 * 480];

extern std::string nastawy[];

void ClearScreen();
void UpdateIO();

//Sterowanie=============================================================================================================================================================================

//Menu
short menu(GamePtr game);
//Ustawienia
void settings(GamePtr game);
short viewEditedValue(short modifiedValue, std::string modifiedString, short max);
short getNumber();
short toNumber(short key);
//Zastosowanie ustawieñ
void applySettings(GamePtr game);
void utrudnij(GamePtr game);
//Pozosta³e
std::vector<int> generatePositions(volatile long timer, int objwidth);
bool checkPositions(int newposition, std::vector<int> positions, int nrofelements, int objwidth);
void values(GamePtr game);
void moveTank(TankPtr tank);
short rozrzutf(long timer);
short generateRocketSpeed(long timer);
//==============================================================================================================================================================================================

//Interfejsy:
void viewMenu(GamePtr game);
void viewSettings(GamePtr game);
void gameStats(GamePtr game);
void gameOver(GamePtr game);

//LCD
void drawLetter(int, int, int, int, long, long);
void drawObject(short positionX, short positionY,  const short owidth, const short olength, int color);
void drawTank(short posX, short posY);
void printText(std::string napis, int x, int y, long textclr=0xffffff, long bckgrd=0x0);
void drawBitmap(const unsigned long* bitmap, int posX, int posY, int length, int height);
void draw1bitBitmap(const unsigned char* bitmap, int posX, int posY, int length, int height, long colour);

#endif /* FUNCTIONS_H_ */



//short moveTank(short actualXPos, short actualYPos, short move,  TankPtr tank);
//void decision(short key, TankPtr tank);
