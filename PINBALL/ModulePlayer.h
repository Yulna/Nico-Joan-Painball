#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "SDL\include\SDL.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	
	void IncreaseScore(int points);
	void LoseLife();

	int GetLife();
	int GetScore();

public:
	bool pickedJackpot;

private:

	int score;
	int life;
	bool active;


};