#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>

using namespace std;

class FluidGrid
{
	int W, H;
	vector<float> x0, x, v0, v;

	FluidGrid(int w , int h)
	{
		W = w;
		H = h;
		x0.reserve(W * H);
		v0.reserve(W * H);
		x.reserve(W * H);
		v.reserve(W * H);
	}

	float operator () (int x, int y)
	{
		int i = y*W + x;
		//return grid[i]; 
	}
	
};

class Example : public olc::PixelGameEngine
{
public:

	Example()
	{
		sAppName = "QuadTrees";
	}

	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		return true;
	}
	

};


int main()
{
	Example demo;
	if (demo.Construct(1024, 1024, 1, 1))
		demo.Start();

	return 0;
}