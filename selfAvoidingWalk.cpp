#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>
#include <random>
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       

using namespace std;


class Sketch : public olc::PixelGameEngine
{
public:

	Sketch()
	{
		sAppName = "Self Avoiding Walk";
	}


	//directions and cell status
	static const int LEFT = 1;
	static const int UP = 2;
	static const int RIGHT = 3;
	static const int DOWN = 4;
	static const int UNVISITED = 0;
	static const int ORIGIN = -1;

	struct Cell
	{
		int parentDir = UNVISITED;
		vector<int> neighborDirs;
	};

	const int ox = 0;
	const int oy = 0;
	int w = 50;
	int h = 50;
	int x;
	int y;
	int iters = 0;


	vector<Cell> grid;


	bool OnUserCreate() override
	{
		x = ox;
		y = oy;
		grid = vector<Cell>(w * h);
		cell(ox, oy).parentDir = ORIGIN;

		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		//get spaces to walk
		if (x > 0)
		{
			if (cell(x - 1, y).parentDir == UNVISITED)
			{
				cell(x, y).neighborDirs.push_back(LEFT);
			}
		}
		if (x < w - 1)
		{
			if (cell(x + 1, y).parentDir == UNVISITED)
			{
				cell(x, y).neighborDirs.push_back(RIGHT);
			}
		}
		if (y > 0)
		{
			if (cell(x, y - 1).parentDir == UNVISITED)
			{
				cell(x, y).neighborDirs.push_back(UP);
			}
		}
		if (y < h - 1)
		{
			if (cell(x, y + 1).parentDir == UNVISITED)
			{
				cell(x, y).neighborDirs.push_back(DOWN);
			}
		}

		

		if (cell(x, y).neighborDirs.size() != 0)
		{
			srand(time(NULL));
			int pos = rand() % cell(x, y).neighborDirs.size();
			int dir = cell(x, y).neighborDirs[pos];
			
			switch (dir)
			{
			case DOWN:
				y++;
				cell(x, y).parentDir = UP;
				break;
			case UP:
				y--;
				cell(x, y).parentDir = DOWN;
				break;
			case RIGHT:
				x++;
				cell(x, y).parentDir = LEFT;
				break;
			case LEFT:
				x--;
				cell(x, y).parentDir = RIGHT;
				break;
			}

			iters++;

			cout << "Iters: " << iters << endl;
		}
		else
		{
			cout << "Stuck in " << iters << " iters " << endl;
		}
		
		//Drawing--------------
		Clear(olc::BLACK);
		//path
		int dx = x;
		int dy = y;
		int pdx, pdy;
		while (dx != ox || dy != oy)
		{
			pdx = dx;
			pdy = dy;

			cout << dx << " " << dy << " " << cell(dx, dy).parentDir << endl;

			switch (cell(dx, dy).parentDir)
			{
			case DOWN:
				dy++;
				break;
			case UP:
				dy--;
				break;
			case RIGHT:
				dx++;
				break;
			case LEFT:
				dx--;
				break;
			}

			DrawLine(pdx*2+1, pdy*2+1, dx*2+1, dy*2+1);
			
		}


		return true;
	}

	Cell &cell(int i, int j)
	{
		return grid[i + w * j];
	}

};


int main()
{
	Sketch sketch;
	if (sketch.Construct(101, 101, 4, 4))
		sketch.Start();

	return 0;
}
