#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <iostream>
#include <vector>  

using namespace olc;

/*
* Berke Algül 13.03.2023
* 
* Controls:
*  W-S : move forward-nackward
*  A-D : rotate
*  Q-E : adjust FOV
*/


struct DDA_Result
{
	vi2d hit_cell;
	float distance;
	int side;
};


class RayCastWorld : public olc::PixelGameEngine
{
public:
	const float player_movement_speed = 5.0f;
	const float player_rotation_speed = 2.0f;

	float fov = 1.7; 
	const float fov_speed = 0.1;

	vf2d player_pos;
	vf2d player_dir = { -1, 0 };
	vf2d plane = { 0, 0.66 };

	float player_angle = 0;

	//std::vector<std::vector<int>> field;
	//vi2d field_size = { 20, 20 };

	vf2d grid_scale;

	vi2d field_size = { 24, 24 };
	int field[24][24] =
	{
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
	  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

	bool OnUserCreate() override
	{
		player_pos = field_size / 2;
		grid_scale = { float(ScreenWidth()) / field_size.x, float(ScreenHeight()) / field_size.y };
		return true;
	}

	bool OnUserUpdate(float dt) override
	{
		Render();
		//RenderMap();
		UpdatePlayer(dt);

		return true;
	}

	void UpdatePlayer(float dt)
	{
		if (GetKey(olc::Key::W).bHeld) 
		{
			if (field[int(player_pos.x + player_dir.x * player_movement_speed * dt)][int(player_pos.y)] == 0)
			{
				player_pos.x += player_dir.x * player_movement_speed * dt;
			}
			if (field[int(player_pos.x)][int(player_pos.y + player_dir.y * player_movement_speed * dt)] == 0)
			{
				player_pos.y += player_dir.y * player_movement_speed * dt;
			}
		} 
		if (GetKey(olc::Key::S).bHeld)
		{
			if (field[int(player_pos.x - player_dir.x * player_movement_speed * dt)][int(player_pos.y)] == 0)
			{
				player_pos.x -= player_dir.x * player_movement_speed * dt;
			}
			if (field[int(player_pos.x)][int(player_pos.y - player_dir.y * player_movement_speed * dt)] == 0)
			{
				player_pos.y -= player_dir.y * player_movement_speed * dt;
			}
		}
		
		if (GetKey(olc::Key::D).bHeld)
		{
			plane      = RotateVector(-player_rotation_speed * dt, plane);
			player_dir = RotateVector(-player_rotation_speed * dt, player_dir);
		}
		if (GetKey(olc::Key::A).bHeld)
		{
			plane	   = RotateVector(player_rotation_speed * dt, plane);
			player_dir = RotateVector(player_rotation_speed * dt, player_dir);
		}

		if (GetKey(olc::Key::E).bHeld)
		{
			fov += fov_speed * dt;
		}
		if (GetKey(olc::Key::Q).bHeld)
		{
			fov -= fov_speed * dt;
		}
	}

	void RenderMap()
	{
		DrawGrid();

		for (int i = 0; i < ScreenWidth(); i++)
		{
			float x = ((2 * i) / float(ScreenWidth())) - 1;
			auto ray_dir = player_dir / tan(fov / 2) + plane * x;

			DDA_Result result = RunDDA(ray_dir);

			DrawDDARay(ray_dir, result.distance);
		}
	}

	void Render()
	{
		Clear(olc::BLACK);

		for (int i = 0; i < ScreenWidth(); i++)
		{
			auto player_dir_scaled = player_dir / tan(fov / 2);
			float x = ((2 * i) / float(ScreenWidth())) - 1;
			auto ray_dir = player_dir_scaled + plane * x;

			DDA_Result result = RunDDA(ray_dir);
			
			float p_dist = (result.distance / ray_dir.mag() - 1) * player_dir_scaled.mag();

			float h = (ScreenHeight()*2) / p_dist;
			Pixel p = GetColorOfCell(result.hit_cell, result.side);

			FillRect(i, ScreenHeight() / 2 - h / 2, 1, h, p);
		}
	}

	void DrawGrid()
	{
		for (int i = 0; i < field_size.x; i++)
		{
			for (int j = 0; j < field_size.y; j++)
			{
				Pixel p = GetColorOfCell(vi2d(i, j), 0);

				FillRect(i * grid_scale.x, j * grid_scale.y, grid_scale.x, grid_scale.y, p);
				DrawRect(i * grid_scale.x, j * grid_scale.y, grid_scale.x, grid_scale.y, olc::BLACK);
			}
		}
	}

	void DrawDDARay(vf2d dir, float dis)
	{
		DrawLine(player_pos*grid_scale, (player_pos + dir * dis) * grid_scale, BLACK);
		FillCircle(player_pos * grid_scale, 4, olc::RED);
		FillCircle((player_pos + dir * dis) * grid_scale, 4, olc::RED);
	}

	vf2d RotateVector(float angle, vf2d v)
	{
		vf2d u;
		u.x = v.x * cosf(angle) - v.y * sinf(angle);
		u.y = v.x * sinf(angle) + v.y * cosf(angle);
		return u;
	}

	Pixel GetColorOfCell(vi2d cell_pos, int side)
	{
		Pixel p;

		switch (field[cell_pos.x][cell_pos.y])
		{
			case 1:  p = RED;    break; //red
			case 2:  p = GREEN;  break; //green
			case 3:  p = BLUE;   break; //blue
			case 4:  p = WHITE;  break; //white
			case 0:  p = GREY;  break; //white
			default: p = YELLOW; break; //yellow
		}

		if (side == 1) p /= 2;

		return p;
	}

	DDA_Result RunDDA(olc::vf2d dir)
	{
		float distance = 0;
		float max_distance = 100;
		int side = 0;

		bool hit = false;

		olc::vf2d RayUnitDistance = { sqrt(1 + (dir.y / dir.x) * (dir.y / dir.x)), sqrt(1 + (dir.x / dir.y) * (dir.x / dir.y)) };
		olc::vf2d RayTravelDistance;
		olc::vi2d MapCheck(player_pos);
		olc::vf2d Step;

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
				side = 0;
			}
			else
			{
				MapCheck.y += Step.y;
				distance = RayTravelDistance.y;
				RayTravelDistance.y += RayUnitDistance.y;
				side = 1;
			}

			if (MapCheck.x < 0 || MapCheck.x > field_size.x - 1 || MapCheck.y < 0 || MapCheck.y > field_size.y - 1)
			{
				break;
			}

			if (field[MapCheck.x][MapCheck.y] != 0)
			{
				hit = true;
			}
		}

		DDA_Result result;
		result.distance = distance; 
		result.hit_cell = MapCheck;
		result.side = side;

		return result;
	}
};


int main()
{
	RayCastWorld world;
	if (world.Construct(1080, 720, 1, 1))
		world.Start();

	return 0;
}
