#include "core.hpp"

//--------------------------------------------------------------------------------

#undef max
#undef min

#include <cmath>

#include "compiler.hpp"

//--------------------------------------------------------------------------------

#define THREAD_COUNTS 30

//--------------------------------------------------------------------------------

test::Core::Core(const std::string& aDatabasePath) noexcept :
    mDBQ            (aDatabasePath),
    mFinalVerdict   (Test::TestVerdict::OK),
    mFinalTime      (0),
    mFinalMemory    (0),
    mThreadSignals  (THREAD_COUNTS)
{}

//--------------------------------------------------------------------------------

void
test::Core::run(int aID) noexcept
{
    auto partInfo = mDBQ.getSubmissionInfo(aID);
    mDBQ.prepareTestsStatement(partInfo.problemID);

    //TODO: remove (this is for debug)
    partInfo.solutionFileName = "2/plus.cpp";

    auto solProc = prepareSolutionProcess(partInfo);
    auto checkProc = prepareCheckerProcess(partInfo);
    solProc->setLimits(partInfo.timeMemLim);

    Test testTemplate(solProc, checkProc, &mThreadSignals);
    testTemplate.setLimits(partInfo.timeMemLim);
    mTests.resize(THREAD_COUNTS, testTemplate);

    check(aID);
}

//--------------------------------------------------------------------------------

std::shared_ptr<proc::Process>
test::Core::prepareSolutionProcess(data::SubmissionInfo& aSubInfo) const noexcept
{
    CodeInfo codeInfo(CodeInfo::CodeInfoType::Submission);
    codeInfo.setFileName(std::move(aSubInfo.solutionFileName));
    return prepareFile(codeInfo, aSubInfo);
}

//--------------------------------------------------------------------------------

std::shared_ptr<proc::Process>
test::Core::prepareCheckerProcess(data::SubmissionInfo& aSubInfo) const noexcept
{
    CodeInfo codeInfo(CodeInfo::CodeInfoType::Checker);
    codeInfo.setFileName(std::move(aSubInfo.checkerFileName));
    return prepareFile(codeInfo, aSubInfo);

}

//--------------------------------------------------------------------------------

std::shared_ptr<proc::Process> 
test::Core::prepareFile(CodeInfo& aCodeInfo, data::SubmissionInfo& aSubInfo) 
    const noexcept
{
    std::shared_ptr<proc::Process> result = std::make_shared<proc::Process>();

    aCodeInfo.setDesirableOutputFileName(std::to_string(aSubInfo.ID));
    aCodeInfo.setLanguage();

    Compiler comp;
    auto cmd = comp.getExecutableCommand(std::move(aCodeInfo));
    result->setComand(std::move(cmd));
    return result;
}

//--------------------------------------------------------------------------------

void
test::Core::check(uint64_t aID) noexcept
{
    START_LOG_BLOCK("Checking_participant_code");

    while(!mThreadSignals.isAllThreadsFinished())
    {         
        auto signal = mThreadSignals.getSignal();
        if (signal.has_value())
        {
            WRITE_LOG("Signal_number", signal.value());
            auto& test = mTests[signal.value()];
            auto verdict = test.getVerdict();

            if (mFinalVerdict != Test::TestVerdict::OK) 
            {
                mFinalVerdict = verdict;
            }
            mFinalTime = std::max(mFinalTime, test.getUsedTime());
            mFinalMemory = std::max(mFinalMemory, test.getUsedMemory());

            if (!mThreadSignals.isCheckingFinalTests()) 
            {
                test.run(mDBQ);
            }
        }  
    }

    auto tempVerdict = verdictTostring(mFinalVerdict);
    mDBQ.writeResult(aID, tempVerdict, mFinalTime, mFinalMemory);

    WRITE_LOG     ("Final_result:", tempVerdict);
    WRITE_LOG     ("Final_time:",   mFinalTime);
    END_LOG_BLOCK ("Final_memory:", mFinalMemory);
}

std::string
test::Core::verdictTostring(const Test::TestVerdict& aVerdict) const noexcept
{
    std::string result;
    switch (aVerdict)
    {
    case Test::TestVerdict::OK :
        result = "ok";
        break;
    case Test::TestVerdict::WA :
        result = "wa";
        break;
    case Test::TestVerdict::TLE :
        result = "tl";
        break;
    case Test::TestVerdict::MLE :
        result = "ml";
        break;
    case Test::TestVerdict::PE :
        result = "wa";
        break;
    case Test::TestVerdict::NUN :
        result = "wa";
        break;
    }
    return result;
}

//--------------------------------------------------------------------------------
