#ifndef CORE_H
#define CORE_H

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

#include <list>

#include "domain/string.hpp"
#include "database/database_query.hpp"

#include "process/process.hpp"

#include "test.hpp"

//--------------------------------------------------------------------------------

namespace test
{
    class Core
    {
    public:
        Core(std::string aDatabasePath) noexcept;
        ~Core() = default;
        void run(int aID) noexcept;

    private:
        std::list<Test> mTests;
        data::DatabaseQuery mDBQ;

        std::string mFinalVerdict;
        uint_64 mFinalTime;
        uint_64 mFinalMemory;

        std::shared_ptr<proc::Process> prepareSolutionProcess(SubmissionInfo& aSubInfo)    noexcept;
        std::shared_ptr<proc::Process> prepareCheckerProcess(SubmissionInfo& aSubInfo)     noexcept;
        
        std::shared_ptr<proc::Process> prepareFile(CodeInfo& aCodeInfo, SubmissionInfo& aSubInfo)     noexcept;
        
        void check(uint_64 aID) noexcept;
    };
}

//--------------------------------------------------------------------------------

#endif // !CORE_H
