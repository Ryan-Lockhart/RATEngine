#include "mt_engine.hpp"

namespace rat
{
	namespace random
	{
        MTEngine::MTEngine() :
            IGenerator(), m_Generator(GetSeed()) { }

        MTEngine::MTEngine(seed_t seed) :
            IGenerator(seed), m_Generator(GetSeed()) { }

        void MTEngine::SetSeed(seed_t seed)
        {
            IGenerator::SetSeed(seed);
            m_Generator.seed(seed);
        }

        seed_t MTEngine::GetSeed() const { return IGenerator::GetSeed(); }

        int MTEngine::NextInt() { return std::uniform_int_distribution<int>()(m_Generator); }

        int MTEngine::NextInt(int min, int max)
        {
            try { CheckRange<int>(min, max); }
            catch (InvalidRangeException) { if (min == max) return min; }

            return std::uniform_int_distribution<int>(min, max)(m_Generator);
        }

        float MTEngine::NextFloat() { return std::uniform_real_distribution<float>()(m_Generator); }

        float MTEngine::NextFloat(float min, float max) { CheckRange<float>(min, max); return std::uniform_real_distribution<float>(min, max)(m_Generator); }

        double MTEngine::NextDouble() { return std::uniform_real_distribution<double>()(m_Generator); }

        double MTEngine::NextDouble(double min, double max) { CheckRange<double>(min, max); return std::uniform_real_distribution<double>(min, max)(m_Generator); }

        bool MTEngine::NextBool() { return std::bernoulli_distribution()(m_Generator); }

        bool MTEngine::NextBool(double probability) { return std::bernoulli_distribution(probability)(m_Generator); }

        std::vector<int> MTEngine::Ints(int amount)
        {
            auto distribution = std::uniform_int_distribution<int>();
            auto int_vec = std::vector<int>(amount);

            for (int i = 0; i < amount; i++)
                int_vec[i] = distribution(m_Generator);

            return int_vec;
        }

        void MTEngine::Ints(std::vector<int>& vec, int amount, bool append)
        {
            auto distribution = std::uniform_int_distribution<int>();

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }

        std::vector<int> MTEngine::Ints(int amount, int min, int max)
        {
            CheckRange<int>(min, max);

            auto distribution = std::uniform_int_distribution<int>(min, max);
            auto int_vec = std::vector<int>(amount);

            for (int i = 0; i < amount; i++)
                int_vec[i] = distribution(m_Generator);

            return int_vec;
        }

        void MTEngine::Ints(std::vector<int>& vec, int amount, int min, int max, bool append)
        {
            CheckRange<int>(min, max);

            auto distribution = std::uniform_int_distribution<int>(min, max);

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }

        std::vector<float> MTEngine::Floats(int amount)
        {
            auto distribution = std::uniform_real_distribution<float>();
            auto float_vec = std::vector<float>(amount);

            for (int i = 0; i < amount; i++)
                float_vec[i] = distribution(m_Generator);

            return float_vec;
        }

        void MTEngine::Floats(std::vector<float>& vec, int amount, bool append)
        {
            auto distribution = std::uniform_real_distribution<float>();

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }

        std::vector<float> MTEngine::Floats(int amount, float min, float max)
        {
            CheckRange<float>(min, max);

            auto distribution = std::uniform_real_distribution<float>(min, max);
            auto float_vec = std::vector<float>(amount);

            for (int i = 0; i < amount; i++)
                float_vec[i] = distribution(m_Generator);

            return float_vec;
        }

        void MTEngine::Floats(std::vector<float>& vec, int amount, float min, float max, bool append)
        {
            CheckRange<float>(min, max);

            auto distribution = std::uniform_real_distribution<float>(min, max);

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }

        std::vector<double> MTEngine::Doubles(int amount)
        {
            auto distribution = std::uniform_real_distribution<double>();
            auto double_vec = std::vector<double>(amount);

            for (int i = 0; i < amount; i++)
                double_vec[i] = distribution(m_Generator);

            return double_vec;
        }

        void MTEngine::Doubles(std::vector<double>& vec, int amount, bool append)
        {
            auto distribution = std::uniform_real_distribution<double>();

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }

        std::vector<double> MTEngine::Doubles(int amount, double min, double max)
        {
            CheckRange<double>(min, max);

            auto distribution = std::uniform_real_distribution<double>(min, max);
            auto double_vec = std::vector<double>(amount);

            for (int i = 0; i < amount; i++)
                double_vec[i] = distribution(m_Generator);

            return double_vec;
        }

        void MTEngine::Doubles(std::vector<double>& vec, int amount, double min, double max, bool append)
        {
            CheckRange<double>(min, max);

            auto distribution = std::uniform_real_distribution<double>(min, max);

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }

        std::vector<bool> MTEngine::Bools(int amount)
        {
            auto distribution = std::bernoulli_distribution();
            auto bool_vec = std::vector<bool>(amount);

            for (int i = 0; i < amount; i++)
                bool_vec[i] = distribution(m_Generator);

            return bool_vec;
        }

        void MTEngine::Bools(std::vector<bool>& vec, int amount, bool append)
        {
            auto distribution = std::bernoulli_distribution();

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }

        std::vector<bool> MTEngine::Bools(int amount, double probability)
        {
            auto distribution = std::bernoulli_distribution(probability);
            auto bool_vec = std::vector<bool>(amount);

            for (int i = 0; i < amount; i++)
                bool_vec[i] = distribution(m_Generator);

            return bool_vec;
        }

        void MTEngine::Bools(std::vector<bool>& vec, int amount, double probability, bool append)
        {
            auto distribution = std::bernoulli_distribution(probability);

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }
	}
}
