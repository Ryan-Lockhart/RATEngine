#include "mt_engine.hpp"

namespace rat
{
	namespace Random
	{
		MTEngine* Generator = nullptr;
		bool m_Initialized = false;

		MTEngine* GetGenerator()
		{
			if (!m_Initialized)
				Initialize();

			return Generator;
		}

		/// <summary>
		/// Constructor with random seed and specified distribution
		/// </summary>
		/// <param name="min">Lowest possible value</param>
		/// <param name="max">Highest possible value</param>
		void Initialize()
		{
			if (!m_Initialized)
			{
				Generator = new MTEngine();
				m_Initialized = true;
			}
		}

		/// <summary>
		/// Constructor with specified seed and distribution
		/// </summary>
		/// <param name="seed">The initial seed for the mersennes' twister</param>
		/// <param name="min">Lowest possible value</param>
		/// <param name="max">Highest possible value</param>
		void Initialize(uint64_t seed)
		{
			if (!m_Initialized)
			{
				Generator = new MTEngine(seed); // Seeds the engine with the specified seed
				m_Initialized = true;
			}
		}

		bool Initialized() { return m_Initialized; }

		void Close()
		{
			delete Generator;
		}

		MTEngine::MTEngine()
		{
			ptr_Generator = new std::random_device();
			ptr_Twister = new std::mt19937_64((*ptr_Generator)());
		}

		MTEngine::MTEngine(uint64_t seed)
		{
			ptr_Generator = new std::random_device();
			ptr_Twister = new std::mt19937_64(seed);
		}

		/// <summary>
		/// Clean up the instance when it goes out of scope
		/// </summary>
		MTEngine::~MTEngine()
		{
			delete ptr_Generator;
			delete ptr_Twister;
		}		

		/// <summary>
		/// Re-seeds the mersennes' twister with a random seed
		/// </summary>
		void MTEngine::Seed()
		{
			if (!m_Initialized)
				Initialize();

			ptr_Twister->seed((*ptr_Generator)());
		}

		/// <summary>
		/// Re-seeds the mersennes' twister with a specified seed
		/// </summary>
		/// <param name="seed">The value used to re-seed</param>
		void MTEngine::Seed(size_t seed)
		{
			if (!m_Initialized)
				Initialize();

			ptr_Twister->seed(seed);
		}

		/// <summary>
		/// Retrieves the next random integer from specified distribution
		/// </summary>
		/// <param name="min">Lowest possible value</param>
		/// <param name="max">Highest possible value</param>
		/// <returns>A random integer from mt19937</returns>
		int MTEngine::Next(int min, int max)
		{
			if (!m_Initialized)
				Initialize();

			std::uniform_int_distribution<int> dis;
			if (min > max)
				dis = std::uniform_int_distribution<int>(max, min);
			else dis = std::uniform_int_distribution<int>(min, max);

			return dis(*ptr_Twister);
		}

		/// <summary>
		/// Retrieves the next random bool
		/// </summary>
		/// <returns>A random bool from mt19937</returns>
		bool MTEngine::NextBool()
		{
			if (!m_Initialized)
				Initialize();

			std::bernoulli_distribution dis = std::bernoulli_distribution();
			return dis(*ptr_Twister);
		}

		/// <summary>
		/// Retrieves the next random bool with given weight
		/// </summary>
		/// <param name="weight">The likelihood that a bool will be true</param>
		/// <returns>A random bool from mt19937</returns>
		bool MTEngine::NextBool(double weight)
		{
			if (!m_Initialized)
				Initialize();

			std::bernoulli_distribution dis = std::bernoulli_distribution(weight);
			return dis(*ptr_Twister);
		}

		/// <summary>
		/// Retrieves the next random percentile from specified distribution
		/// </summary>
		/// <param name="min">Lowest possible value</param>
		/// <param name="max">Highest possible value</param>
		/// <returns>A random float from mt19937</returns>
		double MTEngine::NextPercentile()
		{
			if (!m_Initialized)
				Initialize();

			std::uniform_real_distribution<double> dis = std::uniform_real_distribution<double>(0.0, 1.0);
			return dis(*ptr_Twister);
		}

		/// <summary>
		/// Retrieves the next random double from specified distribution
		/// </summary>
		/// <param name="min">Lowest possible value</param>
		/// <param name="max">Highest possible value</param>
		/// <returns>A random float from mt19937</returns>
		double MTEngine::NextDouble(double min, double max)
		{
			if (!m_Initialized)
				Initialize();

			std::uniform_real_distribution<double> dis = std::uniform_real_distribution<double>(min, max);
			return dis(*ptr_Twister);
		}

		/// <summary>
		/// Retrieves the next random float from specified distribution
		/// </summary>
		/// <param name="min">Lowest possible value</param>
		/// <param name="max">Highest possible value</param>
		/// <returns>A random float from mt19937</returns>
		float MTEngine::NextFloat(float min, float max)
		{
			if (!m_Initialized)
				Initialize();

			std::uniform_real_distribution<float> dis = std::uniform_real_distribution<float>(min, max);
			return dis(*ptr_Twister);
		}
	}
}
