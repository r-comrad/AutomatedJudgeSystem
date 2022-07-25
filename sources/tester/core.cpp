#include "core.hpp"

//--------------------------------------------------------------------------------

#undef max
#undef min

#include <cmath>

#include "compiler.hpp"

//--------------------------------------------------------------------------------

#define THREAD_COUNTS 30
#define DEBUG_PLUS_SOLUTION_SUBMISSION 1

//--------------------------------------------------------------------------------

test::Core::Core
(
    std::string aDatabasePath
) noexcept :
    mDBQ            (aDatabasePath),
    mFinalVerdict   (OK),
    mFinalTime      (0),
    mFinalMemory    (0)
{}

//--------------------------------------------------------------------------------

void
test::Core::run(int aID) noexcept
{
    auto partInfo = mDBQ.getSubmissionInfo(aID);
    mDBQ.prepareTestsStatement(partInfo.problemID);

    //TODO: remove (this is for debug)
    partInfo.solutionFileName = "2\\plus.cpp";

    auto solProc = prepareSolutionProcess(partInfo);
    auto checkProc = prepareCheckerProcess(partInfo);
    solProc->setLimits(partInfo.timeMemLim);

    Test testTemplate(solProc, checkProc);
    testTemplate.setLimits(partInfo.timeMemLim);
    mTests.resize(THREAD_COUNTS, testTemplate);

    check(aID);
}

//--------------------------------------------------------------------------------

std::shared_ptr<proc::Process>
test::Core::prepareSolutionProcess(SubmissionInfo& aSubInfo) noexcept
{
    CodeInfo codeInfo(CodeInfo::CodeInfoType::Submission);
    codeInfo.setFileName(std::move(aSubInfo.solutionFileName));
    return prepareFile(codeInfo, aSubInfo);
}

//--------------------------------------------------------------------------------

std::shared_ptr<proc::Process>
test::Core::prepareCheckerProcess(SubmissionInfo& aSubInfo) noexcept
{
    CodeInfo codeInfo(CodeInfo::CodeInfoType::Checker);
    codeInfo.setFileName(std::move(aSubInfo.checkerFileName));
    return prepareFile(codeInfo, aSubInfo);

}

//--------------------------------------------------------------------------------

std::shared_ptr<proc::Process> 
test::Core::prepareFile(CodeInfo& aCodeInfo, SubmissionInfo& aSubInfo) noexcept
{
    std::shared_ptr<proc::Process> result = std::make_shared<proc::Process>();

    aCodeInfo.setDesirableOutputFileName(std::to_string(aSubInfo.ID));
    aCodeInfo.setLanguage();

    Compiler comp;
    auto cmd = comp.getExecutableCommand(std::move(aCodeInfo));
    result->setComand(cmd);
    return result;
}

//--------------------------------------------------------------------------------

void
test::Core::check(uint_64 aID) noexcept
{
    START_LOG_BLOCK("Checking_participant_code");

 mTests.erase( mTests.begin());

    bool stillTesting = true;
    while(mTests.size())
    {
        for(auto it = mTests.begin(); mTests.size() && it != mTests.end();)
        {          
            if (it->isFinished())
            {
                auto verdict = it->getVerdict();
                stillTesting &= verdict == OK & !it->testsAreOver();

                if (mFinalVerdict == OK) 
                {
                    mFinalVerdict = verdict;
                }
                mFinalTime = std::max(mFinalTime, it->getUsedTime());
                mFinalMemory = std::max(mFinalMemory, it->getUsedMemory());

                if (stillTesting) 
                {
                    it->run(mDBQ);
                }
                else
                {
                    if (mTests.size() == 1 || it == mTests.begin()) 
                    {
                        int yy;
                        yy = 0;
                    }
                    auto temp = it;
if (it != mTests.begin()) --temp;
else temp = ++mTests.begin();
                    mTests.erase(it);
                    it = temp;
                }
            }  

            ++it;
        }
    }

    mDBQ.writeResult(aID, mFinalVerdict, mFinalTime, mFinalMemory);

    WRITE_LOG     ("Final_result:", mFinalVerdict);
    WRITE_LOG     ("Final_time:",   mFinalTime);
    END_LOG_BLOCK ("Final_memory:", mFinalMemory);
}

//--------------------------------------------------------------------------------
