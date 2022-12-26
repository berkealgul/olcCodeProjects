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

	const int w = 50;
	const int h = 30;
	const int cell_w = 10;
	const int pixel_w = 2;

	int target_i = 0;
	int target_j = 0; 

	vector<vector<float>> field;
	vector<vector<float>> heatmap;


	bool OnUserCreate() override
	{
		field = vector<vector<float>>(h, vector<float>(w));
		heatmap = vector<vector<float>>(h, vector<float>(w));

	}
	 
	bool OnUserUpdate(float dt) override
	{
		Clear(olc::DARK_BLUE);

		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				float c = heatmap[j][i];
				auto p = olc::Pixel(c*10, c*10, c*10);
				FillRect(i*cell_w, j*cell_w, cell_w, cell_w, p);
			}
		}

		return true;
	}

	void CalculateHeatMap()
	{
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				heatmap[j][i] = abs(i - target_i) + abs(j - target_j);
			}
		}
	}



};


int main()
{
	Sketch sketch;

	if (sketch.Construct(sketch.w * sketch.cell_w, sketch.h * sketch.cell_w, sketch.pixel_w, sketch.pixel_w))
		sketch.Start();

	return 0;
}
