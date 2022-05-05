#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

struct Rect
{
	int x, y;
};

class QuadTree
{
public:
	QuadTree** leafs;
	int* aBounds; // mid x-y
	Rect* rect;

	QuadTree(int *aBounds_, Rect *rect_)
	{
		aBounds = aBounds_;
		rect = rect_;
	}

	void addRect(Rect* rect)
	{
		if(!leafs)
			leafs = new QuadTree * [4];

		int idx = getLeafIdx(rect);

		if (leafs[idx])
			leafs[idx]->addRect(rect);
		else
			leafs[idx] = new QuadTree();

	}

	int getLeafIdx(Rect* rect)
	{
		return (rect->x < aBounds[0]) + (rect->y < aBounds[1]);
	}


};


class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{


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
