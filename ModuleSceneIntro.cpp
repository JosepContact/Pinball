#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"

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

	Dead = false;
	end_game = false;
	ball_count = 3;

	BallisUp = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	spring = App->textures->Load("pinball/spring.png");
	right_kicker = App->textures->Load("pinball/right_kicker.png");
	left_kicker = App->textures->Load("pinball/left_kicker.png");
	foreground = App->textures->Load("pinball/foreground.png");
	background = App->textures->Load("pinball/background.png");
	ball = App->textures->Load("pinball/ball.png");
	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	// Loading FX
	music = App->audio->LoadFx("pinball/music.ogg");
	kicker_fx = App->audio->LoadFx("pinball/kicker.wav");
	bouncy_fx = App->audio->LoadFx("pinball/bouncy_ball.wav");
	fail_fx = App->audio->LoadFx("pinball/fail.wav");
	
	lightboosts[Ll].tex = App->textures->Load("pinball/Lights/Ll.png");
	lightboosts[Ul].tex = App->textures->Load("pinball/Lights/Uu.png");
	lightboosts[Vl].tex = App->textures->Load("pinball/Lights/Vv.png");
	lightboosts[Il].tex = App->textures->Load("pinball/Lights/Ii.png");
	lightboosts[Tl].tex = App->textures->Load("pinball/Lights/Tt.png");
	lightboosts[Pp].tex = App->textures->Load("pinball/Lights/P.png");
	lightboosts[Op].tex = App->textures->Load("pinball/Lights/o.png");
	lightboosts[Wp].tex = App->textures->Load("pinball/Lights/w.png");
	lightboosts[Ep].tex = App->textures->Load("pinball/Lights/e.png");
	lightboosts[Rp].tex = App->textures->Load("pinball/Lights/R.png");
	lightboosts[Dp].tex = App->textures->Load("pinball/Lights/d.png");
	lightboosts[Up].tex = App->textures->Load("pinball/Lights/u.png");
	lightboosts[Np].tex = App->textures->Load("pinball/Lights/n.png");
	lightboosts[Kp].tex = App->textures->Load("pinball/Lights/k.png");
	lightboosts[Ww].tex = App->textures->Load("pinball/Lights/Ww.png");
	lightboosts[Iw].tex = App->textures->Load("pinball/Lights/Ii.png");
	lightboosts[Nw].tex = App->textures->Load("pinball/Lights/Nn.png");
	lightboosts[LCK].tex = App->textures->Load("pinball/Lights/LCK.png");
	lightboosts[TwoP].tex = App->textures->Load("pinball/Lights/2p.png");
	lightboosts[ThreeP].tex = App->textures->Load("pinball/Lights/3p.png");

	
	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);
	// Sensors
	SetSensors();

	// Static Bodies
	// Chains
	RDTriangle = App->physics->CreateChain(0, 0, RDTriangle_pts, 6);
	RDTriangle->body->SetType(b2_staticBody);
	BckgroundCol = App->physics->CreateChain(0, 0, BckgroundCol_pts, 178);
	BckgroundCol->body->SetType(b2_staticBody);
	BckPatch = App->physics->CreateChain(0, 0, BckPatch_pts, 12);
	BckPatch->body->SetType(b2_staticBody);
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
	//Ramps
	StartingRamp = App->physics->CreateChain(0, 0, StartingRamp_pts, 50);
	StartingRamp->body->SetType(b2_staticBody);
	TRRamp = App->physics->CreateChain(0, 0, TRRamp_pts, 160);
	TRRamp->body->SetType(b2_staticBody);
	TopRamp = App->physics->CreateChain(0, 0, TopRamp_pts, 94);
	TopRamp->body->SetType(b2_staticBody);
	GridRamp = App->physics->CreateChain(0, 0, GridRamp_pts, 172);
	GridRamp->body->SetType(b2_staticBody);

	// Rectangles
	GreyBlocker = App->physics->CreateRectangle(250, 80, 3, 40);
	GreyBlocker->body->SetType(b2_staticBody);
	GridRampPatch = App->physics->CreateRectangle(136, 135, 15, 63);
	GridRampPatch->body->SetType(b2_staticBody);
	BBPatch = App->physics->CreateRectangle(390, 160, 10, 50);
	BBPatch->body->SetType(b2_staticBody);

	// Bouncy Bodies
	BouncyDL = App->physics->CreateChain(0, 0, BouncyDL_pts, 16);
	BouncyDL->body->SetType(b2_staticBody);
	BouncyDL->body->GetFixtureList()->SetRestitution(BOUNCY_TRIANGLES_PWR);
	BouncyDR = App->physics->CreateChain(0, 0, BouncyDR_pts, 14);
	BouncyDR->body->SetType(b2_staticBody);
	BouncyDR->body->GetFixtureList()->SetRestitution(BOUNCY_TRIANGLES_PWR);
	
	// Red Rectangles
	uint rrit = 0;
	// Left
	RedRectangles.add(App->physics->CreateRectangle(122, 179, 7, 25));
	RedRectangles.add(App->physics->CreateRectangle(21, 179, 7, 25));
	RedRectangles.add(App->physics->CreateRectangle(88, 184, 7, 25));
	RedRectangles.add(App->physics->CreateRectangle(54, 184, 7, 25));
	// Right												  
	RedRectangles.add(App->physics->CreateRectangle(305, 139, 7, 25));
	RedRectangles.add(App->physics->CreateRectangle(271, 139, 7, 25));
	RedRectangles.add(App->physics->CreateRectangle(339, 139, 7, 25));
	RedRectangles.add(App->physics->CreateRectangle(373, 139, 7, 25));
	for (p2List_item<PhysBody*>* rr = RedRectangles.getFirst(); rr != NULL; rr = rr->next) {
		rr->data->body->SetType(b2_staticBody);
		if (rrit++ < 4)
			isUp.add(rr->data);
		else
			isDown.add(rr->data);
	}

	// Bouncy Bodies Set Up
	BouncyCircles.add(App->physics->CreateCircle(305, 275, 30));
	BouncyCircles.add(App->physics->CreateCircle(268, 206, 30));
	BouncyCircles.add(App->physics->CreateCircle(370, 214, 30));

	for (p2List_item<PhysBody*>* bc = BouncyCircles.getFirst(); bc != NULL; bc = bc->next) {
		bc->data->listener = this;
		bc->data->body->SetType(b2_staticBody);
		bc->data->body->GetFixtureList()->SetRestitution(BOUNCY_CIRCLES_PWR);
		isDown.add(bc->data);
	}

	// Setting up Up/Down Layers
	isUp.add(StartingRamp);
	isUp.add(TRRamp);
	isUp.add(TopRamp);
	isUp.add(GridRamp);

	isDown.add(BckgroundCol);
	isDown.add(RDTriangle);
	isDown.add(LShapeL);
	isDown.add(LShapeR);
	isDown.add(TRRampE);
	isDown.add(TRRed);
	isDown.add(TLRed);
	isDown.add(GreyBlocker);
	isDown.add(BouncyDL);
	isDown.add(BouncyDR);
	isDown.add(BckPatch);
	isDown.add(GridRampPatch);
	isDown.add(BBPatch);

	// --- Setting Layers

	// Light Boosts

	for (int i = 0; i < lightnum::__LAST; i++) {
		lightboosts[i].isLighted = false;
		lightboosts[i].rect.x = 0;
		lightboosts[i].rect.y = 0;
	}

	lightboosts[Ll].rect.w = 27;
	lightboosts[Ll].rect.h = 27;
	lightboosts[Ll].pos = { 22, 217 };

	lightboosts[Ul].rect.w = 27;
	lightboosts[Ul].rect.h = 27;
	lightboosts[Ul].pos = { 57, 217 };

	lightboosts[Vl].rect.w = 27;
	lightboosts[Vl].rect.h = 27;
	lightboosts[Vl].pos = { 92, 216 };

	lightboosts[Il].rect.w = 27;
	lightboosts[Il].rect.h = 27;
	lightboosts[Il].pos = { 37, 277 };

	lightboosts[Tl].rect.w = 27;
	lightboosts[Tl].rect.h = 27;
	lightboosts[Tl].pos = { 78, 277 };

	lightboosts[Pp].rect.w = 42;
	lightboosts[Pp].rect.h = 36;
	lightboosts[Pp].pos = { 34, 532 };

	lightboosts[Op].rect.w = 43;
	lightboosts[Op].rect.h = 34;
	lightboosts[Op].pos = { 45, 508 };

	lightboosts[Wp].rect.w = 43;
	lightboosts[Wp].rect.h = 34;
	lightboosts[Wp].pos = { 57, 483 };

	lightboosts[Ep].rect.w = 35;
	lightboosts[Ep].rect.h = 46;
	lightboosts[Ep].pos = { 110, 415 };

	lightboosts[Rp].rect.w = 35;
	lightboosts[Rp].rect.h = 46;
	lightboosts[Rp].pos = { 176, 407 };

	lightboosts[Dp].rect.w = 35;
	lightboosts[Dp].rect.h = 46;
	lightboosts[Dp].pos = { 208, 418 };

	lightboosts[Up].rect.w = 35;
	lightboosts[Up].rect.h = 46;
	lightboosts[Up].pos = { 312, 415 };

	lightboosts[Np].rect.w = 47;
	lightboosts[Np].rect.h = 33;
	lightboosts[Np].pos = { 371, 517 };

	lightboosts[Kp].rect.w = 47;
	lightboosts[Kp].rect.h = 33;
	lightboosts[Kp].pos = { 376, 546 };

	lightboosts[Ww].rect.w = 27;
	lightboosts[Ww].rect.h = 27;
	lightboosts[Ww].pos = { 276, 148 };

	lightboosts[Iw].rect.w = 27;
	lightboosts[Iw].rect.h = 27;
	lightboosts[Iw].pos = { 309, 149 };

	lightboosts[Nw].rect.w = 27;
	lightboosts[Nw].rect.h = 27;
	lightboosts[Nw].pos = { 342, 149 };

	lightboosts[LCK].rect.w = 57;
	lightboosts[LCK].rect.h = 48;
	lightboosts[LCK].pos = { 356, 456 };

	lightboosts[TwoP].rect.w = 37;
	lightboosts[TwoP].rect.h = 59;
	lightboosts[TwoP].pos = { 132, 341 };
	
	lightboosts[ThreeP].rect.w = 36;
	lightboosts[ThreeP].rect.h = 57;
	lightboosts[ThreeP].pos = { 287, 353 };
	// --- Light Boosts

	// Light Boosts Sensors
	
	for (int i = 0; i < lightnum::__LAST; i++) {
		LightBoost* lb = &lightboosts[i];
		lb->sensor = App->physics->CreateRectangleSensor(lb->pos.x + lb->rect.w / 2, lb->pos.y + lb->rect.h / 2, lb->rect.w, lb->rect.h);
	}

	// ---

	AddBall();

	// --- Music Playing


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
	// INPUT ---------------------------
	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 11));
		circles.getLast()->data->listener = this;
		circles.getLast()->data->body->SetBullet(true);
		circles.getLast()->data->body->SetFixedRotation(true);
		circles.getLast()->data->body->GetFixtureList()->SetRestitution(0.3); 
	}
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN && circles.count() > 0)
	{
		b2Vec2 point;
		circles.getLast()->data->body->GetLocalPoint(point);
		circles.getLast()->data->body->ApplyForce(b2Vec2(4, 0), point, true);
	}
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN && circles.count() > 0)
	{
		b2Vec2 point;
		circles.getLast()->data->body->GetLocalPoint(point);
		circles.getLast()->data->body->ApplyForce(b2Vec2(-4, 0), point, true);
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN && App->player->lifes == 0)
	{
		App->player->lifes = 5;
		if (App->player->score > App->player->highscore)
			App->player->highscore = App->player->score;

		App->player->score = 0;
		BallisUp = true;
		AddBall();
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
	for (int i = 0; i < lightnum::__LAST; i++) {
		if (lightboosts[i].isLighted == true) {
			App->renderer->Blit(lightboosts[i].tex, lightboosts[i].pos.x, lightboosts[i].pos.y, &lightboosts[i].rect);
		}
	}

		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(ball, x, y, NULL, 1.0f, c->data->GetRotation());
		// StartingRamp Switch
		if (StartingRampSensor->Contains(x, y) && BallisUp) {
			BallisUp = false;
		}
		
		//TRRamp Switch
		if (TRRampSensor->Contains(x + c->data->width, y)) {
			BallisUp = true;
			c->data->body->SetLinearVelocity({15, 70});
		}
		if (TRRampExit->Contains(x, y) || GridRampExitR->Contains(x, y + c->data->height / 3)) {
			if(BallisUp)
				c->data->body->SetLinearVelocity({0, 0});
			BallisUp = false;
		}
		if (TopRampSensor->Contains(x + c->data->width * 2, y + c->data->height)) {
			BallisUp = true;
			c->data->body->SetLinearVelocity({ 20, 5 });
		}
		if(GridRampSensor->Contains(x, y)) {
			BallisUp = true;
		}
		if (TopRampExit->Contains(x, y))
			BallisUp = false;
		if (GridRampExitL->Contains(x, y) && BallisUp == true)
		{
			b2Vec2 point;
			circles.getLast()->data->body->GetLocalPoint(point);
			circles.getLast()->data->body->ApplyForce(b2Vec2(0, -200), point, true);
			BallisUp = false;
		}

		for (int i = 0; i < lightnum::__LAST; i++) {
			if (lightboosts[i].sensor->Contains(x + c->data->width, y + c->data->height)) {
				lightboosts[i].isLighted = true;
			}
		}
		if (StartingBall->Contains(x, y)) {
			ball_available = true;
		}
		else ball_available = false;

		c = c->next;
	}

	if (!BallisUp) {
		App->renderer->Blit(foreground, 0, 0, NULL, 0, 0);
	}


	iPoint ball_p;
	circles.getLast()->data->GetPosition(ball_p.x, ball_p.y);

	if (!BallisUp && ball_p.x < 115)
		App->renderer->DrawCircle(ball_p.x + circles.getLast()->data->width, ball_p.y + circles.getLast()->data->height, 11, 255, 255, 255);

	for (p2List_item<PhysBody*>* id = isDown.getFirst(); id != NULL; id = id->next) {
		id->data->body->SetActive(!BallisUp);
	}
	for (p2List_item<PhysBody*>* iu = isUp.getFirst(); iu != NULL; iu = iu->next) {
		iu->data->body->SetActive(BallisUp);
	}
	
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && circles.count() > 0 && ball_available == true)
	{
		spring_y += 0.5f;
		App->renderer->Blit(spring, spring_x, spring_y, NULL, 0, 0);
		ball_force -= 2;
	}
	else {
		spring_y = 884.0f;
		App->renderer->Blit(spring, spring_x, spring_y, NULL, 0, 0);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP && circles.count() > 0 && ball_available == true) {

		b2Vec2 point;
		circles.getLast()->data->body->GetLocalPoint(point);
		if (ball_force < -160)
		circles.getLast()->data->body->ApplyForce(b2Vec2(0, -160), point, true);
		else circles.getLast()->data->body->ApplyForce(b2Vec2(0, ball_force), point, true);
		circles.getLast()->data->listener = this;
	}
	
	

	// FX EFFECTS --------------------------------------------

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	App->audio->PlayFx(kicker_fx);
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		App->audio->PlayFx(kicker_fx);

	if (music_playing == false) {
		App->audio->PlayFx(music, -1);
		music_playing = true;
	}

	// CHECK END GAME --------------------------------------
	if (end_game == true) {
		AddBall();
		SetSensors();
		end_game = false;
		BallisUp = true;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	for (p2List_item<PhysBody*>* bc = BouncyCircles.getFirst(); bc != NULL; bc = bc->next) {
		if (bodyA == bc->data)
		{
			App->audio->PlayFx(bouncy_fx);
			App->player->score += 100;
		}
	}

	if (FailSensor->body == bodyA->body && bodyB->body == circles.getLast()->data->body)
	{
		if (App->player->lifes > 0)
		{
			App->player->lifes = App->player->lifes - 1;
			App->audio->PlayFx(fail_fx);
			if (App->player->lifes != 0) {
				end_game = true;
			}
		}
	}
}

