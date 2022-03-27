#ifndef TESTLIB_MESSAGE_H
#define TESTLIB_MESSAGE_H

#include <string>

#include "main/flags.hpp"

#include "domain/type.hpp"

struct TestLibMessage
{
public:
	std::string mTest;
	std::string mAnswer;
	std::string mOutput;

	std::string mTestSize;
	std::string mAnswerSize;
	std::string mOutputSize;

	void makeTestSizes();
	void makeAnswerSizes();
	void makeOutputSizes();
private:
	union
	{
		uint_8 message[8];
		uint_64 value;
	} StringSizeCoder{};

	void makeSize(std::string& aStr, std::string& aSize);
};

#endif //TESTLIB_MESSAGE_H