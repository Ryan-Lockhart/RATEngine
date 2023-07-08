#include "letter.hpp"
#include "alphabet.hpp"
#include "probability_matrix.hpp"
#include "generator.hpp"

namespace rat
{
	namespace NameGenerator
	{
		bool Letter::IsValid() const
		{
			if (index < 0)
				return false;
			else if (uppercase < 0)
				return false;
			else if (undercase < 0)
				return false;
			else if (vowel == false && consonant == false)
				return false;
			else return true;
		}

		Letter Letter::GetRandom(bool needConsonant, bool needVowel, const Alphabet& alphabet, Random::IGenerator& generator)
		{
			int tries{ 0 };
			const int maxTries = alphabet.Size();

			while (tries < maxTries)
			{
				Letter ret = alphabet[generator.NextInt(0, 27)];

				if ((needConsonant && ret.vowel) || (needVowel && ret.consonant))
					return ret;
				else tries++;
			}

			return null;
		}

		Letter Letter::GetRandom(int prevNum, bool needConsonant, bool needVowel, const Alphabet& alphabet, const ProbabilityMatrix& matrix, Random::IGenerator& generator)
		{
			int tries{ 0 };
			const int maxTries = alphabet.Size();

			while (tries < maxTries)
			{
				Letter ret = GetWeighted(prevNum, alphabet, matrix, generator);

				if ((needConsonant && ret.vowel) || (needVowel && ret.consonant))
					return ret;
				else tries++;
			}

			return null;
		}

		Letter Letter::GetWeighted(int i, const Alphabet& alphabet, const ProbabilityMatrix& matrix, Random::IGenerator& generator)
		{
			double rand = generator.NextDouble(0.0, 1.0);
			double total = 0.0;

			for (int j = 0; j < alphabet.Size(); j++)
			{
				total += matrix.at(i, j);

				if (rand <= total || j == alphabet.Size())
					return alphabet[j];
			}

			return null;
		}

		bool Letter::operator==(const Letter& other) const
		{
			return
				vowel == other.vowel &&
				consonant == other.consonant &&
				index == other.index &&
				uppercase == other.uppercase &&
				undercase == other.undercase;
		}

		bool operator==(const Letter& a, const Letter& b)
		{
			return
				a.vowel == b.vowel &&
				a.consonant == b.consonant &&
				a.index == b.index &&
				a.uppercase == b.uppercase &&
				a.undercase == b.undercase;
		}
	}
}
