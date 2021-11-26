#include "database_query.h"

//UPDATE core_solutions SET result = hh WHERE id = 10
//SELECT * FROM core_solutions WHERE id = 10
//UPDATE core_solutions SET lang = "C:\task1\checker\checker.cpp" WHERE id = 10

MDatabaseQuery::MDatabaseQuery(std::wstring aDatabasePath) :
    mDatabase(aDatabasePath)
{
}

MDatabaseQuery::~MDatabaseQuery()
{
}

void MDatabaseQuery::makeTestCatalog(SubmissionInformation& aSudmissionInformation)
{
    getIDInformation(aSudmissionInformation);
    getLimitsInformation(aSudmissionInformation);
    getTests(aSudmissionInformation.mContestID, aSudmissionInformation);
}

void MDatabaseQuery::writeResult(int aID, std::string aResult, int aTime, int aMemory)
{
    WD_LOG("Updating database");

    mDatabase.update("core_solutions", "result = '" + aResult + "'" +
        ", time = " + std::to_string(aTime) +
        ", memory = " + std::to_string(aMemory), "id = " + std::to_string(aID));
    mDatabase.step();
    mDatabase.closeStatment();

    WD_LOG("Database updated?");
    WD_END_LOG;
}

void MDatabaseQuery::getIDInformation(SubmissionInformation& aSudmissionInformation)
{
    WD_LOG("Geting ID and name from database");

    mDatabase.select("core_solutions", "file_name, contest_id, lang", "id = " + std::to_string(aSudmissionInformation.id));
    mDatabase.step();

    aSudmissionInformation.mSolutionFileName = getString(mDatabase.getTextFromRow(0));
    aSudmissionInformation.mContestID = mDatabase.getInt64FromRow(1);
    aSudmissionInformation.mCheckerFileName = getString(mDatabase.getTextFromRow(2));
    mDatabase.closeStatment();

    WD_LOG("File name: " << makeGoodString(aSudmissionInformation.mSolutionFileName));
    WD_LOG("Contest ID: " << aSudmissionInformation.mContestID);
    WD_END_LOG;
}

void MDatabaseQuery::getLimitsInformation(SubmissionInformation& aSudmissionInformation)
{
    WD_LOG("Geting limits from database");

    mDatabase.select("core_contests", "time_limit, memory_limit", "id = " + std::to_string(aSudmissionInformation.mContestID));
    mDatabase.step();
    aSudmissionInformation.mTimeLimit = mDatabase.getInt64FromRow(0);
    aSudmissionInformation.mMemoryLimit = mDatabase.getInt64FromRow(1);
    mDatabase.closeStatment();

    WD_LOG("Time limit: " << aSudmissionInformation.mTimeLimit);
    WD_LOG("Memory limit: " << aSudmissionInformation.mMemoryLimit);
    WD_END_LOG;
}

void MDatabaseQuery::getTests(int aContestID, SubmissionInformation& aSudmissionInformation)
{
    WD_LOG("Geting test from database");

    mDatabase.select("core_test", "input, output", "contest_id = " + std::to_string(aContestID));
    int cnt = 0;
    for (;; ++cnt)
    {
        mDatabase.step();
        //if (mDatabase.step() != SQLITE_OK) break; TODO: fixing that shit
        const unsigned char* input = mDatabase.getTextFromRow(0);
        const unsigned char* output = mDatabase.getTextFromRow(1);
        if (input == NULL) break;

        std::ofstream taskFile(TEST_PATH + std::to_wstring(cnt));
        std::ofstream ansFile(ANSWERS_PATH + std::to_wstring(cnt));

        if (!taskFile.is_open())
        {
            WD_ERROR(database.0, "Can't open file " + makeGoodString(TEST_PATH + std::to_wstring(cnt)));
            continue;
        }
        if (!ansFile.is_open())
        {
            WD_ERROR(database.0, "Can't open file " + makeGoodString(ANSWERS_PATH + std::to_wstring(cnt)));
            continue;
        }

        for (int i = 0; input[i];) taskFile << input[i++];
        for (int i = 0; output[i];) ansFile << output[i++];
    }
    aSudmissionInformation.mTestsCount = cnt;

    mDatabase.closeStatment();

    WD_LOG("Tests taken from pashae");
    WD_END_LOG;
}