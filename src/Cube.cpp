#include "Cube.h"

#include "Sprite.h"
#include "Utils.h"

#include <SDL_image.h>

Cube::Cube(SDL_Renderer* renderer, const Vec2& pos, double angle) : pos(pos), start_pos(pos), angle(angle)
{
	sprite = new Sprite(renderer, "res/cube.png");

	original_vertices[0] = { pos.x, pos.y };
	original_vertices[1] = { pos.x + size.x, pos.y };
	original_vertices[2] = { pos.x + size.x, pos.y + size.y };
	original_vertices[3] = { pos.x, pos.y + size.y };
	
	forward = { cosf(Deg2Radf(angle)), sinf(Deg2Radf(angle)) };
	up = { sinf(Deg2Radf(angle)), -cosf(Deg2Radf(angle)) };

	center = { pos.x + size.x / 2.f, pos.y + size.y / 2.f };

	std::copy(original_vertices, original_vertices + 4, vertices);

	Update();
}

Cube::~Cube()
{
	delete sprite;
}

const SDL_Rect* Cube::GetRect()
{
	rect = { (int)pos.x, (int)pos.y, (int)size.x, (int)size.y };
	return &rect;
}


void Cube::Move(float dt)
{
	forward = { cosf(Deg2Radf(angle)), sinf(Deg2Radf(angle)) };
	up = { sinf(Deg2Radf(angle)), -cosf(Deg2Radf(angle)) };
	//up = { forward.y, -forward.x };


	// Apply acceleration to velocity
	acc.x = mass * gravity * sinf(Deg2Radf(angle)) * dt;

	/*
	float a_g = gravity * sinf(Deg2Rad(angle)); // gravitional acc 
	float a_f = frictionCoeff * gravity * cosf(Deg2Rad(angle)); // friction acc?

	acc.x = (a_g + (vel.x > 0 ? -a_f : a_f)); // add or sub friction acc

	vel.x += acc.x * dt; // apply acc
	*/

	//float l = vel.x * vel.x / (2 * gravity * (sinf(a) + mu * cosf(a)));
	//double v_down = sqrt(2 * gravity * l * (sinf(a) - mu * cosf(a)));
	//vel.x += v_down * cosf(a) * dt;

	//double theta = Deg2Radf(angle);

	//float a = gravity * sin(theta) - frictionCoeff * gravity * cos(theta); // przyspieszenie w dó³

	
	if (frictionCoeff != 0)
	{
		// Apply friction
		float normalForce = mass * gravity * cosf(Deg2Radf(angle));
		double frictionForce = frictionCoeff * normalForce;  // Friction force
		double frictionAcc = frictionForce / mass;  // Convert force to acceleration

		// Apply friction acceleration (opposes movement)
		if (vel.x > 0) vel.x -= frictionAcc * dt;
		else if (vel.x < 0) vel.x += frictionAcc * dt;

		if (vel.y > 0) vel.y -= frictionAcc * dt;
		else if (vel.y < 0) vel.y += frictionAcc * dt;

		// Prevent jittering (stop when very slow)
		if (fabs(vel.x) < 0.1f) vel.x = 0;
		if (fabs(vel.y) < 0.1f) vel.y = 0;
	}
	

	prev_pos = pos;

	//pos.x += forward.x * vel.x * dt + up.x * vel.y * dt;
	//pos.y += forward.y * vel.x * dt + up.y * vel.y * dt;

	pos.x += vel.x * cosf(Deg2Radf(angle)) * dt - vel.y * sinf(Deg2Radf(angle)) * dt;
	pos.y += vel.x * sinf(Deg2Radf(angle)) * dt + up.y * vel.y * cosf(Deg2Radf(angle)) * dt;

	center.x += vel.x * cosf(Deg2Radf(angle)) * dt - vel.y * sinf(Deg2Radf(angle)) * dt;
	center.y += vel.x * sinf(Deg2Radf(angle)) * dt + up.y * vel.y * cosf(Deg2Radf(angle)) * dt;

	Vec2 diff = prev_pos - pos;

	//center.x += diff.x;
	//center.y += diff.y;
	//center.x += forward.x * vel.x * dt + up.x * vel.y * dt;
	//center.y += forward.y * vel.x * dt + up.y * vel.y * dt;

	Update();
}

void Cube::Update()
{
	original_vertices[0] = { center.x - size.x / 2, center.y - size.y / 2 };
	original_vertices[1] = { center.x + size.x / 2, center.y - size.y / 2 };
	original_vertices[2] = { center.x + size.x / 2, center.y + size.y / 2 };
	original_vertices[3] = { center.x - size.x / 2, center.y + size.y / 2 };

	for (int i = 0; i < 4; i++)
	{
		Vec2& v = vertices[i];
		const Vec2& original = original_vertices[i];

		v.x = (original.x - center.x) * cosf(Deg2Radf(angle)) - (original.y - center.y) * sinf(Deg2Radf(angle)) + center.x;
		v.y = (original.x - center.x) * sinf(Deg2Radf(angle)) + (original.y - center.y) * cosf(Deg2Radf(angle)) + center.y;
	}
}


void Cube::Restart()
{
	pos = start_pos;
	center = { pos.x + size.x / 2.f, pos.y + size.y / 2.f };

	//original_vertices[0] = { pos.x, pos.y };
	//original_vertices[1] = { pos.x + size.x, pos.y };
	//original_vertices[2] = { pos.x + size.x, pos.y + size.y };
	//original_vertices[3] = { pos.x, pos.y + size.y };

	original_vertices[0] = { center.x - size.x / 2, center.y - size.y / 2 };
	original_vertices[1] = { center.x + size.x / 2, center.y - size.y / 2 };
	original_vertices[2] = { center.x + size.x / 2, center.y + size.y / 2 };
	original_vertices[3] = { center.x - size.x / 2, center.y + size.y / 2 };


	Update();
}

void Cube::Render(SDL_Renderer* renderer)
{
	sprite->Render(renderer, pos, angle, true);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawLine(renderer, vertices[0].x, vertices[0].y, vertices[1].x, vertices[1].y);
	SDL_RenderDrawLine(renderer, vertices[1].x, vertices[1].y, vertices[2].x, vertices[2].y);
	SDL_RenderDrawLine(renderer, vertices[2].x, vertices[2].y, vertices[3].x, vertices[3].y);
	SDL_RenderDrawLine(renderer, vertices[3].x, vertices[3].y, vertices[0].x, vertices[0].y);

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_Rect a = { center.x, center.y, 10, 10 };
	SDL_RenderDrawRect(renderer, &a);

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_Rect b = { pos.x, pos.y, 5, 5 };
	SDL_RenderDrawRect(renderer, &b);

	// Forward vector
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(renderer, center.x + forward.x, center.y + forward.y, center.x + forward.x * 100, center.y + forward.y * 100);
	
	// Up vector
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawLine(renderer, center.x + up.x, center.y + up.y, center.x + up.x * 100, center.y + up.y * 100);

	//SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	//Vec2 forward2 = { cosf(Deg2Radf(angle)), sinf(Deg2Radf(angle)) };
	//SDL_RenderDrawLine(renderer, center.x + forward2.x, center.y + forward2.y, center.x + forward2.x * 100, center.y + forward2.y * 100);


	//for (const Vec2& v : vertices)
	//{
	//	SDL_RenderDrawPoint(renderer, v.x, v.y);
	//}

}