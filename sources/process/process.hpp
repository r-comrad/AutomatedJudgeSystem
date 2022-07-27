#ifndef PROCESS_HPP
#define PROCESS_HPP

//--------------------------------------------------------------------------------

#include "pipe_windows_process.hpp"

//--------------------------------------------------------------------------------

namespace proc
{
    class Process : public PipeWindowsProcess
    {
    public:
        Process() noexcept = default;
        virtual ~Process() = default;

        Process(const Process& other) noexcept = default;
        Process& operator=(const Process& other) noexcept = default;

        Process(Process&& other) noexcept = default;
        Process& operator=(Process&& other) noexcept = default;
    };
} 

//--------------------------------------------------------------------------------

#endif // !PROCESS_HPP