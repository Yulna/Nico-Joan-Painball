#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include"ModuleAudio.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	score = 0;
	life = 3;
	active = false;
	pickedJackpot = false;

	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

void ModulePlayer::IncreaseScore(int points)
{
	score += points;
}

void ModulePlayer::LoseLife()
{
	life--;
	active = false;
}

int ModulePlayer::GetLife()
{
	return life;
}

int ModulePlayer::GetScore()
{
	return score;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) && (life > 0))
	{
		App->physics->KickersForce(b2Vec2(0, 50), b2Vec2(0, 0), LEFT);
	}
	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		App->audio->PlayFx(App->scene_intro->Kicker_fx);

	if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) && (life > 0))
	{
		App->physics->KickersForce(b2Vec2(0, -50), b2Vec2(0, 0), RIGHT);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		App->audio->PlayFx(App->scene_intro->Kicker_fx);


	if ((App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) && !active && life > 0)
	{
		App->scene_intro->SpawnPlayer();
		active = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN && App->physics->debug)
	{
		life--;
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN && App->physics->debug)
	{
		life++;
	}
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN && App->physics->debug)
	{
		IncreaseScore(1000);
	}


	if (life <= 0)
	{
		App->scene_intro->game_over = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		life = 3;
		score = 0;
		pickedJackpot = false;
		App->scene_intro->game_over = false;
		App->scene_intro->GameOverFxenabled = false;
		App->physics->SpawnBoss();
		App->scene_intro->ResetJackpot();
	}


	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PostUpdate()
{




	return UPDATE_CONTINUE;
}



