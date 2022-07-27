#ifndef BASE_PROCESS_HPP
#define BASE_PROCESS_HPP

//--------------------------------------------------------------------------------

#include <optional>

#include "domain/type.hpp"
#include "domain/string.hpp"
#include "domain/pair.hpp"
#include "database/submission_info.hpp"

//--------------------------------------------------------------------------------
//                              PROCESS CREATION CONSTANTS 
//--------------------------------------------------------------------------------

#define MAX_TIME_LIMIT 5000
#define MAX_MEMORY_LIMIT 5000

//--------------------------------------------------------------------------------

namespace proc
{
    class BaseProcess
    {
    public:
        enum class PypeType { ZERO = 0, NO_ZERO = 1 };

        /*
        \brief Base process constructor that initialize time and memory 
            usage limits for child process.
            (all child process have max time and memory usage limits
            that defined in MAX_TIME_LIMIT and MAX_MEMORY_LIMIT)
        */
        BaseProcess() noexcept;
        virtual ~BaseProcess() = default;

        BaseProcess(const BaseProcess& other) noexcept = default;
        BaseProcess& operator=(const BaseProcess& other) noexcept = default;

        BaseProcess(BaseProcess&& other) noexcept = default;
        BaseProcess& operator=(BaseProcess&& other) noexcept = default;

        virtual void setComand(const dom::StringTable& aParameters) noexcept = 0;
        
        /*
        \brief Create a child process with the specified parameters.
        */
        virtual void create() noexcept = 0;

        /*
        \brief Sets time and memory usage limits.
        \param aTimeMemLimits Child process maximum time usage and 
            process maximum memory usage.
        */
        void setLimits(const Limits& aTimeMemLimits) noexcept;

        /*
        \brief Executing the child process without time and memory 
            usage evaluation.
        \return Returns true if the process is completed successfully.
        */
        virtual bool run() noexcept = 0;

        /*
        \brief Executing the child process with time and memory 
            usage evaluation.
        \return Returns the time and memory used by the process if 
            the process is completed successfully.
        */
        virtual std::optional<dom::Pair<uint_64>> runWithLimits() noexcept = 0;

        virtual void write(const std::string& aMessage) noexcept = 0;
        virtual void read(std::string& aMessage) noexcept = 0;

    protected:
        uint_64 mTimeLimit;
        uint_64 mMemoryLimit;
    };

}

//--------------------------------------------------------------------------------

#endif // !BASE_PROCESS_HPP
