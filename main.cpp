#include "main.h"

using namespace std;

int main(int argc, char *argv[])
{
	bool paused = true, shoot=false, click=0;
	short out, key=0, gameIteration=0, change=0, rozrzut=18, rocketSpeed=2;
	vector<int> pozycjeRakiet;

	SystemInit();

	while(1)                                       							// do tej pêtli program wchodzi po wyjsciu z nowej gry
	{
		vector<RocketPtr> rakiety;
		vector<BulletPtr> naboje;
		SettingsPtr settings = make_shared<Settings>();
		settings->applyHardness();
		StatsPtr stats = make_shared<Stats>(settings->getPlayerName(), settings->getTankLife());
		TankPtr tank = make_shared<Tank>(settings->getTankLife());
		GamePtr game = make_shared<Game>(tank, settings, stats, 1);
		gameIteration=0;
		while (1)
		{
			UpdateIO();
			//PrintDiagnosticInfo();

//OBSLUGA KLAWISZY====================================================================================================================================================================
			key =getKey();
			usleep(50000);

			// RUCH CZOLGU====================================================================================================================================================================
			if(key == KEY_RIGHT)
			{
				tank->setSpeed(game->getSettings()->getMaxTankSpeed());
				click=1;
			}
			else if(key == KEY_LEFT)
			{
				tank->setSpeed(-(game->getSettings()->getMaxTankSpeed()));
				click=1;
			}
			else if(game->getSettings()->isStaticRide())
			{
				tank->setSpeed(0);
			}
			else
			{
				click=0;
				change=1;
			}

			if(click==1) change++;

			if(!(game->getSettings()->isStaticRide()))
			{
				if(change==2)
				{
					tank->setSpeed(0);
				}
				if(game->getTank()->getSpeed()==0)
				{
					change=0;
				}
			}
			// KONIEC RUCHU CZOLGU====================================================================================================================================================================


			if(game->getTank()->getSpeed()!=0)
				moveTank(tank);

			if(key!=0 || game->isFirstGame())
			{
				if(key==KEY_ESC) paused=true;
				if(paused)
				{
					out=menu(game);
					if(out==1)
					{
						paused=false;
						game->setFirstGame(false);
					}
				}
				else
				{
					if(key==KEY_SPACE) shoot=true;
				}
			}

// GRA================================================================================================================================================================================
			if(!paused)
			{
				drawBitmap(bmgame, 0, 0, 640, 480);
				gameStats(game);
				if(game->getStats()->getPoints()%50000==0)
					{
						utrudnij(game);
					}
				// KOLIZJE NABOJE-RAKIETY============================================================================================================================================
				for(unsigned int a=0; a < rakiety.size(); a++)
				{
					for(unsigned int b=0; b < naboje.size(); b++)
					{
						if( (( rakiety[a]->getXpos() + rakiety[a]->getWidth()-3 ) >= naboje[b]->getPosX() )
								and ( naboje[b]->getPosX() >= (rakiety[a]->getXpos()+3 - naboje[b]->getSize()) )) 							// ( -szer naboju+1)
						{
							if( (rakiety[a]->getYpos() + rakiety[a]->getLength()) > naboje[b]->getPosY()
									and naboje[b]->getPosY() > rakiety[a]->getYpos())
							{
								rakiety[a]->hit();
								naboje.erase(naboje.begin()+b);
								b-=1;
								game->getStats()->addPoints();
							}
						}
					}
				}

				// RYSOWANIE CZO³GU================================================================================================================================================
					drawTank(tank->getPosX (), tank->getPosY());	// ruch czo³gu


				// KOLIZJE RAKIETY-CZO£G============================================================================================================================================
				for(unsigned int r=0; r<rakiety.size(); r++)
				{
					short rakietaX=rakiety[r]->getXpos();
					if((rakietaX+rakiety[r]->getWidth())>tank->getPosX() and rakietaX<(tank->getPosX()+40))
					{
						if((rakiety[r]->getYpos()+rakiety[r]->getLength())>=tank->getPosY() and rakiety[r]->getYpos()<tank->getPosY()+40)
						{
							rakiety.erase(rakiety.begin()+r);
							r-=1;
							game->getTank()->hit();
							game->getStats()->setTankLife(game->getTank()->getLife());
						}
					}
				}

				// NOWE RAKIETY===========================================================================================================================================
				if(gameIteration%game->getSettings()->getIterations()==0)																						// nowe rakiety raz na 10 iteracji
				{
					pozycjeRakiet=generatePositions(globalTimer_ms, 24);														// generuje pozycje dla kazdej nowej lini rakiet
					for(unsigned int i=0; i<pozycjeRakiet.size(); i++)																// tworzenie nowej lini rakiet z pozycjami z wskazPoz
					{
						rocketSpeed=generateRocketSpeed(globalTimer_ms+i);
						rakiety.push_back(make_shared<Rocket>(24, 32, pozycjeRakiet[i], 0, game->getSettings()->getRocketLife(), rocketSpeed));
					}
				}

				// RUCH I RYSOWANIE RAKIET================================================================================================================================
				for(unsigned int j=0; j<rakiety.size(); j++)																			// wykonuje siê tyle razy ile jest wszystkich rakiet
				{
					draw1bitBitmap(bmrocket,  rakiety[j]->getXpos(),  rakiety[j]->getYpos(),  rakiety[j]->getWidth(), rakiety[j]->getLength(), rakiety[j]->getColour());
//					drawObject(rakiety[j]->getXpos(), rakiety[j]->getYpos(),  rakiety[j]->getWidth(), rakiety[j]->getLength(), rakiety[j]->getColour());	// rysuje wszystkie rakiety z wektora
					rakiety[j]->moveRocket();																	//przesuwa kazda rakietê o zadan¹ wartoœc
					if(rakiety[j]->getYpos()>(480-rakiety[j]->getLength()) or rakiety[j]->getActualLife()<=0)										//rakiety które s¹ dalej niz zadana wartoœc s¹ usuwane
					{
						if(rakiety[j]->getActualLife()>0)
						{
							game->getStats()->substractPoints();
							game->getTank()->hit();
							game->getStats()->setTankLife(game->getTank()->getLife());
						}
						rakiety.erase(rakiety.begin()+j);
							j-=1;																				// !!!! zmniejszone j ¿eby nie by³o luki w odczycie po usuniêciu rakiety
					}
				}

				// RUCH I RYSOWANIE NABOI================================================================================================================================
				rozrzut=rozrzutf(globalTimer_ms);
				if(shoot)
				{
					naboje.push_back(make_shared<Bullet>(4, tank->getPosX()+rozrzut, tank->getPosY()-2));
				}
				for(unsigned int r=0; r<naboje.size(); r++)
				{
					drawObject(naboje[r]->getPosX(), naboje[r]->getPosY(), naboje[r]->getSize(), naboje[r]->getSize(), 0xffcc33);
					naboje[r]->moveBullet(game->getSettings()->getBulletSpeed());
					if(naboje[r]->getPosY()<10)																				//rakiety które s¹ dalej niz zadana wartoœc s¹ usuwane
					{
						naboje.erase(naboje.begin()+r);
						r-=1;																							// !!!! zmniejszone j ¿eby nie by³o luki w odczycie po usuniêciu rakiety
					}
				}

				// GAME OVER================================================================================================================================
				if(game->getTank()->getLife()<=0)
					{
						paused=true;
						ClearScreen();
						gameOver(game);
						out=3;
					}

				pozycjeRakiet.clear();
				gameIteration++;
				ClearScreen();
			} //koniec ifa not paused

			shoot=false;
			if(out==3) break;
		} //while wewn

		rakiety.clear();
		naboje.clear();
	} //while zewn
}

