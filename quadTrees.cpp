#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <time.h>
#include <memory>
#include <vector>

using namespace std;
using namespace olc;


struct Ball
{
	const int r = 4;
	const float s = 25.0f;

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
		vx = (vx - 0.5) * 2;
		vy = (vx - 0.5) * 2;
	}

	bool CollidesWith(const shared_ptr<Ball>& b)
	{
		float d = powf((x - b->x), 2) + powf((y - b->y), 2);
		return d <= powf(2 * r, 2);
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

	shared_ptr<QuadTree> ul_tree, ur_tree, lr_tree, ll_tree;
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
		if (depth > MAX_DEPTH)
		{
			balls.push_back(ball);
			return;
		}

		if (ball->x < cx && ball->y < cy) // upper left
		{
			if (!ul_tree.get()) // it is null
			{
				int w_ = w / 2;
				int h_ = h / 2;
				int cx_ = cx - (w_ / 2);
				int cy_ = cy - (h_ / 2);
				ul_tree = shared_ptr<QuadTree>(new QuadTree(cx_, cy_, w_, h_, depth + 1));
			}
		
			ul_tree->insert_ball(ball);
		}
		else if (ball->x > cx && ball->y < cy) // upper right
		{
			if (!ur_tree.get()) // it is null
			{
				int w_ = w / 2;
				int h_ = h / 2;
				int cx_ = cx + (w_ / 2);
				int cy_ = cy - (h_ / 2);
				ur_tree = shared_ptr<QuadTree>(new QuadTree(cx_, cy_, w_, h_, depth + 1));
			}
		
			ur_tree->insert_ball(ball);
		}
		else if (ball->x > cx && ball->y > cy) // lower right
		{
			if (!lr_tree.get()) // it is null
			{
				int w_ = w / 2;
				int h_ = h / 2;
				int cx_ = cx + (w_ / 2);
				int cy_ = cy + (h_ / 2);
				lr_tree = shared_ptr<QuadTree>(new QuadTree(cx_, cy_, w_, h_, depth + 1));
			}
		
			lr_tree->insert_ball(ball);
		}
		else // lower left
		{
			if (!ll_tree.get()) // it is null
			{
				int w_ = w / 2;
				int h_ = h / 2;
				int cx_ = cx - (w_ / 2);
				int cy_ = cy + (h_ / 2);
				ll_tree = shared_ptr<QuadTree>(new QuadTree(cx_, cy_, w_, h_, depth + 1));
			}
		
			ll_tree->insert_ball(ball);
		}
	
	}

	static vector<shared_ptr<Ball>> GetBallsToCheck(shared_ptr<QuadTree> head_tree, shared_ptr<Ball> const& ball)
	{
		shared_ptr<QuadTree> host_tree = FindTreeOfBall(head_tree, ball);

		vector<shared_ptr<Ball>> target_balls = host_tree->balls;

		if (host_tree->ul_tree.get())
		{
			target_balls.insert(target_balls.end(), host_tree->ul_tree->balls.begin(), host_tree->ul_tree->balls.end());
		}
		if (host_tree->ur_tree.get())
		{
			target_balls.insert(target_balls.end(), host_tree->ur_tree->balls.begin(), host_tree->ur_tree->balls.end());
		}
		if (host_tree->ll_tree.get())
		{
			target_balls.insert(target_balls.end(), host_tree->ll_tree->balls.begin(), host_tree->ll_tree->balls.end());
		}
		if (host_tree->lr_tree.get())
		{
			target_balls.insert(target_balls.end(), host_tree->lr_tree->balls.begin(), host_tree->lr_tree->balls.end());
		}

		return target_balls;
	}

	static shared_ptr<QuadTree> FindTreeOfBall(shared_ptr<QuadTree> tree,  shared_ptr<Ball> const& ball)
	{
		if (!tree.get()) { return NULL; }

		for (auto b : tree->balls)
		{
			if (b == ball) { return tree; }
		}

		if (ball->x < tree->cx && ball->y < tree->cy) // upper left
		{
			return FindTreeOfBall(tree->ul_tree, ball);
		}
		else if (ball->x > tree->cx && ball->y < tree->cy) // upper right
		{
			return FindTreeOfBall(tree->ur_tree, ball);
		}
		else if (ball->x > tree->cx && ball->y > tree->cy) // lower right
		{
			return FindTreeOfBall(tree->lr_tree, ball);
		}
		else // lower left
		{
			return FindTreeOfBall(tree->ll_tree, ball);
		}

		return NULL;
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
	shared_ptr<QuadTree> t;

	bool OnUserCreate() override
	{
		const int N_BALLS = 100;

		for (int i = 0; i < N_BALLS; i++)
		{
			float x = ((float)rand() / RAND_MAX) * ScreenWidth();
			float y = ((float)rand() / RAND_MAX) * ScreenHeight();
			auto b = shared_ptr<Ball>(new Ball(x, y));
			balls.push_back(b);
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::DARK_BLUE);

		t = shared_ptr<QuadTree>(new QuadTree(ScreenWidth() / 2, ScreenHeight() / 2, ScreenWidth(), ScreenHeight(), 0));

		for (auto const &b : balls)
		{
			b->x += b->vx * b->s * fElapsedTime;
			b->y += b->vy * b->s * fElapsedTime;
			
			if (b->x < 0 || b->x > ScreenWidth())
			{
				b->vx *= -1;
			}

			if (b->y < 0 || b->y > ScreenHeight())
			{
				b->vy *= -1;
			}

			FillCircle(b->x, b->y, b->r, olc::WHITE);
			t->insert_ball(b);
		}

		DrawQuadTree(t);

		//int checks = UpdateCollisionsBF();
		int checks = UpdateCollisions();

		return true;
	}

	void DrawQuadTree(shared_ptr<QuadTree> &t)
	{
		if (!t.get()) { return; }

		DrawLine(t->cx, t->cy - t->h / 2, t->cx, t->cy + t->h / 2, olc::YELLOW);
		DrawLine(t->cx - t->w / 2, t->cy, t->cx + t->w / 2, t->cy, olc::YELLOW);

		DrawQuadTree(t->ul_tree);
		DrawQuadTree(t->ur_tree);
		DrawQuadTree(t->ll_tree);
		DrawQuadTree(t->lr_tree);
	}

	int UpdateCollisionsBF()
	{
		int checks = 0;

		for (auto const& b : balls)
		{
			for (auto const& b_ : balls)
			{
				if (b == b_) { continue; }
				checks++;
				if (!b->CollidesWith(b_)) { continue; }
				b->vx *= -1;
				b->vy *= -1;
				break;
			}
		}

		return checks;
	}

	int UpdateCollisions()
	{
		int checks = 0;

		for (auto b : balls)
		{
			auto balls_to_check = QuadTree::GetBallsToCheck(t, b);

			for (auto b_ : balls_to_check)
			{
				if (b == b_) { continue; }
				checks++;
				if (!b->CollidesWith(b_)) { continue; }
				b->vx *= -1;
				b->vy *= -1;
				break;
			}
		}

		return checks;
	}

};


int main()
{
	srand((unsigned)time(NULL));

	Example demo;
	if (demo.Construct(1024, 1024, 1, 1))
		demo.Start();

	return 0;
}
