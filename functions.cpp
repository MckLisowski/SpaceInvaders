#include "functions.h"

unsigned int GRAPH[640 * 480];
extern unsigned char mapa_czcionki;
std::string nastawy[4];


//STEROWANIE================================================================================================================================================================

//Menu================================================================================================================================================================
short menu(GamePtr game)
{
	viewMenu(game);
	while(1)
		{
			UpdateIO();
			short key=getKey();
			usleep(80000);
			if(key == KEY_Q)          //Q, WZNOW/GRAJ
			{
				return 1;
			}
			else if(key == KEY_W)          //W, USTAWIENIA
			{
				settings(game);
				viewMenu(game);
			}
			else if(key == KEY_E)          //E, staticride
			{
				game->getSettings()->changeStaticRide();
				viewMenu(game);
			}
			else if(key == KEY_R)          //R, TRUDNOSC
			{
				if(game->getSettings()->getPoziomTrudnosci()<3)
					game->getSettings()->levelUp();
				else
					game->getSettings()->setPoziomTrudnosci(1);
				game->getSettings()->applyHardness();
				applySettings(game);
				viewMenu(game);
			}
			else if(key == KEY_T)          //F, NOWA GRA
			{
				return 3;     //firstGame=0;
			}
		}
	return 1;
}

//Ustawienia================================================================================================================================================================
void settings(GamePtr game)
{
	short key;
	viewSettings(game);
	while(1)
	{
		std::string modifiedString="";
		short modifiedValue=0, max=5, nowaWart=0;

		UpdateIO();
	    key=getKey();
		usleep(60000);

		if(key== KEY_A)
		{
			modifiedString="SZYBKOSC POCISKOW: ";
			modifiedValue=game->getSettings()->getBulletSpeed();
			max=15;
			nowaWart=viewEditedValue(modifiedValue, modifiedString, max);
			game->getSettings()->setBulletSpeed(nowaWart);
		}
		else if(key== KEY_S)
		{
			modifiedString="ZYCIE RAKIET:      ";
			modifiedValue=game->getSettings()->getRocketLife();
			max=12;
			nowaWart=viewEditedValue(modifiedValue, modifiedString, max);
			game->getSettings()->setRocketLife(nowaWart);
		}
		else if(key== KEY_D)
		{
			modifiedString="MAX SZYBKOSC CZOLGU:   ";
			modifiedValue=game->getSettings()->getMaxTankSpeed();
			max=30;
			nowaWart=viewEditedValue(modifiedValue, modifiedString, max);
			game->getSettings()->setMaxTankSpeed(nowaWart);
		}
		else if(key== KEY_F)
		{
			modifiedString="CZESTOSC RAKIET:   ";
			modifiedValue=game->getSettings()->getIterations();
			max=30;
			nowaWart=viewEditedValue(modifiedValue, modifiedString, max);
			game->getSettings()->setIterations(nowaWart);
		}
		else if(key== KEY_G)
		{
			break;
		}
		viewSettings(game);
	}
	applySettings(game);
}

