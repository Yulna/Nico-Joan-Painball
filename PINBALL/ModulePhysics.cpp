#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"
#include "ModuleSceneIntro.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;
	/*
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* big_ball = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(diameter) * 0.5f;

	b2FixtureDef fixture;
	fixture.shape = &shape;
	big_ball->CreateFixture(&fixture);*/
	
	// Pivot 0, 0
	int Pinball_exterior[164] = {
		28, 448,
		39, 423,
		16, 413,
		13, 406,
		13, 374,
		17, 368,
		23, 365,
		23, 350,
		8, 336,
		8, 305,
		10, 297,
		17, 294,
		46, 294,
		53, 298,
		55, 305,
		56, 319,
		61, 310,
		61, 291,
		13, 266,
		13, 231,
		15, 224,
		23, 221,
		23, 195,
		7, 181,
		7, 159,
		12, 154,
		63, 154,
		63, 148,
		14, 123,
		14, 89,
		16, 80,
		24, 78,
		24, 72,
		16, 72,
		10, 70,
		8, 63,
		8, 16,
		11, 10,
		17, 7,
		144, 6,
		150, 9,
		152, 16,
		152, 65,
		150, 70,
		144, 72,
		136, 72,
		136, 79,
		145, 81,
		147, 87,
		147, 122,
		97, 148,
		97, 154,
		147, 154,
		153, 160,
		153, 182,
		137, 197,
		137, 222,
		145, 225,
		147, 231,
		147, 267,
		99, 291,
		99, 310,
		104, 320,
		105, 305,
		108, 298,
		115, 295,
		144, 295,
		150, 298,
		152, 305,
		152, 334,
		137, 352,
		137, 365,
		144, 368,
		147, 375,
		147, 407,
		144, 413,
		120, 424,
		128, 452,
		184, 452,
		184, -14,
		-23, -14,
		-23, 449
	};



	App->scene_intro->ground = CreateChain(0, 0 - (App->renderer->camera.y / SCREEN_SIZE), Pinball_exterior, 164, NULL);


	//kicker 1
	// Pivot 0, 0
	int kicker1[10] = {
		23, 12,
		2, 10,
		0, 5,
		2, 1,
		8, 1
	};


	kickerjoint = CreateRectangleKickerPoint(53, 411, 1,1);
	kicker = CreatePolygon(48, 407, kicker1, 10, 100, -2);
	
	revolutedef.bodyA = kickerjoint->body;
	revolutedef.bodyB = kicker->body;
	revolutedef.localAnchorB = b2Vec2(0.1, 0.1);
	revolutedef.enableLimit = true;
	revolutedef.lowerAngle = -(3.14 /3);
	revolutedef.upperAngle = (3.14 / 32);
	revolutedef.collideConnected = false;
	revolute_joint = (b2RevoluteJoint*)world->CreateJoint(&revolutedef);
	
	

	// Pivot 0, 0
	int kicker2[10] = {
		0, 12,
		20, 10,
		23, 5,
		20, 1,
		14, 2
	};


	kickerjointV2 = CreateRectangleKickerPoint(106, 411, 1, 1);
	kickerV2 = CreatePolygon(86, 406, kicker2, 10, 100, -2);

	revolutedefV2.bodyA = kickerjointV2->body;
	revolutedefV2.bodyB = kickerV2->body;
	revolutedefV2.localAnchorB = b2Vec2(0.33, 0.1);
	revolutedefV2.enableLimit = true;
	revolutedefV2.lowerAngle = -(3.14 / 32);
	revolutedefV2.upperAngle = (3.14 / 3);
	revolutedefV2.collideConnected = false;
	revolute_joint = (b2RevoluteJoint*)world->CreateJoint(&revolutedefV2);
	

	
	spikyball1 = CreateCircle(31,319,5,STATIC,6, 0.3f);
	ghost1 = CreateCircle(31, 324, 5, DINAMIC,50, 0.6f);
	/*b2DistanceJointDef disdef;
	disdef.bodyA = spikyball->body;
	disdef.bodyB = ghost->body;
	disdef.collideConnected = false;
	disdef.length = 0.25f;*/
	b2RevoluteJointDef revdef;
	revdef.bodyA = spikyball1->body;
	revdef.bodyB = ghost1->body;
	revdef.collideConnected = false;
	revdef.enableMotor = true;
	revdef.motorSpeed = -2;
	revdef.maxMotorTorque = 10;
	revdef.localAnchorB = b2Vec2(0.25, 0);

	revolutemotorghost1 = (b2RevoluteJoint*)world->CreateJoint(&revdef);

	spikyball2=CreateCircle(134, 318, 5,STATIC,6,0.3f);
	ghost2= CreateCircle(134, 326, 5, DINAMIC, 50, 0.3f);
	b2RevoluteJointDef revdef2;
	revdef2.bodyA = spikyball2->body;
	revdef2.bodyB = ghost2->body;
	revdef2.collideConnected = false;
	revdef2.enableMotor = true;
	revdef2.motorSpeed = 2;
	revdef2.maxMotorTorque = 10;
	revdef2.localAnchorB = b2Vec2(-0.25, 0);
	revolutemotorghost2 = (b2RevoluteJoint*)world->CreateJoint(&revdef2);
	
	kinematicrect = CreateKinematicRectangle(80, 172, 24, 14);
	kinematicrect->body->SetLinearVelocity(b2Vec2(1, 0));

	//como hacer sensores
	paco = CreateRectangleSensor(0, 0, 10, 10);
	paco->listener = App->scene_intro;
	//Walls
	fatkirby = CreateCircle(81,210,12,STATIC,6,0.3f);
	CreateFloatingWalls();

	return true;
}

