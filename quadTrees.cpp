#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <memory>
#include <vector>

using namespace std;


struct Ball
{
	int x;
	int y;
	int r;

	Ball(int x_, int y_, int z_)
	{
		x = x_;
		y = y_;
		r = z_;
	}
};


class QuadTree
{
public:
	
	const static int MAX_DEPTH = 5;

	int cx;
	int cy;
	int w;
	int h;
	int depth;

	unique_ptr<QuadTree> ul_leaf, ur_leaf, lr_leaf, ll_leaf;
	vector<unique_ptr<Ball>> balls;

	QuadTree(int cx_, int cy_, int w_, int h_, int depth_)
	{
		cx = cx_;
		cy = cy_;
		w = w_;
		h = h_;
		depth = depth_;
	}

	void insert_ball(unique_ptr<Ball> ball)
	{
		
		if (ball->x < cx && ball->y < cy) // upper left
		{
			if (!ul_leaf.get()) // it is null
			{

			}
			else
			{

			}
		}
		else if (ball->x > cx && ball->y < cy) // upper right
		{

		}
		else if (ball->x > cx && ball->y > cy) // lower right
		{

		}
		else // lower left
		{

		}

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

		return true;;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::DARK_BLUE);

		return true;
	}


};


int main()
{
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}
