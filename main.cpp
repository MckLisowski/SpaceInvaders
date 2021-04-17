#include "main.h"

using namespace std;

int main(int argc, char *argv[])
{
	bool paused = true, shoot=false, click=0;
	short out, key=0, gameIteration=0, change=0, rozrzut=18, rocketSpeed=2;
	vector<int> pozycjeRakiet;

	SystemInit();

	while(1)                                       							// do tej pętli program wchodzi po wyjsciu z nowej gry
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
								and ( naboje[b]->getPosX() >= (rakiety[a]->getXpos()+3 - naboje[b]->getSize()) )) 								// ( -szer naboju+1)
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

				// RYSOWANIE CZOłGU================================================================================================================================================
					drawTank(tank->getPosX (), tank->getPosY());	// ruch czołgu


				// KOLIZJE RAKIETY-CZOŁG============================================================================================================================================
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
				if(gameIteration%game->getSettings()->getIterations()==0)														// nowe rakiet co określoną ilość iteracji
				{
					pozycjeRakiet=generatePositions(globalTimer_ms, 24);														
					for(unsigned int i=0; i<pozycjeRakiet.size(); i++)														
					{
						rocketSpeed=generateRocketSpeed(globalTimer_ms+i);
						rakiety.push_back(make_shared<Rocket>(24, 32, pozycjeRakiet[i], 0, game->getSettings()->getRocketLife(), rocketSpeed));
					}
				}

				// RUCH I RYSOWANIE RAKIET================================================================================================================================
				for(unsigned int j=0; j<rakiety.size(); j++)																// wykonuje się tyle razy ile jest wszystkich rakiet
				{
					draw1bitBitmap(bmrocket,  rakiety[j]->getXpos(),  rakiety[j]->getYpos(),  rakiety[j]->getWidth(), rakiety[j]->getLength(), rakiety[j]->getColour());
//					drawObject(rakiety[j]->getXpos(), rakiety[j]->getYpos(),  rakiety[j]->getWidth(), rakiety[j]->getLength(), rakiety[j]->getColour());				// rysuje wszystkie rakiety z wektora
					rakiety[j]->moveRocket();																	//przesuwa kazda rakietę o zadaną wartośc
					if(rakiety[j]->getYpos()>(480-rakiety[j]->getLength()) or rakiety[j]->getActualLife()<=0)									//rakiety które są dalej niz zadana wartośc są usuwane
					{
						if(rakiety[j]->getActualLife()>0)
						{
							game->getStats()->substractPoints();
							game->getTank()->hit();
							game->getStats()->setTankLife(game->getTank()->getLife());
						}
						rakiety.erase(rakiety.begin()+j);
							j-=1;																		// !!!! zmniejszone j żeby nie było luki w odczycie po usunięciu rakiety
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
					if(naboje[r]->getPosY()<10)																	
					{
						naboje.erase(naboje.begin()+r);
						r-=1;																			
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