//===============================================================================================================================================================
//=============KONIEC MAIN=======================================================================================================================================
//===============================================================================================================================================================

void* TimerThread(void* arguments)
{
    struct timespec destTime;
    clock_gettime(CLOCK_MONOTONIC,&destTime);
	while(1)
	{
		destTime.tv_nsec+=1000000;
		if(destTime.tv_nsec>=1E9)
		{
			destTime.tv_nsec-=1E9;
			destTime.tv_sec++;
		}
		globalTimer_ms++;
		if(!(globalTimer_ms%1000)) flash_ligth=0xFF;
			clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &destTime, NULL);
	}
return NULL;
}

void SystemInit()
{
	FrameBufferFD = open("/dev/fb0", O_RDWR);
	ioctl(FrameBufferFD, FBIOGET_FSCREENINFO, &fix_info);
	framebuffer = mmap(NULL, fix_info.line_length * 480, PROT_READ | PROT_WRITE,MAP_SHARED, FrameBufferFD, 0);
	inputEventsFD = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
	pthread_create(&tID, NULL, TimerThread, NULL);
	struct timeval tTime;
	gettimeofday(&tTime,NULL);
	startTime_ms=tTime.tv_sec*1000+tTime.tv_usec/1000;
}

/*void PrintDiagnosticInfo()
{
	struct timeval tTime;
	gettimeofday(&tTime,NULL);
	long TimeDiff=(tTime.tv_sec*1000+tTime.tv_usec/1000)-startTime_ms;
	//long actTime=globalTimer_ms;
	//if((frame_count%20)==0) printf("%i:: %li:%li=>%li KEYS: %i, %i, %i, %i, %x\n",frame_count,actTime,TimeDiff,actTime-TimeDiff,getKey(), Jx, Jy,JRz,JoYAct.ButtonStates);
}*/

