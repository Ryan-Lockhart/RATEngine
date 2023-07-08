#include "name_list.hpp"

#include <iostream>
#include <fstream>

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

		NameList::NameList(const std::vector<std::string>& names)
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

		const std::vector<std::string>& NameList::Expose()
		{
			
		}

		void NameList::Save(const std::string& path)
		{
			Sort();

			char previousLetter = '\0';

			for (auto& name : m_Names)
			{
				if (name == "") break;

				if (char.ToLower(name[0]) != char.ToLower(previousLetter) && previousLetter != '\0')
					writer.WriteLine();

				writer.Write(name + ";");

				previousLetter = name[0];
			}
		}

		void NameList::Add(const std::string& name)
		{

		}

		void NameList::Remove(const std::string& name)
		{

		}

		ProbabilityMatrix NameList::CreateMatrix()
		{

		}

		void NameList::Sort()
		{

		}
	}
}
