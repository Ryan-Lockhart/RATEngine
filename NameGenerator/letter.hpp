#pragma once

namespace rat
{
	namespace Random
	{
		class IGenerator;
	}

	namespace NameGenerator
	{
		class Alphabet;
		class ProbabilityMatrix;

		struct Letter
		{
		public:

			const int index;

			const int uppercase;
			const int undercase;

			const bool vowel;
			const bool consonant;

			bool IsValid() const;

			static Letter GetRandom(bool needConsonant, bool needVowel, const Alphabet& alphabet, Random::IGenerator& generator);
			static Letter GetRandom(int prevNum, bool needConsonant, bool needVowel, const Alphabet& alphabet, const ProbabilityMatrix& matrix, Random::IGenerator& generator);
			static Letter GetWeighted(int i, const Alphabet& alphabet, const ProbabilityMatrix& matrix, Random::IGenerator& generator);

			bool operator==(const Letter& other) const;
			friend bool operator==(const Letter& a, const Letter& b);
		};

		constexpr Letter null{ -1, -1, -1, false, false };
	}
}
