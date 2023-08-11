#include "name_list.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

namespace rat
{
	namespace NameGenerator
	{
		NameList::NameList()
		{
			
		}

		NameList::NameList(const std::string& path)
		{
			Load(path);
		}

		NameList::NameList(const std::list<std::string>& names)
		{
			for (auto& name : names)
				if (name != "")
					m_Names.push_back(name);
		}

		void NameList::Load(const std::string& path, bool overwrite)
		{
			if (overwrite) m_Names.clear();

			std::ifstream reader(path);
			std::string line;

			while (std::getline(reader, line, ';'))
			{
				if (line != "" && line != "\n")
					m_Names.push_back(line);
			}

			reader.close();

			Sort();
		}

		const std::list<std::string>& NameList::Expose()
		{
			return m_Names;
		}

		void NameList::Save(const std::string& path)
		{
			Sort();

			std::ofstream writer(path);

			char previousLetter = '\0';

			for (auto& name : m_Names)
			{
				if (name == "") break;

				if (std::tolower(name[0]) != std::tolower(previousLetter) && previousLetter != '\0')
					writer << std::endl;

				writer << name + ";";

				previousLetter = name[0];
			}

			writer.close();
		}

		void NameList::Add(const std::string& name)
		{
			m_Names.push_back(name);
		}

		void NameList::Remove(const std::string& name)
		{
			m_Names.remove(name);
		}

		ProbabilityMatrix NameList::CreateMatrix()
		{
			auto weights = new double[NameGenerator::DefaultAlphabet, Globals.Alphabet.Size];

			for (auto& name : m_Names)
			{
				for (int i = 0; i < name.size() - 1; i++)
				{
					Letter letter_one = Globals.Alphabet[name[i]];
					Letter letter_two = Globals.Alphabet[name[i + 1]];

					weights[letter_one.Index, letter_two.Index]++;
				}
			}

			return new ProbabilityMatrix(ref weights);
		}

		void NameList::Sort()
		{
			m_Names.sort();
		}
	}
}