short viewEditedValue(short modifiedValue, std::string modifiedString, short max)
{
	std::string nowyNapis=modifiedString;
	char znak;
	short nowaLiczba=0, i=1;
	short cyfra=0;
	ClearScreen();
	drawBitmap(bmmenu, 0, 0, 640, 480);
	printText("WPISZ NOWA WARTOSC   MAX: "+std::to_string(max), 180, 150);                        //wypisanie przed pierwszym pobraniem cyfry
	printText("NACISNIJ NIE CYFRE BY ZATWIERDZIC I WYJSC", 110, 360, 0x00AAFF);
	printText(nowyNapis, 210, 220);
	ClearScreen();

	while(cyfra>=0 and i<1000)											//jak cyfra<0 tzn ¿e koniec zmiany wartosci, jak i>1000 tzn ¿e wartosc wiêksza ni¿ 999
	{
		cyfra=getNumber();												// wczytanie cyfry z klawiatury. jak nie cyfra to zwraca -1

		if(cyfra<0)break;												// jak zwróci -1 to wychodzi z pêtli
		else															// jak cyfra to cyfra
		{
			// sekcja cyfry
			nowaLiczba*=i;										// mnozenie liczby przez mno¿nik(1, 10, 100 itd) przed dodaniem nowej cyfry na msc jednosci
			nowaLiczba+=cyfra;									// dodanie nowej cyfry na miejsce jednoœci
			i*=10;												// zwiêkszenie mnoznika 10razy
			// sekcja znaku
			znak=cyfra+48;										// ustawienie chara na wartosc cyfry
			nowyNapis.push_back(znak);							// dodanie znaku na koniec napisu

			if(nowaLiczba>max)									// jezeli liczba jest wieksza niz dopuszczalna to zerujemy wszystkie wartosci i pobiera cyfry od nowa
			{
				nowyNapis=modifiedString;
				nowaLiczba=0;
				i=1;
			}
			drawBitmap(bmmenu, 0, 0, 640, 480);
			printText("WPISZ NOWA WARTOSC   MAX: "+std::to_string(max), 180, 150);                        //wypisanie przed pierwszym pobraniem cyfry
			printText("NACISNIJ NIE CYFRE BY ZATWIERDZIC I WYJSC", 110, 360, 0x00AAFF);
			printText(nowyNapis, 210, 220);									// wypisywanie napisu z kazdym nowym znakiem
			ClearScreen();
		}
	}
	if(nowaLiczba==0)
	{
		nowaLiczba=modifiedValue;
		nowyNapis=modifiedString+std::to_string(modifiedValue);
	}
	return nowaLiczba;											//zwraca now¹ wartosc lub star¹ je¿eli nic nie zmienilismy
}

short getNumber()
{
	short key=0;
	while(key==0)
	{
		UpdateIO();
		key = getKey();
		usleep(120000);
	}
	short cyfra= toNumber(key);
	return cyfra;
}

short toNumber(short key)
{
	if(key>=71 and key<=82)                //klawiatura numeryczna cyfry
	{
		if (key<73)
			return key-64;
		else if (key>79)
		{
			if(key==82) return 0;
			else return key-78;
		}
		else
			return key-71;
	}
	else if(key>=KEY_1 and key<=KEY_0)        	  // klawiatura g³ówna cyfry
	{
		if(key==11) return 0;
		return key-1;
	}
	return -1;      //jak klikniemy nie cyfrê
}

//Zastosowanie ustawien================================================================================================================================================================
void applySettings(GamePtr game)
{
	game->getTank()->setLife(game->getSettings()->getTankLife());
	game->getStats()->setTankLife(game->getSettings()->getTankLife());
}

void utrudnij(GamePtr game)
{
	short bltSpeed=game->getSettings()->getBulletSpeed();
	short rcktLife=game->getSettings()->getRocketLife();
	short tnkSpeed=game->getSettings()->getMaxTankSpeed();
	short iter=game->getSettings()->getIterations();

	if(bltSpeed>4)
		game->getSettings()->setBulletSpeed(bltSpeed-1);
	if(rcktLife<15)
		game->getSettings()->setRocketLife(rcktLife+1);
	if(tnkSpeed>11)
		game->getSettings()->setMaxTankSpeed(tnkSpeed-1);
	if(iter>6)
		game->getSettings()->setIterations(iter-1);
}

//Pozosta³e=====================================================================================================================
void moveTank(TankPtr tank)
{
	short motion=(short)tank->getSpeed();
	short posx=tank->getPosX();
	//short posy=tank->getPosY();

 	if(motion<0)
	{
		if(posx>-(motion))
			posx+=motion;
	}
	if (motion>0)
	{
		if(posx<(580-(40+motion)))
			posx+=motion;
	}
/*	if (key==KEY_UP)
	{
		if(posy>motion)
			posy-=motion;
	}
	if (key==KEY_DOWN)
	{
		if(posy<=(440-motion))
			posy+=motion;
	}*/
	tank->setPosX(posx);
//	tank->setPosY(posy);
}



std::vector<int> generatePositions(volatile long timer, int objwidth)
{
	std::vector<int> positions;
	int randpos;
	short rightLimit=540;

	if(timer%4==0)
		positions.push_back(randr(rightLimit));

	else if(timer%5==0)
		for(int i=0; i<3; i++)
		{
			for(int j = 0; j<3; j++)
			{
				if(not checkPositions(randpos = randr(rightLimit), positions, i, objwidth))
					break;
			}
			positions.push_back(randpos);
		}
	return positions;
}

