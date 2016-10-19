#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

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
	p2List<PhysBody*> ricks;
	PhysBody* RDTriangle;
	PhysBody* BckgroundCol;
	PhysBody* LShapeL;
	PhysBody* LShapeR;
	PhysBody* TRRampE;
	PhysBody* TRRed;
	PhysBody* TLRed;
	PhysBody* BouncyDL;
	PhysBody* BouncyDR;

	PhysBody* sensor;
	bool sensed;
	SDL_Texture* background;
	SDL_Texture* ball;
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

private:
	// Pivot 0, 0
	int RDTriangle_pts[6] = {
		33, 907,
		33, 839,
		138, 909
	};

	// Pivot 0, 0
	int TLRed_pts[34] = {
		108, 406,
		122, 401,
		100, 208,
		90, 127,
		93, 116,
		99, 103,
		85, 108,
		65, 126,
		51, 145,
		42, 167,
		42, 192,
		46, 221,
		50, 244,
		58, 263,
		73, 303,
		83, 335,
		101, 387
	};

	// Pivot 0, 0
	int BouncyDL_pts[16] = {
		64, 666,
		71, 659,
		78, 661,
		85, 675,
		119, 773,
		117, 781,
		105, 783,
		64, 755
	};

	// Pivot 0, 0
	int BouncyDR_pts[14] = {
		336, 776,
		340, 784,
		351, 783,
		394, 755,
		393, 665,
		383, 659,
		372, 666
	};

	// Pivot 0, 0
	int TRRed_pts[30] = {
		389, 290,
		378, 255,
		399, 211,
		401, 159,
		392, 138,
		378, 126,
		375, 106,
		380, 93,
		395, 92,
		426, 116,
		433, 128,
		434, 154,
		430, 182,
		420, 217,
		404, 257
	};

	// Pivot 0, 0
	int TRRampE_pts[22] = {
		277, 377,
		273, 370,
		273, 284,
		280, 274,
		299, 275,
		350, 301,
		374, 330,
		348, 406,
		336, 401,
		346, 306,
		298, 281
	};

	// Pivot 0, 0
	int LShapeR_pts[14] = {
		425, 676,
		421, 676,
		420, 770,
		318, 843,
		325, 849,
		419, 785,
		424, 775
	};

	// Pivot 0, 0
	int LShapeL_pts[18] = {
		33, 682,
		36, 681,
		37, 769,
		40, 777,
		47, 781,
		141, 843,
		131, 849,
		41, 788,
		32, 781
	};

	// Pivot 0, 0
	int BckgroundCol_pts[178] = {
		452, 905,
		322, 907,
		450, 822,
		450, 651,
		443, 637,
		433, 630,
		423, 619,
		417, 600,
		426, 582,
		432, 572,
		433, 553,
		425, 520,
		419, 492,
		426, 477,
		430, 467,
		425, 458,
		415, 448,
		407, 434,
		467, 158,
		468, 129,
		456, 98,
		447, 82,
		435, 74,
		416, 63,
		390, 56,
		258, 56,
		238, 63,
		220, 73,
		211, 89,
		208, 107,
		209, 127,
		209, 172,
		240, 169,
		239, 99,
		262, 98,
		269, 101,
		269, 117,
		267, 207,
		246, 248,
		242, 373,
		245, 382,
		255, 400,
		262, 410,
		240, 407,
		219, 399,
		183, 386,
		173, 376,
		163, 313,
		210, 298,
		220, 290,
		236, 285,
		233, 260,
		157, 285,
		137, 197,
		128, 186,
		128, 152,
		139, 144,
		157, 135,
		179, 124,
		194, 114,
		200, 97,
		191, 74,
		175, 67,
		139, 67,
		120, 76,
		140, 59,
		137, 57,
		107, 58,
		76, 69,
		53, 83,
		42, 91,
		25, 112,
		13, 140,
		6, 174,
		5, 211,
		9, 260,
		48, 430,
		52, 450,
		49, 465,
		10, 546,
		11, 556,
		35, 581,
		35, 594,
		28, 604,
		15, 615,
		9, 627,
		7, 642,
		4, 986,
		327, 985
	};


};
