#include <SDL.h>
#include <SDL_image.h>
//#include <SDL_ttf.h>
#include <iostream>
#include <cassert>

#include "Vec2.h"
#include "Cube.h"
#include "Sprite.h"
#include "Utils.h"
#include "UI.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const float gravity = 9.81f;

struct Rect
{
	Vec2 m_start;
	Vec2 m_end;
	Vec2 m_vertices[4];
	int m_thicknes = 5;
	double m_angle;

	Vec2 pos;

	Rect(const Vec2& start, const Vec2& end) : m_start(start), m_end(end)
	{
		m_vertices[0] = m_start;
		m_vertices[1] = { m_start.x + m_thicknes, m_start.y };
		m_vertices[2] = { m_start.x + m_thicknes, m_start.y + m_thicknes };
		m_vertices[3] = m_end;

		pos = { m_start.x, m_end.y };
	}

	void Draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

		for (int i = 0; i < m_thicknes; i++)
		{
			SDL_RenderDrawLine(renderer, m_start.x, m_start.y+i, m_end.x, m_end.y+i);
			SDL_RenderDrawLine(renderer, m_start.x+i, m_start.y, m_end.x+i, m_end.y);
		}
	}
};

struct Triangle
{
	Vec2 m_pos;
	Vec2 m_vertices[3];
	double m_angle;

	/*
	//Triangle(const Vec2& v1, const Vec2& v2, const Vec2& v3)
	//{
	//	m_pos = v1;
	//	m_vertices[0] = v1;
	//	m_vertices[1] = v2;
	//	m_vertices[2] = v3;
	//}

	//Triangle(const Vec2& end, int adjacent_len, double angle) : m_angle(angle)
	//{
	//	// opposite = adjacent_len * tan(angle)
	//	Vec2 m_end = end;
	//	//if (angle > 30)
	//	//{
	//	//	adjacent_len /= 3;
	//	//	m_end.x -= end.x / 2;
	//	//}
	//	// end of hypotenuse and adjacent
	//	m_vertices[1] = m_end;

	//	// start of adjacent
	//	m_vertices[2].x = m_vertices[1].x - adjacent_len;
	//	m_vertices[2].y = m_end.y;

	//	// start of hypotenuse
	//	m_vertices[0].x = m_vertices[2].x;
	//	m_vertices[0].y = m_end.y - adjacent_len * tanf(Deg2Radf(angle));

	//	m_pos = m_vertices[0];
	//}

	//Triangle(float y_end, int adjacent_len, double angle) : m_angle(angle)
	//{
	//	// opposite = adjacent_len * tan(angle)
	//	if (angle > 30)
	//	{
	//		adjacent_len /= 1.5;
	//		y_end += y_end / 2;
	//	}
	//	if (angle > 50)
	//	{
	//		adjacent_len /= 2;
	//	}
	//	Vec2 m_end = { adjacent_len, y_end };
	//	m_vertices[1] = m_end;

	//	// start of adjacent
	//	m_vertices[2].x = m_vertices[1].x - adjacent_len;
	//	m_vertices[2].y = m_end.y;

	//	// start of hypotenuse
	//	m_vertices[0].x = m_vertices[2].x;
	//	m_vertices[0].y = m_end.y - adjacent_len * tanf(Deg2Radf(angle));

	//	m_pos = m_vertices[0];
	//}

	//Triangle(const Vec2& end, int adjacent_len, double angle) : m_angle(angle)
	//{
	//	// opposite = adjacent_len * tan(angle)
	//
	//	// end of hypotenuse and adjacent
	//	m_vertices[1] = end;
	//
	//	// start of adjacent
	//	m_vertices[2].x = m_vertices[1].x - adjacent_len;
	//	m_vertices[2].y = end.y;
	//
	//	// start of hypotenuse
	//	m_vertices[0].x = m_vertices[2].x;
	//	m_vertices[0].y = end.y - adjacent_len * tan(Deg2Rad(angle));
	//
	//}
	*/
	
	// Start & end point and an angle
	Triangle(const Vec2& start, const Vec2& end, double angle) : m_angle(angle)
	{
		m_vertices[1] = end;
		m_vertices[2] = start;

		float length = sqrt((end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y));
		length = length * tan(Deg2Rad(m_angle));

		m_vertices[0] = { start.x, start.y - length };
	}


	void Update()
	{
		float length = sqrt((m_vertices[1].x - m_vertices[2].x) * (m_vertices[1].x - m_vertices[2].x) + (m_vertices[1].y - m_vertices[2].y) * (m_vertices[1].y - m_vertices[2].y));
		length = length * tan(Deg2Rad(m_angle));

		m_vertices[0] = { m_vertices[2].x, m_vertices[2].y - length };
	}

	void Draw(SDL_Renderer* renderer)
	{
		// hypotenuse - magenta
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderDrawLine(renderer, m_vertices[0].x, m_vertices[0].y, m_vertices[1].x, m_vertices[1].y);

		// adjacent - red
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(renderer, m_vertices[1].x, m_vertices[1].y, m_vertices[2].x, m_vertices[2].y);

		// opposite - green
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawLine(renderer, m_vertices[2].x, m_vertices[2].y, m_vertices[0].x, m_vertices[0].y);
	}
};