bool checkPositions(int newposition, std::vector<int> positions,  int nrofelements, int objwidth)
{
	for(int i = 0; i < nrofelements; i++)
		if( std::abs(newposition - positions[i]) < objwidth)
			return 1;
	return 0;
}

	/*else if(timer%6==0)
		{
			for(int i=0; i<5; i++)
				{//(i*60)+10
					pozycje.push_back(randr(rightLimit));
				}
		}
	else if(timer%7==0)
		{
			for(int i=0; i<6; i++)
				{//640-(i*50)
					pozycje.push_back(randr(rightLimit));
				}
		}*/
	/*else
		{
			for(int i=0; i<5; i++)
				{//640-(i*40)
					pozycje.push_back(randr(rightLimit));
				}
		}*/

void values(GamePtr game)                                                       // funkcja ³¹cz¹ca wartosci z game z nazwami
{
	nastawy[0] = "SZYBKOSC POCISKOW:   "+std::to_string(game->getSettings()->getBulletSpeed());
	nastawy[1] = "ZYCIE RAKIET:        "+std::to_string(game->getSettings()->getRocketLife());
	nastawy[2] = "MAX SZYBKOSC CZOLGU: "+std::to_string(game->getSettings()->getMaxTankSpeed());
	nastawy[3] = "CZESTOSC RAKIET:     "+std::to_string(game->getSettings()->getIterations());
}

short rozrzutf(long timer)
{
	switch(timer%5)
	{
		case 0:
			return 16;
		case 1:
			return 17;
		case 2:
			return 18;
		case 3:
			return 19;
		default:
			return 20;
	}
}

short generateRocketSpeed(long timer)
{
	switch(timer%4)
	{
		case 0:
			return 1;
		case 1:
			return 2;
		case 2:
			return 3;
		default:
			return 4;
	}

}

//KONIEC STEROWANIA================================================================================================================================================================


// Interfejsy==========================================================================================================

void viewMenu(GamePtr game)
{
	std::string stan, trudnosc;
			if(game->getSettings()->isStaticRide())
				stan="ON";
			else
				stan="OFF";

			if(game->getSettings()->getPoziomTrudnosci()==1)
				trudnosc="LATWY";
			else if(game->getSettings()->getPoziomTrudnosci()==2)
				trudnosc="SREDNI";
			else
				trudnosc="TRUDNY";

		drawBitmap(bmmenu, 0, 0, 640, 480);

		printText("SPACE INVADERS", 240, 60, 0x00FF00);

		if(game->isFirstGame())
		{
			printText("Q", 200, 170, 0xFFAA00);
			printText("                GRAJ", 210, 170);
		}
		else
		{
			printText("Q", 200, 170, 0xFFAA00);
			printText("               WZNOW", 210, 170);
			printText("T", 200, 370, 0xFFAA00);
			printText("            NOWA GRA", 210, 370);
		}

		printText("W", 200, 220, 0xFFAA00);
		printText("          USTAWIENIA", 210, 220);
		printText("E", 200, 270, 0xFFAA00);
		printText("   STATIC RIDE:  "+stan, 210, 270);
		printText("R", 200, 320, 0xFFAA00);
		printText("   POZIOM TRUDNOSCI:  "+trudnosc, 210, 320);
		ClearScreen();
}

void viewSettings(GamePtr game)
{
	values(game);

	//drawBitmap(bmmenu, 0, 0, 640, 480);
	printText("SPACE INVADERS", 240, 20, 0x00FF00);

	printText("ZMIANA POZIOMU TRUDNOSCI SPOWODUJE", 130, 50);
	printText("  NADPISANIE WSZYSTKICH WARTOSCI  ", 130, 70);

	printText("WYBIERZ WARTOSC KTORA CHCESZ ZMIENIC:", 110, 100);

	printText("A", 160, 150, 0xFFAA00);
	printText(nastawy[0], 210, 150);

	printText("S", 160, 200, 0xFFAA00);
	printText(nastawy[1], 210, 200);

	printText("D", 160, 250, 0xFFAA00);
	printText(nastawy[2], 210, 250);

	printText("F", 160, 300, 0xFFAA00);
	printText(nastawy[3], 210, 300);

	printText("G                   POWROT", 160, 350, 0x00AAFF);
	ClearScreen();
}

