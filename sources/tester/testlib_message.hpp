#ifndef TESTLIB_MESSAGE_HPP
#define TESTLIB_MESSAGE_HPP

//--------------------------------------------------------------------------------

#include <string>

#include "domain/type.hpp"

//--------------------------------------------------------------------------------

struct TestLibMessage
{
public:
    std::string mTest;
    std::string mAnswer;
    std::string mOutput;

    std::string mTestSize;
    std::string mAnswerSize;
    std::string mOutputSize;

    void makeTestSizes() noexcept;
    void makeAnswerSizes() noexcept;
    void makeOutputSizes() noexcept;
    
private:
    union
    {
        uint_8 message[8];
        uint_64 value;
    } StringSizeCoder{};

    void makeSize(std::string& aStr, std::string& aSize) noexcept;
};

//--------------------------------------------------------------------------------

#endif // !TESTLIB_MESSAGE_HPP