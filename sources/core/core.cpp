#include "core.hpp"

//--------------------------------------------------------------------------------
//				CHILD PROCESS CREATION IMPLIMENTATION 
//--------------------------------------------------------------------------------


#undef max
#undef min

#include <cmath>

#define THREAD_COUNTS 1
#define DEBUG_PLUS_SOLUTION_SUBMISSION 1

#if DEBUG_PLUS_SOLUTION_SUBMISSION
#define _DEBUG_SOL_SUB_
#endif

Core::Core
(
    std::string aDatabasePath
) :
    mDBQ            (aDatabasePath),
    mChecksMutexs   (THREAD_COUNTS),
    mChecksInfo     (THREAD_COUNTS),
    mFinishedTest   (0)
{
    mProblemInfo.mResult = "ok";
    for (uint_32 i = 0; i < THREAD_COUNTS; ++i)
    {
        mChecks.push_back(new std::thread([](){}));
    }

}

Core::~Core()
{
    for (uint_32 i = 0; i < mChecks.size(); ++i)
    {
        if (mChecks[i] != NULL)
        {
            mChecks[i]->join();
            delete mChecks[i];
        }
    }
}

void
Core::run(int aID) noexcept
{
    auto partInfo = mDBQ.getSubmissionInfo(aID);
    mProblemInfo.mID = aID;
    mProblemInfo.mContestID = partInfo.problemID;
    partInfo.solutionFileName = "2\\plus.cpp";

    prepareSolutionProcess(partInfo);
    prepareCheckerProcess(partInfo);
    mSolutionProcess.setLimits(partInfo.timeLimit, partInfo.memoryLimit);

    check();
}

void
Core::prepareSolutionProcess(SubmissionInfo& aSubInfo) noexcept
{
    cor::CodeInfo codeInfo(cor::CodeInfo::CodeInfoType::Submission);
    codeInfo.setFileName(std::move(aSubInfo.solutionFileName));
    codeInfo.setDesirableOutputFileName(std::to_string(aSubInfo.ID));
    //TODO: set language
    codeInfo.setLanguage();

    sys::Compiler comp;
    auto cmd = comp.getExecutableCommand(std::move(codeInfo));
    mSolutionProcess.setComand(cmd);
}

void
Core::prepareCheckerProcess(SubmissionInfo& aSubInfo) noexcept
{
    cor::CodeInfo codeInfo(cor::CodeInfo::CodeInfoType::Checker);
    codeInfo.setFileName(std::move(aSubInfo.checkerFileName));
    codeInfo.setDesirableOutputFileName(std::to_string(aSubInfo.ID));
    //TODO: set language
    codeInfo.setLanguage();

    sys::Compiler comp;
    auto cmd = comp.getExecutableCommand(std::move(codeInfo));
    mCheckerProcess.setComand(cmd);
}

bool
Core::resultEvoluation(int aCheckNum)
{
    START_LOG_BLOCK("Result_evoluation");
    WRITE_LOG("time:", mChecksInfo[aCheckNum].mUsedTime);
    WRITE_LOG("memory:", mChecksInfo[aCheckNum].mUsedMemory);
    bool result = false;

    mProblemInfoLock.lock();
    if (mChecksInfo[aCheckNum].mResult.substr(0, 2) != "ok")
    {
        mProblemInfo.mResult = mChecksInfo[aCheckNum].mResult;
        START_LOG_BLOCK("Result_not_okay");
        END_LOG_BLOCK("Today_result_is:", mProblemInfo.mResult);
    }
    else if (mChecksInfo[aCheckNum].mUsedTime > mProblemInfo.mTimeLimit * 1000000)
    {
        mProblemInfo.mResult = "tle";
        START_LOG_BLOCK("Result_is_TLE");
        WRITE_LOG("wanted:", mProblemInfo.mTimeLimit);
        END_LOG_BLOCK("received:", mChecksInfo[aCheckNum].mUsedTime);
    }
    else if (mChecksInfo[aCheckNum].mUsedMemory > mProblemInfo.mMemoryLimit)
    {
        mProblemInfo.mResult = "mle";
        START_LOG_BLOCK("Result_is_MLE");
        WRITE_LOG("wanted:", mProblemInfo.mMemoryLimit);
        END_LOG_BLOCK("received:", mChecksInfo[aCheckNum].mUsedMemory);
    }
    else
    {
        result = true;
        if (mProblemInfo.mUsedTime < mChecksInfo[aCheckNum].mUsedTime)
            mProblemInfo.mUsedTime = mChecksInfo[aCheckNum].mUsedTime;
        if (mProblemInfo.mUsedMemory < mChecksInfo[aCheckNum].mUsedMemory)
            mProblemInfo.mUsedMemory = mChecksInfo[aCheckNum].mUsedMemory;
        START_LOG_BLOCK("ok_test_passed");
        END_LOG_BLOCK();
    }
    mProblemInfoLock.unlock();

    END_LOG_BLOCK();
    return result;
}

