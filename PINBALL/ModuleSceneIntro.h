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
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> floating_walls;	
	PhysBody* ground;
	Animation spikyball1Anim;
	Animation ghostanim;
	Animation cloudanim;
	Animation fatkirbyanim;
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
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
	bool kirbyumbrella;
	int TimesKickCloud;
	bool raincloud;
	bool extappear;


	TripleKirby* tKirby;

	bool printThObj;

};
