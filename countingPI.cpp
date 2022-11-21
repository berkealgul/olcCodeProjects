#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>

using namespace std;


class Sketch : public olc::PixelGameEngine
{
public:

	Sketch()
	{
		sAppName = "PI Counter";
	}


	float m1 = 1;
	float m2;
	float v = 80;
	float x1 = 20;
	float x2 = 80;
	float v1;
	float v2;
	
	int digits = 2;
	int w1 = 10;
	int w2 = 30;
	int hits = 0;


	bool OnUserCreate() override
	{
		v1 = v;
		v2 = -v;
		m2 = pow(100, digits-1);
		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		//Sim Step-------------
		x1 += v1 * dt;
		x2 += v2 * dt;

		//small cube hits wall
		if (x1 < 0)
		{
			hits++;
			x1 = 0;
			v1 *= -1;
		}
		//big cube hits small one
		else if (x2 - x1 <= w1)
		{
			hits++;
			x2 = x1+w1;

			float v1_ = ((m1 - m2) * v1 + (2 * m2 * v2)) / (m1 + m2);
			float v2_ = ((2 * m1 * v1) + (m2 - m1) * v2) / (m1 + m2);
			v1 = v1_;
			v2 = v2_;
		}

		//Drawing--------------
		Clear(olc::BLACK);
		DrawString(10, 10, std::to_string(hits), olc::WHITE);
		//boxes
		FillRect(x1, ScreenHeight()/2-w1, w1, w1, olc::RED);
		FillRect(x2, ScreenHeight() / 2 - w2, w2, w2, olc::RED);
		//floor
		FillRect(0, ScreenHeight()/2, ScreenWidth()-1, ScreenHeight()/2-1, olc::Pixel(175, 175 ,175));

		return true;
	}
	
};


int main()
{
	Sketch sketch;
	if (sketch.Construct(250, 150, 2, 2))
		sketch.Start();

	return 0;
}
