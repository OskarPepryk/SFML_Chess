#include "stdafx.h"
#include "Direction.h"

namespace Chess
{
	namespace Directions
	{

		DirectionSet allDirections
		{ {
				//Up
				{ 1, 0 },
				//Down
				{ -1, 0 },
				//Right
				{ 0, 1 },
				//Left
				{ 0, -1 },
				//Up,Right
				{ 1, 1 },
				//Down,Right
				{ -1, 1 },
				//Down, left
				{ -1, -1 },
				//Up, left
				{ 1, -1 }
			} };
		
		DirectionSet rightAngles
		{ {
			//Up
			{ 1, 0 },
			//Down
			{ -1, 0 },
			//Right
			{ 0, 1 },
			//Left
			{ 0, -1 }
		} };

		DirectionSet diagonals
		{ {
			//Up,Right
			{ 1, 1 },
			//Down,Right
			{ -1, 1 },
			//Down, left
			{ -1, -1 },
			//Up, left
			{ 1, -1 }
		} };

		DirectionSet knight
		{ {
			{ 2, 1 },
		{ 1,2 },
		{ -2, 1 },
		{ -1,2 },
		{ 2, -1 },
		{ 1,-2 },
		{ -2,- 1 },
		{ -1,-2 },

		{}
			} };
	}
}