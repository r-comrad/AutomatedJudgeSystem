#ifndef TEST_HPP
#define TEST_HPP

//--------------------------------------------------------------------------------

#include <map>
#include <thread>
#include <memory>

#include "database/database_query.hpp"

#include "process/process.hpp"

#include "testlib_message.hpp"
#include "thread_signals.hpp"

//--------------------------------------------------------------------------------

namespace test
{
    class Test
    {
    public:
        enum class TestVerdict {NUN, OK, WA, TLE, MLE, PE};

        Test(std::shared_ptr<proc::Process> aSolutionTemplate,
            std::shared_ptr<proc::Process> aCheckerTemplate,
            ThreadSignals* aThreadSignals) noexcept;
        ~Test();

        Test(const Test& other) noexcept;
        Test& operator=(const Test& other) noexcept;

        Test(Test&& other) noexcept = default;
        Test& operator=(Test&& other) noexcept = default;

        bool isFinished() const noexcept;    
        bool testsAreOver() const noexcept;

        TestVerdict getVerdict() const noexcept;
        uint_64 getUsedTime() const noexcept;
        uint_64 getUsedMemory() const noexcept;

        void setLimits(const Limits& aTimeMemLimits) noexcept;

        void run(data::DatabaseQuery& aDBQ) noexcept;

    private:
        proc::Process mSolutionProcess;
        proc::Process mCheckerProcess;
        std::shared_ptr<proc::Process> mSolutionTemplate;
        std::shared_ptr<proc::Process> mCheckerTemplate;

        test::ThreadSignals* mThreadSignals;
        std::thread mTestThread;
        TestLibMessage mTLM;

        //TODO: error test number                
        uint_64 mTestNumber;
        const uint_64 mNumberOfTester;
        static uint_64 globalTestersNumber;

        TestVerdict mVerdict;
        uint_64 mUsedTime;
        uint_64 mUsedMemory;
        uint_64 mTimeLimit;
        uint_64 mMemoryLimit;

        void runTesting(data::DatabaseQuery& aDBQ) noexcept;

        void checkTest() noexcept;
        bool getTest(data::DatabaseQuery& aDBQ) noexcept;
        void resultEvoluation() noexcept;
    };
}

//--------------------------------------------------------------------------------

#endif // !TEST_HPP