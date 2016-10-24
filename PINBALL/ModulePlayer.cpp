#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"
#include"ModuleAudio.h"
#include"ModuleSceneIntro.h"
ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	score = 0;

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

// Update: draw background
update_status ModulePlayer::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		App->physics->KickersForce(b2Vec2(0, 50), b2Vec2(0, 0), LEFT);
		App->audio->PlayFx(App->scene_intro->Kicker_fx);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		App->physics->KickersForce(b2Vec2(0, -50), b2Vec2(0, 0), RIGHT);
		App->audio->PlayFx(App->scene_intro->Kicker_fx);
	}

	return UPDATE_CONTINUE;
}



