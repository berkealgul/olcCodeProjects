#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>

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

	struct Cell
	{
		int parentDir = UNVISITED; 
		vector<int> neighborDirs;
	};

	const int ox = 0;
	const int oy = 0;
	int w = 10;
	int h = 10;
	int x;
	int y;


	vector<Cell> grid;


	bool OnUserCreate() override
	{
		x = ox;
		y = oy;
		grid = vector<Cell>(w*h);

		return true;
	}
	 
	bool OnUserUpdate(float dt) override
	{
		Cell cell = grid[IX(x, y)];

		//get spaces to walk
		if (x > 0)
		{
			if (grid[IX(x - 1, y)].parentDir == UNVISITED)
			{
				cell.neighborDirs.push_back(LEFT);
			}
		}
		if (x < w-1)
		{
			if (grid[IX(x + 1, y)].parentDir == UNVISITED)
			{
				cell.neighborDirs.push_back(RIGHT);
			}
		}
		if (y > 0)
		{
			if (grid[IX(x, y - 1)].parentDir == UNVISITED)
			{
				cell.neighborDirs.push_back(UP);
			}
		}
		if (y > 0)
		{
			if (grid[IX(x, y + 1)].parentDir == UNVISITED)
			{
				cell.neighborDirs.push_back(DOWN);
			}
		}

		//Drawing--------------
		Clear(olc::BLACK);
		//path
		int dx = x;
		int dy = y;
		while (dx != ox || dy != oy)
		{
			switch (grid[IX(x, y)])
			{
			case PARENT_BELOW: 
				dy++;
				break;
			case PARENT_ABOVE:
				dy--;
			case PARENT_RIGHT:
				dx++;
				break;
			case PARENT_LEFT:
				dx--;
				break;
			}

			DrawLine(dx, dy, x, y);
		}
		

		return true;
	}

	vector<int> getSpaceToWalk(int x, int y)
	{
		vector<int> space;

		if()
	}

	int IX(int i, int j)
	{
		return i + ScreenWidth() * j;
	}
	
};


int main()
{
	Sketch sketch;
	if (sketch.Construct(10, 10, 50, 50))
		sketch.Start();

	return 0;
}
