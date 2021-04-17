#ifndef SMARTPTR_H_
#define SMARTPTR_H_

#include <memory>

class Rocket;
class Tank;
class Game;
class Bullet;
class Settings;
class Stats;

typedef std::shared_ptr<Stats> StatsPtr;
typedef std::shared_ptr<Settings> SettingsPtr;
typedef std::shared_ptr<Bullet> BulletPtr;
typedef std::shared_ptr<Game> GamePtr;
typedef std::shared_ptr<Tank> TankPtr;
typedef std::shared_ptr<Rocket> RocketPtr;

#endif /* SMARTPTR_H_ */
