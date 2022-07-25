#include "database/database_query.hpp"

//--------------------------------------------------------------------------------
//                        DATABASE QUERYS HANDLER IMPLEMENTATION
//--------------------------------------------------------------------------------

//UPDATE core_solutions SET result = hh WHERE id = 10
//SELECT * FROM core_solutions WHERE id = 10
//UPDATE core_solutions SET lang = "C:\task1\checker\checker.cpp" WHERE id = 10
//UPDATE core_solutions SET file_name = "2/1.py" WHERE id = 3
//UPDATE core_contests SET time_limit = 1000 WHERE id = 20
//UPDATE core_contests SET memory_limit = 262144 WHERE id = 21

data::DatabaseQuery::DatabaseQuery (const std::string& aDatabasePath) noexcept :
    mDatabase                   (aDatabasePath),
    mReservedStatementNumber    (0),
    mTestNum                    (1),
    mTestAreOver                (false)
{}

SubmissionInfo 
data::DatabaseQuery::getSubmissionInfo(int ID) noexcept
{
    SubmissionInfo result;
    result.ID = ID;

    getParticipantInfo(result);
    getCheckerInfo(result);

#ifdef BILL_WINDOWS
#elif defined(LINUS_LINUX)
    result.timeLimit /= 1000;
#endif 

    return result;
}

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

// void
// data::DatabaseQuery::getNextTest
// (
//     TestLibMessage&         aTLM
// ) noexcept
std::optional<data::DatabaseQuery::TestData>
data::DatabaseQuery::getNextTest() noexcept
{
    std::optional<TestData> result;
    if (!mTestAreOver)
    {
        TestData data;
        WRITE_LOG("Taking_next_test");
    
        mTestMutex.lock();
        //if (mDatabase.step() != SQLITE_OK) break; TODO: fixing that shit
        mDatabase.step(mReservedStatementNumber);
        auto input = mDatabase.getTextFromRow(0, mReservedStatementNumber);
        auto output = mDatabase.getTextFromRow(1, mReservedStatementNumber);
        mTestMutex.unlock();

        if (input.has_value())
        {
            data.input = std::move(input.value());
            data.output = std::move(output.value());
            data.testNum = mTestNum++;
            result = std::make_optional<TestData>(std::move(data));
        }
        else
        {
            mTestAreOver = true;
        }
    }

    return result;
}

// void
// data::DatabaseQuery::getAllTests(ProblemInformation& aSudmissionInformation) noexcept
// {
//     START_LOG_BLOCK("Geting_test_from_database");

//     mDatabase.select("core_test", "input, output", "contest_id = " + std::to_string(aSudmissionInformation.mContestID));
//     int cnt = 0;
//     for (;; ++cnt)
//     {
//         mDatabase.step();
//         //if (mDatabase.step() != SQLITE_OK) break; TODO: fixing that shit
//         auto input = mDatabase.getTextFromRow(0);
//         auto output = mDatabase.getTextFromRow(1);
//         if (!input.has_value()) break;

//         std::ofstream taskFile(TEST_PATH + std::to_string(aSudmissionInformation.mID) + "-" + std::to_string(cnt));
//         std::ofstream ansFile(ANSWERS_PATH + std::to_string(aSudmissionInformation.mID) + "-" + std::to_string(cnt));

//         if (!taskFile.is_open())
//         {
//             WRITE_ERROR("MDatabaseQuery", 10, "Can't_open_file", TEST_PATH + std::to_string(cnt));
//             continue;//TODO
//         }
//         if (!ansFile.is_open())
//         {
//             WRITE_ERROR("MDatabaseQuery", 11, "Can't_open_file", ANSWERS_PATH + std::to_string(cnt));
//             continue;
//         }

//         auto inStr = input.value().getFront();
//         auto outStr = output.value().getFront();

//         for (int i = 0; inStr[i];) taskFile << inStr[i++];
//         for (int i = 0; outStr[i];) ansFile << outStr[i++];
//     }
//     aSudmissionInformation.mTestsCount = cnt;

//     mDatabase.closeStatment();

//     END_LOG_BLOCK("Tests_taken_from_pashae");
// }

void
data::DatabaseQuery::prepareTestsStatement(uint_64 aProblemID) noexcept
{
    START_LOG_BLOCK("Prepare_geting_test_from_database");
    mDatabase.select("core_test", "input, output", "contest_id = " +
        std::to_string(aProblemID), mReservedStatementNumber);
    END_LOG_BLOCK("I'm_ready");
}

void
data::DatabaseQuery::getParticipantInfo(SubmissionInfo& aSubmissionInfo) noexcept
{
    START_LOG_BLOCK("Geting_ID_and_name_from_database");

    mDatabase.select("core_solutions", "file_name, contest_id", "id = " + std::to_string(aSubmissionInfo.ID));
    mDatabase.step();

    aSubmissionInfo.problemID = mDatabase.getInt64FromRow(1);
    aSubmissionInfo.solutionFileName = mDatabase.getTextFromRow(0).value();
    mDatabase.closeStatment();

    WRITE_LOG("Problem_ID:", aSubmissionInfo.problemID);
    END_LOG_BLOCK("File_name:", aSubmissionInfo.solutionFileName);
}

void
data::DatabaseQuery::getCheckerInfo(SubmissionInfo& aSubmissionInfo) noexcept
{
    START_LOG_BLOCK("Geting_limits_from_database");

    mDatabase.select("core_contests", "time_limit, memory_limit, checker", "id = " + std::to_string(aSubmissionInfo.problemID));
    mDatabase.step();
    aSubmissionInfo.timeMemLim = { uint_64(mDatabase.getInt64FromRow(0)), uint_64(mDatabase.getInt64FromRow(1))};
    aSubmissionInfo.checkerFileName = mDatabase.getTextFromRow(2).value();
    mDatabase.closeStatment();

    WRITE_LOG("Time_limit:", aSubmissionInfo.timeMemLim.timeLimit);
    END_LOG_BLOCK("Memory_limit:", aSubmissionInfo.timeMemLim.memoryLimit);
}