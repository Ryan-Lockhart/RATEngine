#pragma once

#include "generator.hpp"

namespace rat
{
	namespace random
	{
		class MTEngine : public IGenerator
		{
		public:

			MTEngine();
			MTEngine(seed_t seed);

			virtual int NextInt() override;
			virtual int NextInt(int min, int max) override;

			virtual float NextFloat() override;
			virtual float NextFloat(float min, float max) override;

			virtual double NextDouble() override;
			virtual double NextDouble(double min, double max) override;

			virtual bool NextBool() override;
			virtual bool NextBool(double probability) override;

			virtual std::vector<int> Ints(int amount) override;
			virtual void Ints(std::vector<int>& vec, int amount, bool append) override;

			virtual std::vector<int> Ints(int amount, int min, int max) override;
			virtual void Ints(std::vector<int>& vec, int amount, int min, int max, bool append) override;

			virtual std::vector<float> Floats(int amount) override;
			virtual void Floats(std::vector<float>& vec, int amount, bool append) override;

			virtual std::vector<float> Floats(int amount, float min, float max) override;
			virtual void Floats(std::vector<float>& vec, int amount, float min, float max, bool append) override;

			virtual std::vector<double> Doubles(int amount) override;
			virtual void Doubles(std::vector<double>& vec, int amount, bool append) override;

			virtual std::vector<double> Doubles(int amount, double min, double max) override;
			virtual void Doubles(std::vector<double>& vec, int amount, double min, double max, bool append) override;

			virtual std::vector<bool> Bools(int amount) override;
			virtual void Bools(std::vector<bool>& vec, int amount, bool append) override;

			virtual std::vector<bool> Bools(int amount, double probability) override;
			virtual void Bools(std::vector<bool>& vec, int amount, double probability, bool append) override;

		protected:

			virtual void SetSeed(seed_t seed) override;
			virtual seed_t GetSeed() const override;

		private:

			std::mt19937_64 m_Generator;

		};
	}
}