void
Core::check()
{
    START_LOG_BLOCK("Checking_participant_code");

    mDBQ.prepareTestsStatement(mProblemInfo);
   //pipesTesting(0);

    for (bool isStillTesting = true; isStillTesting;)
    {
        isStillTesting = false;
        for (uint_32 i = 0; i < mChecks.size(); ++i)
        {
            bool needEvaluateResult = false;

            mChecksMutexs[i].lock();
            isStillTesting |= !mChecksInfo[i].mIsFinishedTesting;
            if (mChecksInfo[i].mIsFinishedTesting && mChecks[i] != NULL)
            {
                mChecks[i]->join();
                ++mFinishedTest;
                isStillTesting = true;
                delete mChecks[i];

                isStillTesting = resultEvoluation(i);

                if (!mProblemInfo.mTestsAreOver && isStillTesting) {
                    mChecksInfo[i].mIsFinishedTesting = false;
                    mChecks[i] = new std::thread(&Core::pipesTesting, this,
                        i//, std::ref(aSolutionName), std::ref(aCheckerName)
                        //i, aSolutionName, aCheckerName
                    );
                }
                else
                {
                    mChecks[i] = NULL;
                }
            }

            mChecksMutexs[i].unlock();

        }
    }

    mProblemInfo.remakeResultForPasha();
    mDBQ.writeResult(mProblemInfo.mID, mProblemInfo.mResult, 
        mProblemInfo.mUsedTime, mProblemInfo.mUsedMemory);

    WRITE_LOG("Final_result:", mProblemInfo.mResult);
    WRITE_LOG("Final_time:", mProblemInfo.mUsedTime);
    END_LOG_BLOCK("Final_memory:", mProblemInfo.mUsedMemory);
}

void
Core::pipesTesting(int aThreadNum)
{
    START_LOG_BLOCK("Checking_test#", aThreadNum + mFinishedTest);
    if (aThreadNum + mFinishedTest == 66)
    {
        int yy = 0;
        ++yy;
        std::cout << yy;
    }
    if (aThreadNum + mFinishedTest == 67)
    {
        int yy = 0;
        ++yy;
        std::cout << yy;
    }

    TestLibMessage TLM;
    mGetTestLock.lock();
    if (!mProblemInfo.mTestsAreOver)
    {
        mDBQ.getNextTest(mProblemInfo, TLM);
    }
    if (mProblemInfo.mTestsAreOver)
    {
        mChecksMutexs[aThreadNum].lock();
        mChecksInfo[aThreadNum].mIsFinishedTesting = true;
        mChecksInfo[aThreadNum].mResult = "ok";
        WRITE_LOG("Tests_are_over");
        mChecksMutexs[aThreadNum].unlock();
        mGetTestLock.unlock();
        END_LOG_BLOCK();
        return;
    }
    mGetTestLock.unlock();

    TLM.makeTestSizes();

    //MyProcess code(mSolutionParameters, mProblemInfo.mTimeLimit, mProblemInfo.mMemoryLimit);
    //proc::PipeProcess code(mSolutionParameters, mProblemInfo.mTimeLimit, mProblemInfo.mMemoryLimit);

    proc::PipeWindowsProcess code = mSolutionProcess;
    //code.IORedirection();
    code.create();

    code.writePipe(TLM.mTest);
    //std::pair<uint_64, uint_64> cur = code.runWithLimits(mProblemInfo.mTimeLimit, mProblemInfo.mMemoryLimit);
    auto testRes = code.runWithLimits();
    if (!testRes)
    {
        mChecksInfo[aThreadNum].mResult = "tle";
        mChecksMutexs[aThreadNum].lock();
        mChecksInfo[aThreadNum].mIsFinishedTesting = true;
        mChecksMutexs[aThreadNum].unlock();
        END_LOG_BLOCK();
        return;
    }
    code.readPipe(TLM.mOutput);

    mChecksInfo[aThreadNum].mUsedTime = testRes.value().first;
    mChecksInfo[aThreadNum].mUsedMemory = testRes.value().second;

    //MyProcess checker(mCheckerParameters);
    //proc::PipeProcess checker(mCheckerParameters);
    proc::PipeWindowsProcess checker = mCheckerProcess;
    //checker.IORedirection();
    checker.create();

//    TLM.mAnswer.pop_back();
//    TLM.mAnswer.pop_back();

    TLM.makeOutputSizes();
    TLM.makeAnswerSizes();

    checker.writePipe(TLM.mTestSize,    proc::Process::PypeType::NO_ZERO);
    checker.writePipe(TLM.mTest,        proc::Process::PypeType::NO_ZERO);

    checker.writePipe(TLM.mOutputSize,  proc::Process::PypeType::NO_ZERO);
    checker.writePipe(TLM.mOutput,      proc::Process::PypeType::NO_ZERO);

    checker.writePipe(TLM.mAnswerSize,  proc::Process::PypeType::NO_ZERO);
    checker.writePipe(TLM.mAnswer,      proc::Process::PypeType::NO_ZERO);

    START_LOG_BLOCK("Test:", TLM.mTest);
    WRITE_LOG("Output:", TLM.mOutput);
    END_LOG_BLOCK("Answer:", TLM.mAnswer);
    END_LOG_BLOCK();

    checker.run();
    checker.readPipe(mChecksInfo[aThreadNum].mResult);

    mChecksMutexs[aThreadNum].lock();
    mChecksInfo[aThreadNum].mIsFinishedTesting = true;
    mChecksMutexs[aThreadNum].unlock();
}