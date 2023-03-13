#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <vector>  

using namespace olc;


class DDA_Demo : public olc::PixelGameEngine
{
public:
	const int w = 20;
	const int h = 20;


	const float cell_edit_timeout = 0.5f;
	const float player_speed = 10.0f;

	float time_since_last_cell_edit = 0;

	vi2d grid_scale;
	vf2d player_pos;

	int last_edited_cell_i = -1;
	int last_edited_cell_j = -1;

	std::vector<std::vector<int>> field;


	bool OnUserCreate() override
	{
		field = std::vector<std::vector<int>>(w, std::vector<int>(h));
		grid_scale.x = ScreenWidth() / w;
		grid_scale.y = ScreenHeight() / h;
		player_pos.x = (float)w / 2;
		player_pos.y = (float)h / 2;

		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		DrawGrid();
		UpdateGrid(dt);
		UpdatePlayer(dt);

		olc::vf2d dir = (olc::vf2d(GetMouseX(), GetMouseY()) - (player_pos * grid_scale)).norm();

		std::vector<vf2d> checkpoints;
 		float distance = RunDDA(dir, checkpoints);

		std::cout << distance << std::endl;

		DrawDDARay(checkpoints);

		return true;
	}

	void DrawDDARay(std::vector<olc::vf2d> checkpoints)
	{
		DrawLine(checkpoints[0], checkpoints[checkpoints.size() - 1]);
		FillCircle(checkpoints[0], 1, olc::RED);
		FillCircle(checkpoints[checkpoints.size() - 1], 1, olc::RED);
		//for (int i = 0; i < checkpoints.size(); i++)
		//{
		//	FillCircle(checkpoints[i], 1, olc::RED);
		//	//FillRect(checkpoints[i].x, checkpoints[i].y, grid_scale.x, grid_scale.y, olc::GREEN);
		//}
	}

	void UpdatePlayer(float dt)
	{
		if (GetKey(olc::Key::W).bHeld) player_pos.y -= player_speed * dt;
		if (GetKey(olc::Key::S).bHeld) player_pos.y += player_speed * dt;
		if (GetKey(olc::Key::D).bHeld) player_pos.x += player_speed * dt;
		if (GetKey(olc::Key::A).bHeld) player_pos.x -= player_speed * dt;
	}

	void UpdateGrid(float dt)
	{
		if (GetKey(olc::Key::R).bPressed)
		{
			field = std::vector<std::vector<int>>(w, std::vector<int>(h)); // restart map
		} 

		time_since_last_cell_edit += dt;

		if (GetMouse(0).bHeld )
		{
			int i = GetMouseX() / grid_scale.x;
			int j = GetMouseY() / grid_scale.y;

			if ((i == last_edited_cell_i && j == last_edited_cell_j) && time_since_last_cell_edit < cell_edit_timeout) { return; }

			field[i][j] = field[i][j] == 1 ? 0 : 1;
			last_edited_cell_i = i;
			last_edited_cell_j = j;

			time_since_last_cell_edit = 0;
		}
	}

	void DrawGrid()
	{
		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				olc::Pixel p;

				if (field[i][j] == 0)
				{
					p = olc::DARK_BLUE;
				}
				else
				{
					p = olc::DARK_RED;
				}
				FillRect(i * grid_scale.x, j * grid_scale.y, grid_scale.x, grid_scale.y, p);
				DrawRect(i * grid_scale.x, j * grid_scale.y, grid_scale.x, grid_scale.y, olc::YELLOW);
			}
		}
	}

	float RunDDA(olc::vf2d dir, std::vector<vf2d>& Checkpoints)
	{
		float distance = 0;
		float max_distance = 100;

		bool hit = false;

		olc::vf2d RayUnitDistance = { sqrt(1 + (dir.y / dir.x) * (dir.y / dir.x)), sqrt(1 + (dir.x / dir.y) * (dir.x / dir.y)) };
		olc::vf2d RayTravelDistance;
		olc::vi2d MapCheck(player_pos);
		olc::vf2d Step;

		Checkpoints.push_back(player_pos * grid_scale);

		if (dir.x < 0)
		{
			Step.x = -1;
			RayTravelDistance.x = (player_pos.x - float(MapCheck.x)) * RayUnitDistance.x;
		}
		else
		{
			Step.x = 1;
			RayTravelDistance.x = (float(MapCheck.x + 1) - player_pos.x) * RayUnitDistance.x;
		}

		if (dir.y < 0)
		{
			Step.y = -1;
			RayTravelDistance.y = (player_pos.y - float(MapCheck.y)) * RayUnitDistance.y;
		}
		else
		{
			Step.y = 1;
			RayTravelDistance.y = (float(MapCheck.y + 1) - player_pos.y) * RayUnitDistance.y;
		}

		while (distance < max_distance && !hit)
		{
			if (RayTravelDistance.x <= RayTravelDistance.y)
			{
				MapCheck.x += Step.x;
				distance = RayTravelDistance.x;
				RayTravelDistance.x += RayUnitDistance.x;
			}
			else
			{
				MapCheck.y += Step.y;
				distance = RayTravelDistance.y;
				RayTravelDistance.y += RayUnitDistance.y;
			}

			Checkpoints.push_back(MapCheck * grid_scale);

			if (MapCheck.x < 0 || MapCheck.x > w - 1 || MapCheck.y < 0 || MapCheck.y > h - 1)
			{
				break;
			}

			if (field[MapCheck.x][MapCheck.y] != 0)
			{
				hit = true;
			}
		}

		Checkpoints.push_back((player_pos + dir * distance) * grid_scale);

		return distance;
	}

};


int main()
{
	DDA_Demo demo;
	if (demo.Construct(201, 201, 4, 4))
		demo.Start();

	return 0;
}
