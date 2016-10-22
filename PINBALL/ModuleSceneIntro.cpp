#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include"ModuleRender.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	

	circle = App->textures->Load("pinball/sadBall.png"); 
	background = App->textures->Load("pinball/KirbyPinball.png");
	kicker = App->textures->Load("pinball/kicker.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	spikyBall = App->textures->Load("pinball/spikyBall.png");
	spikyball1Anim.PushBack({ 0,0,17,16 });
	spikyball1Anim.PushBack({ 18,0,17,16 });
	spikyball1Anim.speed = 0.015f;

	ghost = App->textures->Load("pinball/ghost.png");
	ghostanim.PushBack({0,0,16,15});
	ghostanim.PushBack({ 18,0,16,15 });
	ghostanim.PushBack({ 37,0,16,15 });
	ghostanim.PushBack({56,0,16,15 });
	ghostanim.PushBack({ 73,3,14,11 });
	//
	ghostanim.PushBack({ 90,0,14,15 });
	ghostanim.PushBack({ 107,0,16,15 });
	ghostanim.PushBack({ 124,3,14,11 });
	ghostanim.PushBack({ 141,0,14,15 });
	ghostanim.speed = 0.02f;
	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
	cloud = App->textures->Load("pinball/movingCloud.png");
	cloudanim.PushBack({0,1,24,14});
	cloudanim.PushBack({27,0,26,16});
	
	cloudanim.speed = 0.07f;
	fatkirbytext = App->textures->Load("pinball/bigKirby.png");
	fatkirbyanim.PushBack({ 0,17,26,25 });
	if (kirbyumbrella == true) {
		fatkirbyanim.PushBack({ 29,17,26,28 });
		fatkirbyanim.PushBack({ 58,17,26,28 });
		fatkirbyanim.PushBack({ 86,11,29,32 });
		fatkirbyanim.PushBack({ 118,3,25,40 });
		fatkirbyanim.PushBack({ 146,3,25,40 });
		fatkirbyanim.PushBack({ 174,5,25,38 });
		fatkirbyanim.PushBack({ 202,0,25,43 });
	}
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 5,DINAMIC,25,0));
		circles.getLast()->data->listener = this;

	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 20, 10));
	}



	//Spawning test objects
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		int kicker[12] = {
			2, 1,
			0, 7,
			13, 11,
			22, 10,
			8, 2,
		};


		boxes.add(App->physics->CreatePolygon(App->input->GetMouseX(), App->input->GetMouseY(), kicker, 10, 1.0f, -2));
		
	}

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		leftkick = true;
		App->physics->KickersForce(b2Vec2(0, 50), b2Vec2(0, 0));

	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		rightkick = true;
		App->physics->KickersForce(b2Vec2(0, 50), b2Vec2(5, 0));
		
	}
	

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------

	//Draw Background
	if (ground != NULL)
	{
		int x, y;
		ground->GetPosition(x, y);
		App->renderer->Blit(background, x, y, NULL, 1.0f, ground->GetRotation());
	}

	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		
		App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	if (kicker) {
		int x, y;
		App->physics->kicker->GetPosition(x, y);
		SDL_Rect rect = {0,0, 22, 12};
		App->renderer->Blit(kicker, x, y, &rect, 1.0f, App->physics->kicker->GetRotation(), -0.2, 0);
	}

	if (kicker)
	{
		int x, y;
		App->physics->kickerV2->GetPosition(x, y);
		SDL_Rect rect = { 24,0, 22, 12 };
		App->renderer->Blit(kicker, x, y, &rect, 1.0f, App->physics->kickerV2->GetRotation(), -0.2, 0);

	}



	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 17;
	rect.h = 16;
	int x, y;
	App->physics->spikyball1->GetPosition(x,y);
	App->renderer->Blit(spikyBall, x - (App->physics->spikyball1->width / 2), y - (App->physics->spikyball1->height / 2), &(spikyball1Anim.GetCurrentFrame()));
	App->physics->spikyball2->GetPosition(x, y);
	App->renderer->Blit(spikyBall, x - (App->physics->spikyball2->width / 2), y - (App->physics->spikyball2->height / 2), &(spikyball1Anim.GetCurrentFrame()));
	rect.x = 0;
	rect.y = 0;
	rect.w = 16;
	rect.h = 15;
	App->physics->ghost1->GetPosition(x, y);
	App->renderer->Blit(ghost, x - (App->physics->ghost1->width / 2), y - (App->physics->ghost1->height / 2), &(ghostanim.GetCurrentFrame()));
	App->physics->ghost2->GetPosition(x, y);
	App->renderer->Blit(ghost, x - (App->physics->ghost2->width / 2), y - (App->physics->ghost2->height / 2), &(ghostanim.GetCurrentFrame()));
	App->physics->kinematicrect->GetPosition(x, y);
	App->renderer->Blit(cloud, x, y, &(cloudanim.GetCurrentFrame()));
	App->physics->fatkirby->GetPosition(x, y);
	App->renderer->Blit(fatkirbytext, x - (App->physics->ghost2->width / 2), y - (App->physics->ghost2->height / 2), &(fatkirbyanim.GetCurrentFrame()));
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;
	//como hacer sensores
	App->audio->PlayFx(bonus_fx);
	if (bodyA != nullptr && bodyB != nullptr) {
		if (bodyA->body == App->physics->kinematicrect->body || bodyB->body == App->physics->kinematicrect->body) {
			if (TimesKickCloud == 3) {
				cloudanim.PushBack({ 56,0,26,16 });
			}
			TimesKickCloud++;
		}
		
	}
	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
