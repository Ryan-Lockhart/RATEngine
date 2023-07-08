#pragma once

#include <map>

#include "letter.hpp"

namespace rat
{
	namespace NameGenerator
	{
		class Alphabet
		{
		public:

			Alphabet(const std::map<int, Letter>& letters);

			const Letter& operator[](int index) const;

			bool Contains(int character) const;

			int FindIndex(int character) const;

			int ToUpper(int character) const;
			int ToUnder(int character) const;

			int Size() const;

		private:

			std::map<int, Letter> m_Letters;

		};
	}
}
