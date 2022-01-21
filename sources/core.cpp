#include "core.h"

#undef max
#undef min

#include <math.h>

#define THREAD_COUNTS 30

Core::Core
(
    std::string aDatabasePath
) :
    mDBQ            (aDatabasePath),
    mChecksMutexs   (THREAD_COUNTS),
    mChecksInfo     (THREAD_COUNTS),
    mFinishedTest   (0)
{
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
Core::run
(
    int aID
)

{
    mProblemInfo.mID = aID;
    mDBQ.prepareForTesting(mProblemInfo);

#ifdef BILL_WINDOWS
    mProblemInfo.mSolutionFileName =  makeWindowString(mProblemInfo.mSolutionFileName);
    mProblemInfo.mCheckerFileName = makeWindowString(mProblemInfo.mCheckerFileName);
#endif // BILL_WINDOWS

    std::string solutionName = makeExecutable(MAEDIA + mProblemInfo.mSolutionFileName,
        SOLUTION_PATH + "-" + std::to_string(mProblemInfo.mID));
    std::string checkerName = makeExecutable(MAEDIA + mProblemInfo.mCheckerFileName,
        CHECKER_PATH + "-" + std::to_string(mProblemInfo.mID));
    
    check(solutionName, checkerName);
}

Core::Language
Core::getLanguage
(
    std::string aFileName
)
{
    int num = aFileName.find('.');
    if (num == -1) return Core::Language::NUN;

    Core::Language result = Core::Language::NUN;
    std::string fileExtension = aFileName.substr(num + 1);
    if (fileExtension == "cpp") result = Core::Language::MAGIC_CPP;
    if (fileExtension == "py") result = Core::Language::SNAKE;
    return result;
}

std::string
Core::compile
(
    std::string aFileName,
    std::string aOutName, 
    Language aLanguage
)
{
    std::string result = aOutName;
    if (aLanguage == Language::MAGIC_CPP)
    {
        result += ".exe";
        //process.IORedirection(L"", L"");
        std::string comand = COMPILERS + "magicCPPCompiler.cmd" + " " + 
            aFileName + " " + result;
        std::string ss = "";
        PipeProcess compiler(ss, comand);
        compiler.run();
        
#if defined(_DBG_) && defined(COMPILER_LOG_OUTPUT)
        std::string compilerOutput;
        compiler.readPipe(compilerOutput);
        WD_LOG(compilerOutput);
#endif
    }
    else if (aLanguage == Language::SNAKE)
    {
        //result += ".py";
        //copyFile(aFileName, result);
        //result = "python " + result;
        result = "python " + aFileName;
    }
    for (int i = 0; i < 1e7; ++i);
    return result;
}

std::string
Core::makeExecutable
(
    std::string aFileName,
    std::string aOutputName
)
{
    Core::Language language = getLanguage(aFileName);
    return compile(aFileName, aOutputName, language);
}

void
Core::check
(
    std::string& aSolutionName,
    std::string& aCheckerName
)
{
#ifdef _DBG_
    std::vector<std::pair<uint_64, uint_64>> allTimes;
    mProblemInfo.mMemoryLimit += 1000000;
#endif // _DBG_

    mDBQ.prepareTestsStatement(mProblemInfo);

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
                if (!mProblemInfo.mTestsAreOver) {
                    mChecksInfo[i].mIsFinishedTesting = false;
                    mChecks[i] = new std::thread(&Core::pipesTesting, this,
                        i, std::ref(aSolutionName), std::ref(aCheckerName)
                        //i, aSolutionName, aCheckerName
                    );
                }
                else
                {
                    mChecks[i] = NULL;
                }

                if (mProblemInfo.mUsedTime < mChecksInfo[i].mUsedTime)
                    mProblemInfo.mUsedTime = mChecksInfo[i].mUsedTime;
                if (mProblemInfo.mUsedMemory < mChecksInfo[i].mUsedMemory)
                    mProblemInfo.mUsedMemory = mChecksInfo[i].mUsedMemory;
                mProblemInfo.mResult = mChecksInfo[i].mResult;

                needEvaluateResult = true;
            }

            mChecksMutexs[i].unlock();

            if (needEvaluateResult)
            {
                if (mProblemInfo.mResult.substr(0, 2) != "ok")
                {
                    WD_LOG("Result not okay");
                    WD_LOG("Today result is: " << mProblemInfo.mResult);
                    isStillTesting = false;
                    break;
                }
                else if (mProblemInfo.mTimeLimit < mProblemInfo.mUsedTime)
                {
                    mProblemInfo.mResult = "tle";
                    WD_LOG("Result is TLE: wanted " << mProblemInfo.mTimeLimit <<
                        " vs received " << mProblemInfo.mUsedTime);
                    isStillTesting = false;
                    break;
                }
                else if (mProblemInfo.mMemoryLimit < mProblemInfo.mUsedMemory)
                {
                    mProblemInfo.mResult = "mle";
                    WD_LOG("Result is MLE: wanted " << mProblemInfo.mMemoryLimit <<
                        " vs received " << mProblemInfo.mUsedMemory);
                    isStillTesting = false;
                    break;
                }

                WD_END_LOG;
            }
        }
    }

    mProblemInfo.remakeResultForPasha();
    mDBQ.writeResult(mProblemInfo.mID, mProblemInfo.mResult, 
        mProblemInfo.mUsedTime, mProblemInfo.mUsedMemory);

    WD_LOG("Final result : " + mProblemInfo.mResult);
    WD_LOG("Final time : " << mProblemInfo.mUsedTime);
    WD_LOG("Final memory : " << mProblemInfo.mUsedMemory);
    WD_END_LOG;
}

