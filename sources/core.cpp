#include "core.h"

#undef max
#undef min

#include <math.h>

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

    for (char* i : mSolutionParameters)
    {
        if (i != NULL) delete i;
    }

    for (char* i : mCheckerParameters)
    {
        if (i != NULL) delete i;
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
    makeWindowString(mProblemInfo.mSolutionFileName);
    makeWindowString(mProblemInfo.mCheckerFileName);
#endif // BILL_WINDOWS
    mProblemInfo.mSolutionFileName.clear();

#ifdef _DEBUG_SOL_SUB_
#if  defined(BILL_WINDOWS)
    mProblemInfo.mSolutionFileName = "2\\plus.cpp";
#elif defined(LINUS_LINUX)
    mProblemInfo.mSolutionFileName = "2/plus.cpp";
#endif 
#endif

    makeExecutable
    (
        MAEDIA + mProblemInfo.mSolutionFileName,
        SOLUTION_PATH + "-" + std::to_string(mProblemInfo.mID), 
        mSolutionParameters
    );
    makeExecutable
    (
        MAEDIA + mProblemInfo.mCheckerFileName,
        CHECKER_PATH + "-" + std::to_string(mProblemInfo.mID),
        mCheckerParameters
    );
    
    check();
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

void
Core::compile
(
    std::string aFileName,
    std::string aOutName, 
    Language aLanguage,
    std::vector<char*>& aCpmandParameters
)
{
    //aCpmandParameters.push_back(new char);
    if (aLanguage == Language::MAGIC_CPP)
    {
        aCpmandParameters.push_back(newStrCopy((aOutName + ".exe")));
        //std::cout << aCpmandParameters.back() << std::endl;
        //std::strcat(aCpmandParameters.back(), ".exe");
        //for (const char* i = aOutName.c_str(); *i; ++i) std::cout << (*i);
        //std::cout << std::endl;
        //for (char* i = aCpmandParameters.back(); *i; ++i) std::cout << (*i);
        //std::cout << std::endl;
        std::vector<char*> comand;
        //comand.push_back((char*) "");
#ifdef BILL_WINDOWS
        comand.push_back(newStrCopy((CPP_COMPILER_NAME)));
#elif defined(LINUS_LINUX)
        comand.push_back(newCharPtrCopy("g++"));
#endif

        comand.push_back(newStrCopy(aFileName));

#ifdef LINUS_LINUX
        comand.push_back(newCharPtrCopy("-o"));
#endif

        comand.push_back(aCpmandParameters.back());
        comand.push_back(NULL);
        //MyProcess compiler(comand);
        PipeProcess compiler(comand);
        compiler.run();
        delete comand[0];
        delete comand[1];
        //exit(0);
        
#if defined(_DBG_) && defined(COMPILER_LOG_OUTPUT)
        std::string compilerOutput;
        compiler.readPipe(compilerOutput);
        WD_LOG(compilerOutput);
#endif
    }
    else if (aLanguage == Language::SNAKE)
    {
#ifdef BILL_WINDOWS
        aCpmandParameters.push_back(newCharPtrCopy(""));
#endif
        aCpmandParameters.push_back(newCharPtrCopy("python3"));
        aCpmandParameters.push_back(newStrCopy(aFileName));
    }
    aCpmandParameters.push_back(NULL);
    //for (int i = 0; i < 1e7; ++i);
    //return result;
}

void
Core::makeExecutable
(
    std::string aFileName,
    std::string aOutputName,
    std::vector<char*>& aCpmandParameters
)
{
    Core::Language language = getLanguage(aFileName);
    return compile(aFileName, aOutputName, language, aCpmandParameters);
}

bool
Core::resultEvoluation(int aCheckNum)
{
    mProblemInfoLock.lock();
    if (mChecksInfo[aCheckNum].mResult.substr(0, 2) != "ok")
    {
        mProblemInfo.mResult = mChecksInfo[aCheckNum].mResult;
        WD_LOG("Result not okay");
        WD_LOG("Today result is: " << mProblemInfo.mResult);
        mProblemInfoLock.unlock();
        return false;
    }
    else if (mChecksInfo[aCheckNum].mUsedTime > mProblemInfo.mTimeLimit * 1000000)
    {
        mProblemInfo.mResult = "tle";
        WD_LOG("Result is TLE: wanted " << mProblemInfo.mTimeLimit <<
                                        " vs received " << mChecksInfo[aCheckNum].mUsedTime);
        mProblemInfoLock.unlock();
        return false;
    }
    else if (mChecksInfo[aCheckNum].mUsedMemory > mProblemInfo.mMemoryLimit)
    {
        mProblemInfo.mResult = "mle";
        WD_LOG("Result is MLE: wanted " << mProblemInfo.mMemoryLimit <<
                                        " vs received " << mChecksInfo[aCheckNum].mUsedMemory);
        mProblemInfoLock.unlock();
        return false;
    }

    if (mProblemInfo.mUsedTime < mChecksInfo[aCheckNum].mUsedTime)
        mProblemInfo.mUsedTime = mChecksInfo[aCheckNum].mUsedTime;
    if (mProblemInfo.mUsedMemory < mChecksInfo[aCheckNum].mUsedMemory)
        mProblemInfo.mUsedMemory = mChecksInfo[aCheckNum].mUsedMemory;
    mProblemInfoLock.unlock();

    return true;
    WD_END_LOG;
}

void
Core::check
(
    //std::string& aSolutionName,
    //std::string& aCheckerName
    //const std::vector<char*>& aSolutionParameters,
    //const std::vector<char*>& aCheckerParameters
)
{
#ifdef _DBG_
    //mProblemInfo.mMemoryLimit += 1000000;
#endif // _DBG_

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

    WD_LOG("Final result : " + mProblemInfo.mResult);
    WD_LOG("Final time : " << mProblemInfo.mUsedTime);
    WD_LOG("Final memory : " << mProblemInfo.mUsedMemory);
    WD_END_LOG;
}

void
Core::pipesTesting
(
    int             aThreadNum
    //const std::vector<char*>& aSolutionParameters,
    //const std::vector<char*>& aCheckerParameters
    //std::string&    aSolutionName,
    //std::string&    aCheckerName
    //std::string aSolutionName,
    //std::string aCheckerName
)
{
    WD_LOG("Checking test #" << aThreadNum + mFinishedTest);
    WD_LOG("Runing test #" << aThreadNum + mFinishedTest);
//if (aThreadNum == 100)
//{
//    int yy = 0;
//    ++yy;
//std::cout << yy;
//
//}
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
        WD_LOG("Tests are over");
        mChecksMutexs[aThreadNum].unlock();
        mGetTestLock.unlock();
        return;
    }
    mGetTestLock.unlock();

    TLM.makeTestSizes();

    //MyProcess code(mSolutionParameters, mProblemInfo.mTimeLimit, mProblemInfo.mMemoryLimit);
    PipeProcess code(mSolutionParameters, mProblemInfo.mTimeLimit, mProblemInfo.mMemoryLimit);

    code.writePipe(TLM.mTest);
    //std::pair<uint_64, uint_64> cur = code.runWithLimits(mProblemInfo.mTimeLimit, mProblemInfo.mMemoryLimit);
    std::pair<uint_64, uint_64> cur = code.runWithLimits();
    if (cur.first == KILLING_PROCESS_TIME_VALUE && 
        cur.second == KILLING_PROCESS_MEMORY_VALUE)
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

    //MyProcess checker(mCheckerParameters);
    PipeProcess checker(mCheckerParameters);

//    TLM.mAnswer.pop_back();
//    TLM.mAnswer.pop_back();

    TLM.makeOutputSizes();
    TLM.makeAnswerSizes();

    checker.writePipe(TLM.mTestSize,    PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mTest,        PipeProcess::PypeType::NO_ZERO);

    checker.writePipe(TLM.mOutputSize,  PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mOutput,      PipeProcess::PypeType::NO_ZERO);

    checker.writePipe(TLM.mAnswerSize,  PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mAnswer,      PipeProcess::PypeType::NO_ZERO);

#if defined(_DBG_) && defined (CODE_OUTPUT_LOG)
    WD_LOG("Test: " + TLM.mTest + "\nOutput: " + TLM.mOutput + "\nAnswer: " + TLM.mAnswer);
#endif

    checker.run();
    checker.readPipe(mChecksInfo[aThreadNum].mResult);

    mChecksMutexs[aThreadNum].lock();
    mChecksInfo[aThreadNum].mIsFinishedTesting = true;
    mChecksMutexs[aThreadNum].unlock();
}