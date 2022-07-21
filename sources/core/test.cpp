#include "test.hpp"

Test::Test
(
    std::shared_ptr<proc::Process> aSolutionTemplate,
    std::shared_ptr<proc::Process> aCheckerTemplate
) noexcept :
    mSolutionTemplate   (aSolutionTemplate),
    mCheckerTemplate    (aCheckerTemplate),
    mTestState          (Test::TestState::Finished),
    mVerdict            (OK),
    mUsedTime           (0),
    mUsedMemory         (0),
    mTestThread         ([](){})
{}

Test::~Test() 
{
    mTestThread.join();
}

Test::Test(const Test& other) noexcept
{
    mTestState = Test::TestState::Finished;
    mTestThread = std::thread([](){});

    mVerdict = OK;
    mUsedTime = 0;
    mUsedMemory = 0;

    mSolutionTemplate = other.mSolutionTemplate;
    mCheckerTemplate = other.mCheckerTemplate;   

    mTimeLimit = other.mTimeLimit;    
    mMemoryLimit = other.mMemoryLimit;    
}

bool
Test::isFinished() const noexcept
{
    return mTestState == TestState::Finished;
}
bool
Test::testsAreOver() const noexcept
{
    return mTestState == TestState::RunOut;
}

std::string 
Test::getVerdict() const noexcept
{
    return mVerdict;
}

uint_64 
Test::getUsedTime() const noexcept
{
    return mUsedTime;
}

uint_64
Test::getUsedMemory() const noexcept
{
    return mUsedMemory;
}

void 
Test::setTimeLimit(uint_64 aTimeLimit) noexcept
{
    mTimeLimit = aTimeLimit;
}

void 
Test::setMemoryLimit(uint_64 aMemoryLimit) noexcept
{
    mMemoryLimit = aMemoryLimit;
}

void 
Test::run(data::DatabaseQuery& aDBQ) noexcept
{
    mTestThread.join();
    mTestState = TestState::Testing;
    mTestThread = std::thread(&Test::runTesting, this, std::ref(aDBQ));
}

void 
Test::runTesting(data::DatabaseQuery& aDBQ) noexcept
{
    getTest(aDBQ);
    if (!testsAreOver()) 
    {
        checkTest();
        mTestState = TestState::Finished;
    }
}

void 
Test::checkTest() noexcept
{
    START_LOG_BLOCK("Checking_test#", mTestNumber);

    mSolutionProcess = *mSolutionTemplate;
    mSolutionProcess.create();
    mSolutionProcess.write(mTLM.mTest);
    mSolutionProcess.write("\n");
    auto testRes = mSolutionProcess.runWithLimits();

    if (!testRes)
    {
        mVerdict = TLE;
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
        mCheckerProcess.read(mVerdict);
    }

    resultEvoluation();
}

void 
Test::getTest(data::DatabaseQuery& aDBQ) noexcept
{
    auto testData = aDBQ.getNextTest();
    if (testData.has_value())
    {
        mTLM.mTest = std::move(testData.value().input);
        mTLM.mAnswer = std::move(testData.value().output);   
        mTLM.makeTestSizes();
        mTLM.makeAnswerSizes();

        mTestNumber = testData.value().testNum;
    }
    else
    {
        mTestState = TestState::RunOut;
    }
}

void
Test::resultEvoluation() noexcept
{
    START_LOG_BLOCK("Result_evoluation");
    WRITE_LOG("time:",      mUsedTime);
    WRITE_LOG("memory:",    mUsedMemory);

    if (mVerdict.substr(0, 2) != "ok")
    {
        mVerdict = WA;
        START_LOG_BLOCK("Result_not_okay");
        END_LOG_BLOCK("Today_result_is:", mVerdict);
    }
    else if (mUsedTime > mTimeLimit * 1000000)
    {
        mVerdict = TLE;
        START_LOG_BLOCK("Result_is_TLE");
        WRITE_LOG("wanted:", mUsedTime);
        END_LOG_BLOCK("received:", mTimeLimit);
    }
    else if (mUsedMemory > mMemoryLimit)
    {
        mVerdict = MLE;
        START_LOG_BLOCK("Result_is_MLE");
        WRITE_LOG("wanted:", mUsedMemory);
        END_LOG_BLOCK("received:", mMemoryLimit);
    }
    else
    {
        mVerdict = OK;
        START_LOG_BLOCK("ok_test_passed");
        END_LOG_BLOCK();
    }

    END_LOG_BLOCK();
}
