#include "letter.hpp"
#include "alphabet.hpp"

#include <stdexcept>

namespace rat
{
	namespace NameGenerator
	{
        const auto DefaultAlphabet = std::map<int, Letter>
        {
            // The letter 'A'
            {  0, Letter{ 0,  0, 26, true, false } },
            // The letter 'B'
            {  1, Letter{ 1,  1, 27, false, true } },
            // The letter 'C'
            {  2, Letter{ 2,  2, 28, false, true } },
            // The letter 'D'
            {  3, Letter{ 3,  3, 29, false, true } },
            // The letter 'E'
            {  4, Letter{ 4,  4, 30, true, false } },
            // The letter 'F'
            {  5, Letter{ 5,  5, 31, false, true } },
            // The letter 'G'
            {  6, Letter{ 6,  6, 32, false, true } },
            // The letter 'H'
            {  7, Letter{ 7,  7, 33, false, true } },
            // The letter 'I'
            {  8, Letter{ 8,  8, 34, true, false } },
            // The letter 'J'
            {  9, Letter{ 9,  9, 35, false, true } },
            // The letter 'K'
            { 10, Letter{ 10, 10, 36, false, true } },
            // The letter 'L'
            { 11, Letter{ 11, 11, 37, false, true } },
            // The letter 'M'
            { 12, Letter{ 12, 12, 38, false, true } },
            // The letter 'N'
            { 13, Letter{ 13, 13, 39, false, true } },
            // The letter 'O'
            { 14, Letter{ 14, 14, 40, true, false } },
            // The letter 'P'
            { 15, Letter{ 15, 15, 41, false, true } },
            // The letter 'Q'
            { 16, Letter{ 16, 16, 42, false, true } },
            // The letter 'R'
            { 17, Letter{ 17, 17, 43, false, true } },
            // The letter 'S'
            { 18, Letter{ 18, 18, 44, false, true } },
            // The letter 'T'
            { 19, Letter{ 19, 19, 45, false, true } },
            // The letter 'U'
            { 20, Letter{ 20, 20, 46, true, false } },
            // The letter 'V'
            { 21, Letter{ 21, 21, 47, false, true } },
            // The letter 'W'
            { 22, Letter{ 22, 22, 48, false, true } },
            // The letter 'X'
            { 23, Letter{ 23, 23, 49, false, true } },
            // The letter 'Y'
            { 24, Letter{ 24, 24, 50,  true, true } },
            // The letter 'Z'
            { 25, Letter{ 25, 25, 51, false, true } },
        };

        const auto ExtendedAlphabet = std::map<int, Letter>
        {
            // The letter 'A'
            {  0, Letter{ 0,  0, 26, true, false } },
            // The letter 'B'
            {  1, Letter{ 1,  1, 27, false, true } },
            // The letter 'C'
            {  2, Letter{ 2,  2, 28, false, true } },
            // The letter 'D'
            {  3, Letter{ 3,  3, 29, false, true } },
            // The letter 'E'
            {  4, Letter{ 4,  4, 30, true, false } },
            // The letter 'F'
            {  5, Letter{ 5,  5, 31, false, true } },
            // The letter 'G'
            {  6, Letter{ 6,  6, 32, false, true } },
            // The letter 'H'
            {  7, Letter{ 7,  7, 33, false, true } },
            // The letter 'I'
            {  8, Letter{ 8,  8, 34, true, false } },
            // The letter 'J'
            {  9, Letter{ 9,  9, 35, false, true } },
            // The letter 'K'
            { 10, Letter{ 10, 10, 36, false, true } },
            // The letter 'L'
            { 11, Letter{ 11, 11, 37, false, true } },
            // The letter 'M'
            { 12, Letter{ 12, 12, 38, false, true } },
            // The letter 'N'
            { 13, Letter{ 13, 13, 39, false, true } },
            // The letter 'O'
            { 14, Letter{ 14, 14, 40, true, false } },
            // The letter 'P'
            { 15, Letter{ 15, 15, 41, false, true } },
            // The letter 'Q'
            { 16, Letter{ 16, 16, 42, false, true } },
            // The letter 'R'
            { 17, Letter{ 17, 17, 43, false, true } },
            // The letter 'S'
            { 18, Letter{ 18, 18, 44, false, true } },
            // The letter 'T'
            { 19, Letter{ 19, 19, 45, false, true } },
            // The letter 'U'
            { 20, Letter{ 20, 20, 46, true, false } },
            // The letter 'V'
            { 21, Letter{ 21, 21, 47, false, true } },
            // The letter 'W'
            { 22, Letter{ 22, 22, 48, false, true } },
            // The letter 'X'
            { 23, Letter{ 23, 23, 49, false, true } },
            // The letter 'Y'
            { 24, Letter{ 24, 24, 50,  true, true } },
            // The letter 'Z'
            { 25, Letter{ 25, 25, 51, false, true } },

            // The letter 'A' with umlaut (Ä / ä)
            { 26, Letter{ 26, 52, 62, true, false } },
            // The letter 'E' with umlaut (Ë / ë)
            { 27, Letter{ 27, 53, 63, true, false } },
            // The letter 'I' with umlaut (Ï / ï)
            { 28, Letter{ 28, 54, 64, true, false } },
            // The letter 'O' with umlaut (Ö / ö)
            { 29, Letter{ 29, 55, 65, true, false } },
            // The letter 'U' with umlaut (Ü / ü)
            { 30, Letter{ 30, 56, 66, true, false } },

            // The letter 'A' with angstrom (Å / å)
            { 31, Letter{ 31, 57, 67, true, false } },
            // The 'thorn' letter (Ş / ş)
            { 32, Letter{ 32, 58, 68, true, false } },
            // The 'eth' letter (Ğ / ğ)
            { 33, Letter{ 33, 59, 69, true, false } },
            // The 'AE' ligature (Æ / æ)
            { 34, Letter{ 34, 60, 70, true, false } },
            // The 'OE' ligature (Ø / ø)
            { 35, Letter{ 35, 61, 71, true, false } },
        };

		Alphabet::Alphabet(const std::map<int, Letter>& letters) :
            m_Letters(letters) { }

		const Letter& Alphabet::operator[](int index) const
		{
            try { return m_Letters.at(index); }
            catch (std::out_of_range) { return null; }
		}

		bool Alphabet::Contains(int character) const
		{
            for (auto& pair : m_Letters)
            {
                auto& letter = pair.second;

                if (letter.uppercase == character || letter.undercase == character)
                    return true;
            }
            
            return false;
		}

        int Alphabet::FindIndex(int character) const
        {
            for (auto& pair : m_Letters)
            {
                auto& index = pair.first;
                auto& letter = pair.second;

                if (letter.uppercase == character || letter.undercase == character)
                    return index;
            }

            return -1;
        }

        int Alphabet::ToUpper(int character) const
        {
            for (auto& pair : m_Letters)
            {
                auto& index = pair.first;
                auto& letter = pair.second;

                if (letter.uppercase == character || letter.undercase == character)
                    return letter.uppercase;
            }
        }

        int Alphabet::ToUnder(int character) const
        {
            for (auto& pair : m_Letters)
            {
                auto& index = pair.first;
                auto& letter = pair.second;

                if (letter.uppercase == character || letter.undercase == character)
                    return letter.undercase;
            }
        }

		inline int Alphabet::Size() const { return m_Letters.size(); }
	}
}
