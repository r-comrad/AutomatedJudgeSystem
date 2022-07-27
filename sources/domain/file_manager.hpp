#ifndef FILES_MANAGER_HPP
#define FILES_MANAGER_HPP

#include <fstream>
#include <vector>

#include "type.hpp"
#include "string.hpp"
#include "error_message.hpp"

namespace dom
{
    void copyFile(const std::string& aFromFileName, const std::string& aToFileName);

    class File
    {
    public:
        File(const std::string& aFileName);
        ~File();

        void write(const std::vector<std::string>& aMessage);
        template<typename... Args>
        void write(Args... args)
        {
            (void) std::initializer_list<bool>
            {
                static_cast<bool>(mOut << args << mDelimiter)...
            };
            mOut << '\n';
        }

        void setDelimiter(const std::string& aDelimiter);
        void writeEndl();

        void close();

    private:
        std::ofstream mOut;
        std::string mDelimiter;
    };
} // namespace dom

#endif // !FILES_MANAGER_HPP