void ModuleSceneIntro::AddBall()
{
	circles.add(App->physics->CreateCircle(471, 870, 11));
	circles.getLast()->data->listener = this;
	circles.getLast()->data->body->SetFixedRotation(true);
	circles.getLast()->data->body->GetFixtureList()->SetRestitution(0.5);
	circles.getLast()->data->body->SetBullet(true);
}

void ModuleSceneIntro::SetSensors()
{
	StartingBall = FailSensor = App->physics->CreateRectangleSensor(467, 867, 35, 35);
	FailSensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 20, SCREEN_WIDTH, 50);
	FailSensor->listener = this;
	StartingRampSensor = App->physics->CreateRectangleSensor(325, 75, 5, 30);
	TRRampSensor = App->physics->CreateRectangleSensor(320, 320, 60, 15);
	TRRampSensor->body->SetTransform(TRRampSensor->body->GetPosition(), DEGTORAD * 15);
	TRRampExit = App->physics->CreateCircle(407, 721, 16);
	TRRampExit->body->SetType(b2_staticBody);
	TRRampExit->body->GetFixtureList()->SetSensor(true);
	GridRampExitR = App->physics->CreateCircle(54, 719, 16);
	GridRampExitR->body->SetType(b2_staticBody);
	GridRampExitR->body->GetFixtureList()->SetSensor(true);
	GridRampSensor = App->physics->CreateRectangleSensor(145, 127, 5, 40);
	GridRampExitL = App->physics->CreateRectangleSensor(18, 870, 30, 10);
	TopRampSensor = App->physics->CreateRectangleSensor(92, 80, 5, 60);
	TopRampExit = App->physics->CreateRectangleSensor(425, 250, 40, 5);
}

