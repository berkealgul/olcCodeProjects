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
	float m2 = 100;
	float v = 50;
	float x1 = 20;
	float x2 = 80;
	float v1;
	float v2;

	int w1 = 10;
	int w2 = 30;

	bool OnUserCreate() override
	{
		v1 = v;
		v2 = -v;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		//Sim Step-------------
		x1 += v1 * fElapsedTime;
		x2 += v2 * fElapsedTime;

		//small cube hits wall
		if (x1 < 0)
		{
			x1 = 0;
			v1 *= -1;
		}

		//big cube hits small one
		if (x2 - x1 <= w1)
		{
			x2 = x1+w1;
			v2 *= -1;
			v1 *= -1;
		}


		//Drawing--------------
		Clear(olc::BLACK);
		//std::to_string(vCursor.x)
		DrawString(10, 10, "Hits", olc::WHITE);
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