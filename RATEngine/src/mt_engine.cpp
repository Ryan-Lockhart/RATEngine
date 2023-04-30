#include "mt_engine.hpp"

/// <summary>
/// Constructor with random seed and specified distribution
/// </summary>
/// <param name="min">Lowest possible value</param>
/// <param name="max">Highest possible value</param>
MTEngine::MTEngine() :
	m_Twister(m_Generator()) { }

/// <summary>
/// Constructor with specified seed and distribution
/// </summary>
/// <param name="seed">The initial seed for the mersennes' twister</param>
/// <param name="min">Lowest possible value</param>
/// <param name="max">Highest possible value</param>
MTEngine::MTEngine(size_t seed) :
	m_Twister(seed) { }

/// <summary>
/// Re-seeds the mersennes' twister with a random seed
/// </summary>
void MTEngine::Seed()
{ m_Twister.seed(m_Generator()); }

/// <summary>
/// Re-seeds the mersennes' twister with a specified seed
/// </summary>
/// <param name="seed">The value used to re-seed</param>
void MTEngine::Seed(size_t seed)
{ m_Twister.seed(seed); }

/// <summary>
/// Retrieves the next random integer from specified distribution
/// </summary>
/// <param name="min">Lowest possible value</param>
/// <param name="max">Highest possible value</param>
/// <returns>A random integer from mt19937</returns>
int MTEngine::Next(int min, int max)
{
	std::uniform_int_distribution<int> dis = std::uniform_int_distribution<int>(min, max);
	return dis(m_Twister);
}

/// <summary>
/// Retrieves the next random bool
/// </summary>
/// <returns>A random bool from mt19937</returns>
bool MTEngine::NextBool()
{
	std::uniform_int_distribution<int> dis = std::uniform_int_distribution<int>(0, 1);
	return (bool)dis(m_Twister);
}

/// <summary>
/// Retrieves the next random float from specified distribution
/// </summary>
/// <param name="min">Lowest possible value</param>
/// <param name="max">Highest possible value</param>
/// <returns>A random float from mt19937</returns>
float MTEngine::NextFloat(float min, float max)
{
	std::uniform_real_distribution<float> dis = std::uniform_real_distribution<float>(min, max);
	return dis(m_Twister);
}
