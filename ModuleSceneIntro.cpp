#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

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

	BallisUp = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	right_kicker = App->textures->Load("pinball/right_kicker.png");
	left_kicker = App->textures->Load("pinball/left_kicker.png");
	foreground = App->textures->Load("pinball/foreground.png");
	background = App->textures->Load("pinball/background.png");
	ball = App->textures->Load("pinball/ball.png");
	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	
	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
	
	// Sensors
	StartingRampSensor = App->physics->CreateRectangleSensor(325, 80, 25, 40);
	LoopRampSensor = App->physics->CreateRectangleSensor(225, 150, 40, 10);
	LoopRampTrigger = App->physics->CreateRectangleSensor(205, 165, 2, 40);
	TRRampSensor = App->physics->CreateRectangleSensor(320, 310, 60, 4);

	// Static Bodies
	// Chains
	RDTriangle = App->physics->CreateChain(0, 0, RDTriangle_pts, 6);
	RDTriangle->body->SetType(b2_staticBody);
	BckgroundCol = App->physics->CreateChain(0, 0, BckgroundCol_pts, 178);
	BckgroundCol->body->SetType(b2_staticBody);
	LShapeL = App->physics->CreateChain(0, 0, LShapeL_pts, 18);
	LShapeL->body->SetType(b2_staticBody);
	LShapeR = App->physics->CreateChain(0, 0, LShapeR_pts, 14);
	LShapeR->body->SetType(b2_staticBody);
	TRRampE = App->physics->CreateChain(0, 0, TRRampE_pts, 22);
	TRRampE->body->SetType(b2_staticBody);
	TRRed = App->physics->CreateChain(0, 0, TRRed_pts, 30);
	TRRed->body->SetType(b2_staticBody);
	TLRed = App->physics->CreateChain(0, 0, TLRed_pts, 34);
	TLRed->body->SetType(b2_staticBody);
	StartingRamp = App->physics->CreateChain(0, 0, StartingRamp_pts, 54);
	StartingRamp->body->SetType(b2_staticBody);
	LoopRampOut = App->physics->CreateChain(0, 0, LoopRampOut_pts, 34);
	LoopRampOut->body->SetType(b2_staticBody);
	LoopRampTriggered = App->physics->CreateChain(0, 0, LoopRampTriggered_pts, 34);
	LoopRampTriggered->body->SetType(b2_staticBody);
	TRRamp = App->physics->CreateChain(0, 0, TRRamp_pts, 160);
	TRRamp->body->SetType(b2_staticBody);

	// Rectangles
	GreyBlocker = App->physics->CreateRectangle(280, 80, 3, 40);
	GreyBlocker->body->SetType(b2_staticBody);

	// Circles
	LoopRampIn = GreyBlocker = App->physics->CreateCircle(191, 193, 17);
	GreyBlocker->body->SetType(b2_staticBody);

	// Bouncy Bodies
	BouncyDL = App->physics->CreateChain(0, 0, BouncyDL_pts, 16);
	BouncyDL->body->SetType(b2_staticBody);
	BouncyDL->body->GetFixtureList()->SetRestitution(BOUNCY_TRIANGLES_PWR);
	BouncyDR = App->physics->CreateChain(0, 0, BouncyDR_pts, 14);
	BouncyDR->body->SetType(b2_staticBody);
	BouncyDR->body->GetFixtureList()->SetRestitution(BOUNCY_TRIANGLES_PWR);
	


	// Bouncy Bodies Set Up
	BouncyCircles.add(App->physics->CreateCircle(305, 275, 31));
	BouncyCircles.add(App->physics->CreateCircle(268, 206, 31));
	BouncyCircles.add(App->physics->CreateCircle(370, 214, 31));

	for (p2List_item<PhysBody*>* bc = BouncyCircles.getFirst(); bc != NULL; bc = bc->next) {
		bc->data->body->SetType(b2_staticBody);
		bc->data->body->GetFixtureList()->SetRestitution(BOUNCY_CIRCLES_PWR);
	}

	p2List<PhysBody*> DeactivateQue;

	DeactivateQue.add(TRRamp);
	DeactivateQue.add(BckgroundCol);
	DeactivateQue.add(TRRed);
	DeactivateQue.add(LoopRampOut);
	DeactivateQue.add(LoopRampTriggered);

	for (p2List_item<PhysBody*>* it = DeactivateQue.getFirst(); it != NULL; it = it->next) {
		it->data->body->SetActive(false);
	}

	ball_available = true;

	circles.add(App->physics->CreateCircle(471, 870, 11));
	circles.getLast()->data->listener = this;
	circles.getLast()->data->body->SetFixedRotation(true);
	circles.getLast()->data->body->GetFixtureList()->SetRestitution(0.3);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}


update_status ModuleSceneIntro::Update()
{

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 11));
		circles.getLast()->data->listener = this;
	}
		

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && circles.count() > 0 && ball_available == true)
	{
			b2Vec2 point;
			circles.getLast()->data->body->GetLocalPoint(point);
			circles.getLast()->data->body->ApplyForce(b2Vec2(0, -160), point, true);
			circles.getLast()->data->listener = this;
			ball_available = false;
		
	}

	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	App->renderer->Blit(background, 0, 0, NULL, 0, 0);

	App->renderer->Blit(left_kicker, 69, 802, NULL, 1.0f, App->physics->left_rotation - 2);
	App->renderer->Blit(right_kicker, 253, 802, NULL, 1.0f, App->physics->right_rotation + 2);

	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, NULL, 1.0f, c->data->GetRotation());
		// StartingRamp Switch
		if (StartingRampSensor->Contains(x, y)) {
			StartingRamp->body->SetActive(false);
			BckgroundCol->body->SetActive(true);
			TRRed->body->SetActive(true);
			BallisUp = false;
		}
		// LoopRampSensor Switch
		//if (LoopRampSensor->Contains(x, y + c->data->height)) {
		//	LoopRampOut->body->SetActive(true);
		//	BckgroundCol->body->SetActive(false);
		//	LoopRampTriggered->body->SetActive(false);
		//}
		//
		//// LoopRampTrigger Switch
		//if (LoopRampTrigger->Contains(x, y)) {
		//	LoopRampOut->body->SetActive(false);
		//	LoopRampTriggered->body->SetActive(true);
		//	BallisUp = true;
		//}

		//TRRamp Switch
		if (TRRampSensor->Contains(x, y)) {
			TRRamp->body->SetActive(true);
			BallisUp = true;
		}

		c = c->next;
	}

	/*
	c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
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
	*/
	if (!BallisUp) {
		App->renderer->Blit(foreground, 0, 0, NULL, 0, 0);
	}
	BckgroundCol->body->SetActive(!BallisUp);
	TRRed->body->SetActive(!BallisUp);
	TRRampE->body->SetActive(!BallisUp);
	BouncyDR->body->SetActive(!BallisUp);
	BouncyDL->body->SetActive(!BallisUp);
	for (p2List_item<PhysBody*>* bc = BouncyCircles.getFirst(); bc != NULL; bc = bc->next) {
	bc->data->body->SetActive(!BallisUp);
	}
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

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
