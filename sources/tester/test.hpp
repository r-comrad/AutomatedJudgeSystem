#ifndef TEST_HPP
#define TEST_HPP

#include <map>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <future>
#include <memory>

#include "database/database_query.hpp"

#include "process/process.hpp"

#include "testlib_message.hpp"

#include "compiler.hpp"

#define OK      "OK"
#define WA      "WA"
#define TLE     "TL"
#define MLE     "ML"
#define PE      "PE"

class Test
{
public:
    enum class TestState { Testing, Finished, RunOut };

    Test(std::shared_ptr<proc::Process> aSolutionTemplate,
        std::shared_ptr<proc::Process> aCheckerTemplate) noexcept;

    Test(Test&& other) noexcept = default;
    Test(const Test& other) noexcept;

    Test& operator=(Test&& other) noexcept = default;

    ~Test();

    bool isFinished() const noexcept;    
    bool testsAreOver() const noexcept;

    std::string getVerdict() const noexcept;
    uint_64 getUsedTime() const noexcept;
    uint_64 getUsedMemory() const noexcept;

    void setLimits(const Limits& aTimeMemLimits) noexcept;

    void run(data::DatabaseQuery& aDBQ) noexcept;

private:
    int mTestNumber;
    proc::Process mSolutionProcess;
    proc::Process mCheckerProcess;

    std::atomic<TestState> mTestState;

    TestLibMessage mTLM;

    std::shared_ptr<proc::Process> mSolutionTemplate;
    std::shared_ptr<proc::Process> mCheckerTemplate;

    std::string mVerdict;
    uint_64 mUsedTime;
    uint_64 mUsedMemory;
    uint_64 mTimeLimit;
    uint_64 mMemoryLimit;

    std::thread mTestThread;

    void runTesting(data::DatabaseQuery& aDBQ) noexcept;

    void checkTest() noexcept;
    void getTest(data::DatabaseQuery& aDBQ) noexcept;
    void resultEvoluation() noexcept;
};

#endif // !TEST_HPP