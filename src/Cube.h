#pragma once

#include "Vec2.h"

#include <SDL_rect.h>
#include <SDL_render.h>

class Sprite;

class Cube
{
	SDL_Rect rect = { (int)pos.x, (int)pos.y, (int)size.x, (int)size.y };
	Sprite* sprite = nullptr;
public:
	Vec2 pos;
	Vec2 size = { 64, 64 };

	Vec2 vertices[4];
	Vec2 original_vertices[4];

	Vec2 start_pos;

	Vec2 forward;
	Vec2 up;


	float gravity = 9.81f;
	float angle = 0.f;
	float mass = 100.f; // kg


	double frictionCoeff = 0.01;
	Vec2 vel = {0.f, 0.f};
	Vec2 acc = { 0.f, 0 };

	Vec2 prev_pos = { 0.f, 0.f };

	Vec2 center;
	bool shouldStop = false;
	bool isOnSlope = false;
	bool isGoingUp = true;

	Cube(SDL_Renderer* renderer, const Vec2& pos, float angle);
	~Cube();
	const SDL_Rect* GetRect();
	void Render(SDL_Renderer* renderer);
	void Move(float dt);
	void Update();
	void AlignToFloor(Vec2 floor_vertex);

	//void Restart();

};

