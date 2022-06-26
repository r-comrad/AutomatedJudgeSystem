#include "suffix_tree.hpp"

void
alg::SuffixTree::add(std::string aName, uint_32 aFinishNumber)
{
    std::reverse(aName.begin(), aName.end());
    mRoot->add(std::move(aName), aFinishNumber);
}

alg::SuffixTree::Node::Node() :
    mIsFinish       (false),
    mFinishNumber   (0)
{}

void
alg::SuffixTree::Node::add(std::string&& aName, uint_32 aFinishNumber)
{
    if (!aName.size())
    {
        this->mIsFinish = true;
        this->mFinishNumber = aFinishNumber;
        return;
    }
    char temp = aName.back();
    aName.pop_back();
    this->mNextNodes[temp]->add(std::move(aName), aFinishNumber);
}

uint_32 get(std::string&& aName);