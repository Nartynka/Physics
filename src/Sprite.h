#pragma once

#include <cassert>

#include "SDL_render.h"
#include "SDL_image.h"


class Sprite
{
public:

	Sprite(SDL_Renderer* renderer, const char* path)
	{
		SDL_Surface* surf = IMG_Load(path);
		assert(surf != nullptr && "Texture Surface is null!!!");
		tex = SDL_CreateTextureFromSurface(renderer, surf);

		rect.w = surf->w;
		rect.h = surf->h;

		SDL_FreeSurface(surf);
	}

	~Sprite()
	{
		SDL_DestroyTexture(tex);
	}

	void Render(SDL_Renderer* renderer, const Vec2& pos, double angle = 0, bool bcenter_rot = true)
	{
		rect.x = (int)pos.x;
		rect.y = (int)pos.y;

		SDL_Point center = { 0,0 };
		
		SDL_RenderCopyEx(renderer, tex, nullptr, &rect, angle, bcenter_rot ? nullptr : &center, SDL_FLIP_NONE);
	}

	void Scale(const Vec2& scale)
	{
		rect.w *= (int)scale.x;
		rect.h *= (int)scale.y;
	}

private:
	SDL_Texture* tex = nullptr;
	const char* path = "";
	SDL_Rect rect{};
};

