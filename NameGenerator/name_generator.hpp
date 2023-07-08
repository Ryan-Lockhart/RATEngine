#pragma once

#include <stack>
#include <string>
#include <memory>

namespace rat
{
	namespace NameGenerator
	{
		class NameList;
		class ProbabilityMatrix;

		enum class NameStatus
		{
			Pending,
			Approved,
			Rejected,
		};

		class NameEntry
		{
		public:

			/// <summary>
			/// Construct a name entry with its index and name
			/// </summary>
			/// <param name="index">The index within the approval queue</param>
			/// <param name="name">The name that will await approval</param>
			NameEntry(int index, const std::string& name);

			/// <summary>
			/// Construct a name entry with its index and name
			/// </summary>
			/// <param name="index">The index within the approval queue</param>
			/// <param name="name">The name that will await approval</param>
			/// <param name="status">The starting status for the name entry</param>
			NameEntry(int index, const std::string& name, const NameStatus& status);

			const int& Index() const;
			const std::string& Name() const;
			const NameStatus& Status() const;
			
		private:

			int m_Index;
			std::string m_Name;
			NameStatus m_Status;

		};

		class NameGenerator
		{
		public:

			const std::string& Name() const;
			const std::string& Description() const;
			const std::string& Path() const;

			const NameList& Names() const;
			const ProbabilityMatrix& Weights() const;

			size_t QueueSize() const;

			/// <summary>
			/// Manually process the approval queue one name at a time
			/// </summary>
			void PopSingle();

			/// <summary>
			/// Manually process the approval queue in a list UI
			/// </summary>
			void PopMulti();

			NameGenerator(const std::string& name, const std::string& description, const std::string& path, bool regenerate = false);
			NameGenerator(const std::string& name, const std::string& description, const std::string& path, const NameList& names, const ProbabilityMatrix& weights);

			void Save();
			void Load(const std::string& path);

			void GenerateName();
			void GenerateNames(int amount);

		private:

			std::string m_Name;
			std::string m_Description;
			std::string m_Path;

			std::shared_ptr<NameList> ptr_Names;
			std::shared_ptr<ProbabilityMatrix> ptr_Weights;

			std::stack<std::string> m_ApprovalQueue;

		};
	}
}
