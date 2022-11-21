#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>

using namespace std;

class FluidGrid
{
public:

	const int iter = 8;
	int N;
	float dt;
	float diff;
	float visc;

	vector<float> s;
	vector<float> density;

	vector<float> Vx;
	vector<float> Vy;
	vector<float> Vz;

	vector<float> Vx0;
	vector<float> Vy0;
	vector<float> Vz0;


	FluidGrid(int N, float dt, float visc, float diff)
	{
		this->N = N;
		this->dt = dt;
		this->visc = visc;
		this->diff = diff;

		s.resize(N*N);
		density.resize(N*N);

		Vx.resize(N * N);
		Vy.resize(N * N);
		Vz.resize(N * N);

		Vx0.resize(N * N);
		Vy0.resize(N * N);
		Vz0.resize(N * N);
	}

	void set_bnd(int b, vector<float>& x)
	{
		for (int k = 1; k < N - 1; k++) {
			for (int i = 1; i < N - 1; i++) {
				x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
				x[IX(i, N - 1)] = b == 2 ? -x[IX(i, N - 2)] : x[IX(i, N - 2)];
			}
		}

		for (int k = 1; k < N - 1; k++) {
			for (int j = 1; j < N - 1; j++) {
				x[IX(0, j)] = b == 1 ? -x[IX(1, j)] : x[IX(1, j)];
				x[IX(N - 1, j)] = b == 1 ? -x[IX(N - 2, j)] : x[IX(N - 2, j)];
			}
		}

		x[IX(0, 0)] = 0.5 * (x[IX(1, 0)] + x[IX(0, 1)]);
		x[IX(0, N - 1)] = 0.5 * (x[IX(1, N - 1)] + x[IX(0, N - 2)]);
		x[IX(N - 1, 0)] = 0.5 * (x[IX(N - 2, 0)] + x[IX(N - 1, 1)]);
		x[IX(N - 1, N - 1)] = 0.5 * (x[IX(N - 2, N - 1)] + x[IX(N - 1, N - 2)]);
	}

	void lin_solve(int b, vector<float>& x, vector<float>& x0, float a, float c)
	{
		float cRecip = 1.0 / c;
		for (int k = 0; k < iter; k++) {
			for (int j = 1; j < N - 1; j++) {
				for (int i = 1; i < N - 1; i++) {
					x[IX(i, j)] =
						(x0[IX(i, j)]
							+ a * (x[IX(i + 1,j)]
								+ x[IX(i - 1, j)]
								+ x[IX(i, j + 1)]
								+ x[IX(i, j - 1)]
								)) * cRecip;
				}
			}
			
			set_bnd(b, x);
		}
	}

	void diffuse(int b, vector<float>& x, vector<float>& x0)
	{
		float a = dt * diff * (N - 2) * (N - 2);
		lin_solve(b, x, x0, a, 1 + 6 * a);
	}

	void advect(int b, vector<float>& d, vector<float>& d0, vector<float>& velocX, vector<float>& velocY)
	{
		float i0, i1, j0, j1;

		float dtx = dt * (N - 2);
		float dty = dt * (N - 2);

		float s0, s1, t0, t1;
		float tmp1, tmp2, x, y;

		float Nfloat = N - 2;
		float ifloat, jfloat;
		int i, j;

		for (j = 1, jfloat = 1; j < N - 1; j++, jfloat++) 
		{
			for (i = 1, ifloat = 1; i < N - 1; i++, ifloat++) 
			{
				tmp1 = dtx * velocX[IX(i, j)];
				tmp2 = dty * velocY[IX(i, j)];
				x = ifloat - tmp1;
				y = jfloat - tmp2;

				if (x < 0.5f) x = 0.5f;
				if (x > Nfloat + 0.5f) x = Nfloat + 0.5f;
				i0 = floorf(x);
				i1 = i0 + 1.0f;
				if (y < 0.5f) y = 0.5f;
				if (y > Nfloat + 0.5f) y = Nfloat + 0.5f;
				j0 = floorf(y);
				j1 = j0 + 1.0f;

				s1 = x - i0;
				s0 = 1.0f - s1;
				t1 = y - j0;
				t0 = 1.0f - t1;

				int i0i = i0;
				int i1i = i1;
				int j0i = j0;
				int j1i = j1;

				d[IX(i, j)] =
					s0 * (t0 * d0[IX(i0i, j0i)] + t1 * d0[IX(i0i, j1i)]) +
					s1 * (t0 * d0[IX(i1i, j0i)] + t1 * d0[IX(i1i, j1i)]);
			}
		}
		
		set_bnd(b, d);
	}

	void project(vector<float>& velocX, vector<float>& velocY, vector<float>& p, vector<float>& div)
	{
		for (int j = 1; j < N - 1; j++) 
		{
			for (int i = 1; i < N - 1; i++) 
			{
				div[IX(i, j)] = -0.5f * (
					velocX[IX(i + 1, j)]
					- velocX[IX(i - 1, j)]
					+ velocY[IX(i, j + 1)]
					- velocY[IX(i, j - 1)]
					) / N;
				p[IX(i, j)] = 0;
			}
		}
		
		set_bnd(0, div);
		set_bnd(0, p);
		lin_solve(0, p, div, 1, 6);

		for (int j = 1; j < N - 1; j++) {
			for (int i = 1; i < N - 1; i++) {
				velocX[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)]
					- p[IX(i - 1, j)]) * N;
				velocY[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)]
					- p[IX(i, j - 1)]) * N;
			}
		}
		
		set_bnd(1, velocX);
		set_bnd(2, velocY);
	}

	void step()
	{
		diffuse(1, Vx0, Vx);
		diffuse(2, Vy0, Vy);
	
		project(Vx0, Vy0, Vx, Vy);
	
		advect(1, Vx, Vx0, Vx0, Vy0);
		advect(2, Vy, Vy0, Vx0, Vy0);
	
		project(Vx, Vy, Vz, Vx0);
	
		diffuse(0, s, density);
		advect(0, density, s, Vx, Vy);
	}

	void addDensity(int x, int y, float amount)
	{
		density[IX(x, y)] += amount;
	}

	void addVelocity(int x, int y, float amountX, float amountY)
	{
		int i = IX(x, y);

		Vx[i] += amountX;
		Vy[i] += amountY;
	}


	int IX(int i, int j)
	{
		return i + j*N;
	}

	float operator () (int x, int y)
	{
		return density[IX(x, y)];
	}
	
};


class Sketch : public olc::PixelGameEngine
{
public:

	int angle = 0;

	FluidGrid *fluid;

	Sketch()
	{
		sAppName = "Dummy Fluid Simulation";
	}

	bool OnUserCreate() override
	{
		fluid = new FluidGrid(ScreenWidth(), 0.2, 0.00001, 0);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		int cx = 0.5 * ScreenWidth() * 0.2;
		int cy = 0.5 * ScreenHeight() * 0.2;
		
		for (int i = -1; i <= 1; i++) 
		{
			for (int j = -1; j <= 1; j++) 
			{
				fluid->addDensity(cx + i, cy + j, 10);
			}
		}
		

		angle += (3.14 / 3) * fluid->dt;
		fluid->addVelocity(cx, cy, cos(angle)*0.2, cos(angle)*0.2);

		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
			{
				int d = (*fluid)(x, y);
				auto p = olc::Pixel(d*225, d*225, d*225);
				Draw(x, y, p);
			}
		
		fluid->step();

		return true;
	}
	
};


int main()
{
	Sketch sketch;
	if (sketch.Construct(150, 150, 2, 2))
		sketch.Start();

	return 0;
}
