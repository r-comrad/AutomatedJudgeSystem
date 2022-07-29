#include "test.hpp"

//--------------------------------------------------------------------------------

uint64_t test::Test::globalTestersNumber = 0;

//--------------------------------------------------------------------------------

test::Test::Test
(
    std::shared_ptr<proc::Process> aSolutionTemplate,
    std::shared_ptr<proc::Process> aCheckerTemplate,
    ThreadSignals* aThreadSignals
) noexcept :
    mNumberOfTester     (0),
    mTestNumber         (0),
    mSolutionTemplate   (aSolutionTemplate),
    mCheckerTemplate    (aCheckerTemplate),
    mThreadSignals      (aThreadSignals),
    mVerdict            (TestVerdict::OK),
    mUsedTime           (0),
    mUsedMemory         (0),
    mTestThread         ([](){})
{}

//--------------------------------------------------------------------------------

test::Test::~Test() 
{
    mTestThread.join();
}

//--------------------------------------------------------------------------------

test::Test::Test(const Test& other) noexcept :
    mNumberOfTester     (globalTestersNumber++),
    mTestNumber         (0),
    mSolutionTemplate   (other.mSolutionTemplate),
    mCheckerTemplate    (other.mCheckerTemplate),
    mThreadSignals      (other.mThreadSignals),
    mVerdict            (TestVerdict::OK),
    mUsedTime           (0),
    mUsedMemory         (0),
    mTestThread         ([](){})
{
    mTimeLimit = other.mTimeLimit;    
    mMemoryLimit = other.mMemoryLimit; 
    mThreadSignals->push(mNumberOfTester);    
}

//--------------------------------------------------------------------------------

test::Test& 
test::Test::operator=(const Test& other) noexcept
{

    *this = std::move(Test(other));
    return *this;
}

//--------------------------------------------------------------------------------

test::Test::TestVerdict
test::Test::getVerdict() const noexcept
{
    return mVerdict;
}

//--------------------------------------------------------------------------------

uint64_t
test::Test::getUsedTime() const noexcept
{
    return mUsedTime;
}

//--------------------------------------------------------------------------------

uint64_t
test::Test::getUsedMemory() const noexcept
{
    return mUsedMemory;
}

//--------------------------------------------------------------------------------

void 
test::Test::setLimits(const data::Limits& aTimeMemLimits) noexcept
{
    mTimeLimit = aTimeMemLimits.timeLimit;
    mMemoryLimit = aTimeMemLimits.memoryLimit;
}

//--------------------------------------------------------------------------------

void 
test::Test::run(data::DatabaseQuery& aDBQ) noexcept
{
    mThreadSignals->pop(mNumberOfTester);
    mTestThread.join();
    mTestThread = std::thread(&Test::runTesting, this, std::ref(aDBQ));
}

//--------------------------------------------------------------------------------

void 
test::Test::runTesting(data::DatabaseQuery& aDBQ) noexcept
{
    if (getTest(aDBQ)) 
    {
        checkTest();
        mThreadSignals->push(mNumberOfTester);
    }
    else
    {
        mThreadSignals->finishCurrentThread();
    }
}

//--------------------------------------------------------------------------------

void 
test::Test::checkTest() noexcept
{
    START_LOG_BLOCK("Checking_test#", mTestNumber);

    mSolutionProcess = *mSolutionTemplate;
    mSolutionProcess.create();
    mSolutionProcess.write(mTLM.mTest);
    mSolutionProcess.write("\n");
    auto testRes = mSolutionProcess.runWithLimits();

    if (!testRes)
    {
        mVerdict = TestVerdict::TLE;
    }
    else
    {
        mUsedTime = testRes.value().first;
        mUsedMemory = testRes.value().second;
        mSolutionProcess.read(mTLM.mOutput);
        mTLM.makeOutputSizes();

        START_LOG_BLOCK ("Test:",   mTLM.mTest);
        WRITE_LOG       ("Output:", mTLM.mOutput);
        END_LOG_BLOCK   ("Answer:", mTLM.mAnswer);
        END_LOG_BLOCK();

        mCheckerProcess = *mCheckerTemplate;
        mCheckerProcess.create();

        mCheckerProcess.write(mTLM.mTestSize);
        mCheckerProcess.write(mTLM.mTest);

        mCheckerProcess.write(mTLM.mOutputSize);
        mCheckerProcess.write(mTLM.mOutput);

        mCheckerProcess.write(mTLM.mAnswerSize);
        mCheckerProcess.write(mTLM.mAnswer);

        mCheckerProcess.run();
        
        std::string temp;
    }

    resultEvoluation();
}

//--------------------------------------------------------------------------------

bool 
test::Test::getTest(data::DatabaseQuery& aDBQ) noexcept
{
    bool result = false;
    auto testData = aDBQ.getNextTest();
    if (testData.has_value())
    {
        mTLM.mTest = std::move(testData.value().input);
        mTLM.mAnswer = std::move(testData.value().output);   
        mTLM.makeTestSizes();
        mTLM.makeAnswerSizes();
        mTestNumber = testData.value().testNum;
        result = true;
    }
    return result;
}

//--------------------------------------------------------------------------------

void
test::Test::resultEvoluation() noexcept
{
    START_LOG_BLOCK("Result_evoluation");
    WRITE_LOG("time:",   mUsedTime);
    WRITE_LOG("memory:", mUsedMemory);

    std::string temp;
    mCheckerProcess.read(temp);
    if (temp.substr(0, 2) != "ok")
    {
        mVerdict = TestVerdict::WA;
        START_LOG_BLOCK("Result_not_okay");
        END_LOG_BLOCK("Checker_output:", temp);
    }
    else if (mUsedTime > mTimeLimit * 1000000)
    {
        mVerdict = TestVerdict::TLE;
        START_LOG_BLOCK("Result_is_TLE");
        WRITE_LOG("wanted:", mUsedTime);
        END_LOG_BLOCK("received:", mTimeLimit);
    }
    else if (mUsedMemory > mMemoryLimit)
    {
        mVerdict = TestVerdict::MLE;
        START_LOG_BLOCK("Result_is_MLE");
        WRITE_LOG("wanted:", mUsedMemory);
        END_LOG_BLOCK("received:", mMemoryLimit);
    }
    else
    {
        mVerdict = TestVerdict::OK;
        START_LOG_BLOCK("ok_test_passed");
        END_LOG_BLOCK();
    }

    END_LOG_BLOCK();
}

//--------------------------------------------------------------------------------
