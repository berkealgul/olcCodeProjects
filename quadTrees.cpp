#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <memory>
#include <vector>

using namespace std;
using namespace olc;


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

	void draw()
	{
		olc::DrawLine(cx , cy - h/2, cx, cy + h / 2, olc::YELLOW);
		olc::DrawLine(cx - w / 2, cy, cx + w / 2, cy, olc::YELLOW);
	}

	void insert_ball(unique_ptr<Ball> &ball)
	{
		if (depth >= MAX_DEPTH)
		{
			balls.push_back(ball);
			return;
		}

		if (ball->x < cx && ball->y < cy) // upper left
		{
			if (!ul_leaf.get()) // it is null
			{
				ul_leaf = unique_ptr<QuadTree>(new QuadTree(cx / 2, cy / 2, w / 2, h / 2, depth + 1));
			}

			ul_leaf->insert_ball(ball);
		}
		else if (ball->x > cx && ball->y < cy) // upper right
		{
			if (!ur_leaf.get()) // it is null
			{
				ur_leaf = unique_ptr<QuadTree>(new QuadTree(cx * 1.5, cy / 2, w / 2, h / 2, depth + 1));
			}

			ur_leaf->insert_ball(ball);
		}
		else if (ball->x > cx && ball->y > cy) // lower right
		{
			if (!lr_leaf.get()) // it is null
			{
				lr_leaf = unique_ptr<QuadTree>(new QuadTree(cx * 1.5, cy * 1.5, w / 2, h / 2, depth + 1));
			}

			lr_leaf->insert_ball(ball);
		}
		else // lower left
		{
			if (!ll_leaf.get()) // it is null
			{
				ll_leaf = unique_ptr<QuadTree>(new QuadTree(cx / 2, cy  * 1.5, w / 2, h / 2, depth + 1));
			}

			ll_leaf->insert_ball(ball);
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
