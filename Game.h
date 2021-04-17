#ifndef GAME_H_
#define GAME_H_


#include "SmartPtr.h"

class Game {

	TankPtr tank;
	SettingsPtr settings;
	StatsPtr stats;
	bool firstGame;

public:
	Game(TankPtr ctank, SettingsPtr csettings, StatsPtr cstats, bool cfirstGame);
	virtual ~Game();


	const TankPtr& getTank() const {
		return tank;
	}

	void setTank(const TankPtr &tank) {
		this->tank = tank;
	}


	bool isFirstGame() const {
		return firstGame;
	}

	void setFirstGame(bool firstGame) {
		this->firstGame = firstGame;
	}

	const SettingsPtr& getSettings() const {
		return settings;
	}

	void setSettings(const SettingsPtr &settings) {
		this->settings = settings;
	}

	const StatsPtr& getStats() const {
		return stats;
	}

	void setStats(const StatsPtr &stats) {
		this->stats = stats;
	}
};

#endif /* GAME_H_ */
