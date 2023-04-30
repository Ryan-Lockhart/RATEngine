#ifndef MT_ENGINE_H

#define MT_ENGINE_H
#include <random>

class MTEngine
{
public:
	MTEngine();
	MTEngine(size_t seed);

	void Seed();
	void Seed(size_t seed);

	// Generates a random integer between min and max
	int Next(int min, int max);
	// Generates a random bool of true or false
	bool NextBool();
	// Generates a random float between min and max
	float NextFloat(float min, float max);

private:
	// A random device used for generating unique seeds
	std::random_device m_Generator;

	// A random generator used for returning various types
	std::mt19937 m_Twister;
};

#endif
