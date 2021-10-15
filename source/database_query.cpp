#include "database_query.h"

//int DatabaseQuery::takeProblemId(sqlite3* ppDb)
//{
//    sqlite3_stmt* ppStmt = NULL;
//    sqlite3_prepare_v2(
//        ppDb,              /* Database handle */
//        "SELECT * FROM core_solutions;",             /* SQL statement, UTF-8 encoded */
//        -1,                 /* Maximum length of zSql in bytes. */
//        &ppStmt,             /* OUT: Statement handle */
//        NULL                /* OUT: Pointer to unused portion of zSql */
//    );
//
//    sqlite3_step(ppStmt);
//    int result = sqlite3_column_int64(ppStmt, 4);
//    sqlite3_finalize(ppStmt);
//    return result;
//}
//
//int DatabaseQuery::coutRows(sqlite3* ppDb, int problemNum)
//{
//    sqlite3_stmt* ppStmt = NULL;
//    std::string ss = "SELECT COUNT(input) FROM core_test WHERE contest_id = " + std::to_string(problemNum) + ";";
//    std::cout << ss << std::endl;
//    sqlite3_prepare_v2(
//        ppDb,              /* Database handle */
//        ss.c_str(),             /* SQL statement, UTF-8 encoded */
//        -1,                 /* Maximum length of zSql in bytes. */
//        &ppStmt,             /* OUT: Statement handle */
//        NULL                /* OUT: Pointer to unused portion of zSql */
//    );
//    sqlite3_step(ppStmt);
//    int result = sqlite3_column_int(ppStmt, 0);
//    return result;
//}
//
//void DatabaseQuery::takeProblemId(sqlite3* ppDb, int problemNum)
//{
//    sqlite3_stmt* ppStmt = NULL;
//    std::string ss = "SELECT input, output FROM core_test WHERE contest_id = " + std::to_string(problemNum) + ";";
//    std::cout << ss << std::endl;
//    sqlite3_prepare_v2(
//        ppDb,              /* Database handle */
//        ss.c_str(),             /* SQL statement, UTF-8 encoded */
//        -1,                 /* Maximum length of zSql in bytes. */
//        &ppStmt,             /* OUT: Statement handle */
//        NULL                /* OUT: Pointer to unused portion of zSql */
//    );
//    sqlite3_step(ppStmt);
//    auto result = sqlite3_column_text(ppStmt, 0);
//    std::cout << result << std::endl;
//    std::cout << sqlite3_column_text(ppStmt, 1) << std::endl;
//    sqlite3_step(ppStmt);
//    std::cout << sqlite3_column_text(ppStmt, 0) << std::endl;
//    std::cout << sqlite3_column_text(ppStmt, 1) << std::endl;
//    sqlite3_finalize(ppStmt);
//
//    int i = 0;
//    //for (; sqlite3_column_text(ppStmt, 0) != NULL; ++i) sqlite3_step(ppStmt);
//    for (; sqlite3_column_bytes(ppStmt, 0) != 0; ++i)  sqlite3_step(ppStmt);;
//    std::cout << i << std::endl;
//
//    // return result;
//}
//
//void A()
//{
//    sqlite3* ppDb = NULL;
//
//    if (sqlite3_open("U:\\_Private\\ChineseTester\\ChineseTester\\resources\\db.sqlite3", &ppDb)
//        != SQLITE_OK)
//    {
//        std::cout << "ERROR: can't open database";
//    }
//
//    int result = takeProblemId(ppDb);
//    std::cout << result << std::endl;
//    //std::cout << coutRows(ppDb, result) << std::endl;
//    //takeProblemId(ppDb, result);
//    sqlite3_close(ppDb);
//}

MDatabaseQuery::MDatabaseQuery(std::wstring aDatabasePath, std::wstring aTaskPath) :
    mDatabase(aDatabasePath)
{
    mTaskPath = aTaskPath;
    mTaskFile.open(aTaskPath + L"init");

#ifdef  _DBG_
    if (!mTaskFile.is_open()) std::cout << "!!!!!! Can't open file " + makeGoodString(aTaskPath) + "init" << std::endl;
#endif //  _DBG_

}

MDatabaseQuery::~MDatabaseQuery()
{
    mTaskFile.close();
}

void MDatabaseQuery::makeTestCatalog(int aID)
{
   // mDatabase.select("core_test", "input, output", "contest_id = " + std::to_string(mContestID));

   // auto nn = this;
    getIDInformation(aID);
    getLimitsInformation();
    getTests();
}

//UPDATE core_solutions SET result = hh WHERE id = 10
//SELECT * FROM core_solutions WHERE id = 10

void MDatabaseQuery::writeResult(std::string aResult, int aTime, int aMemory)
{
#ifdef  _DBG_
    std::cout << "Updating database" << std::endl;
#endif //  _DBG_

    mDatabase.update("core_solutions", "result = '" + aResult + "'" +
        ", time = " + std::to_string(aTime) +
        ", memory = " + std::to_string(aMemory), "id = " + std::to_string(mGlobalId));
    mDatabase.step();
    mDatabase.closeStatment();
}

void MDatabaseQuery::getIDInformation(int aID)
{
#ifdef  _DBG_
    std::cout << "Geting ID and name from database" << std::endl;
#endif //  _DBG_

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

#ifdef  _DBG_
    std::cout << "ontest ID: " << mContestID << std::endl;
#endif //  _DBG_
}

void MDatabaseQuery::getLimitsInformation()
{
#ifdef  _DBG_
    std::cout << "Geting limits from database" << std::endl;
#endif //  _DBG_

    mDatabase.select("core_contests", "time_limit, memory_limit", "id = " + std::to_string(mContestID));
    mDatabase.step();
    mTimeLimit = mDatabase.getInt64FromRow(0);
    mMemoryLimit = mDatabase.getInt64FromRow(1);
    mTaskFile << "\nTime limit: " << mTimeLimit;
    mTaskFile << "\nMemory limit: " << mMemoryLimit << "\n";
    mDatabase.closeStatment();
}

void MDatabaseQuery::getTests()
{
#ifdef  _DBG_
    std::cout << "Geting test from database" << std::endl;
#endif //  _DBG_

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

#ifdef  _DBG_
        if (!taskFile.is_open()) std::cout << "!!!!!! Can't open file " + makeGoodString(testAddress + std::to_wstring(cnt)) << std::endl;
#endif //  _DBG_
#ifdef  _DBG_
        if (!ansFile.is_open()) std::cout << "!!!!!! Can't open file " + makeGoodString(ansAddress + std::to_wstring(cnt)) << std::endl;
#endif //  _DBG_

        for (int i = 0; input[i];) taskFile << input[i++];
        for (int i = 0; output[i];) ansFile << output[i++];
    }

    mDatabase.closeStatment();

#ifdef  _DBG_
    std::cout << cnt << " tests taken from pasha" << std::endl;
#endif //  _DBG_
}