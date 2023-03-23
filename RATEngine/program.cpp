#include "utility.hpp"
#include "engine.hpp"

rat::Size gridSize{ 32, 32 };
rat::Size glyphSize{ 12, 12 };
std::string path{ "" };


int main(int argc, char* argv[])
{
	rat::Engine engine{ gridSize, path, glyphSize };

	return 0;
}