void UpdateIO()
{
	struct input_event ev;
	while (read(inputEventsFD, &ev, sizeof ev) > 0) {
		if (ev.type == EV_KEY)
		{
			switch (ev.value)
			{
				case 1:
					gRawKeys[0] = ev.code;
					break;
				case 0:
					gRawKeys[0] = 0;
					break;
				default:
					break;
			}
				switch (ev.code)
				{
					case KEY_KP4: JoYAct.AxisX =-127*(ev.value&1); break;
					case KEY_KP6: JoYAct.AxisX =127*(ev.value&1); break;
					case KEY_KP8: JoYAct.AxisY =-127*(ev.value&1); break;
					case KEY_KP2: JoYAct.AxisY =127*(ev.value&1); break;
					case KEY_RIGHT:
						if(ev.value == 1) JoYAct.ButtonStates |=BUTTON_2;
						if(ev.value == 0) JoYAct.ButtonStates &=~BUTTON_2;
						break;
					case KEY_LEFT:
						if(ev.value == 1) JoYAct.ButtonStates |=BUTTON_4;
						if(ev.value == 0) JoYAct.ButtonStates &=~BUTTON_4;
						break;
					case KEY_UP:
						if(ev.value == 1) JoYAct.ButtonStates |=BUTTON_1;
						if(ev.value == 0) JoYAct.ButtonStates &=~BUTTON_1;
						break;
					case KEY_DOWN:
						if(ev.value == 1) JoYAct.ButtonStates |=BUTTON_3;
						if(ev.value == 0) JoYAct.ButtonStates &=~BUTTON_3;
						break;
					default:
						break;
				}
		}
	}
}

void ClearScreen()
{
	int JRz = 0;

	unsigned int bckgmask =getKey()>>2;

	int stage_max= -JRz/20;
	if (stage_max < 1)		stage_max = 1;
	if (stage_max > 9)		stage_max = 9;

	if (JoYAct.ButtonStates & BUTTON_SELECT)
		stage_max = 24;

	stage++;
	for (int a = stage % stage_max; a < (640 * 480); a += stage_max)
	{
		((u_int32_t*) framebuffer)[fix_info.line_length / 4 * (a / 640)
				+ a % 640] = GRAPH[a];
		GRAPH[a] =  bckgmask;      //0xffffff  ((GRAPH[a] & 0xfefefe) >> 1) |
	}
}
