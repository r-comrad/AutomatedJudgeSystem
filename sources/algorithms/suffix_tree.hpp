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
		SuffixTree();
		~SuffixTree() = default;

		void add(const std::string& aName, uint_32 aFinishNumber);
		uint_32 get(const std::string& aName);

	private:
		struct Node
		{	
			Node();
			~Node() = default;

			bool mIsFinish;
			uint_32 mFinishNumber;
			std::map<char, std::shared_ptr<Node>> mNextNodes;

			void add(const std::string& aName, uint_32 aFinishValue, size_t aCurNum = 0);
			uint_32 get(const std::string& aName, size_t aCurNum = 0);
		};

		std::shared_ptr<Node> mRoot;
	};
}


#endif // SUFFIX_TREE_HPP!