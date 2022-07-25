#ifndef WINDOWS_PIPE_PROCESS_H
#define WINDOWS_PIPE_PROCESS_H

//--------------------------------------------------------------------------------

#include <string>

#include "windows_process.hpp"

namespace proc 
{
    class PipeWindowsProcess : public WindowsProcess
    {
    public:
        PipeWindowsProcess() noexcept = default;

        PipeWindowsProcess(PipeWindowsProcess&& other) noexcept = default;
        PipeWindowsProcess(const PipeWindowsProcess& other) noexcept = default;

        PipeWindowsProcess& operator=(PipeWindowsProcess&& other) noexcept = default;
        PipeWindowsProcess& operator=(const PipeWindowsProcess& other) noexcept = default;

        virtual ~PipeWindowsProcess();

        void read(std::string& result) noexcept;
        void write(const std::string& aMessage) noexcept;
        virtual void IORedirection() noexcept;
        virtual void create() noexcept;

    private:
        virtual void closeHandles() noexcept;

        bool mIOSet = false;
        HANDLE mThisSTDIN;
        HANDLE mThisSTDOUT;
        HANDLE mChildSTDIN;
        HANDLE mChildSTDOUT;
    };
}

//--------------------------------------------------------------------------------

#endif // !WINDOWS_PIPE_PROCESS_H