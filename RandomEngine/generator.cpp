#include "generator.hpp"

#include "mt_engine.hpp"
#include "lcg_engine.hpp"
#include "fib_engine.hpp"

namespace rat
{
	namespace random
	{
		void IGenerator::SetSeed(seed_t seed) { m_Seed = seed; }

		seed_t IGenerator::GetSeed() const { return m_Seed; }

		IGenerator::IGenerator() :
			m_Seed(std::random_device()()) { }

		IGenerator::IGenerator(seed_t seed) :
			m_Seed(seed) { }

		template<typename T>
		inline void IGenerator::CheckRange(T& min, T& max)
		{
			if (min < max) return;
			else if (min > max) std::swap(min, max);
			else throw new InvalidRangeException("An invalid range was given for random generation!");
		}

		InvalidRangeException::InvalidRangeException(const char* message) :
			message(message) { }

		const char* InvalidRangeException::what() const { return message; }
	}
}
