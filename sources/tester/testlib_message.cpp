#include "testlib_message.hpp"

void
TestLibMessage::makeTestSizes()
{
    mTestSize.clear();
    mTestSize.reserve(8);
    makeSize(mTest, mTestSize);
}

void
TestLibMessage::makeAnswerSizes()
{
    mAnswerSize.clear();
    mAnswerSize.reserve(8);
    makeSize(mAnswer, mAnswerSize);
}

void
TestLibMessage::makeOutputSizes()
{
    mOutputSize.clear();
    mOutputSize.reserve(8);
    makeSize(mOutput, mOutputSize);
}

void 
TestLibMessage::makeSize(std::string& aStr, std::string& aSize)
{
    StringSizeCoder.value = aStr.size();
    for (int i = 0; i < 8; ++i) aSize.push_back(StringSizeCoder.message[i]);
}