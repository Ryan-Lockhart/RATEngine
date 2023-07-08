#include "probability_matrix.hpp"
#include <cmath>
#include <algorithm>

#include <iostream>
#include <fstream>

#include "alphabet.hpp"
#include "name_list.hpp"

namespace rat
{
	namespace NameGenerator
	{
		void ProbabilityMatrix::Normalize()
		{
			for (int i = 0; i < m_Alphabet.Size(); i++)
				NormalizeIndex(i);
		}

		void ProbabilityMatrix::NormalizeIndex(int index)
		{
			double count = 0.0;

			for (int j = 0; j < m_Alphabet.Size(); j++)
				count += at(index, j);

			if (count > 0.0)
				for (int j = 0; j < m_Alphabet.Size(); j++)
					at(index, j) /= count;
			else
				for (int j = 0; j < m_Alphabet.Size(); j++)
					at(index, j) = std::pow(m_Alphabet.Size(), -1);
		}

		ProbabilityMatrix::ProbabilityMatrix(const std::string& path, const Alphabet& alphabet) :
			m_Alphabet(alphabet)
		{
			Reset();
			Load(path);
		}

		ProbabilityMatrix::ProbabilityMatrix(const std::vector<double>& weights, const Alphabet& alphabet) :
			m_Alphabet(alphabet)
		{
			for (int i = 0; i < m_Alphabet.Size(); i++)
				for (int j = 0; j < m_Alphabet.Size(); j++)
					at(i, j) = weights[i, j];

			Normalize();
		}

		ProbabilityMatrix::ProbabilityMatrix(const NameList& names, const Alphabet& alphabet) :
			m_Alphabet(alphabet)
		{
			Reset();
			Generate(names);
		}

		void ProbabilityMatrix::Reset()
		{
			for (int i = 0; i < m_Alphabet.Size(); i++)
				for (int j = 0; j < m_Alphabet.Size(); j++)
					at(i, j) = 0.0;

			Normalize();
		}

		void ProbabilityMatrix::Load(const std::vector<std::string>& names)
		{
			for (int i = 0; i < names.size(); i++)
			{
				const std::string name = names[i];

				if (name.size() < 1) return;

				for (int j = 0; j < name.size() - 1; j++)
				{
					int first_index = m_Alphabet.FindIndex(name[j]);
					int second_index = m_Alphabet.FindIndex(name[j + 1]);

					at(first_index, second_index)++;
				}
			}

			Normalize();
		}

		void ProbabilityMatrix::Load(const std::string& path)
		{
			std::ifstream reader(path);
			std::string line;
			std::vector<std::string> weights;

			while (std::getline(reader, line, ';'))
			{
				if (line != "" && line != "\n")
					weights.push_back(line);
			}

			reader.close();

			int count = 0;

			for (int i = 0; i < m_Alphabet.Size(); i++)
				for (int j = 0; j < m_Alphabet.Size(); j++)
				{
					at(i, j) = std::stod(weights[count]);

					count++;
				}

			Normalize();
		}

		void ProbabilityMatrix::Save(const std::string& path)
		{
			Normalize();

			std::ofstream writer(path);

			for (int i = 0; i < m_Alphabet.Size(); i++)
			{
				std::string line;

				for (int j = 0; j < m_Alphabet.Size(); j++)
				{
					if (j != 0)
						line += ";";

					line += std::to_string(at(i, j));
				}

				writer << line;
			}
		}

		void ProbabilityMatrix::Generate(const NameList& names)
		{
			for (int i = 0; i < names.Size(); i++)
			{
				const std::string& name = names[i];

				for (int j = 0; j < name.size() - 1; j++)
				{
					int first_index = m_Alphabet.FindIndex(name[j]);
					int second_index = m_Alphabet.FindIndex(name[j + 1]);

					at(first_index, second_index)++;
				}
			}

			Normalize();
		}

		void ProbabilityMatrix::Modulate(const std::string& name, double value)
		{
			for (int i = 0; i < name.size() - 1; i++)
				at(name[i], name[i + 1]) *= std::clamp<double>(value, 0.0, 2.0);

			Normalize();
		}

		double ProbabilityMatrix::at(int i, int j) const
		{
			return m_Weights[j * m_Alphabet.Size() + i];
		}
		double& ProbabilityMatrix::at(int i, int j)
		{
			return m_Weights[j * m_Alphabet.Size() + i];
		}
	}
}
