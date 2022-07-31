#ifndef DATABASE_QUERY_HPP
#define DATABASE_QUERY_HPP

//--------------------------------------------------------------------------------

#include <mutex>
#include <optional>

#include "domain/string.hpp"

#include "database.hpp"
#include "submission_info.hpp"

//--------------------------------------------------------------------------------

namespace data
{
    class DatabaseQuery
    {
    private:
        struct TestData
        {
            dom::CharArray input;
            dom::CharArray output;
            uint32_t testNum;
        };

    public:
        /*
        \brief Construct querys handler for base with specifien name
        \param aDatabasePath Path to database.
        */
        DatabaseQuery(const std::string& aDatabasePath) noexcept;
        ~DatabaseQuery() = default;

        /*
        \brief Retrieves the problem information (ID, file names, restrictions) 
        from the database and puts it in the SubmissionInfo structure.
        */
        SubmissionInfo getSubmissionInfo(int ID) noexcept;

        /*
        \brief Writing result of submission check in database.
        \param aID Submission ID.
        \param aResult Result of submission check.
        \param aTime Submission time usage.
        \param aMemory Submission memory usage.
        */
        void writeResult(int aID, const std::string&  aResult, int aTime, 
            int aMemory) noexcept;

        /*
        \brief Retrieves the next test from the database and puts it in 
            TestLibMessage struct. 
            (according to current statment step and 
            numbering of tests in the database)
        \param aSudmissionInformation Problem info (ID) for test search.
        \param TestLibMessage  TestLibMessage structure for obtaining test.
        */
        std::optional<TestData> getNextTest() noexcept;

        void prepareTestsStatement(uint64_t aProblemID) noexcept;

    private:
        Database mDatabase;
        int mReservedStatementNumber;

        std::mutex mTestMutex;
        uint32_t mTestNum;
        bool mTestAreOver;

        void getParticipantInfo(SubmissionInfo& aSubmissionInfo) noexcept;
        void getCheckerInfo(SubmissionInfo& aSubmissionInfo) noexcept;
    };
}

//--------------------------------------------------------------------------------

#endif // !DATABASE_QUERY_HPP
