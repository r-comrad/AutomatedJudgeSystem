#include "database/database_query.hpp"

//--------------------------------------------------------------------------------

data::DatabaseQuery::DatabaseQuery (const std::string& aDatabasePath) noexcept :
    mDatabase                   (aDatabasePath),
    mReservedStatementNumber    (0),
    mTestNum                    (1),
    mTestAreOver                (false)
{}

//--------------------------------------------------------------------------------

data::SubmissionInfo 
data::DatabaseQuery::getSubmissionInfo(int ID) noexcept
{
    SubmissionInfo result;
    result.ID = ID;

    getParticipantInfo(result);
    getCheckerInfo(result);

    #ifdef LINUS_LINUX
    result.timeMemLim.timeLimit /= 1000;
    #endif 

    return result;
}

//--------------------------------------------------------------------------------

void 
data::DatabaseQuery::writeResult
(
    int                 aID, 
    const std::string&  aResult,
    int                 aTime, 
    int                 aMemory
) noexcept
{
    START_LOG_BLOCK("Updating_database");
    WRITE_LOG("Updating...");

    mDatabase.update("core_solutions", "result = '" + aResult + "'" +
        ", time = " + std::to_string(aTime) +
        ", memory = " + std::to_string(aMemory), "id = " + std::to_string(aID));
    mDatabase.step();
    mDatabase.closeStatment();

    END_LOG_BLOCK("Database_updated?");
}

//--------------------------------------------------------------------------------

std::optional<data::DatabaseQuery::TestData>
data::DatabaseQuery::getNextTest() noexcept
{
    std::optional<TestData> result;

    TestData data;
    uint32_t tempNum;
    std::optional<dom::CharArray> input;
    std::optional<dom::CharArray> output;

    mTestMutex.lock();
    if (!mTestAreOver)
    {
        WRITE_LOG("Taking_next_test");
        //if (mDatabase.step() != SQLITE_OK) break; TODO: fixing that
        mDatabase.step(mReservedStatementNumber);
        input = mDatabase.getTextFromRow(0, mReservedStatementNumber);
        output = mDatabase.getTextFromRow(1, mReservedStatementNumber);
        tempNum = mTestNum++;

        if (!input.has_value())
        {
            mTestAreOver = true;
        }
    }
    mTestMutex.unlock();

    if (input.has_value())
    {
        data.input = std::move(input.value());
        data.output = std::move(output.value());
        data.testNum = tempNum;
        result = std::make_optional<TestData>(std::move(data));
    }

    return result;
}

//--------------------------------------------------------------------------------

void
data::DatabaseQuery::prepareTestsStatement(uint64_t aProblemID) noexcept
{
    START_LOG_BLOCK("Prepare_geting_test_from_database");
    mDatabase.select("core_test", "input, output", "contest_id = " +
        std::to_string(aProblemID), mReservedStatementNumber);
    END_LOG_BLOCK("I'm_ready");
}

//--------------------------------------------------------------------------------

void
data::DatabaseQuery::getParticipantInfo(SubmissionInfo& aSubmissionInfo) noexcept
{
    START_LOG_BLOCK("Geting_ID_and_name_from_database");

    mDatabase.select("core_solutions", "file_name, contest_id", "id = " + 
        std::to_string(aSubmissionInfo.ID));
    mDatabase.step();

    aSubmissionInfo.problemID = mDatabase.getInt64FromRow(1);

    auto fileName = mDatabase.getTextFromRow(0);
    if (fileName.has_value())
    {
        aSubmissionInfo.solutionFileName = std::move(fileName.value());
    }
    else
    {
        WRITE_ERROR("DatabaseQuery", "getParticipantInfo", 10, 
            "Can't_take_solution_filename_from_database");
    }
    mDatabase.closeStatment();

    WRITE_LOG("Problem_ID:", aSubmissionInfo.problemID);
    END_LOG_BLOCK("File_name:", aSubmissionInfo.solutionFileName);
}

//--------------------------------------------------------------------------------

void
data::DatabaseQuery::getCheckerInfo(SubmissionInfo& aSubmissionInfo) noexcept
{
    START_LOG_BLOCK("Geting_limits_from_database");

    mDatabase.select("core_contests", "time_limit, memory_limit, checker", 
        "id = " + std::to_string(aSubmissionInfo.problemID));
    mDatabase.step();
    aSubmissionInfo.timeMemLim = { uint64_t(mDatabase.getInt64FromRow(0)), 
        uint64_t(mDatabase.getInt64FromRow(1))};
    
    auto fileName = mDatabase.getTextFromRow(2);
    if (fileName.has_value())
    {
        aSubmissionInfo.checkerFileName = std::move(fileName.value());
    }
    else
    {
        WRITE_ERROR("DatabaseQuery", "getParticipantInfo", 10, 
            "Can't_take_checker_filename_from_database");
    }
    
    mDatabase.closeStatment();

    WRITE_LOG("Time_limit:", aSubmissionInfo.timeMemLim.timeLimit);
    END_LOG_BLOCK("Memory_limit:", aSubmissionInfo.timeMemLim.memoryLimit);
}

//--------------------------------------------------------------------------------
