#pragma once

#include <random>
#include <cstdint>

namespace rat
{
	namespace Random
	{
		class MTEngine
		{
		public:

			// Constructor with random seed and specified distribution
			MTEngine();

			// Constructor with specified seed and distribution
			MTEngine(uint64_t seed);

			// Destructor
			~MTEngine();

			void Seed();
			void Seed(uint64_t seed);

			// Generates a random integer between min and max
			int Next(int min, int max);
			// Generates a random bool of true or false
			bool NextBool();
			// Generates a random bool of true or false given a weight
			bool NextBool(double weight);
			// Generates a random double between 0.0 and 1.0
			double NextPercentile();
			// Generates a random double between min and max
			double NextDouble(double min, double max);
			// Generates a random float between min and max
			float NextFloat(float min, float max);

		private:
			// Pointer to the random device used to seed the engine
			std::random_device* ptr_Generator;

			// Pointer to the engine
			std::mt19937_64* ptr_Twister;
		};

		extern MTEngine* Generator;
		extern bool m_Initialized;

		MTEngine* GetGenerator();

		void Initialize();
		void Initialize(uint64_t seed);

		bool Initialized();

		void Close();
	}
}