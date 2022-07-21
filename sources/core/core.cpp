#include "core.hpp"

//--------------------------------------------------------------------------------
//				CHILD PROCESS CREATION IMPLIMENTATION 
//--------------------------------------------------------------------------------


#undef max
#undef min

#include <cmath>

#define THREAD_COUNTS 30
#define DEBUG_PLUS_SOLUTION_SUBMISSION 1

#if DEBUG_PLUS_SOLUTION_SUBMISSION
#define _DEBUG_SOL_SUB_
#endif

Core::Core
(
    std::string aDatabasePath
) noexcept :
    mDBQ            (aDatabasePath),
    mFinalVerdict   (OK),
    mFinalTime      (0),
    mFinalMemory    (0)
{
    // mProblemInfo.mResult = "ok";
    // for (uint_32 i = 0; i < THREAD_COUNTS; ++i)
    // {
    //     mChecks.push_back(new std::thread([](){}));
    // }

}

Core::~Core()
{
    // for (uint_32 i = 0; i < mChecks.size(); ++i)
    // {
    //     if (mChecks[i] != NULL)
    //     {
    //         mChecks[i]->join();
    //         delete mChecks[i];
    //     }
    // }
}

void
Core::run(int aID) noexcept
{
    auto partInfo = mDBQ.getSubmissionInfo(aID);
    mDBQ.prepareTestsStatement(partInfo.problemID);
    partInfo.solutionFileName = "2\\plus.cpp";

    auto solProc = prepareSolutionProcess(partInfo);
    auto checkProc = prepareCheckerProcess(partInfo);
    solProc->setLimits(partInfo.timeLimit, partInfo.memoryLimit);

    Test testTemplate(solProc, checkProc);
    testTemplate.setTimeLimit(partInfo.timeLimit);
    testTemplate.setMemoryLimit(partInfo.memoryLimit);
    mTests.resize(THREAD_COUNTS, testTemplate);
    //testTemplate.setSolutionTemplate(solProc);
    //testTemplate.setCheckerTemplate(checkProc);

    check(aID);
}

std::shared_ptr<proc::Process>
Core::prepareSolutionProcess(SubmissionInfo& aSubInfo) noexcept
{
    std::shared_ptr<proc::Process> result = std::make_shared<proc::Process>();

    cor::CodeInfo codeInfo(cor::CodeInfo::CodeInfoType::Submission);
    codeInfo.setFileName(std::move(aSubInfo.solutionFileName));
    codeInfo.setDesirableOutputFileName(std::to_string(aSubInfo.ID));
    //TODO: set language
    codeInfo.setLanguage();

    sys::Compiler comp;
    auto cmd = comp.getExecutableCommand(std::move(codeInfo));
    result->setComand(cmd);
    return result;

    // return std::make_shared<proc::Process>();
}

std::shared_ptr<proc::Process>
Core::prepareCheckerProcess(SubmissionInfo& aSubInfo) noexcept
{
    std::shared_ptr<proc::Process> result = std::make_shared<proc::Process>();

    cor::CodeInfo codeInfo(cor::CodeInfo::CodeInfoType::Checker);
    codeInfo.setFileName(std::move(aSubInfo.checkerFileName));
    codeInfo.setDesirableOutputFileName(std::to_string(aSubInfo.ID));
    //TODO: set language
    codeInfo.setLanguage();

    sys::Compiler comp;
    auto cmd = comp.getExecutableCommand(std::move(codeInfo));
    result->setComand(cmd);
    return result;

    // return std::make_shared<proc::Process>();
}

void
Core::check(uint_64 aID) noexcept
{
    START_LOG_BLOCK("Checking_participant_code");

    bool stillTesting = true;
    for (auto it = --mTests.end(); mTests.size(); --it)
    {
        if (it == mTests.begin()) it = --mTests.end();

        if (it->isFinished())
        {
            if (it->getVerdict() != OK) 
            {
                mFinalVerdict = it->getVerdict();
                stillTesting = false;
            }
            mFinalTime = std::max(mFinalTime, it->getUsedTime());
            mFinalMemory = std::max(mFinalMemory, it->getUsedMemory());

            if (stillTesting) it->run(mDBQ);
            else
            {
                if (mTests.size() == 1) break;
                mTests.erase(it++);
            }
        }

        if (stillTesting && it->testsAreOver())
        {
            if (mTests.size() == 1) break;
            mTests.erase(it++);
            stillTesting = false;
        }    
    }
    mTests.clear();

    mDBQ.writeResult(aID, mFinalVerdict, mFinalTime, mFinalMemory);

    WRITE_LOG("Final_result:", mFinalVerdict);
    WRITE_LOG("Final_time:", mFinalTime);
    END_LOG_BLOCK("Final_memory:", mFinalMemory);
}