//Kicker force
void ModulePhysics::KickersForce(b2Vec2 vectforce, b2Vec2 posit) {
	if (App->scene_intro->leftkick == true) {
		revolutedef.bodyB->ApplyForce(vectforce, posit, true);
		App->scene_intro->leftkick = false;
	}
	else if (App->scene_intro->rightkick == true) {
		revolutedefV2.bodyB->ApplyForce(vectforce, posit, true);
		App->scene_intro->rightkick = false;
	}
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, CircleTypes type, int density, float rest)
{
	b2BodyDef body;
	if (type == DINAMIC) {
		body.type = b2_dynamicBody;
	}
	else if (type == KINEMATIC) {
		body.type = b2_kinematicBody;
	}
	else if (type == STATIC) {
		body.type = b2_staticBody;
	}

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	if (type == DINAMIC) {
		body.bullet = true;
	}
	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = density;
	fixture.restitution = rest;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}



PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);



	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}


PhysBody* ModulePhysics::CreateKinematicRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_kinematicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);



	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.restitution = 0.3f;
	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}



//Tes purposes only, Erase/Change later-------------------------------------------------------------------------------------------------------------------------
PhysBody* ModulePhysics::CreateRectangleKicker(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape shape;
	shape.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);
	
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}
PhysBody* ModulePhysics::CreateRectangleKickerPoint(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, int filterIndex)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.filter.groupIndex = filterIndex;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}





update_status ModulePhysics::Update() {

	int xkin, ykin;
	kinematicrect->GetPosition(xkin, ykin);
	if (xkin<10) {
		kinematicrect->body->SetLinearVelocity(b2Vec2(1, 0));
	}
	else if (xkin>125) {
		kinematicrect->body->SetLinearVelocity(b2Vec2(-1, 0));
	}


	return UPDATE_CONTINUE;
}


// 
update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		//
		bool debug_joint = false;

		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x * SCREEN_SIZE), METERS_TO_PIXELS(pos.y * SCREEN_SIZE), METERS_TO_PIXELS(shape->m_radius * SCREEN_SIZE), 100, 100, 255);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
			}
			break;
			}
			

			//Get mous postion in a body
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN) {

				mouse_pos.x = PIXEL_TO_METERS(App->input->GetMouseX());
				mouse_pos.y = PIXEL_TO_METERS(App->input->GetMouseY());

				b2Transform trans = b->GetTransform();
				if (f->GetShape()->TestPoint(trans, mouse_pos) == true) {
					mouse_body = b;
					debug_joint = true;
					LOG("HE ENCONTRADO ALGO!");
				}

			}

		}


		//Debug mouse joint
		if (debug_joint == true) {
			b2MouseJointDef def;
			def.bodyA = ground;
			def.bodyB = mouse_body;
			def.target = mouse_pos;
			def.dampingRatio = 0.5f;
			def.frequencyHz = 2.0f;
			def.maxForce = 100.0f * mouse_body->GetMass();
			mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
		}


		if ((mouse_joint != nullptr) && (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)) {

			mouse_joint->SetTarget(b2Vec2(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY())));
			App->renderer->DrawLine(METERS_TO_PIXELS( mouse_joint->GetAnchorB().x) , METERS_TO_PIXELS( mouse_joint->GetAnchorB().y), App->input->GetMouseX(), App->input->GetMouseY(), 255, 100, 100);
		}
	

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint != nullptr)
		{

			world->DestroyJoint(mouse_joint);
			mouse_joint = nullptr;

			LOG("Joint succesfully destroyed");

		}
	}


	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}


//
PhysBody* ModulePhysics::CreatePolygon(int x, int y, int* points, int size, float dens, int filterIndex)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape shape;


	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}
	shape.Set(p, size / 2);


	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = dens;
	fixture.filter.groupIndex = filterIndex;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = 0;
	pbody->height = 0;

	return pbody;
}





//
void ModulePhysics::CreateFloatingWalls()
{
	// Pivot 0, 0
	int LeftBottomFloor[12] = {
		48, 413,
		52, 406,
		29, 395,
		29, 377,
		24, 377,
		24, 400
	};


	// Pivot 0, 0
	int RightBottomFloor[12] = {
		112, 413,
		108, 406,
		131, 395,
		131, 377,
		136, 377,
		136, 400
	};

	
	CreateChain(0 , 0, LeftBottomFloor, 12, -2);
	CreateChain(0, 0, RightBottomFloor, 12, -2);



}