void gameStats(GamePtr game)
{
	std::string trudnosc="";
	if(game->getSettings()->getPoziomTrudnosci()==1)
					trudnosc="LATWY";
	else if(game->getSettings()->getPoziomTrudnosci()==2)
					trudnosc="SREDNI";
	else
					trudnosc="TRUDNY";

	printText("GRACZ", 580, 50);
	printText(game->getStats()->getPlayer(), 580, 70);
	printText("LEVEL", 580, 100);
	printText("   "+std::to_string(game->getStats()->getLevel()), 580, 130);
	printText("PUNKTY", 580, 200);
	printText(std::to_string(game->getStats()->getPoints()), 580, 230);
	printText("ZYCIE", 580, 300);
	printText(std::to_string(game->getStats()->getTankLife()), 600, 330);
	printText("POZIOM", 580, 400);
	printText("TRUDNO", 580, 420);
	printText(trudnosc, 580, 450);
}

void gameOver(GamePtr game)
{
	short key=0;
	drawBitmap(bmgameover, 0, 0, 640, 480);
	printText("LEVEL:  "+std::to_string(game->getStats()->getLevel()), 260, 340);
	printText("PUNKTY: "+std::to_string(game->getStats()->getPoints()), 260, 370);
	printText("WCISNIJ SPACJE ABY ZACZAC OD NOWA", 150, 440);
	ClearScreen();
	while(key!=KEY_SPACE)
	{
		UpdateIO();
		key = getKey();
		usleep(50000);
	}
}

//LCD===========================================================================================================================================================================================================


void printText(std::string napis, int x, int y, long textclr, long bckgrd )
{
	 int fy0 =0, fx0 =0;
	 char kod;
	 	 for(int i=0; i<(int)napis.length(); i++)
	 	 {
	 		 kod=(int)napis[i];
	 		 if(kod<52)                       // 52 to kod ASCII pierwszej litery z 2 wiersza, wiêc to jest if obejmuj¹cy tylko 1 wwiersz
	 		 {
	 			 fy0=0; fx0=0+((kod-32)*25);  // w pionie jesteœmy na 0, bo 1 wiersz, a w poziomie ruszamy siê od 0 co 25, bo tyle jest miêdzy kolejnymi literami
	 		 }
	 		 else if(kod<72)
	 		 {
	 			 fy0=25; fx0=0+((kod-52)*25);
	 		 }
	 		 else
	 		 {
	 			 fy0=50; fx0=0+((kod-72)*25);
	 		 }
	 		 drawLetter(fx0, fy0, x, y, textclr, bckgrd);      // funkcja wypisujaca po jednej literze. fx0-poziom w pliku czcionki, fy0-pion w pliku czcionki, x- x na wyswietlaczu, y
	 		 x+=10;                           // odleg³oœc liter od siebie
	 	 }
}

void drawLetter(int fx0, int fy0, int x0, int y0, long textclr, long bckgrd  )
{
	unsigned char* source_data=&mapa_czcionki;

	for (int y=0; y<15; y++)
    	for (int x=0; x<10; x++)
    	{
    		GRAPH[x+x0+(y+y0)*640] = 0x0;
    		if(source_data[x+fx0+(y+fy0)*512])
    		{
    			GRAPH[x+x0+(y+y0)*640]  |= (long)( (float)source_data[x+fx0+(y+fy0)*512]/255*(textclr&0xFF0000));
    			GRAPH[x+x0+(y+y0)*640]  |= (long)( (float)source_data[x+fx0+(y+fy0)*512]/255*(textclr&0x00FF00));
    			GRAPH[x+x0+(y+y0)*640]  |= (long)( (float)source_data[x+fx0+(y+fy0)*512]/255*(textclr&0x0000FF));
    		}
    		else
    			GRAPH[x+x0+(y+y0)*640] = bckgrd;
    	}
}

