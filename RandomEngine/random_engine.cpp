#include "random_engine.hpp"

#include "mt_engine.hpp"
#include "lcg_engine.hpp"
#include "fib_engine.hpp"

namespace rat
{
	namespace random
	{
		RandomEngine* RandomEngine::m_Instance { nullptr };

		RandomEngine::RandomEngine(IGenerator* generator) :
			m_Generator(generator) { }

		RandomEngine::~RandomEngine()
		{ }

		void RandomEngine::Initialize(const GeneratorTypes& type)
		{
			if (m_Instance == nullptr)
			{
				switch (type)
				{
				case GeneratorTypes::MTEngine:
					m_Instance = new RandomEngine{ new MTEngine() };
					break;
				case GeneratorTypes::LCGEngine:
					m_Instance = new RandomEngine{ new LCGEngine() };
					break;
				case GeneratorTypes::FibonacciEngine:
					m_Instance = new RandomEngine{ new FibonacciEngine() };
					break;
				}
			}			
		}

		void RandomEngine::Initialize(const GeneratorTypes& type, seed_t seed)
		{
			if (m_Instance == nullptr)
			{
				switch (type)
				{
				case GeneratorTypes::MTEngine:
					m_Instance = new RandomEngine{ new MTEngine(seed) };
					break;
				case GeneratorTypes::LCGEngine:
					m_Instance = new RandomEngine{ new LCGEngine(seed) };
					break;
				case GeneratorTypes::FibonacciEngine:
					m_Instance = new RandomEngine{ new FibonacciEngine(seed) };
					break;
				}
			}
		}

		IGenerator* RandomEngine::GetGenerator()
		{
			if (m_Instance == nullptr) throw RandomEngineUninitializedException("Random Engine is uninitialized!");
			return m_Instance->m_Generator.get();
		}

		RandomEngineUninitializedException::RandomEngineUninitializedException( const char* message) :
			message(message) { }

		const char* RandomEngineUninitializedException::what() const { return message; }
	}
}
