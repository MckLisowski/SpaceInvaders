#include "Game.h"

Game::Game(TankPtr ctank, SettingsPtr csettings, StatsPtr cstats, bool cfirstGame) {
	this->tank=ctank;
	this->settings=csettings;
	this->stats=cstats;
	this->firstGame=cfirstGame;
}

Game::~Game() {
	// TODO Auto-generated destructor stub
}

