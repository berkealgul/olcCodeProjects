#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using namespace olc;

class Game : public olc::PixelGameEngine
{
public:
	vf2d x, v, a, g;
	vf2d target;
	vf2d targetOrigin;
	vf2d launchDir;
	float m = 1;
	float travelTime;
	bool simulating;
	float timeToCalculate;

	float freeTravelTime = 0.5;
	float calculationInterval =  0.05;
	float totalTravelTime = 3;
	float launchPower = 500;

	// render params
	olc::Pixel particleColor = olc::BLUE;
	olc::Pixel targetColor = olc::DARK_MAGENTA;
	olc::Pixel launchDirColor = olc::RED;
	int particleR = 6;
	int targetR = 2;
	int launchVectorRenderLenght = 15;

	int appState = 0; //0-standby 1-targeting 2-simulating

	Game() { sAppName = "2D Trajectory Demo"; }

public:
	bool OnUserCreate() override {
		ResetSimVariables();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		target = GetMousePos();
		
		Clear(olc::DARK_CYAN);
		FillCircle(x, particleR, particleColor); // particle
		FillCircle(target, targetR, targetColor);
		
		// logic update
		if (appState == 0) {
			if (GetMouse(Mouse::LEFT).bHeld) {
				appState = 1;
				targetOrigin = target;
			} 
		} else if(appState == 1) {
			if (!GetMouse(Mouse::LEFT).bHeld) {
				// Start sim
				appState = 2;
				ApplyForceToParticle((launchDir * launchPower)+g);
			} 
			else {
				launchDir = (target - targetOrigin).norm();
				DrawLine(x, x + launchDir * launchVectorRenderLenght, launchDirColor);
			}
		} else {
			if (GetMouse(Mouse::RIGHT).bPressed || travelTime <= 0) {
				// End and reset sim
				ResetSimVariables();
				return true;
			}

			timeToCalculate += fElapsedTime;
			travelTime -= fElapsedTime;
			ApplyMotionModel(fElapsedTime);

			// solve for force
			if (timeToCalculate < calculationInterval) {
				return true;
			}
			timeToCalculate = 0;
			auto dx = target - x;
			auto a_ = (dx - (v * travelTime)) / (travelTime * travelTime);
			ApplyForceToParticle(CalculateForce(a_ - a) * m);
		}

		return true;
	}

	// maybe apply fancy pid 
	// also mass is not included here
	vf2d CalculateForce(vf2d errorA) {
		return errorA;
	}

	void ApplyMotionModel(float dt) {
		v += (a * dt);
		x += (v * dt);
	}

	void ApplyForceToParticle(vf2d F) {
		a += F / m;
	}

	void ResetSimVariables() {
		x = vf2d(50, 50);
		g = vf2d(0, 9.81); // +y is down
		v = vf2d(0, 0);
		a = vf2d(0, 0);
		appState = 0;
		travelTime = totalTravelTime;
		timeToCalculate = 0;
	}

};


int main()
{
	Game game;
	if (game.Construct(256, 200, 4, 4))
		game.Start();
    return 0;
}