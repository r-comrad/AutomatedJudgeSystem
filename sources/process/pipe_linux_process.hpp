#ifndef PIPE_LINUX_PROCESS_H
#define PIPE_LINUX_PROCESS_H

//--------------------------------------------------------------------------------

#include <string>



class PipeLinuxProcess //: public LinuxProcess
{
public:
    enum PypeType { ZERO = 0, NO_ZERO = 1 };


    PipeLinuxProcess() noexcept;
    virtual ~PipeLinuxProcess() = default;

    void readPipe(str_ref result);
    void writePipe(str_ref aMessage, PypeType aType = ZERO);

private:
    virtual void IORedirection();

    int mParentPipes[2];
    int mChildPipes[2];
};

//--------------------------------------------------------------------------------

#endif // !PIPE_LINUX_PROCESS_H