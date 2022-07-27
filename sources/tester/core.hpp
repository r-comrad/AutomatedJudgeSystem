#ifndef CORE_H
#define CORE_H

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

#include <list>

#include "domain/string.hpp"
#include "database/database_query.hpp"

#include "process/process.hpp"

#include "test.hpp"
#include "code_info.hpp"
#include "thread_signals.hpp"

//--------------------------------------------------------------------------------

namespace test
{
    class Core
    {
    public:
        Core(const std::string& aDatabasePath) noexcept;
        ~Core() = default;

        Core(const Core& other) noexcept = default;
        Core& operator=(const Core& other) noexcept = default;

        Core(Core&& other) noexcept = default;
        Core& operator=(Core&& other) noexcept = default;
        
        void run(int aID) noexcept;

    private:
        test::ThreadSignals mThreadSignals;
        std::vector<Test> mTests;
        data::DatabaseQuery mDBQ;

        Test::TestVerdict mFinalVerdict;
        uint_64 mFinalTime;
        uint_64 mFinalMemory;

        std::shared_ptr<proc::Process> prepareSolutionProcess
            (SubmissionInfo& aSubInfo) const noexcept;
        std::shared_ptr<proc::Process> prepareCheckerProcess
            (SubmissionInfo& aSubInfo) const noexcept;
        
        std::shared_ptr<proc::Process> prepareFile
            (CodeInfo& aCodeInfo, SubmissionInfo& aSubInfo) const noexcept;
        
        void check(uint_64 aID) noexcept;

        std::string verdictTostring(const Test::TestVerdict& aVerdict) 
            const noexcept;
    };
}

//--------------------------------------------------------------------------------

#endif // !CORE_H