void Restart(const UI::Options& opt, Cube* cube, Triangle& floor, Rect* wall)
{
	floor = Triangle({ 10, SCREEN_HEIGHT + (opt.angle > 40 ? opt.angle : -10) }, { SCREEN_WIDTH - (opt.angle > 30 ? 7 * opt.angle : 20), SCREEN_HEIGHT + (opt.angle > 40 ? opt.angle : -10) }, opt.angle);
	floor.Update();

	wall->m_start = { floor.m_vertices[1].x, 0 };
	wall->m_end = { floor.m_vertices[1].x, SCREEN_HEIGHT };

	cube->vel = { opt.vel, 0 };
	cube->angle = opt.angle;
	cube->frictionCoeff = opt.friction;
	cube->mass = opt.mass;
	cube->start_pos = { floor.m_vertices[1].x - 64 * 2 * cube->forward.x - 64 * 2 * cube->up.x, floor.m_vertices[1].y + 64 * 3 * cube->forward.y + 64 * 4.5f * cube->up.y };
	
	cube->Restart();

}



int main(int argc, char* args[])
{
	float dt = (float)(SDL_GetTicks() / 1000.f);
	float lastTime = 0.f;
	float prevdt = dt;
	// @TODO: FPS capping can cause some issues with physics
	const float DESIRED_DT = 1 / 60.f; // 120 FPS

	int result = SDL_Init(SDL_INIT_VIDEO);
	assert(result == 0 && "SDL could not initialize!");
	
	//result = TTF_Init();
	//assert(result != -1 && "SDL_ttf could not initialize!");

	SDL_Window* window = SDL_CreateWindow("Fizyka", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	assert(window != nullptr && "Window could not be created!");

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	assert(renderer != nullptr && "Renderer could not be created!");

	bool quit = false;
	SDL_Event event;

	UI::Init(window, renderer);


	float a = 30;
	Triangle floor({ 10, SCREEN_HEIGHT + (a > 40 ? a : -10) }, { SCREEN_WIDTH - (a > 30 ? 7 * a : 20), SCREEN_HEIGHT + (a > 40 ? a : -10) }, a);

	// main cube
	Cube cube(renderer, {500, 220}, a);
	
	//Vec2 localOffset = { 32, 32 };
	//Vec2 rotatedOffset;
	//rotatedOffset.x = localOffset.x * cosf(Deg2Rad(cube.angle)) - localOffset.y * sinf(Deg2Rad(cube.angle));
	//rotatedOffset.y = localOffset.x * sinf(Deg2Rad(cube.angle)) + localOffset.y * cosf(Deg2Rad(cube.angle));

	//Vec2 upOffset = { -30.5f * a, -30.5f * a };
	//cube.center = floor.m_vertices[1] - rotatedOffset + (cube.forward * upOffset);
	//cube.pos = { cube.center.x - cube.size.x / 2.f, cube.center.y - cube.size.y / 2.f };

	//cube.original_vertices[0] = { cube.center.x - cube.size.x / 2, cube.center.y - cube.size.y / 2 };
	//cube.original_vertices[1] = { cube.center.x + cube.size.x / 2, cube.center.y - cube.size.y / 2 };
	//cube.original_vertices[2] = { cube.center.x + cube.size.x / 2, cube.center.y + cube.size.y / 2 };
	//cube.original_vertices[3] = { cube.center.x - cube.size.x / 2, cube.center.y + cube.size.y / 2 };

	cube.Update();

	cube.mass = 10;

	cube.vel = { 50.f, 0 };
	
	cube.frictionCoeff = 0.01;

	UI::Options options;

	options.angle = a;
	options.mass = cube.mass;
	options.vel = cube.vel.x;
	options.friction = cube.frictionCoeff;

	Rect wall = { {floor.m_vertices[1].x, 0}, {floor.m_vertices[1].x, SCREEN_HEIGHT}};
	Rect wall2 = { {5, 0}, {5, SCREEN_HEIGHT}};
	while (!quit)
	{
		dt = (SDL_GetTicks() - lastTime) / 1000.f;
		if (dt >= DESIRED_DT)
		{
			while (SDL_PollEvent(&event) != 0)
			{
				if (event.type == SDL_QUIT)	
					quit = true;

				UI::ProcessEvent(event);
			}

			UI::StartFrame();

			
			UI::DrawMenu(&options, [&options, &cube, &floor, &wall]() {
				Restart(options, &cube, floor, &wall);
				});

			UI::DrawStats(&cube);


			cube.Move(dt);

			if (CheckCollision(renderer, cube.vertices, 4, floor.m_vertices, 3))
			{
				cube.isOnSlope = true;

				Vec2 delta = cube.pos - cube.prev_pos;

				cube.pos -= delta;
				cube.center -= delta;

			}
			else
			{
				//cube.isOnSlope = false;
				//cube.acc.y = -GRAVITY;
			}

			//printf("x: %f\n", cube.vel.x);
			if (CheckCollision(renderer, cube.vertices, 4, wall.m_vertices, 4))
			{
				//printf("Wall and slope!\n");
				cube.vel.x += 10;
				cube.vel.x *= -1;

				
				//float e = (cube.mass * (cube.vel.x * cube.vel.x)) / 2;	

				//double v_after = sqrt(v_x_after * v_x_after + v_y_after * v_y_after);
				//float v = sqrt((sinf(a) - cube.frictionCoeff * cosf(a)) / (sinf(a) + cube.frictionCoeff * cosf(a)));

				//cube.vel.x = cube.vel.x * v;

			}

			// wall on the left
			if (CheckCollision(renderer, cube.vertices, 4, wall2.m_vertices, 4))
			{

			}


			wall.Draw(renderer);
			wall2.Draw(renderer);

			floor.Draw(renderer);
			cube.Render(renderer);


			//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
			//SDL_RenderDrawPoint(renderer, cube.pos.x, cube.pos.y);

			UI::EndFrame(renderer);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderPresent(renderer);
			SDL_RenderClear(renderer);

			lastTime = (float)SDL_GetTicks();
			prevdt = dt;

		}
	}

	UI::Shutdown();
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