void
Core::pipesTesting
(
    int             aThreadNum,
    std::string&    aSolutionName,
    std::string&    aCheckerName
    //std::string aSolutionName,
    //std::string aCheckerName
)
{
    WD_LOG("Checking test #" << aThreadNum + mFinishedTest);
    WD_LOG("Runing test #" << aThreadNum + mFinishedTest);

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
        mChecksMutexs[aThreadNum].unlock();
        mGetTestLock.unlock();
        return;
    }
    mGetTestLock.unlock();

    std::string ss = "";
    PipeProcess code(ss, aSolutionName);

    TLM.makeTestSizes();   
    code.writePipe(TLM.mTest);
    std::pair<uint_64, uint_64> cur = code.runWithLimits(mProblemInfo.mTimeLimit, mProblemInfo.mMemoryLimit);
    if (cur.first == -1)
    {
        mChecksInfo[aThreadNum].mResult = "tle";
        mChecksMutexs[aThreadNum].lock();
        mChecksInfo[aThreadNum].mIsFinishedTesting = true;
        mChecksMutexs[aThreadNum].unlock();
        return;
    }
    code.readPipe(TLM.mOutput);

    mChecksInfo[aThreadNum].mUsedTime = cur.first;
    mChecksInfo[aThreadNum].mUsedMemory = cur.second;

    PipeProcess checker(aCheckerName, ss);

    TLM.mAnswer.pop_back();
    TLM.mAnswer.pop_back();
    TLM.makeOutputSizes();
    TLM.makeAnswerSizes();

    checker.writePipe(TLM.mTestSize,    PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mTest,        PipeProcess::PypeType::NO_ZERO);

    checker.writePipe(TLM.mOutputSize,  PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mOutput,      PipeProcess::PypeType::NO_ZERO);

    checker.writePipe(TLM.mAnswerSize,  PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mAnswer,      PipeProcess::PypeType::NO_ZERO);

#ifdef _DBG_
    WD_LOG("Test: " + TLM.mTest + "\nOutput: " + TLM.mOutput + "\nAnswer: " + TLM.mAnswer);
#endif

    checker.run();
    checker.readPipe(mChecksInfo[aThreadNum].mResult);

    mChecksMutexs[aThreadNum].lock();
    mChecksInfo[aThreadNum].mIsFinishedTesting = true;
    mChecksMutexs[aThreadNum].unlock();
}