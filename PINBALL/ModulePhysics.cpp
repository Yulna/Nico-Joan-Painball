#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"
#include "ModuleSceneIntro.h"
#include "ModulePlayer.h"

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
	otherthrowobj = false;
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
	ballupumbrella = false;
	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;


	
	leftKickers = new p2List<PhysBody*>;
	rightKickers = new p2List<PhysBody*>;
	triangles = new p2List<PhysBody*>;

	
	// Pivot 0, 0
	// Pivot 0, 0
	int KirbyPinball[122] = {
		40, 425,
		12, 413,
		10, 371,
		21, 362,
		21, 350,
		7, 332,
		6, 303,
		14, 293,
		50, 293,
		55, 305,
		55, 319,
		61, 310,
		61, 289,
		10, 267,
		10, 229,
		18, 220,
		23, 218,
		23, 189,
		6, 178,
		8, 156,
		15, 152,
		63, 153,
		63, 147,
		10, 124,
		9, 87,
		18, 77,
		8, 70,
		8, 13,
		15, 8,
		142, 7,
		150, 14,
		152, 69,
		136, 73,
		150, 84,
		148, 127,
		97, 149,
		97, 154,
		151, 156,
		152, 179,
		136, 194,
		138, 220,
		151, 229,
		149, 267,
		99, 291,
		99, 311,
		104, 319,
		106, 297,
		115, 293,
		150, 294,
		156, 306,
		153, 337,
		139, 352,
		141, 364,
		150, 372,
		150, 390,
		150, 413,
		120, 426,
		160, 424,
		159, -1,
		0, 0,
		0, 423
	};

	


	App->scene_intro->ground = CreateChain(0, 0 - (App->renderer->camera.y / SCREEN_SIZE), KirbyPinball, 122, NULL);


	//Kickers Left
	BuildLeftKickers(leftKickers);
	//Kickers Right
	BuildRightKickers(rightKickers);

	//Death sensor
	App->scene_intro->deathSens = CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 10, SCREEN_WIDTH, 2);

	
	spikyball1 = CreateCircle(31,319,5,b2_staticBody,6, 1, 0);
	ghost1 = CreateCircle(31, 324, 7, b2_dynamicBody,500, 1, 0);
	
	b2RevoluteJointDef revdef;
	revdef.bodyA = spikyball1->body;
	revdef.bodyB = ghost1->body;
	revdef.collideConnected = false;
	revdef.enableMotor = true;
	revdef.motorSpeed = -2;
	revdef.maxMotorTorque = 100;
	revdef.localAnchorB = b2Vec2(0.25, 0);

	revolutemotorghost1 = (b2RevoluteJoint*)world->CreateJoint(&revdef);


	spikyball2=CreateCircle(130, 318, 5,b2_staticBody,6,1, 0);
	ghost2= CreateCircle(134, 326, 7, b2_dynamicBody, 500, 1, 0);

	b2RevoluteJointDef revdef2;
	revdef2.bodyA = spikyball2->body;
	revdef2.bodyB = ghost2->body;
	revdef2.collideConnected = false;
	revdef2.enableMotor = true;
	revdef2.motorSpeed = 2;
	revdef2.maxMotorTorque = 100;
	revdef2.localAnchorB = b2Vec2(-0.25, 0);
	revolutemotorghost2 = (b2RevoluteJoint*)world->CreateJoint(&revdef2);
	


	kinematicrect = CreateRectangle(80, 172, 24, 14, b2_kinematicBody);
	kinematicrect->body->SetLinearVelocity(b2Vec2(1, 0));

	sun = CreateCircle(27, 40, 10, b2_kinematicBody, 10, 2,0);
	sun->body->SetLinearVelocity(b2Vec2(1, 0));

	//Walls
	fatkirby = CreateCircle(81,210,12,b2_staticBody,6,0.3f,0);
	CreateFloatingWalls();
	//Activation of left extinguisher
	extinguisher = CreateRectangleSensor(127, 388, 1, 1);
	extinguisher->listener = App->scene_intro;
	Useextinguisher = CreateRectangleSensor(20, 388, 3, 7);
	Useextinguisher->listener = App->scene_intro;
	//Activation of right extinguisher2
	extinguisher2 = CreateRectangleSensor(34, 388, 1, 1);
	extinguisher2->listener = App->scene_intro;
	Useextinguisher2 = CreateRectangleSensor(141, 388, 3, 7);
	Useextinguisher2->listener = App->scene_intro;
	//Activation of right extinguisher3
	extinguisher3 = CreateRectangleSensor(34, 244, 1, 1);
	extinguisher3->listener = App->scene_intro;
	Useextinguisher3 = CreateRectangleSensor(141, 244, 3, 7);
	Useextinguisher3->listener = App->scene_intro;
	//Activation of right extinguisher4
	extinguisher4 = CreateRectangleSensor(126, 244, 1, 1);
	extinguisher4->listener = App->scene_intro;
	Useextinguisher4 = CreateRectangleSensor(19, 244, 3, 7);
	Useextinguisher4->listener = App->scene_intro;
	//Activation of right extinguisher5
	extinguisher5 = CreateRectangleSensor(127, 100, 1, 1);
	extinguisher5->listener = App->scene_intro;
	Useextinguisher5 = CreateRectangleSensor(20, 100, 3, 7);
	Useextinguisher5->listener = App->scene_intro;
	//Activation of right extinguisher6
	extinguisher6 = CreateRectangleSensor(34, 100, 1, 1);
	extinguisher6->listener = App->scene_intro;
	Useextinguisher6 = CreateRectangleSensor(141, 100, 3, 7);
	Useextinguisher6->listener = App->scene_intro;

	midKirby = CreateSensorCircle(80, 384, 7, b2_staticBody, 0, 0);
	leftKirby = CreateSensorCircle(55, 375, 7, b2_staticBody, 0, 0);
	rightKirby = CreateSensorCircle(103, 375, 7, b2_staticBody, 0, 0);

	//Kirby Jackpot

	cloudrightsensor = CreateCircle(18, 198, 12, b2_staticBody, 6, 1, 0);
	cloudrightsensor->listener = App->scene_intro;

	cloudrightsensor2 = CreateCircle(141, 198, 12, b2_staticBody, 6, 1, 0);
	cloudrightsensor2->listener = App->scene_intro;

	DetectFatKirbyAnimation = CreateRectangleSensor(83, 185, 21, 24);
	DetectFatKirbyAnimation->listener = App->scene_intro;


	int sun_moon_cloud[12] = {
		14, 34,
		18, 39,
		13, 43,
		7, 43,
		4, 38,
		7, 34
	};

	Mycloud.add(CreatePolygon(48, 10, sun_moon_cloud, 12,0, 1.2f,-3,b2_staticBody));
	Mycloud.add(CreatePolygon(80, 10, sun_moon_cloud, 12, 0, 1.2f, -3, b2_staticBody));
	Mycloud.add(CreatePolygon(68, -10, sun_moon_cloud, 12, 0, 1.2f, -3, b2_staticBody));
	p2List_item<PhysBody*> *temp= Mycloud.getFirst();
	/*for (int i = 0; i < Mycloud.count(); i++) {
		temp->data->listener = App->scene_intro;
		temp = temp->next;
	}*/


	return true;
}

