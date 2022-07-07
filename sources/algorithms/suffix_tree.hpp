#ifndef SUFFIX_TREE_HPP
#define SUFFIX_TREE_HPP

#include <map>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

#include <domain/type.hpp>

namespace alg
{
	class SuffixTree
	{
	public:
		SuffixTree() noexcept;
		~SuffixTree() = default;

		void add(const std::string& aName, uint_32 aFinishNumber) noexcept;
		uint_32 get(const std::string& aName) const noexcept;

	private:
		struct Node
		{	
			Node();
			~Node() = default;

			bool mIsFinish;
			uint_32 mFinishNumber;
			std::map<char, std::shared_ptr<Node>> mNextNodes;

			void add(const std::string& aName, uint_32 aFinishValue, size_t aCurNum = 0) noexcept;
			uint_32 get(const std::string& aName, size_t aCurNum = 0) const noexcept;
		};

		std::shared_ptr<Node> mRoot;
	};
}


#endif // SUFFIX_TREE_HPP!