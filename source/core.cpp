#include "core.h"

#undef max
#undef min

#include <math.h>

Core::Core
(
    std::wstring aDatabasePath
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

    mDBQ.prepareForTesting(mSubInfo, MDatabaseQuery::DataStructure::MAGIC_IVAN);

    mSubInfo.mSolutionFileName.pop_back();
    mSubInfo.mSolutionFileName.pop_back();
    mSubInfo.mSolutionFileName.pop_back();
    mSubInfo.mSolutionFileName.pop_back();
    mSubInfo.mSolutionFileName += L"plus.cpp";

    std::wstring solutionName = makeExecutable(MAEDIA + makeWindowString(mSubInfo.mSolutionFileName), 
        SOLUTION_PATH + L"-" + std::to_wstring(mSubInfo.id ));
    std::wstring checkerName = makeExecutable(MAEDIA + makeWindowString(mSubInfo.mCheckerFileName), 
        CHECKER_PATH + L"-" + std::to_wstring(mSubInfo.id + 15));
    check(solutionName, checkerName);
}

std::wstring
Core::makeExecutable
(
    std::wstring aFileName, 
    std::wstring aOutputName
)
{
    Core::Language language = getLanguage(aFileName);
    return compile(aFileName, aOutputName, language);
}

Core::Language
Core::getLanguage
(
    std::wstring aFileName
)
{
    int num = aFileName.find('.');
    if (num == -1) return Core::Language::NUN;

    Core::Language result = Core::Language::NUN;
    std::wstring fileExtension = aFileName.substr(num + 1);
    if (fileExtension == L"cpp") result = Core::Language::MagicCPP;
    if (fileExtension == L"py") result = Core::Language::Snake;
    return result;
}

std::wstring
Core::compile
(
    std::wstring aFileName,
    std::wstring aOutName, 
    Language aLanguage
)
{
    std::wstring result = aOutName;
    if (aLanguage == Language::MagicCPP)
    {
        result += L".exe";
        Process process;
        //process.IORedirection(L"", L"");
        std::wstring comand = COMPILERS + L"magicCPPCompiler.cmd" + L" " + 
            aFileName + L" " + result;
        process.create(L"", comand);
        process.run();
    }
    else if (aLanguage == Language::Snake)
    {
        result += L".py";
        copyFile(aFileName, result);
        result = L"python " + result;
    }
    for (int i = 0; i < 1e7; ++i);
    return result;
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
    std::wstring aSolutionName, 
    std::wstring aCheckerName
)
{
#ifdef _DBG_
    //std::vector<std::pair<uint_64, uint_64>> allTimes;
    mSubInfo.mTimeLimit += 100000;
    mSubInfo.mMemoryLimit += 1000000;
#endif // _DBG_

    //for (int testNum = 0; testNum < mSubInfo.mTestsCount; ++testNum)
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
    std::wstring aSolutionName,
    std::wstring aCheckerName
)
{
    WD_LOG("Checking test #" << aTestNum);
    WD_LOG("Runing test #" << aTestNum);
    Process code;
    std::wstring testAddress = TEST_PATH + std::to_wstring(mSubInfo.id) + L"-" + std::to_wstring(aTestNum);
    std::wstring outAddress = OUTPUT_PATH + std::to_wstring(mSubInfo.id) + L"-" + std::to_wstring(aTestNum);

    code.IORedirection(Process::IOType::FILES, testAddress, outAddress);
    code.create(L"", aSolutionName);
    std::pair<uint_64, uint_64> cur = code.runWithLimits(mSubInfo.mTimeLimit, mSubInfo.mMemoryLimit);

    mSubInfo.mUsedTime = std::max(mSubInfo.mUsedTime, cur.first);
    mSubInfo.mUsedMemory = std::max(mSubInfo.mUsedMemory, cur.second);

    WD_LOG("Runing checker #" << aTestNum);
    Process checker;
    std::wstring answerAddress = ANSWERS_PATH + std::to_wstring(mSubInfo.id) + L"-" + std::to_wstring(aTestNum);
    std::wstring resultAddress = RESULT_PATH + std::to_wstring(mSubInfo.id) + L"-" + std::to_wstring(aTestNum);
    std::wstring parameters = L"sas input " + outAddress + L" " + answerAddress;
    checker.IORedirection(Process::IOType::FILES, L"", resultAddress);
    checker.create(aCheckerName, parameters);
    checker.run();

    std::ifstream resultFile(resultAddress);
    resultFile >> mSubInfo.mResult;
}

void
Core::pipesTesting
(
    std::wstring aSolutionName,
    std::wstring aCheckerName
)
{
    WD_LOG("Checking test #" << mSubInfo.mTestsCount);
    WD_LOG("Runing test #" << mSubInfo.mTestsCount);
    Process code;
    code.IORedirection(Process::IOType::PIPES);

    TestLibMessage TLM;
    mDBQ.getNextTest(mSubInfo, TLM);
    TLM.makeTestSizes();   
    code.writePipe(TLM.mTest);

    code.create(L"", aSolutionName);
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
    Process checker;
    //std::wstring answerAddress = ANSWERS_PATH + std::to_wstring(mSubInfo.id) + L"-" + std::to_wstring(aTestNum);
    //std::wstring resultAddress = RESULT_PATH + std::to_wstring(mSubInfo.id) + L"-" + std::to_wstring(aTestNum);
    //std::wstring parameters = L"sas input " + outAddress + L" " + answerAddress;
    //checker.IORedirection(Process::IOType::FILES, L"", resultAddress);
    checker.IORedirection(Process::IOType::PIPES);
    //checker.create(aCheckerName, parameters);
    TLM.makeOutputSizes();
    TLM.makeAnswerSizes();

    checker.writePipe(TLM.mTestSize, Process::PypeType::NO_ZERO);
    checker.writePipe(TLM.mTest, Process::PypeType::NO_ZERO);
    checker.writePipe(TLM.mOutputSize, Process::PypeType::NO_ZERO);
    checker.writePipe(TLM.mOutput, Process::PypeType::NO_ZERO);
    checker.writePipe(TLM.mAnswerSize, Process::PypeType::NO_ZERO);
    checker.writePipe(TLM.mAnswer, Process::PypeType::NO_ZERO);

#ifdef _DBG_
    WD_LOG(TLM.mTest << TLM.mOutput << TLM.mAnswer);
#endif

    //checker.writePipe("\000\000\000\000\000\000\000\000\001\000\000\000\000\000\000\0001\001\000\000\000\000\000\000\0001", Process::PypeType::NO_ZERO);
    checker.create(aCheckerName, L"");
    checker.run();

    checker.readPipe(mSubInfo.mResult);
    //std::ifstream resultFile(resultAddress);
    //resultFile >> mSubInfo.mResult;
}