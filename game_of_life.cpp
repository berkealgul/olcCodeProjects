#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;


class Sketch : public olc::PixelGameEngine
{
public:

	vector<bool> _cells, _cells_buffer;
	float update_interval = 0.5;
	float time_to_update;
	int grid_W = 50;
	int cell_W;
	bool use_buffer;

	Sketch()
	{
		sAppName = "Game of Life";
	}


	bool OnUserCreate() override
	{
		srand(time(nullptr));
		cell_W = ScreenWidth() / grid_W;
		use_buffer = false;
		_cells = vector<bool>(grid_W * grid_W);
		_cells_buffer = vector<bool>(grid_W * grid_W);
		time_to_update = update_interval;
		// Random or custom initialization
		//initialize_random_state(_cells);
		initialize_spacehip(_cells);
		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		time_to_update -= dt;

		if (time_to_update <= 0)
		{
			time_to_update = update_interval;

			if (!use_buffer)
			{
				update_state(_cells, _cells_buffer);
			}
			else
			{
				update_state(_cells_buffer, _cells);
			}

			use_buffer = !use_buffer;
		}

		if (!use_buffer)
		{
			draw_state(_cells);
		}
		else
		{
			draw_state(_cells_buffer);
		}

		return true;
	}

	void update_state(const vector<bool>& cells_read, vector<bool>& cells_write)
	{
		for (int i = 0; i < grid_W; i++)
			for (int j = 0; j < grid_W; j++)
			{
				if (i == 6 && j == 5)
					int x = 0;
				int nNeighbor = get_live_neighbor_count(i, j, cells_read);
				if (nNeighbor < 2) { cells_write[idx(i, j)] = 0; }
				else if (nNeighbor > 3) { cells_write[idx(i, j)] = 0; }
				else if (nNeighbor == 3) { cells_write[idx(i, j)] = 1; }
				else { cells_write[idx(i, j)] = cells_read[idx(i, j)]; }
			}
	}

	void draw_state(const vector<bool>& cells)
	{
		for (int i = 0; i < grid_W; i++)
			for (int j = 0; j < grid_W; j++)
			{
				olc::Pixel p = olc::WHITE;
				if (cells[idx(i, j)]) { p = olc::BLACK; }
				FillRect(cell_W * i, cell_W * j, cell_W, cell_W, p);
			}
	}

	void initialize_random_state(vector<bool>& cells)
	{
		for (int i = 0; i < grid_W; i++)
			for (int j = 0; j < grid_W; j++)
			{
				if (random(10) > 5)
					cells[idx(i, j)] = 1;
				else
					cells[idx(i, j)] = 0;
			}
	}

	void initialize_spacehip(vector<bool>& cells)
	{
		cells[idx(5, 5)] = 1;
		cells[idx(6, 6)] = 1;
		cells[idx(6, 7)] = 1;
		cells[idx(7, 5)] = 1;
		cells[idx(7, 6)] = 1;
	}

	int get_live_neighbor_count(int x, int y, const vector<bool>& cells)
	{
		int count = 0;

		for(int i = x-1; i < x+2; i++)
			for (int j = y-1; j < y+2; j++)
			{
				if(i == x && j == y) { continue; }
				if (i < 0 || i == grid_W || j < 0 || j == grid_W) { continue; }
				if (cells[idx(i, j)]) { count++; }
			}

		return count;
	}

	inline int idx(int i, int j)
	{
		return i * grid_W + j;
	}

	inline int random(int max)
	{
		return rand() / ((RAND_MAX + 1u) / max);
	}
};


int main()
{
	Sketch sketch;
	if (sketch.Construct(500, 500, 2, 2))
		sketch.Start();

	return 0;
}