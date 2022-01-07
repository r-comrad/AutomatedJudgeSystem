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
    //std::wstring sss = makeExecutable(MAIN_PATH + L"plusTwo.cpp", WORK_DIR + L"plusTwo1");
    //Process p;
    //p.IORedirection(Process::IOType::PIPES);
    //p.writePipe("44\n");
    //p.create(sss, L"");
    //p.run();
    //p.writePipe("44\n");
    //std::cout << p.readPipe();
    //return;

    mSubInfo.id = aID;

    mDBQ.prepareForTesting(mSubInfo);
    //mDBQ.getAllTests(mSubInfo);

    mSubInfo.mSolutionFileName.pop_back();
    mSubInfo.mSolutionFileName.pop_back();
    mSubInfo.mSolutionFileName.pop_back();
    mSubInfo.mSolutionFileName.pop_back();
    mSubInfo.mSolutionFileName += "plus.cpp";

    std::string solutionName = makeExecutable(MAEDIA + makeWindowString(mSubInfo.mSolutionFileName), 
        SOLUTION_PATH + "-" + std::to_string(mSubInfo.id ));
    std::string checkerName = makeExecutable(MAEDIA + makeWindowString(mSubInfo.mCheckerFileName), 
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
    }
    else if (aLanguage == Language::SNAKE)
    {
        result += ".py";
        copyFile(aFileName, result);
        result = "python " + result;
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


//std::wstring 
//makeComand
//(
//    char aFileEnding
//)
//{
//    std::wstring curPath = getMainPath() + RESOURCES + L"task1\\";
//    std::map<char, std::wstring> m =
//    {
//        { 'y', L"python " + curPath + L"solution\\plus.py" },
//        { 'p', curPath + L"solution\\" plus.exe" },
//        { 'e',  curPath + L"solution\\plus.exe" }
//    };
//    std::wstring comand = m[aFileEnding];
//    return comand;
//}

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
   // for (int testNum = 0; testNum < mSubInfo.mTestsCount; ++testNum)
   for (;!mSubInfo.mTestsAreOver;)
    {
        //fileTesting(testNum, aSolutionName, aCheckerName);
        pipesTesting(aSolutionName, aCheckerName);

        if (mSubInfo.mResult != "ok")
        {
            //WD_ERROR(mainCheck.0, "Can't open file " + makeGoodString(aTaskPath) + "init");
            WD_LOG("Result not okay");
            WD_LOG("Today result is " << mSubInfo.mResult);
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

//    WD_LOG("Times : ");
//#ifdef _DBG_
//    for (auto& i : allTimes) WD_LOG(i.first << " " << i.second);
//    WD_END_LOG;
//    WD_LOG("Min time : " << (*std::max_element(allTimes.begin(), allTimes.end())).first);
//    WD_LOG("Max time : " << (*std::min_element(allTimes.begin(), allTimes.end())).first);
//#endif // _DBG_
}

void
Core::fileTesting
(
    uint_32 aTestNum,
    std::string aSolutionName,
    std::string aCheckerName
)
{
    WD_LOG("Checking test #" << aTestNum);
    WD_LOG("Runing test #" << aTestNum);

    std::string testAddress = TEST_PATH + std::to_string(mSubInfo.id) + "-" + std::to_string(aTestNum);
    std::string outAddress = OUTPUT_PATH + std::to_string(mSubInfo.id) + "-" + std::to_string(aTestNum);
    std::string ss = "";

    FileProcess participantCode(ss, aSolutionName, testAddress, outAddress);
    std::pair<uint_64, uint_64> cur = participantCode.runWithLimits(mSubInfo.mTimeLimit, mSubInfo.mMemoryLimit);
    //MyProcess* participantCode = new FileProcess (ss, aSolutionName, testAddress, outAddress);
    //std::pair<uint_64, uint_64> cur = participantCode->runWithLimits(mSubInfo.mTimeLimit, mSubInfo.mMemoryLimit);

    mSubInfo.mUsedTime = std::max(mSubInfo.mUsedTime, cur.first);
    mSubInfo.mUsedMemory = std::max(mSubInfo.mUsedMemory, cur.second);

    WD_LOG("Runing checker #" << aTestNum);

    std::string answerAddress = ANSWERS_PATH + std::to_string(mSubInfo.id) + "-" + std::to_string(aTestNum);
    std::string resultAddress = RESULT_PATH + std::to_string(mSubInfo.id) + "-" + std::to_string(aTestNum);
    std::string parameters = "sas input " + outAddress + " " + answerAddress;
    FileProcess checker(aCheckerName, parameters, ss, resultAddress);
    checker.run();
    //MyProcess* checker = new FileProcess (aCheckerName, parameters, ss, resultAddress);
    //checker->run();

    std::ifstream resultFile(resultAddress);
    resultFile >> mSubInfo.mResult;
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
    std::string ss = "";
    PipeProcess code(ss, aSolutionName);
    //code.IORedirection(Process::IOType::PIPES);

    TestLibMessage TLM;
    mDBQ.getNextTest(mSubInfo, TLM);
    TLM.makeTestSizes();   
    code.writePipe(TLM.mTest);

    std::pair<uint_64, uint_64> cur = code.runWithLimits(mSubInfo.mTimeLimit, mSubInfo.mMemoryLimit);   
    code.readPipe(TLM.mOutput);
    //code.closeIO();

    //int aTestNum = mSubInfo.mTestsCount - 1;
    //std::wstring testAddress = TEST_PATH + std::to_wstring(mSubInfo.id) + L"-" + std::to_wstring(aTestNum);
    //std::wstring outAddress = OUTPUT_PATH + std::to_wstring(mSubInfo.id) + L"-" + std::to_wstring(aTestNum);
    //std::ofstream ffo(outAddress);
    //ffo << TLM.mOutput +'\n';
    //ffo.close();

    mSubInfo.mUsedTime = std::max(mSubInfo.mUsedTime, cur.first);
    mSubInfo.mUsedMemory = std::max(mSubInfo.mUsedMemory, cur.second);

    //WD_LOG("Runing checker #" << aTestNum);
    PipeProcess checker(aCheckerName, ss);
    //std::wstring answerAddress = ANSWERS_PATH + std::to_wstring(mSubInfo.id) + L"-" + std::to_wstring(aTestNum);
    //std::wstring resultAddress = RESULT_PATH + std::to_wstring(mSubInfo.id) + L"-" + std::to_wstring(aTestNum);
    //std::wstring parameters = L"sas input " + outAddress + L" " + answerAddress;
    //checker.IORedirection(Process::IOType::FILES, L"", resultAddress);
    //checker.IORedirection(Process::IOType::PIPES);
    //checker.create(aCheckerName, parameters);
    TLM.makeOutputSizes();
    TLM.makeAnswerSizes();

    //TLM.mAnswer.pop_back();
    //TLM.mAnswer.pop_back();
    checker.writePipe(TLM.mTestSize, PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mTest, PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mOutputSize, PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mOutput, PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mAnswerSize, PipeProcess::PypeType::NO_ZERO);
    checker.writePipe(TLM.mAnswer, PipeProcess::PypeType::NO_ZERO);

#ifdef _DBG_
    WD_LOG("Test: " + TLM.mTest + "\n Output: " + TLM.mOutput + "\n Answer: " + TLM.mAnswer);
#endif

    //checker.writePipe("\000\000\000\000\000\000\000\000\001\000\000\000\000\000\000\0001\001\000\000\000\000\000\000\0001", Process::PypeType::NO_ZERO);
    //checker.create(aCheckerName, "");
    checker.run();

    checker.readPipe(mSubInfo.mResult);
    //std::ifstream resultFile(resultAddress);
    //resultFile >> mSubInfo.mResult;
}