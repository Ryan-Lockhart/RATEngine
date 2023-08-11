#pragma once

#include <list>
#include <string>

#include "probability_matrix.hpp"

namespace rat
{
	namespace NameGenerator
	{
		class NameList
		{
        public:

            NameList();
            NameList(const std::string& path);
            NameList(const std::list<std::string>& names);

            void Load(const std::string& path, bool overwrite = true);

            const std::list<std::string>& Expose();

            void Save(const std::string& path);

            void Add(const std::string& name);

            void Remove(const std::string& name);

            ProbabilityMatrix CreateMatrix();

            void Sort();

            size_t Size() const { return m_Names.size(); }

        private:

            std::list<std::string> m_Names;

		};
	}
}
