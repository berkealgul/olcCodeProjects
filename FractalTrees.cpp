#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <time.h>
#include <vector>

#define PI 3.14

class Example : public olc::PixelGameEngine
{
public:

	const int LOWEST_SIZE = 5;
	const double BRANCH_COUNT = 4.0;
	const double BRANCH_ANGLE = 2 * PI / 3;
	const double BRANCH_SIZE_DECAY = 1.85;

	int cx, cy;

	Example()
	{
		srand((unsigned)time(NULL));
		sAppName = "FractalTrees";
	}

	bool OnUserCreate()
	{
		cx = ScreenWidth() / 2;
		cy = ScreenHeight() - ScreenHeight() / 3;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) 
	{
		Clear(olc::GREY);
		DrawLine(cx, cy, cx, ScreenHeight(), olc::VERY_DARK_GREY);
		drawTree(cx, cy, 150, -PI/2);
		return true;
	}

	void drawTree(int x, int y, int size, double angle)
	{
		if (size < LOWEST_SIZE) { return; }

		for (double i = 0; i < BRANCH_COUNT; i++)
		{
			double a = angle + (PI/4) - (i * BRANCH_ANGLE / BRANCH_COUNT);
			int nx = x + size * cos(a);
			int ny = y + size * sin(a);
			DrawLine(x, y, nx, ny, olc::VERY_DARK_GREY);
			drawTree(nx, ny, size / BRANCH_SIZE_DECAY, a);
		}
	}

};


int main()
{
	Example demo;
	if (demo.Construct(600, 600, 1, 1))
		demo.Start();

	return 0;
}