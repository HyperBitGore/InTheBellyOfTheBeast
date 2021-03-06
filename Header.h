#pragma once
#include <iostream>
#include "GoreEngine.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <chrono>

extern int curnload;
extern int curetype;
extern int prog;
extern int level;
extern spxp enem1head;
extern texp enem1tex;
extern spxp enem2head;
extern texp enem2tex;
extern spxp enem3head;
extern texp enem3tex;
extern spxp enem4head;
extern texp enem4tex;

extern spxp boss1head;

extern SDL_Texture* ebullet1tex;

extern std::vector<std::vector<bool>> e1points;
extern std::vector<std::vector<bool>> e2points;
extern std::vector<std::vector<bool>> e3points;
extern std::vector<std::vector<bool>> e4points;

extern Uint32 wallcolor;
extern SDL_Texture* walls;
extern int wallpitch;
extern int score;
extern bool endlessmode;
extern int endlesspopcount;
extern int vertinc;
extern int horinc;
extern int vertstarts[2];
extern int horizontalstarts[2];
extern double popgap;

class Timer {
private:
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> end;
public:
	void startTime() {
		start = std::chrono::steady_clock::now();
	}
	double getTime() {
		end = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	}
	void resetTime() {
		start = std::chrono::steady_clock::now();
		end = std::chrono::steady_clock::now();
	}
};

extern Timer etime;
extern Timer trantime;
extern Timer gaptime;

struct Point {
	int x;
	int y;
};
extern std::vector<int> etypes;
extern std::vector<int> nload;
extern std::vector<Point> spawnloc;
struct Entity {
	float x;
	float y;
	int w;
	int h;
	int health;
};
struct Button : Entity {
	int type;
};
struct Bullet : Entity {
	float trajx;
	float trajy;
	double trajtimer;
	double timermax;
	int type;
	SDL_Texture* tex;
};
struct Enemy : Entity {
	float movetimer;
	float timermax;
	float shootimer;
	float shootmax;
	float animtimer;
	float animmax;
	int type;
	spxp sprites;
	spxp start;
	texp texs;
	texp tstart;
	//Real
	std::vector<std::vector<bool>> points;
	SDL_Surface* surf;
	SDL_Texture* tex;
	bool change;
	std::vector<Point> destroycheck;
	std::vector<Point> destroyed;
	int destn;
	float trajx;
	float trajy;
	//0 - neg x; 1 - pos x; 2 - neg y;
	int offscreen;
	float tgx;
	bool pause;
	double ptime;
	double maxp;
	int pburst;
	int pmax;
	float burstspeed;
	float origshotspeed;
	int pattern;
};
struct Particle : Bullet {
	SDL_Rect pd;
	SDL_Rect rect;
	SDL_Texture* tex;
	bool er;
};

struct Transform {
	int sx;
	int sy;
	int endx;
	int endy;
	int cx;
	int cy;
	double activate;
	double speed;
	float ct;
	Uint32 col;
};
extern std::vector<Transform> transforms;
//Erase when used, or else will continually activate
struct TRActivate {
	Transform trans;
	int health;
};
struct Boss : Entity {
	int type;
	int pattern;
	int phase;
	int burst;
	int maxhealth;
	float trajx;
	float trajy;
	float movetime;
	float maxmove;
	float shtime;
	float shmax;
	float antime;
	float anmax;
	SDL_Surface* surf;
	SDL_Texture* tex;
	spxp sprites;
	std::vector<Point> destroyed;
	std::vector<std::vector<bool>> points;
	bool opposite;
	bool oppositex;
	std::vector<Point> destroychecks;
	std::vector<TRActivate> activates;
};


class Game {
public:
	float trajX(float deg);
	float trajY(float deg);
	Bullet createBullet(float x, float y, int w, int h, float deg, float speed, int type, SDL_Texture* tex);
	bool isColliding(Entity b, Entity e) {
		if (b.x < e.x + e.w && b.x + b.w > e.x && b.y < e.y + e.h && b.y + b.h > e.y) {
			return true;
		}
		return false;
	}
	void constructAlphabet(SDL_Renderer* rend, TTF_Font* font, SDL_Color color, texp& head) {
		Gore gore;
		for (int i = 33; i < 123; i++) {
			char c = i;
			std::string co;
			co.push_back(c);
			SDL_Surface* surf = TTF_RenderText_Solid(font, co.c_str(), color);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surf);
			SDL_FreeSurface(surf);
			gore.insertTex(head, tex, co);
			//std::cout << co << std::endl;
		}
	}

	SDL_Texture* loadBackground(int level, SDL_Renderer* rend);
	void updateBackground(SDL_Renderer* rend, SDL_Texture* tex, Entity* back, double delta);
	
	void generateDestroyChecks(std::vector<Point>& dest, int sx, int sy, int endx, int endy);
	void createEnemy(spxp& enemhead, texp& enemtex, std::vector<Enemy>& enemies, int x, int y, int type, float degree, SDL_Renderer* rend);

	void convertToLvl(std::vector<int>& etypes, std::vector<int>& nload, std::vector<Point>& spawnloc, std::vector<Transform>& transforms, const char* file);
	void loadLevel(std::vector<int>& etypes, std::vector<int>& nload, std::vector<Point>& spawnloc, std::vector<Transform>& transforms, const char* file);
	void levelHandler(std::vector<int>& etypes, std::vector<int>& nload, std::vector<Point>& spawnloc, std::vector<Enemy>& enemies, SDL_Renderer* rend, bool* spawning, bool* bossmode);
	void createEnemyPoints();

	void MassTextureSet(SDL_Texture* tex, int sy, int sx, int endx, int endy, Uint32* pixel, int* pitch);
	bool MassTextureCheck(SDL_Texture* tex, int sy, int sx, int endx, int endy, Uint32* pixel, int* pitch);
	void MassTextureSet(SDL_Texture* tex, std::vector<Point> points, std::vector<Uint32>colors, int* pitch);

	void regrow(std::vector<Point> points, SDL_Texture* tex, SDL_Surface* surf, std::vector<std::vector<bool>>& dest);
	void loadBoss(Boss* boss, int level, SDL_Renderer* rend);
	void bossUpdate(Boss* boss, SDL_Renderer* rend, double delta, std::vector<Bullet>& bullets, Entity* p, std::vector<Transform>& transforms, bool* bossmode, bool* win, bool* menu);

	void updateMenuButtons(std::vector<Button>& buttons, std::vector<Enemy>& enemies, std::vector<Bullet>& bullets, std::vector<Particle>& particles, SDL_Renderer* rend, bool* menu, bool* exitf, Entity* player, Boss* boss, int* lives, bool* hardcore, bool* bossmode);
	void updatePauseButtons(std::vector<Button>& buttons, bool* menu);
	void createButtons(std::vector<Button>& menubs, std::vector<Button>& pbuttons);
	void death(Entity* player, Boss* boss, int* lives, bool* hardcore, bool* bossmode, std::vector<Enemy>& enemies, std::vector<Bullet>& bullets, std::vector<Particle>& particles, SDL_Renderer* rend);

	void endlessPopulate(std::vector<Enemy>& enemies, SDL_Renderer* rend);
};

