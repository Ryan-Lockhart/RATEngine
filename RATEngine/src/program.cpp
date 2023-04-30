#include "utility.hpp"
#include "engine.hpp"

rat::Size gridSize{ 32, 32 };

std::string path{ "Assets/Glyphs/" };

std::string glyph12{ "glyphs_12x12.png" };
rat::Size glyph12Size{ 12, 12 };

std::string glyph8{ "glpyhs_8x8.png" };
rat::Size glyph8Size{ 8, 8 };


int main(int argc, char* argv[])
{
	try { rat::Engine engine{ gridSize, path + glyph12, glyph12Size }; }
	catch(std::exception)
	{
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	return 0;
}