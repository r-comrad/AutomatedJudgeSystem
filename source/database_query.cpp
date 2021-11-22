#include "database_query.h"

//UPDATE core_solutions SET result = hh WHERE id = 10
//SELECT * FROM core_solutions WHERE id = 10

MDatabaseQuery::MDatabaseQuery(std::wstring aDatabasePath, std::wstring aTaskPath) :
    mDatabase(aDatabasePath)
{
    mTaskPath = aTaskPath;
    mTaskFile.open(aTaskPath + L"init");

    if (!mTaskFile.is_open())
        WD_ERROR(database.0, "Can't open file " + makeGoodString(aTaskPath) + "init");
}

MDatabaseQuery::~MDatabaseQuery()
{
    mTaskFile.close();
}

void MDatabaseQuery::makeTestCatalog(SubmissionInformation& aSudmissionInformation)
{
    getIDInformation(aSudmissionInformation);
    getLimitsInformation(aSudmissionInformation);
    getTests();
}

void MDatabaseQuery::writeResult(std::string aResult, int aTime, int aMemory)
{
    WD_LOG("Updating database");

    mDatabase.update("core_solutions", "result = '" + aResult + "'" +
        ", time = " + std::to_string(aTime) +
        ", memory = " + std::to_string(aMemory), "id = " + std::to_string(mGlobalId));
    mDatabase.step();
    mDatabase.closeStatment();

    WD_LOG("Database updated?");
    WD_END_LOG;
}

void MDatabaseQuery::getIDInformation(SubmissionInformation& aSudmissionInformation)
{
    WD_LOG("Geting ID and name from database");
    mGlobalId = aID;

    //SELECT * FROM core_solutions WHERE id = 10
    mDatabase.select("core_solutions", "file_name, contest_id", "id = " + std::to_string(aID));
    mDatabase.step();

    mFileName = mDatabase.getTextFromRow(0);
    mTaskFile << "File_name: ";
    for (int i = 0; mFileName[i];) mTaskFile << mFileName[i++];
    mTaskFile << "\n";

    mContestID = mDatabase.getInt64FromRow(1);
    mTaskFile << "Contest ID: " << mContestID << "\n";;
    mDatabase.closeStatment();
    
    WD_LOG("Contest ID: " << mContestID);
    WD_END_LOG;
}

void MDatabaseQuery::getLimitsInformation()
{
    WD_LOG("Geting limits from database");

    mDatabase.select("core_contests", "time_limit, memory_limit", "id = " + std::to_string(mContestID));
    mDatabase.step();
    mTimeLimit = mDatabase.getInt64FromRow(0);
    mMemoryLimit = mDatabase.getInt64FromRow(1);
    mTaskFile << "\nTime limit: " << mTimeLimit;
    mTaskFile << "\nMemory limit: " << mMemoryLimit << "\n";
    mDatabase.closeStatment();

    WD_LOG("Time limit: " << mTimeLimit);
    WD_LOG("Memory limit: " << mMemoryLimit);
    WD_END_LOG;
}

void MDatabaseQuery::getTests()
{
    WD_LOG("Geting test from database");

    mDatabase.select("core_test", "input, output", "contest_id = " + std::to_string(mContestID));
    std::wstring testAddress = mTaskPath + L"tests\\";
    std::wstring ansAddress = mTaskPath + L"answer\\";
    int cnt = 0;
    for (; cnt < 5; ++cnt)
    {
        if (mDatabase.step() == 0) break;
        const unsigned char* input = mDatabase.getTextFromRow(0);
        const unsigned char* output = mDatabase.getTextFromRow(1);

        std::ofstream taskFile(testAddress + std::to_wstring(cnt));
        std::ofstream ansFile(ansAddress + std::to_wstring(cnt));

        if (!taskFile.is_open())
        {
            WD_ERROR(database.0, "Can't open file " + makeGoodString(testAddress + std::to_wstring(cnt)));
            continue;
        }
        if (!ansFile.is_open())
        {
            WD_ERROR(database.0, "Can't open file " + makeGoodString(ansAddress + std::to_wstring(cnt)));
            continue;
        }

        for (int i = 0; input[i];) taskFile << input[i++];
        for (int i = 0; output[i];) ansFile << output[i++];
    }

    mDatabase.closeStatment();

    WD_LOG("Tests taken from pashae");
    WD_END_LOG;
}