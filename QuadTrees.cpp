#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>

using namespace std;


struct Ball
{
	int x, y, w, h;

	Ball()
	{
		
	}
};


class QuadTree
{
public:
	static const int MAX_DEPTH = 5;

	int x, y; // middle point of quadtree
	int w, h; // half width-height of the boundaries
	int depth;
	QuadTree** leafs;
	vector<Ball*> balls;

	QuadTree(int x, int y, int w, int h, int depth)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		this->depth = depth;
	}

	void addBall(Ball* ball)
	{
		if (depth < QuadTree::MAX_DEPTH)
		{
			if (!leafs) { leafs = new QuadTree * [4]; }

			int idx = getReigonIdx(ball);

			if (!leafs[idx]) { leafs[idx] = createTree(idx); }

			leafs[idx]->addBall(ball);
		}
		else
			balls.push_back(ball);
	}

	QuadTree* createTree(int idx)
	{
		int w_ = w / 2;
		int h_ = h / 2;

		switch (idx)
		{
		case 0:
			return new QuadTree(x-w_, y-h_, w_, h_, depth + 1);
		case 1:
			return new QuadTree(x + w_, y - h_, w_, h_, depth + 1);
		case 2:
			return new QuadTree(x - w_, y + h_, w_, h_, depth + 1);
		case 3:
			return new QuadTree(x + w_, y+- h_, w_, h_, depth + 1);
		}
	}

	int getReigonIdx(Ball* ball)
	{
		int id = 0;

		if (ball->x > x) { id++; }
		if (ball->y > y) { id+=2;}

		return id;
	}

};

class Example : public olc::PixelGameEngine
{
public:
	QuadTree* tree;
	vector<Ball> balls;

	Example()
	{
		sAppName = "QuadTrees";

		generateBalls(10);
	}

	bool OnUserCreate() override
	{

		return true;;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::DARK_BLUE);
		int w = ScreenWidth() / 2;
		int h = ScreenHeight() / 2;
		tree = new QuadTree(w, h, w, h, 0);


		for (Ball b : balls)
			tree->addBall(&b);

		delete tree;

		return true;
	}

	void generateBalls(int n)
	{
		for (int i = 0; i < n; i++)
		{
			balls.push_back(Ball());
		}
	}

};


int main()
{
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}