void drawObject(short positionX, short positionY, const short owidth, const short olength, int color)
{
	for (int b = positionY; b <positionY+olength  ; b++) // pozycja Y w pionie - wiersze
	{
		for (int a = positionX; a < positionX+owidth; a++) //pozycja X w poziomie - od której do której kolumny
		{
			SetPixel(GRAPH, a, b, color);
		}
	}
}

void drawTank(short posX, short posY)
{
	drawBitmap(bmtank, posX, posY, 40, 40);
}

void drawBitmap(const unsigned long* bitmap, int posX, int posY, int length, int height)
{
	for(int dy = 0; dy < height; dy++)
		for(int dx = 0; dx < length; dx++)
			GRAPH[ (posY+dy)*640 + posX + dx ] |= bitmap[dy*length + dx];
}

void draw1bitBitmap(const unsigned char* bitmap, int posX, int posY, int length, int height, long colour)
{
	for(int dy = 0; dy < height; dy++)
		for(int dx = 0; dx < length; dx++)
			if( bitmap[ dy*length/8 + dx/8 ] & (1 << (7 - dx%8)) )
				GRAPH[ (posY+dy)*640 + posX + dx ] |= colour;
}









































/*
 *
 * //printText("A", 160, 140, 0xFFAA00);
	//printText(nastawy[0], 210, 140);
//printText("D", 160, 220, 0xFFAA00);
	//printText(nastawy[2], 210, 220);
	//printText("F", 160, 260, 0xFFAA00);
	//printText(nastawy[3], 210, 260);
 * if(key== KEY_A)
		{
			modifiedString="POZIOM TRUDNOSCI:   ";
			modifiedValue=game->getSettings()->getPoziomTrudnosci();
			max=10;
			nowaWart=viewEditedValue(modifiedValue, modifiedString, max);
			game->getSettings()->setPoziomTrudnosci(nowaWart);
			if(modifiedValue!=nowaWart)
				zmienPoziom(game);
		}
 *
 * else if(key== KEY_F)
		{
			modifiedString="SZYBKOSC RAKIET:   ";
			modifiedValue=game->getSettings()->getRocketSpeed();
			max=5;
			nowaWart=viewEditedValue(modifiedValue, modifiedString, max);
			game->getSettings()->setRocketSpeed(nowaWart);
		}
			else if(key== KEY_D)
		{
			modifiedString="ZYCIE CZOLGU:      ";
			modifiedValue=game->getSettings()->getTankLife();
			max=12;
			nowaWart=viewEditedValue(modifiedValue, modifiedString, max);
			game->getSettings()->setTankLife(nowaWart);
		}
	short szer= tank->getWidth();
	short dlug= tank->getLength();
	float oneThird = posX+((float)szer/3);
	float twoThird = posX+((2*(float)szer)/3);
	float half = posY+((float)dlug/2);

	for(int i=posY; i<posY+dlug; i++)
	{
		for(int j=posX; j<posX+szer; j++)
		{
			if((j>=oneThird && j<=twoThird && i<=half) || (i>half))
			{
				SetPixel(GRAPH, j, i, color);
			}
		}
	}
  short moveTank(short actualXPos, short actualYPos, short move, TankPtr  tank)
{
	short motion=tank->getSpeed();
	if(move==1 && actualXPos>motion)
	{
		return actualXPos-motion;
	}
	else if(move==2 && actualXPos<(640-(tank->getWidth()+motion)))
	{
		return actualXPos+motion;
	}
		if(move==3 && actualPos>motion)
		{
			return actualPos-motion;
		}
		else if(move==4 && actualPos<(actualPos+tank->getWidth()+motion))
		{
			return actualPos+motion;
		}
	else
		return actualXPos;
}
 * void viewMenuResumeGame()
{
	ClearScreen();
		printText("SPACE INVADERS", 220, 60);

		printText("WZNOW         Q", 150, 150);
		printText("USTAWIENIA    W", 150, 200);
		printText("NOWA GRA      E", 150, 250);
		ClearScreen();
}
if((key!=74 and key!=78 and key>=71 and key<=82) or (key>=2 and key<=11)){



short decision(short key)
{
	short move=5;
		if(key>=103 and key<=108)
		{
				if(key==105) move=1;
				else if (key==106) move=2;
				else if (key==103) move=3;
				else if (key==108) move=4;
				else move=5;
		}
	return move;
}
*/
