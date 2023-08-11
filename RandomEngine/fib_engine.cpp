#include "fib_engine.hpp"

namespace rat
{
    namespace random
    {
        FibonacciEngine::FibonacciEngine() :
            IGenerator(), m_Generator(GetSeed()) { }

        FibonacciEngine::FibonacciEngine(seed_t seed) :
            IGenerator(seed), m_Generator(GetSeed()) { }

        void FibonacciEngine::SetSeed(seed_t seed)
        {
            IGenerator::SetSeed(seed);
            m_Generator.seed(seed);
        }

        seed_t FibonacciEngine::GetSeed() const { return IGenerator::GetSeed(); }

        int FibonacciEngine::NextInt() { return std::uniform_int_distribution<int>()(m_Generator); }

        int FibonacciEngine::NextInt(int min, int max) { return std::uniform_int_distribution<int>(min, max)(m_Generator); }

        float FibonacciEngine::NextFloat() { return std::uniform_real_distribution<float>()(m_Generator); }

        float FibonacciEngine::NextFloat(float min, float max) { return std::uniform_real_distribution<float>(min, max)(m_Generator); }

        double FibonacciEngine::NextDouble() { return std::uniform_real_distribution<double>()(m_Generator); }

        double FibonacciEngine::NextDouble(double min, double max) { return std::uniform_real_distribution<double>(min, max)(m_Generator); }

        bool FibonacciEngine::NextBool() { return std::bernoulli_distribution()(m_Generator); }

        bool FibonacciEngine::NextBool(double probability) { return std::bernoulli_distribution(probability)(m_Generator); }

        std::vector<int> FibonacciEngine::Ints(int amount)
        {
            auto distribution = std::uniform_int_distribution<int>();
            auto int_vec = std::vector<int>(amount);

            for (int i = 0; i < amount; i++)
                int_vec[i] = distribution(m_Generator);

            return int_vec;
        }

        void FibonacciEngine::Ints(std::vector<int>& vec, int amount, bool append)
        {
            auto distribution = std::uniform_int_distribution<int>();

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }

        std::vector<int> FibonacciEngine::Ints(int amount, int min, int max)
        {
            auto distribution = std::uniform_int_distribution<int>(min, max);
            auto int_vec = std::vector<int>(amount);

            for (int i = 0; i < amount; i++)
                int_vec[i] = distribution(m_Generator);

            return int_vec;
        }

        void FibonacciEngine::Ints(std::vector<int>& vec, int amount, int min, int max, bool append)
        {
            auto distribution = std::uniform_int_distribution<int>(min, max);

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }

        std::vector<float> FibonacciEngine::Floats(int amount)
        {
            auto distribution = std::uniform_real_distribution<float>();
            auto float_vec = std::vector<float>(amount);

            for (int i = 0; i < amount; i++)
                float_vec[i] = distribution(m_Generator);

            return float_vec;
        }

        void FibonacciEngine::Floats(std::vector<float>& vec, int amount, bool append)
        {
            auto distribution = std::uniform_real_distribution<float>();

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }

        std::vector<float> FibonacciEngine::Floats(int amount, float min, float max)
        {
            auto distribution = std::uniform_real_distribution<float>(min, max);
            auto float_vec = std::vector<float>(amount);

            for (int i = 0; i < amount; i++)
                float_vec[i] = distribution(m_Generator);

            return float_vec;
        }

        void FibonacciEngine::Floats(std::vector<float>& vec, int amount, float min, float max, bool append)
        {
            auto distribution = std::uniform_real_distribution<float>(min, max);

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }

        std::vector<double> FibonacciEngine::Doubles(int amount)
        {
            auto distribution = std::uniform_real_distribution<double>();
            auto double_vec = std::vector<double>(amount);

            for (int i = 0; i < amount; i++)
                double_vec[i] = distribution(m_Generator);

            return double_vec;
        }

        void FibonacciEngine::Doubles(std::vector<double>& vec, int amount, bool append)
        {
            auto distribution = std::uniform_real_distribution<double>();

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }

        std::vector<double> FibonacciEngine::Doubles(int amount, double min, double max)
        {
            auto distribution = std::uniform_real_distribution<double>(min, max);
            auto double_vec = std::vector<double>(amount);

            for (int i = 0; i < amount; i++)
                double_vec[i] = distribution(m_Generator);

            return double_vec;
        }

        void FibonacciEngine::Doubles(std::vector<double>& vec, int amount, double min, double max, bool append)
        {
            auto distribution = std::uniform_real_distribution<double>(min, max);

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }

        std::vector<bool> FibonacciEngine::Bools(int amount)
        {
            auto distribution = std::bernoulli_distribution();
            auto bool_vec = std::vector<bool>(amount);

            for (int i = 0; i < amount; i++)
                bool_vec[i] = distribution(m_Generator);

            return bool_vec;
        }

        void FibonacciEngine::Bools(std::vector<bool>& vec, int amount, bool append)
        {
            auto distribution = std::bernoulli_distribution();

            int start = append ? vec.size() : 0;

            if (append) vec.reserve(vec.size() + amount);
            else { vec.clear(); vec.reserve(amount); }

            int size = vec.size();

            for (int i = start; i < size; i++)
                vec[i] = distribution(m_Generator);
        }

        std::vector<bool> FibonacciEngine::Bools(int amount, double probability)
        {
            auto distribution = std::bernoulli_distribution(probability);
            auto bool_vec = std::vector<bool>(amount);

            for (int i = 0; i < amount; i++)
                bool_vec[i] = distribution(m_Generator);

            return bool_vec;
        }

        void FibonacciEngine::Bools(std::vector<bool>& vec, int amount, double probability, bool append)
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
