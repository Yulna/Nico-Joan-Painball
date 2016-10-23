#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"
#include"p2Point.h"
#include"Animation.h"
#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum CircleTypes {
	STATIC,
	KINEMATIC,
	DINAMIC
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	update_status Update();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, CircleTypes type, int density, float rest);
	PhysBody* CreateSensorCircle(int x, int y, int radius, CircleTypes type, int density, float rest);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateRectangleKickerPoint(int x, int y, int width, int height);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size, int filterIndex);
	PhysBody* CreatePolygon(int x, int y, int* points , int size, float dens, int rest, int filterIndex);
	PhysBody* CreateKinematicRectangle(int x, int y, int width, int height);
	void KickersForce(b2Vec2 vectforce, b2Vec2 posit, sides rl);
	void ApplayVerticalForce(PhysBody* ball);


	PhysBody* CreateKicker( int kickerX, int kickerY, int* points, int size);

	void BuildLeftKickers(p2List<PhysBody*>* leftKickers);
	void BuildRightKickers(p2List<PhysBody*>* rightKickers);

	void CreateKickerRev(PhysBody* b_A, PhysBody* b_B, sides map_side);

	//Floating walls
	void CreateFloatingWalls();
	void throwingRightCloud();


	// b2ContactListener ---
	void BeginContact(b2Contact* contact);




	//Get variables 
	p2List<PhysBody*>* GetLeftKickers();
	p2List<PhysBody*>* GetRightKickers();


	//Temporary test stuff

	PhysBody* spikyball1;
	PhysBody* ghost1;
	PhysBody* spikyball2;
	PhysBody* ghost2;
	PhysBody* kinematicrect;
	PhysBody* fatkirby;
	PhysBody* extinguisher;//to appear the extinguisher
	PhysBody* Useextinguisher;//to use the extinguisher

	PhysBody* sun;
	PhysBody* moon;


	PhysBody* tripleKirby;

	PhysBody* cloudrightsensor;
	PhysBody* cloudrightthrow;
	bool trowrightobj;
	bool otherthrowobj;


private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2RevoluteJoint* revolute_joint;
	b2RevoluteJoint* revolutemotorghost1;
	b2RevoluteJoint* revolutemotorghost2;
	b2Body* ground;
	
	PhysBody* kickerjoint;

	PhysBody* kickerjointV2;
	b2RevoluteJointDef revolutedef;
	b2Body* mouse_body;
	b2Vec2 mouse_pos;

	p2List<PhysBody*>* leftKickers;
	p2List<PhysBody*>* rightKickers;
};