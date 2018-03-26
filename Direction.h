#pragma once
#include <array>

namespace Chess
{
	namespace Directions 
	{
		struct Direction
		{
		public:
			int up = 0;
			int right = 0;
		};
		//Assuming square squares, could be triangular or hexagonal
		using DirectionSet = std::vector<Direction>;
		extern DirectionSet allDirections;
		extern DirectionSet rightAngles;
		extern DirectionSet diagonals;
		extern DirectionSet knight;
	}

}