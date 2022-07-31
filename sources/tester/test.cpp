#include "test.hpp"

//--------------------------------------------------------------------------------

int64_t test::Test::globalTestersNumber = 0;

//--------------------------------------------------------------------------------

test::Test::Test
(
    std::shared_ptr<proc::Process> aSolutionTemplate,
    std::shared_ptr<proc::Process> aCheckerTemplate,
    ThreadSignals* aThreadSignals
) noexcept :
    mNumberOfTester     (-1),
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
    START_LOG_BLOCK("Start_new_test", "Test_cell_num:" + std::to_string(mNumberOfTester));

    if (getTest(aDBQ)) 
    {
        WRITE_LOG("Success", "Test_cell_num:" + std::to_string(mNumberOfTester));
        checkTest();
        mThreadSignals->push(mNumberOfTester);
        WRITE_LOG("Success_finishing", "Test_cell_num:" + std::to_string(mNumberOfTester));
    }
    else
    {
        WRITE_LOG("Failure", "Test_cell_num:" + std::to_string(mNumberOfTester));
        mThreadSignals->finishCurrentThread();
        WRITE_LOG("Failure_finishing", "Test_cell_num:" + std::to_string(mNumberOfTester));
    }

    END_LOG_BLOCK("End_test#", mTestNumber, "Test_cell_num:" + std::to_string(mNumberOfTester));
}

//--------------------------------------------------------------------------------

void 
test::Test::checkTest() noexcept
{
    START_LOG_BLOCK("Checking_test#", mTestNumber, 
        "Test_cell_num:" + std::to_string(mNumberOfTester));
mSpeshalForLinux.lock();
    mSolutionProcess = *mSolutionTemplate;
    mSolutionProcess.create();
    mSolutionProcess.writeData(mTLM.mTest);
    mSolutionProcess.writeData("\n");
    auto testRes = mSolutionProcess.runWithLimits();
mSpeshalForLinux.unlock();

    if (!testRes)
    {
        mVerdict = TestVerdict::TLE;
    }
    else
    {
        mUsedTime = testRes.value().first;
        mUsedMemory = testRes.value().second;
mSpeshalForLinux.lock();
        mSolutionProcess.readData(mTLM.mOutput);
mSpeshalForLinux.unlock();
        mTLM.makeOutputSizes();

        START_LOG_BLOCK ("Test_cell_num:" + std::to_string(mNumberOfTester), "Test:",   mTLM.mTest);
        WRITE_LOG       ("Test_cell_num:" + std::to_string(mNumberOfTester), "Output:", mTLM.mOutput);
        END_LOG_BLOCK   ("Test_cell_num:" + std::to_string(mNumberOfTester), "Answer:", mTLM.mAnswer);
        END_LOG_BLOCK();

mSpeshalForLinux.lock();
        mCheckerProcess = *mCheckerTemplate;
        mCheckerProcess.create();
        mCheckerProcess.writeData(mTLM.mTestSize);
        mCheckerProcess.writeData(mTLM.mTest);

        mCheckerProcess.writeData(mTLM.mOutputSize);
        mCheckerProcess.writeData(mTLM.mOutput);

        mCheckerProcess.writeData(mTLM.mAnswerSize);
        mCheckerProcess.writeData(mTLM.mAnswer);

        mCheckerProcess.run();
mSpeshalForLinux.unlock();
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
    START_LOG_BLOCK("Test_cell_num:" + std::to_string(mNumberOfTester), "Result_evoluation");
    WRITE_LOG("time:",   mUsedTime);
    WRITE_LOG("memory:", mUsedMemory);

    std::string temp;
mSpeshalForLinux.lock();
    mCheckerProcess.readData(temp);
mSpeshalForLinux.unlock();
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
        START_LOG_BLOCK("Test_cell_num:" + std::to_string(mNumberOfTester), "ok_test_passed");
        END_LOG_BLOCK();
    }

    END_LOG_BLOCK();
}

//--------------------------------------------------------------------------------
