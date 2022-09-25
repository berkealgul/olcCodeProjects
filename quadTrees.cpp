#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <time.h>
#include <memory>
#include <vector>

using namespace std;
using namespace olc;


struct Ball
{
	static const int r = 3;

	float x;
	float y;
	float vx;
	float vy;

	Ball(float x_, float y_)
	{
		x = x_;
		y = y_;
		vx = (float)rand() / RAND_MAX;
		vy = (float)rand() / RAND_MAX;
		//vx = (vx - 0.5) * 2;
		//vy = (vx - 0.5) * 2;
	}
};


class QuadTree
{
public:

	const static int MAX_DEPTH = 3;

	int cx;
	int cy;
	int w;
	int h;
	int depth;

	unique_ptr<QuadTree> ul_tree, ur_tree, lr_tree, ll_tree;
	vector<shared_ptr<Ball>> balls;

	QuadTree(int cx_, int cy_, int w_, int h_, int depth_)
	{
		cx = cx_;
		cy = cy_;
		w = w_;
		h = h_;
		depth = depth_;
	}

	void insert_ball(shared_ptr<Ball> ball)
	{
		if (depth >= MAX_DEPTH)
		{
			balls.push_back(ball);
			return;
		}

		if (ball->x < cx && ball->y < cy) // upper left
		{
			if (!ul_tree.get()) // it is null
			{
				ul_tree = unique_ptr<QuadTree>(new QuadTree(cx / 2, cy / 2, w / 2, h / 2, depth + 1));
			}
		
			ul_tree->insert_ball(ball);
		}
		else if (ball->x > cx && ball->y < cy) // upper right
		{
			if (!ur_tree.get()) // it is null
			{
				ur_tree = unique_ptr<QuadTree>(new QuadTree(cx * 1.5, cy / 2, w / 2, h / 2, depth + 1));
			}
		
			ur_tree->insert_ball(ball);
		}
		else if (ball->x > cx && ball->y > cy) // lower right
		{
			if (!lr_tree.get()) // it is null
			{
				lr_tree = unique_ptr<QuadTree>(new QuadTree(cx * 1.5, cy * 1.5, w / 2, h / 2, depth + 1));
			}
		
			lr_tree->insert_ball(ball);
		}
		else // lower left
		{
			if (!ll_tree.get()) // it is null
			{
				ll_tree = unique_ptr<QuadTree>(new QuadTree(cx / 2, cy  * 1.5, w / 2, h / 2, depth + 1));
			}
		
			ll_tree->insert_ball(ball);
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

	vector<shared_ptr<Ball>> balls;
	unique_ptr<QuadTree> t;

	bool OnUserCreate() override
	{
		const int N_BALLS = 1;

		t = unique_ptr<QuadTree>(new QuadTree(128, 120, 256, 240, 1));

		for (int i = 0; i < N_BALLS; i++)
		{
			float x = ((float)rand() / RAND_MAX) * ScreenWidth();
			float y = ((float)rand() / RAND_MAX) * ScreenHeight();
			auto b = shared_ptr<Ball>(new Ball(x, y));
			balls.push_back(b);
			t->insert_ball(b);
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::DARK_BLUE);

		

		for (auto const &b : balls)
		{
			float s = 0;
			b->x += b->vx * s * fElapsedTime;
			b->y += b->vy * s * fElapsedTime;
			FillCircle(b->x, b->y, b->r, olc::WHITE);
		}

		//cout << b.x << " " << b.y << "\n";
		DrawQuadTree(t);
		return true;
	}

	void DrawQuadTree(unique_ptr<QuadTree> &t)
	{
		if (!t.get()) { return; }

		DrawLine(t->cx, t->cy - t->h / 2, t->cx, t->cy + t->h / 2, olc::YELLOW);
		DrawLine(t->cx - t->w / 2, t->cy, t->cx + t->w / 2, t->cy, olc::YELLOW);

		DrawQuadTree(t->ul_tree);
		DrawQuadTree(t->ur_tree);
		DrawQuadTree(t->ll_tree);
		DrawQuadTree(t->lr_tree);
	}

};


int main()
{
	srand((unsigned)time(NULL));

	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();

	return 0;
}
