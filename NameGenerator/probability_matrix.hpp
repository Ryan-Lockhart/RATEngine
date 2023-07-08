#pragma once

#include <vector>
#include <string>

namespace rat
{
	namespace NameGenerator
	{
        class Alphabet;
        class NameList;

		class ProbabilityMatrix
		{
		public:

            ProbabilityMatrix(const std::string& path, const Alphabet& alphabet);
            ProbabilityMatrix(const std::vector<double>& weights, const Alphabet& alphabet);
            ProbabilityMatrix(const NameList& names, const Alphabet& alphabet);

            void Reset();
            void Load(const std::vector<std::string>& names);
            void Load(const std::string& path);
            void Save(const std::string& path);

            void Generate(const NameList& names);
            void Modulate(const std::string& name, double value);

            double at(int i, int j) const;
            double& at(int i, int j);

		private:

			std::vector<double> m_Weights;
            const Alphabet& m_Alphabet;

            void Normalize();
            void NormalizeIndex(int index);

		};
	}
}
