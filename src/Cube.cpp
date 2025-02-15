#include "Cube.h"

#include "Sprite.h"
#include "Utils.h"

#include <SDL_image.h>

Cube::Cube(SDL_Renderer* renderer, const Vec2& pos, float angle) : pos(pos), start_pos(pos), angle(angle)
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
	up = { forward.y, -forward.x };

	// Apply acceleration to velocity
	acc = mass * gravity * sinf(Deg2Radf(angle));
	vel += acc;
	
	if (frictionCoeff != 0)
	{
		// Apply friction
		float normalForce = mass * gravity * cosf(Deg2Radf(angle));
		float frictionForce = frictionCoeff * normalForce;  // Friction force
		float frictionAcc = frictionForce / mass;  // Convert force to acceleration
	
		// Apply friction acceleration (opposes movement)
		if (vel > 0) vel -= frictionAcc;
		else if (vel < 0) vel += frictionAcc;
	
		if (vel > 0) vel -= frictionAcc;
		else if (vel < 0) vel += frictionAcc;
	
		// Prevent jittering (stop when very slow)
		if (fabs(vel) < 0.1f) vel = 0;
		if (fabs(vel) < 0.1f) vel = 0;
	}

	prev_pos = pos;

	pos.x += vel * cosf(Deg2Radf(angle)) * dt;
	pos.y += vel * sinf(Deg2Radf(angle)) * dt;

	center.x += vel * cosf(Deg2Radf(angle)) * dt;
	center.y += vel * sinf(Deg2Radf(angle)) * dt;

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


void Cube::AlignToFloor(Vec2 floor_vertex)
{
	forward = { cosf(Deg2Radf(angle)), sinf(Deg2Radf(angle)) };
	// contact vertex = bottom right vertex of the cube
	// position of the contact vertex in the cube's local space relative to its center.
	Vec2 localOffset = { size.x/2, size.y/2}; // 32 to the right, and 32 down from the center
	Vec2 rotatedOffset = { 0, 0 };
	// rotate local offset
	rotatedOffset.x = localOffset.x * cosf(Deg2Radf(angle)) - localOffset.y * sinf(Deg2Radf(angle));
	rotatedOffset.y = localOffset.x * sinf(Deg2Radf(angle)) + localOffset.y * cosf(Deg2Radf(angle));
	
	Vec2 upOffset = { -64.f, -64.f };
	// calculate new center position so that the contact vertex will touch the floor
	center = floor_vertex - rotatedOffset + (forward * upOffset);
	// pos is unrotated cube position so we sub the unrotated offset
	pos = { center.x - localOffset.x, center.y - localOffset.y };

	// update vertices based on new center
	original_vertices[0] = { center.x - localOffset.x, center.y - localOffset.y };
	original_vertices[1] = { center.x + localOffset.x, center.y - localOffset.y };
	original_vertices[2] = { center.x + localOffset.x, center.y + localOffset.y };
	original_vertices[3] = { center.x - localOffset.x, center.y + localOffset.y };
}

void Cube::Render(SDL_Renderer* renderer)
{
	sprite->Render(renderer, pos, angle, true);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawLine(renderer, (int)vertices[0].x, (int)vertices[0].y, (int)vertices[1].x, (int)vertices[1].y);
	SDL_RenderDrawLine(renderer, (int)vertices[1].x, (int)vertices[1].y, (int)vertices[2].x, (int)vertices[2].y);
	SDL_RenderDrawLine(renderer, (int)vertices[2].x, (int)vertices[2].y, (int)vertices[3].x, (int)vertices[3].y);
	SDL_RenderDrawLine(renderer, (int)vertices[3].x, (int)vertices[3].y, (int)vertices[0].x, (int)vertices[0].y);

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_Rect a = { (int)center.x, (int)center.y, 10, 10 };
	SDL_RenderDrawRect(renderer, &a);

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_Rect b = { (int)pos.x, (int)pos.y, 5, 5 };
	SDL_RenderDrawRect(renderer, &b);

	// Forward vector
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(renderer, (int)(center.x + forward.x), (int)(center.y + forward.y), (int)(center.x + forward.x * 100), (int)(center.y + forward.y * 100));
	
	// Up vector
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawLine(renderer, (int)(center.x + up.x), (int)(center.y + up.y), (int)(center.x + up.x * 100), (int)(center.y + up.y* 100));

	//SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	//Vec2 forward2 = { cosf(Deg2Radf(angle)), sinf(Deg2Radf(angle)) };
	//SDL_RenderDrawLine(renderer, center.x + forward2.x, center.y + forward2.y, center.x + forward2.x * 100, center.y + forward2.y * 100);


	//for (const Vec2& v : vertices)
	//{
	//	SDL_RenderDrawPoint(renderer, v.x, v.y);
	//}

}