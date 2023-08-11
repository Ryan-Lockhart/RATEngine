#pragma once

#include <random>
#include <mutex>
#include <memory>

namespace rat
{
	namespace random
	{
		using seed_t = uint64_t;

		enum class GeneratorTypes
		{
			MTEngine,
			LCGEngine,
			FibonacciEngine
		};

		/// <summary>
		/// A pure virtual base class for RATEngine psuedo random number generation
		/// </summary>
		class IGenerator
		{
		public:

			IGenerator();
			IGenerator(seed_t seed);

			/// <summary>
			/// Generate a single random integer value without a range
			/// </summary>
			/// <returns>A random integer</returns>
			virtual int NextInt() = 0;

			/// <summary>
			/// Generate a single integer value within a range
			/// </summary>
			/// <param name="min">The minimum (inclusive) integer value that can be generated</param>
			/// <param name="max">The maximum (exclusive) integer value that can be generated</param>
			/// <returns></returns>
			virtual int NextInt(int min, int max) = 0;

			/// <summary>
			/// Generate a single random floating point value without a range
			/// </summary>
			/// <returns>A random integer</returns>
			virtual float NextFloat() = 0;

			/// <summary>
			/// Generate a single floating point value within a range
			/// </summary>
			/// <param name="min">The minimum (inclusive) floating point value that can be generated</param>
			/// <param name="max">The maximum (exclusive) floating point value that can be generated</param>
			/// <returns></returns>
			virtual float NextFloat(float min, float max) = 0;

			/// <summary>
			/// Generate a single random double presicion floating point value without a range
			/// </summary>
			/// <returns>A random integer</returns>
			virtual double NextDouble() = 0;

			/// <summary>
			/// Generate a single double presicion floating point value within a range
			/// </summary>
			/// <param name="min">The minimum (inclusive) double presicion floating point value that can be generated</param>
			/// <param name="max">The maximum (exclusive) double presicion floating point value that can be generated</param>
			/// <returns></returns>
			virtual double NextDouble(double min, double max) = 0;

			/// <summary>
			/// Generate a single boolean without any weighted probability
			/// </summary>
			/// <returns>A random integer</returns>
			virtual bool NextBool() = 0;

			/// <summary>
			/// Generate a single boolean with a weighted probability
			/// </summary>
			/// <param name="probability">The weighted probability of generating a true value</param>
			/// <returns></returns>
			virtual bool NextBool(double probability) = 0;

			/// <summary>
			/// Generate a collection of integer values without a range and return by value
			/// </summary>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <returns>A collection of integer values</returns>
			virtual std::vector<int> Ints(int amount) = 0;

			/// <summary>
			/// Generate a collection of integer values without a range using a supplied collection
			/// </summary>
			/// <param name="vec">The supplied collection to be modified</param>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <param name="append"></param>
			virtual void Ints(std::vector<int>& vec, int amount, bool append) = 0;

			/// <summary>
			/// Generate a collection of integer values with a range and return by value
			/// </summary>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <param name="min"></param>
			/// <param name="max"></param>
			/// <returns>A collection of integer values</returns>
			virtual std::vector<int> Ints(int amount, int min, int max) = 0;

			/// <summary>
			/// Generate a collection of integer values with a range using a supplied collection
			/// </summary>
			/// <param name="vec">The supplied collection to be modified</param>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <param name="min"></param>
			/// <param name="max"></param>
			/// <param name="append"></param>
			virtual void Ints(std::vector<int>& vec, int amount, int min, int max, bool append) = 0;

			/// <summary>
			/// Generate a collection of floating point values without a range and return by value
			/// </summary>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <returns>A collection of floating point values</returns>
			virtual std::vector<float> Floats(int amount) = 0;

			/// <summary>
			/// Generate a collection of floating point values without a range using a supplied collection
			/// </summary>
			/// <param name="vec">The supplied collection to be modified</param>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <param name="append"></param>
			virtual void Floats(std::vector<float>& vec, int amount, bool append) = 0;

			/// <summary>
			/// Generate a collection of floating point values with a range and return by value
			/// </summary>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <param name="min"></param>
			/// <param name="max"></param>
			/// <returns>A collection of floating point values</returns>
			virtual std::vector<float> Floats(int amount, float min, float max) = 0;

			/// <summary>
			/// Generate a collection of floating point values with a range using a supplied collection
			/// </summary>
			/// <param name="vec">The supplied collection to be modified</param>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <param name="min"></param>
			/// <param name="max"></param>
			/// <param name="append"></param>
			virtual void Floats(std::vector<float>& vec, int amount, float min, float max, bool append) = 0;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <returns></returns>
			virtual std::vector<double> Doubles(int amount) = 0;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="vec"></param>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <param name="append"></param>
			virtual void Doubles(std::vector<double>& vec, int amount, bool append) = 0;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <param name="min"></param>
			/// <param name="max"></param>
			/// <returns></returns>
			virtual std::vector<double> Doubles(int amount, double min, double max) = 0;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="vec"></param>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <param name="min"></param>
			/// <param name="max"></param>
			/// <param name="append"></param>
			virtual void Doubles(std::vector<double>& vec, int amount, double min, double max, bool append) = 0;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <returns></returns>
			virtual std::vector<bool> Bools(int amount) = 0;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="vec"></param>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <param name="append"></param>
			virtual void Bools(std::vector<bool>& vec, int amount, bool append) = 0;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <param name="probability"></param>
			/// <returns></returns>
			virtual std::vector<bool> Bools(int amount, double probability) = 0;

			/// <summary>
			/// 
			/// </summary>
			/// <param name="vec"></param>
			/// <param name="amount">The size of the collection to be generated</param>
			/// <param name="probability"></param>
			/// <param name="append"></param>
			virtual void Bools(std::vector<bool>& vec, int amount, double probability, bool append) = 0;

		protected:

			virtual void SetSeed(seed_t seed);
			virtual seed_t GetSeed() const;

			template<typename T>
			void CheckRange(T& min, T& max);

		private:

			seed_t m_Seed;

		};

		class InvalidRangeException : std::exception
		{
		private:
			const char* message;

		public:
			InvalidRangeException(const char* message);
			const char* what() const;
		};

		template void IGenerator::CheckRange<int>(int&, int&);
		template void IGenerator::CheckRange<float>(float&, float&);
		template void IGenerator::CheckRange<double>(double&, double&);
}
}
