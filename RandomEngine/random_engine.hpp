#pragma once

#include "generator.hpp"

#include <stdexcept>

namespace rat
{
	namespace random
	{
		class RandomEngine
		{
		private:

			static RandomEngine* m_Instance;

		protected:

			RandomEngine(IGenerator* generator);

			~RandomEngine();

			std::unique_ptr<IGenerator> m_Generator;

		public:

			RandomEngine(RandomEngine& other) = delete;

			void operator=(const RandomEngine&) = delete;

			static IGenerator* GetGenerator();

			static void Initialize(const GeneratorTypes& type);
			static void Initialize(const GeneratorTypes& type, seed_t seed);
		};

		class RandomEngineUninitializedException : std::exception
		{
		private:
			const char* message;

		public:
			RandomEngineUninitializedException(const char* message);
			const char* what() const;
		};
	}
}
