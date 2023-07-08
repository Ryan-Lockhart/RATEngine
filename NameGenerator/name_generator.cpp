#include "name_generator.hpp"

namespace rat
{
	namespace NameGenerator
	{
		NameEntry::NameEntry(int index, const std::string& name) :
			m_Index(index), m_Name(name), m_Status(NameStatus::Pending)
		{
			
		}

		NameEntry::NameEntry(int index, const std::string& name, const NameStatus& status) :
			m_Index(index), m_Name(name), m_Status(status)
		{

		}

		inline const int& NameEntry::Index() const { return m_Index; }

		inline const std::string& NameEntry::Name() const { return m_Name; }

		inline const NameStatus& NameEntry::Status() const { return m_Status; }

		inline const std::string& NameGenerator::Name() const { return m_Name; }

		inline const std::string& NameGenerator::Description() const { return m_Description; }

		inline const std::string& NameGenerator::Path() const { return m_Path; }

		inline const NameList& NameGenerator::Names() const { return *ptr_Names.get(); }

		inline const ProbabilityMatrix& NameGenerator::Weights() const { return *ptr_Weights.get(); }

		inline size_t NameGenerator::QueueSize() const { return m_ApprovalQueue.size(); }

		void NameGenerator::PopSingle()
		{

		}

		void NameGenerator::PopMulti()
		{

		}

		NameGenerator::NameGenerator(const std::string& name, const std::string& description, const std::string& path, bool regenerate) :
			m_Name(name), m_Description(description), m_Path(path)
		{

		}

		NameGenerator::NameGenerator(const std::string& name, const std::string& description, const std::string& path, const NameList& names, const ProbabilityMatrix& weights)
		{

		}

		void NameGenerator::Save()
		{

		}

		void NameGenerator::Load(const std::string& path)
		{

		}

		void NameGenerator::GenerateName()
		{

		}

		void NameGenerator::GenerateNames(int amount)
		{

		}
	}
}
