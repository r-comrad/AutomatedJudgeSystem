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
    mSubInfo.id = aID;
    mDBQ.makeTestCatalog(mSubInfo);
    std::wstring solutionName = makeExecutable(MAEDIA + makeWindowString(mSubInfo.mSolutionFileName), SOLUTION_PATH);
    std::wstring checkerName = makeExecutable(MAEDIA + makeWindowString(mSubInfo.mCheckerFileName), CHECKER_PATH);
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
        std::wstring comand = COMPILERS + L"\\magicCPPCompiler.cmd" + L" " + aFileName + L" " + result;
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

std::wstring 
makeComand
(
    char aFileEnding
)
{
    std::wstring curPath = getMainPath() + RESOURCES + L"task1\\";
    std::map<char, std::wstring> m =
    {
        { 'y', L"python " + curPath + L"solution\\plus.py" },
        { 'p', curPath + L"solution\\plus.exe" },
        { 'e',  curPath + L"solution\\plus.exe" }
    };
    std::wstring comand = m[aFileEnding];
    return comand;
}

void
Core::check
(
    std::wstring aSolutionName, 
    std::wstring aCheckerName
)
{
    std::string resultSS;

    std::map<std::string, std::string> decoder = { {"ok", "OK"}, {"wa", "WA"}, {"tle", "TL"}, {"mle", "ML"}, {"pe", "PE"} };

#ifdef _DBG_
    std::vector<std::pair<uint_64, uint_64>> allTimes;
#endif // _DBG_

    uint_64 mtm = 1e9;
    std::pair<uint_64, uint_64> results = { 0,0 };
    for (int testNum = 0; testNum < mSubInfo.mTestsCount; ++testNum)
    {
        Process code;
        std::wstring testAddress = TEST_PATH + std::to_wstring(testNum);
        std::wstring outAddress = OUTPUT_PATH + std::to_wstring(testNum);

        code.IORedirection(testAddress, outAddress);
        code.create(L"", aSolutionName);
        std::pair<uint_64, uint_64> cur = code.run(0, 0);

#ifdef _DBG_
        allTimes.push_back(cur);
#endif // _DBG_

        results.first = std::max(results.first, cur.first);
        results.second = std::max(results.second, cur.second);
        mtm = std::min(mtm, cur.first);

        Process checker;
        std::wstring answerAddress = ANSWERS_PATH + std::to_wstring(testNum);
        std::wstring resultAddress = RESULT_PATH + std::to_wstring(testNum);
        std::wstring parameters = L"sas input " + outAddress + L" " + answerAddress;
        checker.IORedirection(L"", resultAddress);
        checker.create(aCheckerName, parameters);
        checker.run();

        std::ifstream resultFile(resultAddress);
        resultFile >> resultSS;
        if (resultSS != "ok")
        {
            //WD_ERROR(mainCheck.0, "Can't open file " + makeGoodString(aTaskPath) + "init");
            WD_LOG("Result not okay");
            break;
        }
        else if (mSubInfo.mTimeLimit < results.first)
        {
            resultSS = "tle";
            WD_LOG("Result is TLE" << mSubInfo.mTimeLimit << " vs " << results.first);
            break;
        }
        else if (mSubInfo.mMemoryLimit < results.second)
        {
            resultSS = "mle";
            WD_LOG("Result is MLE" << mSubInfo.mMemoryLimit << " vs " << results.second);
            break;
        }
    }
    resultSS = decoder[resultSS];
    mDBQ.writeResult(mSubInfo.id, resultSS, results.first, results.second);

    WD_LOG("Final result : " + resultSS);
    WD_LOG("Final time : " << results.first);
    WD_LOG("Final memory : " << results.second);
    WD_END_LOG;

    WD_LOG("Times : ");
#ifdef _DBG_
    for (auto& i : allTimes) WD_LOG(i.first << " " << i.second);
    WD_END_LOG;
    WD_LOG("Min time : " << (*std::max_element(allTimes.begin(), allTimes.end())).first);
    WD_LOG("Max time : " << (*std::min_element(allTimes.begin(), allTimes.end())).first);
#endif // _DBG_

    std::cout << results.first << " " << mtm;
}