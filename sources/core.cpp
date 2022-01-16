#include "core.h"

#undef max
#undef min

#include <math.h>

Core::Core
(
    std::string aDatabasePath
) :
    mDBQ    (aDatabasePath)
{}

void
Core::run
(
    int aID
)

{
    mSubInfo.id = aID;
    mDBQ.prepareForTesting(mSubInfo);

#ifdef BILL_WINDOWS
    mSubInfo.mSolutionFileName =  makeWindowString(mSubInfo.mSolutionFileName);
    mSubInfo.mCheckerFileName = makeWindowString(mSubInfo.mCheckerFileName);
#endif // BILL_WINDOWS

    std::string solutionName = makeExecutable(MAEDIA + mSubInfo.mSolutionFileName,
        SOLUTION_PATH + "-" + std::to_string(mSubInfo.id ));
    std::string checkerName = makeExecutable(MAEDIA + mSubInfo.mCheckerFileName,
        CHECKER_PATH + "-" + std::to_string(mSubInfo.id));
    
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
        
#ifdef _DBG_
        std::string compilerOutput;
        //compiler.readPipe(compilerOutput);
#endif
       // WD_LOG(compilerOutput);
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
    std::string aSolutionName,
    std::string aCheckerName
)
{
#ifdef _DBG_
    //std::vector<std::pair<uint_64, uint_64>> allTimes;
    mSubInfo.mMemoryLimit += 1000000;
#endif // _DBG_
    mDBQ.prepareTestsStatement(mSubInfo);
    for (; !mSubInfo.mTestsAreOver;)
    {
        pipesTesting(aSolutionName, aCheckerName);

        if (mSubInfo.mResult.substr(0, 2) != "ok")
        {
            WD_LOG("Result not okay");
            WD_LOG("Today result is: " << mSubInfo.mResult);
            break;
        }
        else if (mSubInfo.mTimeLimit < mSubInfo.mUsedTime)
        {
            mSubInfo.mResult = "tle";
            WD_LOG("Result is TLE: wanted " << mSubInfo.mTimeLimit <<
                " vs received " << mSubInfo.mUsedTime);
            break;
        }
        else if (mSubInfo.mMemoryLimit < mSubInfo.mUsedMemory)
        {
            mSubInfo.mResult = "mle";
            WD_LOG("Result is MLE: wanted " << mSubInfo.mMemoryLimit <<
                " vs received " << mSubInfo.mUsedMemory);
            break;
        }

        WD_END_LOG;
    }

    mSubInfo.remakeResultForPasha();
    mDBQ.writeResult(mSubInfo.id, mSubInfo.mResult, mSubInfo.mUsedTime, mSubInfo.mUsedMemory);

    WD_LOG("Final result : " + mSubInfo.mResult);
    WD_LOG("Final time : " << mSubInfo.mUsedTime);
    WD_LOG("Final memory : " << mSubInfo.mUsedMemory);
    WD_END_LOG;
}

void
Core::pipesTesting
(
    std::string aSolutionName,
    std::string aCheckerName
)
{
    WD_LOG("Checking test #" << mSubInfo.mTestsCount);
    WD_LOG("Runing test #" << mSubInfo.mTestsCount);

    if (mSubInfo.mTestsCount == 100)
    {
        int yy = 0;
        std::cout << yy;
    }

    TestLibMessage TLM;
    mDBQ.getNextTest(mSubInfo, TLM);
    if (mSubInfo.mTestsAreOver) return;

    std::string ss = "";
    PipeProcess code(ss, aSolutionName);

    TLM.makeTestSizes();   
    code.writePipe(TLM.mTest);
    std::pair<uint_64, uint_64> cur = code.runWithLimits(mSubInfo.mTimeLimit, mSubInfo.mMemoryLimit);   
    if (cur.first == -1)
    {
        mSubInfo.mResult = "wa";
        return;
    }
    code.readPipe(TLM.mOutput);

    mSubInfo.mUsedTime = std::max(mSubInfo.mUsedTime, cur.first);
    mSubInfo.mUsedMemory = std::max(mSubInfo.mUsedMemory, cur.second);

    PipeProcess checker(aCheckerName, ss);

    TLM.mAnswer.pop_back();
    TLM.mAnswer.pop_back();
    TLM.makeOutputSizes();
    TLM.makeAnswerSizes();

    checker.writePipe(TLM.mTestSize, PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mTest, PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mOutputSize, PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mOutput, PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mAnswerSize, PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mAnswer, PipeProcess::PypeType::NO_ZERO);

#ifdef _DBG_
    WD_LOG("Test: " + TLM.mTest + "\n Output: " + TLM.mOutput + "\n Answer: " + TLM.mAnswer);
#endif

    checker.run();
    checker.readPipe(mSubInfo.mResult);
}