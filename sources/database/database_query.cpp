#include "database/database_query.hpp"

//--------------------------------------------------------------------------------
//					    DATABASE QUERYS HANDLER IMPLEMENTATION
//--------------------------------------------------------------------------------

//UPDATE core_solutions SET result = hh WHERE id = 10
//SELECT * FROM core_solutions WHERE id = 10
//UPDATE core_solutions SET lang = "C:\task1\checker\checker.cpp" WHERE id = 10
//UPDATE core_solutions SET file_name = "2/1.py" WHERE id = 3
//UPDATE core_contests SET time_limit = 1000 WHERE id = 20
//UPDATE core_contests SET memory_limit = 262144 WHERE id = 21

data::DatabaseQuery::DatabaseQuery (const std::string& aDatabasePath) noexcept :
    mDatabase                   (aDatabasePath),
    mReservedStatementNumber    (0)
{}

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

void
data::DatabaseQuery::getNextTest
(
    ProblemInformation&     aSudmissionInformation, 
    TestLibMessage&         aTLM
) noexcept
{

    WRITE_LOG("Taking_next_test");
    mDatabase.step(mReservedStatementNumber);
    //if (mDatabase.step() != SQLITE_OK) break; TODO: fixing that shit
    const unsigned char* input = mDatabase.getTextFromRow(0, mReservedStatementNumber);
    const unsigned char* output = mDatabase.getTextFromRow(1, mReservedStatementNumber);
    if (input == NULL)
    {
        aSudmissionInformation.mTestsAreOver = true;
        aTLM.mTest = "";
        aTLM.mAnswer = "";
        return;
    }

    aSudmissionInformation.mTestsCount++;
    aTLM.mTest = std::string(reinterpret_cast<const char*>(input));
    //aTLM.mTest += "\n";
    aTLM.mAnswer = std::string(reinterpret_cast<const char*>(output));
}

void
data::DatabaseQuery::getAllTests(ProblemInformation& aSudmissionInformation) noexcept
{
    START_LOG_BLOCK("Geting_test_from_database");

    mDatabase.select("core_test", "input, output", "contest_id = " + std::to_string(aSudmissionInformation.mContestID));
    int cnt = 0;
    for (;; ++cnt)
    {
        mDatabase.step();
        //if (mDatabase.step() != SQLITE_OK) break; TODO: fixing that shit
        const unsigned char* input = mDatabase.getTextFromRow(0);
        const unsigned char* output = mDatabase.getTextFromRow(1);
        if (input == NULL) break;

        std::ofstream taskFile(TEST_PATH + std::to_string(aSudmissionInformation.mID) + "-" + std::to_string(cnt));
        std::ofstream ansFile(ANSWERS_PATH + std::to_string(aSudmissionInformation.mID) + "-" + std::to_string(cnt));

        if (!taskFile.is_open())
        {
            WRITE_ERROR("MDatabaseQuery", 10, "Can't_open_file", TEST_PATH + std::to_string(cnt));
            continue;//TODO
        }
        if (!ansFile.is_open())
        {
            WRITE_ERROR("MDatabaseQuery", 11, "Can't_open_file", ANSWERS_PATH + std::to_string(cnt));
            continue;
        }

        for (int i = 0; input[i];) taskFile << input[i++];
        for (int i = 0; output[i];) ansFile << output[i++];
    }
    aSudmissionInformation.mTestsCount = cnt;

    mDatabase.closeStatment();

    END_LOG_BLOCK("Tests_taken_from_pashae");
}

void
data::DatabaseQuery::prepareTestsStatement(ProblemInformation& aSudmissionInformation) noexcept
{
    aSudmissionInformation.mTestsAreOver = false;
    aSudmissionInformation.mTestsCount = 0;

    START_LOG_BLOCK("Prepare_geting_test_from_database");
    mDatabase.select("core_test", "input, output", "contest_id = " +
        std::to_string(aSudmissionInformation.mContestID), mReservedStatementNumber);
    END_LOG_BLOCK("I'm_ready");
}

data::DatabaseQuery::ParticipantlInfo 
data::DatabaseQuery::getParticipantInfo(uint_64 aID) noexcept
{
    ParticipantlInfo result;

    START_LOG_BLOCK("Geting_ID_and_name_from_database");

    mDatabase.select("core_solutions", "file_name, contest_id", "id = " + std::to_string(aID));
    mDatabase.step();

    result.contestId = mDatabase.getInt64FromRow(1);
    result.fileName= dom::String(mDatabase.getTextFromRow(0));
    mDatabase.closeStatment();

    WRITE_LOG("Contest_ID:", result.contestId);
    END_LOG_BLOCK("File_name:", result.fileName);

    return result;
}

data::DatabaseQuery::CheckInfo
data::DatabaseQuery::getCheckInfo(uint_64 aID) noexcept
{
    data::DatabaseQuery::CheckInfo result;
    START_LOG_BLOCK("Geting_limits_from_database");

    mDatabase.select("core_contests", "time_limit, memory_limit, checker", "id = " + std::to_string(aID));
    mDatabase.step();
    result.timeLimit = mDatabase.getInt64FromRow(0);
    result.memoryLimit = mDatabase.getInt64FromRow(1);
    result.checkerName = dom::String(mDatabase.getTextFromRow(2));
    mDatabase.closeStatment();

    WRITE_LOG("Time_limit:", result.timeLimit);
    END_LOG_BLOCK("Memory_limit:", result.memoryLimit);

    return result;
}