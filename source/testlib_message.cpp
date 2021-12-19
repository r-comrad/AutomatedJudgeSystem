#include "testlib_message.h"

void
TestLibMessage::makeTestSizes()
{
	makeSize(mTest, mAnswer);
}

void
TestLibMessage::makeAnswerSizes()
{
	makeSize(mAnswer, mAnswerSize);
}

void
TestLibMessage::makeOutputSizes()
{
	makeSize(mOutput, mOutputSize);
}

void 
TestLibMessage::makeSize(std::string& aStr, std::string& aSize)
{
	StringSizeCoder.value = aStr.size();
	for (int i = 0; i < 8; ++i) aSize.push_back(StringSizeCoder.message[i]);
}