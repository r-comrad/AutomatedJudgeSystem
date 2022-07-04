#include "suffix_tree.hpp"

alg::SuffixTree::SuffixTree() : 
    mRoot   (new Node)
{}

void
alg::SuffixTree::add(const std::string& aName, uint_32 aFinishNumber)
{
    mRoot->add(aName, aFinishNumber);
}

uint_32 
alg::SuffixTree::get(const std::string& aName)
{
    return mRoot->get(aName);
}

alg::SuffixTree::Node::Node() :
    mIsFinish       (false),
    mFinishNumber   (0)
{}

void
alg::SuffixTree::Node::add(const std::string& aName, uint_32 aFinishValue, size_t aCurNum)
{
    if (aCurNum == aName.size())
    {
        mIsFinish = true;
        mFinishNumber = aFinishValue;
        return;
    }
    if (mNextNodes[aName[aCurNum]] == nullptr) mNextNodes[aName[aCurNum]] = std::make_shared<Node>();
    mNextNodes[aName[aCurNum]]->add(aName, aFinishValue, aCurNum + 1);
}

uint_32
alg::SuffixTree::Node::get(const std::string& aName, size_t aCurNum)
{
    if (mFinishNumber) return mFinishNumber;
    return mNextNodes[aName[aCurNum]]->get(aName, aCurNum + 1);
}