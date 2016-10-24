#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include"Animation.h"

class PhysBody;

class TripleKirby
{
public:
	TripleKirby(PhysBody* body) : pBody(body), state(1)
	{}

	PhysBody* pBody;
	int state;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void SpawnPLayer();
	

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> floating_walls;	
	PhysBody* ground;
	PhysBody* player;
	PhysBody* deathSens;
	Animation spikyball1Anim;
	Animation ghostanim;
	Animation cloudanim;
	Animation fatkirbyanim;
	Animation sadcloudanim;

	Animation rainingcloud;
	Animation kirbyumbrella;
	Animation fatkirbyThrowup;

	Animation sunanim;
	Animation moonanim;

	PhysBody* sensor;
	bool sensed;
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* background;
	SDL_Texture* kicker;
	SDL_Texture* spikyBall;
	SDL_Texture* ghost;
	SDL_Texture* cloud;
	SDL_Texture* fatkirbytext;
	SDL_Texture* tripleKirby;
	SDL_Texture* sun_moon_textures;
	SDL_Texture* extinguishertext;
	SDL_Texture* game_overtext;
	SDL_Rect extinguisherrect;

	uint bonus_fx;
	uint Point_fx;
	uint Kicker_fx;
	uint Ghost_fx;
	uint Triangles_fx;
	uint Fatkirby_fx;
	uint GameOver_fx;
	p2Point<int> ray;
	bool ray_on;
	int TimesKickCloud;
	bool raincloud;
	bool extappear;
	bool extappear2;
	bool extappear3;
	bool extappear4;
	bool extappear5;
	bool extappear6;
	TripleKirby* tKirby;

	bool triangleDraw;
	uint trianglecount;

	int sun_life;
	int moon_life;

	int triX;
	int triY;

	bool printThObj;
	bool printThObj2;

	bool game_over;
};
