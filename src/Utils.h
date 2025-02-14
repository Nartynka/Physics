#pragma once
#include <vector>

#define M_PI 3.14159265358979323846264338327950288

#define SHOW_COLLISION_DEBUG_LINES false

inline double Rad2Deg(double radians)
{
	return radians * 180.0 / M_PI;
}

inline double Deg2Rad(double degrees)
{
	return degrees * M_PI / 180.0;
}

inline float Deg2Radf(double degrees)
{
	return (float)(degrees * M_PI / 180.0);
}


inline bool CheckCollision(SDL_Renderer* renderer, Vec2 shape1[], int shape1_size, Vec2 shape2[], int shape2_size)
{
	/*
	//Vec2 edgeAB = b - a;
	//Vec2 edgeBC = c - b;
	//Vec2 edgeCD = c - d;
	//Vec2 edgeDA = d - a;

	//Vec2 normalAB = { -edgeAB.y, edgeAB.x };
	//Vec2 normalBC = { -edgeBC.y, edgeBC.x };
	//Vec2 normalCD = { -edgeCD.y, edgeCD.x };
	//Vec2 normalDA = { -edgeDA.y, edgeDA.x };
	*/

	/// AXES
	std::vector<Vec2> axes;

	std::vector<Vec2> edges1;
	for (int i = 0; i < shape1_size; i++)
	{
		Vec2 edge;
		if(i + 1 < shape1_size)
			edge = shape1[i + 1] - shape1[i];
		else
			edge = shape1[0] - shape1[i];
		edges1.push_back(edge);
	}

	std::vector<Vec2> edges2;
	for (int i = 0; i < shape2_size; i++)
	{
		Vec2 edge;
		if (i + 1 < shape2_size)
			edge = shape2[i + 1] - shape2[i];
		else
			edge = shape2[0] - shape2[i];
		edges2.push_back(edge);
	}

	// Find normals, our axes
	for (int i = 0; i < edges1.size(); i++)
	{
		Vec2 e = edges1[i];
		e = norm({ -e.y, e.x });
		axes.push_back(norm(e));
	}

	for (int i = 0; i < edges2.size(); i++)
	{
		Vec2 e = edges2[i];
		e = norm({ -e.y, e.x });
		axes.push_back(norm(e));
	}


	// Project vertices of shape1 on to shape1 axes
	std::vector<Vec2> projections1;
	std::vector<Vec2> projections2;
	bool a = true;
	for (const Vec2& axis : axes)
	{
		float min1 = dot(axis, shape1[0]);
		float max1 = min1;
		for (int j = 1; j < shape1_size; j++)
		{
			float p = dot(axis, shape1[j]);

			if (p < min1)
				min1 = p;
			else if (p > max1)
				max1 = p;
		}

		projections1.push_back({ min1, max1 });

		float min2 = dot(axis, shape2[0]);
		float max2 = min2;
		for (int j = 1; j < shape2_size; j++)
		{
			float p = dot(axis, shape2[j]);

			if (p < min2)
				min2 = p;
			else if (p > max2)
				max2 = p;
		}

		projections2.push_back({ min2, max2 });

		if (max1 <= min2 || max2 <= min1)
		{
			a = false;
			//return false;
		}
	}

	if (shape2_size == 4)
	{
#if SHOW_COLLISION_DEBUG_LINES
		/// VISUALIZING
		/// THE AXES
		// THE axes for shape1
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		// X axis
		SDL_RenderDrawLine(renderer, 0, edges1[0].y + 200, 2000, edges1[0].y + 200);
		SDL_RenderDrawLine(renderer, 0, edges1[1].y + 600, 2000, edges1[1].y + 600);
		// Y axis
		SDL_RenderDrawLine(renderer, edges1[0].x + 30, 0, edges1[0].x + 30, 1000);
		SDL_RenderDrawLine(renderer, edges1[1].x + 600, 0, edges1[1].x + 600, 1000);

		// THE axes for shape2
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		// X axis
		SDL_RenderDrawLine(renderer, 0, edges2[0].y + 200, 2000, edges2[0].y + 200);
		SDL_RenderDrawLine(renderer, 0, edges2[1].y + 600, 2000, edges2[1].y + 600);
		// Y axis
		SDL_RenderDrawLine(renderer, edges2[0].x + 60, 0, edges2[0].x + 30, 1000);
		SDL_RenderDrawLine(renderer, edges2[1].x + 600, 0, edges2[1].x + 600, 1000);


		/// SHAPE2
		// Visualizing shape2 projected on to the axes
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		// Projected onto X axis of shape2
		SDL_RenderDrawLine(renderer, abs(projections2[5].x), edges2[0].y + 200, abs(projections2[5].y), edges2[0].y + 200);
		SDL_RenderDrawLine(renderer, abs(projections2[5].x), edges2[1].y + 600, abs(projections2[5].y), edges2[1].y + 600);
		// Projected onto Y axis of shape2
		SDL_RenderDrawLine(renderer, edges2[0].x + 30, projections2[4].x, edges2[0].x + 30, projections2[4].y);
		SDL_RenderDrawLine(renderer, edges2[1].x + 600, projections2[4].x, edges2[1].x + 600, projections2[4].y);

		// Projected onto X axis of shape1
		SDL_RenderDrawLine(renderer, projections2[3].x, edges1[0].y + 200, projections2[3].y, edges1[0].y + 200);
		SDL_RenderDrawLine(renderer, projections2[3].x, edges1[1].y + 600, projections2[3].y, edges1[1].y + 600);
		// Projected onto Y axis of shape1
		SDL_RenderDrawLine(renderer, edges1[0].x + 30, projections2[0].x, edges1[0].x + 30, projections2[0].y);
		SDL_RenderDrawLine(renderer, edges1[1].x + 600, projections2[0].x, edges1[1].x + 600, projections2[0].y);


		/// SHAPE1
		// Visualizing shape1 projected on to the axes
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		// Projected onto X axis of shape1
		SDL_RenderDrawLine(renderer, projections1[3].x, edges1[0].y + 200, projections1[3].y, edges1[0].y + 200);
		SDL_RenderDrawLine(renderer, projections1[3].x, edges1[1].y + 600, projections1[3].y, edges1[1].y + 600);
		// Projected onto Y axis of shape1
		SDL_RenderDrawLine(renderer, edges1[0].x + 30, projections1[0].x, edges1[0].x + 30, projections1[0].y);
		SDL_RenderDrawLine(renderer, edges1[1].x + 600, projections1[0].x, edges1[1].x + 600, projections1[0].y);

		// WE DON'T NEED THE OTHER TWO BECOUSE THE ARE THE SAME BUT REVERSED (end is a start etc.)
		//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		// 1 and 2 are on minus (because we negated the y, not the x) so we have to abs it to see it
		// x
		//SDL_RenderDrawLine(renderer, abs(projections[1].x), shape1[0].y-30, abs(projections[1].y), shape1[0].y-30);
		// y
		//SDL_RenderDrawLine(renderer, shape1[0].x-30, abs(projections[2].x), shape1[0].x-30, abs(projections[2].y));

		// Projected onto X axis of shape2
		SDL_RenderDrawLine(renderer, abs(projections1[5].x), edges2[0].y + 200, abs(projections1[5].y), edges2[0].y + 200);
		SDL_RenderDrawLine(renderer, abs(projections1[5].x), edges2[1].y + 600, abs(projections1[5].y), edges2[1].y + 600);
		// Projected onto Y axis of shape2
		SDL_RenderDrawLine(renderer, edges2[0].x + 30, projections1[4].x, edges2[0].x + 30, projections1[4].y);
		SDL_RenderDrawLine(renderer, edges2[1].x + 600, projections1[4].x, edges2[1].x + 600, projections1[4].y);

#endif

	}

	////normals / axes
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//for (int i = 0; i < shape1_size; i++)
	//{
	//	SDL_RenderDrawLine(renderer, shape1[i].x, shape1[i].y, shape1[i].x + edges1[i].x, shape1[i].y + edges1[i].y);
	//}

	return a;

	//// Check if they overlap
	//for (int i = 0; i < axes.size(); i++)
	//{
	//	// x = min, y = max
	//	// max shape1 < min shape2
	//	// max shape2 < min shape1
	//	if (projections1[i].y < projections2[i].x || projections2[i].y < projections1[i].x)
	//	{
	//		//areOverlaping = false;
	//		//break;
	//		return false;
	//	}
	//}
	//return true;


	/*
	for (int i = 0; i < edges1.size(); i++)
	{
		Vec2 curr_edge = edges1[i];
		float min = dot(curr_edge, shape1[0]);
		float max = min;
		for (int j = 1; j < shape1_size; j++)
		{
			float p = dot(curr_edge, shape1[j]);

			if (p < min)
				min = p;
			else if (p > max)
				max = p;
		}
		
		projections1.push_back({ min, max });
	}

	// Project vertices of shape1 on to shape2 axes
	for (int i = 0; i < edges2.size(); i++)
	{
		Vec2 curr_edge = edges2[i];
		float min = dot(curr_edge, shape1[0]);
		float max = min;
		for (int j = 1; j < shape1_size; j++)
		{
			float p = dot(curr_edge, shape1[j]);

			if (p < min)
				min = p;
			else if (p > max)
				max = p;
		}

		projections1.push_back({ min, max });
	}*/


	// Project vertices of shape2 on to axes
	/*
	for (int i = 0; i < edges2.size(); i++)
	{
		Vec2 curr_edge = norm(edges2[i]);
		float min = dot(curr_edge, shape2[0]);
		float max = min;
		for (int j = 1; j < shape2_size; j++)
		{
			float p = dot(curr_edge, shape2[j]);

			if (p < min)
				min = p;
			else if (p > max)
				max = p;
		}

		projections2.push_back({ min, max });
	}

	for (int i = 0; i < edges1.size(); i++)
	{
		Vec2 curr_edge = norm(edges1[i]);
		float min = dot(curr_edge, shape2[0]);
		float max = min;
		for (int j = 1; j < shape2_size; j++)
		{
			float p = dot(curr_edge, shape2[j]);

			if (p < min)
				min = p;
			else if (p > max)
				max = p;
		}

		projections2.push_back({ min, max });
	}*/

}