//Kicker force
void ModulePhysics::KickersForce(b2Vec2 vectforce, b2Vec2 posit, sides rl) {
	if (rl == LEFT) 
	{
		p2List_item<PhysBody*>* item = leftKickers->getFirst();
		while (item != nullptr)
		{
			item->data->body->ApplyForce(vectforce, posit, true);
			item = item->next;
		}
	}
	else if (rl == RIGHT) {
		p2List_item<PhysBody*>* item = rightKickers->getFirst();
		while (item != nullptr)
		{
			item->data->body->ApplyForce(vectforce, posit, true);
			item = item->next;
		}
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

	//Check if sun or moon are alive and destroy them when they die
	if ((App->scene_intro->sun_life < 0) && (sun))
	{
		world->DestroyBody(sun->body);
		sun = nullptr;

		//Create moon when sun dies
		moon = CreateCircle(28, 28, 10, b2_staticBody, 10, 2,0);
	}


	if ((App->scene_intro->moon_life <0) && moon)
	{
		world->DestroyBody(moon->body);
		moon = nullptr;
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, b2BodyType type, int density, float rest, int index)
{
	b2BodyDef body;

	body.type = type;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	
	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = density;
	fixture.restitution = rest;
	fixture.filter.groupIndex = index;
	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}


PhysBody* ModulePhysics::CreateSensorCircle(int x, int y, int radius, b2BodyType type, int density, float rest) {

	b2BodyDef body;

	body.type = type;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	
	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = density;
	fixture.restitution = rest;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
	
}


PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
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

	kinematicrect->GetPosition(xcloud, ycloud);
	if (App->scene_intro->TimesKickCloud < 3) {
		if (xcloud < 20) {
			kinematicrect->body->SetLinearVelocity(b2Vec2(1, 0));
		}
		else if (xcloud > 115) {
			kinematicrect->body->SetLinearVelocity(b2Vec2(-1, 0));
		}
	}
	else {
		if (xcloud < 70) {
			kinematicrect->body->SetLinearVelocity(b2Vec2(1, 0));
		}
		else if (xcloud > 70) {
			kinematicrect->body->SetLinearVelocity(b2Vec2(-1, 0));
		}
		else if (xcloud == 70) {
			kinematicrect->body->SetLinearVelocity(b2Vec2(0, 0));
			b2Filter filter;
			filter.groupIndex = -1;
			kinematicrect->body->GetFixtureList()->SetFilterData(filter);
		}
	}
	int posobjthrow_x, posobjthrow_y;
	if (trowrightobj == true && otherthrowobj == false) {
		cloudrightthrow = CreateCircle(28, 189, 6, b2_kinematicBody, 50, 0.3f, 0);
		trowrightobj = false;
		otherthrowobj = true;
		
	}
	if (cloudrightthrow) {
		cloudrightthrow->GetPosition(posobjthrow_x, posobjthrow_y);
		if (posobjthrow_x == 137) {
			world->DestroyBody(cloudrightthrow->body);
			cloudrightthrow = nullptr;
			otherthrowobj = false;
		}
	}
	
	int posobjthrow_x2, posobjthrow_y2;
	if (trowrightobj2 == true && otherthrowobj2 == false) {
		cloudrightthrow2 = CreateCircle(133, 189, 6, b2_kinematicBody, 50, 0.3f, 0);
		trowrightobj2 = false;
		otherthrowobj2 = true;

	}
	if (cloudrightthrow2) {
		cloudrightthrow2->GetPosition(posobjthrow_x2, posobjthrow_y2);
		if (posobjthrow_x2 == 26) {
			world->DestroyBody(cloudrightthrow2->body);
			cloudrightthrow2 = nullptr;
			otherthrowobj2 = false;
		}
	}

	if (sun)
	{
		int x, y;
		sun->GetPosition(x, y);
		if (x > 120)
		{
			sun->body->SetLinearVelocity(b2Vec2(-1, 0));
		}
		if (x < 25)
		{
			sun->body->SetLinearVelocity(b2Vec2(1, 0));
		}
	}
	if (App->scene_intro->player != nullptr) {
		App->scene_intro->player->GetPosition(ballx, bally);
		if (bally < 155) {
			b2Filter filter;
			filter.groupIndex = 0;
			kinematicrect->body->GetFixtureList()->SetFilterData(filter);
		}
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
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && (b->GetType() != b2_kinematicBody) ) {

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
		if (debug_joint == true && (b->GetType() != b2_kinematicBody)) {
			b2MouseJointDef def;
			def.bodyA = ground;
			def.bodyB = mouse_body;
			def.target = mouse_pos;
			def.dampingRatio = 0.5f;
			def.frequencyHz = 2.0f;
			def.maxForce = 100.0f * mouse_body->GetMass();
			mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
		}


		if ((mouse_joint != nullptr) && (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT) && (b->GetType() != b2_kinematicBody)) {

			mouse_joint->SetTarget(b2Vec2(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY())));
			App->renderer->DrawLine(METERS_TO_PIXELS( mouse_joint->GetAnchorB().x) , METERS_TO_PIXELS( mouse_joint->GetAnchorB().y), App->input->GetMouseX(), App->input->GetMouseY(), 255, 100, 100);
		}
	

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint != nullptr && (b->GetType() != b2_kinematicBody))
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

	delete triangles;
	delete rightKickers;
	delete leftKickers;

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

p2List<PhysBody*>* ModulePhysics::GetLeftKickers()
{
	return leftKickers;
}

p2List<PhysBody*>* ModulePhysics::GetRightKickers()
{
	return rightKickers;
}

p2List<PhysBody*>* ModulePhysics::GetTriangles()
{
	return triangles;
}


//
PhysBody* ModulePhysics::CreatePolygon(int x, int y, int* points, int size, float dens, int rest, int filterIndex, b2BodyType type)
{
	b2BodyDef body;
	body.type = type;
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
	fixture.restitution = rest;
	fixture.filter.groupIndex = filterIndex;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = 0;
	pbody->height = 0;

	return pbody;
}





PhysBody * ModulePhysics::CreateKicker( int kickerX, int kickerY, int* points, int size)
{
		return CreatePolygon(kickerX, kickerY, points, size, 100, 0, -2, b2_dynamicBody);
}

void ModulePhysics::BuildLeftKickers(p2List<PhysBody*>* leftKickers)
{
	int kicker1[10] = {
		23, 12,
		2, 10,
		0, 5,
		2, 1,
		8, 1
	};

	PhysBody* k = CreateKicker(48, 407, kicker1, 10);
	PhysBody* k2 = CreateRectangle(53, 411, 1, 1, b2_staticBody);

	revolutedef.bodyA = k2->body;
	revolutedef.bodyB = k->body;
	revolutedef.localAnchorB = b2Vec2(0.1 , 0.1);
	revolutedef.enableLimit = true;
	revolutedef.lowerAngle = -(3.14 / 3);
	revolutedef.upperAngle = (3.14 / 32);
	revolutedef.collideConnected = false;
	revolute_joint = (b2RevoluteJoint*)world->CreateJoint(&revolutedef);

	leftKickers->add(k);

	k = CreateKicker(48, 263, kicker1, 10);
	k2 = CreateRectangle(53, 267, 1, 1, b2_staticBody);

	revolutedef.bodyA = k2->body;
	revolutedef.bodyB = k->body;
	revolutedef.localAnchorB = b2Vec2(0.1, 0.1);
	revolutedef.enableLimit = true;
	revolutedef.lowerAngle = -(3.14 / 3);
	revolutedef.upperAngle = (3.14 / 32);
	revolutedef.collideConnected = false;
	revolute_joint = (b2RevoluteJoint*)world->CreateJoint(&revolutedef);

	leftKickers->add(k);


	k = CreateKicker(48, 119, kicker1, 10);
	k2 = CreateRectangle(53, 123, 1, 1, b2_staticBody);

	revolutedef.bodyA = k2->body;
	revolutedef.bodyB = k->body;
	revolutedef.localAnchorB = b2Vec2(0.1, 0.1);
	revolutedef.enableLimit = true;
	revolutedef.lowerAngle = -(3.14 / 3);
	revolutedef.upperAngle = (3.14 / 32);
	revolutedef.collideConnected = false;
	revolute_joint = (b2RevoluteJoint*)world->CreateJoint(&revolutedef);
	
	
	leftKickers->add(k);

}

void ModulePhysics::BuildRightKickers(p2List<PhysBody*>* rightKickers) {

	int kicker1[10] = {
		0, 12,
		20, 10,
		23, 5,
		20, 1,
		14, 1
	};

	PhysBody* k = CreateKicker(86, 407, kicker1, 10);
	PhysBody* k2 = CreateRectangle(107, 411, 1, 1, b2_staticBody);

	revolutedef.bodyA = k2->body;
	revolutedef.bodyB = k->body;
	revolutedef.localAnchorB = b2Vec2(0.33, 0.1);
	revolutedef.enableLimit = true;
	revolutedef.lowerAngle = -(3.14 / 32);
	revolutedef.upperAngle = (3.14 / 3);
	revolutedef.collideConnected = false;
	revolute_joint = (b2RevoluteJoint*)world->CreateJoint(&revolutedef);

	rightKickers->add(k);


	k = CreateKicker(86, 119, kicker1, 10);
	k2 = CreateRectangle(107, 123, 1, 1, b2_staticBody);

	revolutedef.bodyA = k2->body;
	revolutedef.bodyB = k->body;
	revolutedef.localAnchorB = b2Vec2(0.33, 0.1);
	revolutedef.enableLimit = true;
	revolutedef.lowerAngle = -(3.14 / 32);
	revolutedef.upperAngle = (3.14 / 3);
	revolutedef.collideConnected = false;
	revolute_joint = (b2RevoluteJoint*)world->CreateJoint(&revolutedef);

	rightKickers->add(k);

	k = CreateKicker(86, 263, kicker1, 10);
	k2 = CreateRectangle(107, 267, 1, 1, b2_staticBody);

	revolutedef.bodyA = k2->body;
	revolutedef.bodyB = k->body;
	revolutedef.localAnchorB = b2Vec2(0.33, 0.1);
	revolutedef.enableLimit = true;
	revolutedef.lowerAngle = -(3.14 / 32);
	revolutedef.upperAngle = (3.14 / 3);
	revolutedef.collideConnected = false;
	revolute_joint = (b2RevoluteJoint*)world->CreateJoint(&revolutedef);

	rightKickers->add(k);

	
}




//
void ModulePhysics::CreateFloatingWalls()
{

	//Bottom Floor
	// Pivot 0, 0
	int LeftBottomFloor[14] = {
		51, 406,
		28, 395,
		28, 383,
		26, 378,
		25, 383,
		25, 401,
		48, 412
	};
	// Pivot 0, 0
	int RightBottomFloor[14] = {
		109, 406,
		132, 394,
		132, 382,
		134, 377,
		135, 382,
		135, 400,
		112, 412
	};

	CreateChain(0 , 0, LeftBottomFloor, 14, -2);
	CreateChain(0, 0, RightBottomFloor, 14, -2);


	//MiddleFloor
	// Pivot 0, 0
	int LeftMiddleFloor[14] = {
		51, 261,
		28, 250,
		28, 239,
		26, 233,
		25, 239,
		25, 256,
		48, 267
	};
	// Pivot 0, 0
	int RightMiddleFloor[14] = {
		109, 262,
		132, 251,
		132, 239,
		134, 234,
		135, 239,
		135, 257,
		112, 267
	};

	CreateChain(0, 0, LeftMiddleFloor, 14, -2);
	CreateChain(0, 0, RightMiddleFloor, 14, -2);


	//UpperFloor
	// Pivot 0, 0
	int LeftUpperFloor[14] = {
		52, 119,
		28, 107,
		28, 94,
		26, 91,
		25, 94,
		25, 112,
		48, 124
	};
	// Pivot 0, 0
	int RightUpperFloor[14] = {
		108, 118,
		132, 106,
		132, 94,
		134, 91,
		135, 94,
		135, 112,
		113, 123
	};

	CreateChain(0, 0, LeftUpperFloor, 14, -2);
	CreateChain(0, 0, RightUpperFloor, 14, -2);
	




	//Triangles
	// Pivot 0, 0
	int LeftBottomTriangle[6] = {
		40, 378,
		40, 387,
		48, 391
	};
	// Pivot 0, 0
	int RightBottomTriangle[6] = {
		112, 392,
		120, 388,
		120, 379
	};

	CreateChain(0, 0, LeftBottomTriangle, 6, -2);
	CreateChain(0, 0, RightBottomTriangle, 6, -2);

	
	// Pivot 0, 0
	int LeftMiddleTriangle[6] = {
		48, 247,
		40, 243,
		40, 236
	};
	// Pivot 0, 0
	int RightMiddleTriangle[6] = {
		112, 248,
		120, 243,
		120, 236
	};

	CreateChain(0, 0, LeftMiddleTriangle, 6, -2);
	CreateChain(0, 0, RightMiddleTriangle, 6, -2);

	// Pivot 0, 0
	int LeftUpperTriangle[6] = {
		48, 103,
		40, 99,
		40, 92
	};
	// Pivot 0, 0
	int RightUpperTriangle[6] = {
		112, 103,
		120, 99,
		120, 92
	};
	CreateChain(0, 0, LeftUpperTriangle, 6, -2);
	CreateChain(0, 0, RightUpperTriangle, 6, -2);


	//triangles->add(

}

void ModulePhysics::ApplayVerticalForce(PhysBody* ball) {
	ball->body->ApplyForce(b2Vec2(0, -300), b2Vec2(0, 0), true);
}

void ModulePhysics::throwingRightCloud() {

		trowrightobj = true;

}

void ModulePhysics::throwingLeftCloud() {

	trowrightobj2 = true;
}

void ModulePhysics::ballupsideumbrella(PhysBody* Mball) {
	
	kinematicrect->GetPosition(xcloud, ycloud);
	if (App->scene_intro->TimesKickCloud >= 3) {
		if (xcloud == 70) {
			Mball->body->SetLinearVelocity(b2Vec2(0, -15));
			ballupumbrella = true;
			App->scene_intro->TimesKickCloud = 0;
			kinematicrect->body->SetLinearVelocity(b2Vec2(1,0));
		}
	}
	



}
