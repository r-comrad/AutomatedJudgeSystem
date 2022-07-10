#include "pipe_linux_process.hpp"

proc::PipeLinuxProcess::PipeLinuxProcess() noexcept
{
    IORedirection()
}

void
proc::PipeLinuxProcess::create(const std::vector<char*>& aParameters)
{
    //aParameters = aName + aParameters;

    WRITE_LOG("Creating_process_with_name:", aParameters[0]);
    //WD_LOG("Parameters: " << aParameters[1]);
    //signal(SIGCONT, MyProcess::handleContinueSignal);

    mChildPID = fork();
    if (mChildPID == -1)
    {
        WRITE_ERROR("PipeProcess", 10, "Linux", "Can't_create_process");
    }
    else if (!mChildPID)
    {
        //write(mChildPipes[1], aMessage.c_str(),  aMessage.size());

//        fcntl(mPipeA[0], F_SETPIPE_SZ, 65336 * 2);
//        fcntl(mPipeA[1], F_SETPIPE_SZ, 65336 * 2);
//        fcntl(mPipeB[0], F_SETPIPE_SZ, 65336 * 2);
//        fcntl(mPipeB[1], F_SETPIPE_SZ, 65336 * 2);

        dup2(mPipeA[0], STDIN_FILENO);
        dup2(mPipeB[1], STDOUT_FILENO);

        //        fcntl(mPipeA[0], F_SETPIPE_SZ, 65336 * 2);
        //        fcntl(mPipeA[1], F_SETPIPE_SZ, 65336 * 2);
        //        fcntl(mPipeB[0], F_SETPIPE_SZ, 65336 * 2);
        //        fcntl(mPipeB[1], F_SETPIPE_SZ, 65336 * 2);

        rlimit timeLimit;
        timeLimit.rlim_cur = mTimeLimit;
        timeLimit.rlim_max = mTimeLimit;
        setrlimit(RLIMIT_CPU, &timeLimit);

        rlimit memoryLimit;
        memoryLimit.rlim_cur = mMemoryLimit;
        memoryLimit.rlim_max = mMemoryLimit;
        setrlimit(RLIMIT_STACK, &memoryLimit);

        //        dup2(mChildPipes[0], STDIN_FILENO);
        //        dup2(mParentPipes[1], STDOUT_FILENO);
        //        signal(SIGCONT, handleContinueSignal2);
                //if (mIsPaused) pause();

        execvp(aParameters[0], &aParameters[0]);
        //WD_ERROR(linux.exec.0, "cant start process");
    }
    else
    {
        //        fcntl(mPipeA[0], F_SETPIPE_SZ, 65336 * 2);
        //        fcntl(mPipeA[1], F_SETPIPE_SZ, 65336 * 2);
        //        fcntl(mPipeB[0], F_SETPIPE_SZ, 65336 * 2);
        //        fcntl(mPipeB[1], F_SETPIPE_SZ, 65336 * 2);
    }


    /*
    int id = fork();
    if (id == 0)
    {
        int i = 0;
        int j = 0;
        //std::vector<const char*> param;
        char* param[10];
        while (true)
        {
            std::string s;
            for(;aParameters[i] != ' '; ++i) s.push_back(aParameters[i]);
            ++i;
            param[j] = (char*) s.c_str();
            ++j;
        }*/
        //char** param2 =
        //param.push_back(NULL);
       /* param[j] = NULL;
        execv((char*) aName.c_str(), param);
    }*/
}

#define BUFFER_SIZE 65336 * 10

void
proc::PipeLinuxProcess::IORedirection()
{
    WRITE_LOG("Rederecting_input_and_output_to_pipe");

    pipe(mPipeA);
    pipe(mPipeB);


    fcntl(mPipeA[0], F_SETPIPE_SZ, BUFFER_SIZE);
    fcntl(mPipeA[1], F_SETPIPE_SZ, BUFFER_SIZE);
    fcntl(mPipeB[0], F_SETPIPE_SZ, BUFFER_SIZE);
    fcntl(mPipeB[1], F_SETPIPE_SZ, BUFFER_SIZE);
}


void
proc::PipeLinuxProcess::readPipe(str_ref result)
{
#ifdef PIPE_LOGS
    WRITE_LOG("Reading_from_pipe");
#endif // !PIPE_LOG_OUTPUT

    result.clear();
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    while (read(mPipeB[0], &buf, 1024) == 1024)
    {
        result += std::string(buf);
        memset(buf, 0, sizeof(buf));
    }
    result += std::string(buf);

    //    char buf[1024];
    //    while (read(mPipefd[0], &buf, 1) > 0)
    //        write(STDOUT_FILENO, &buf, 1);
    //    close(mPipefd[0]);

#ifdef PIPE_LOGS
    WD_END_LOG;
#endif // !PIPE_LOG_OUTPUT
}

void
proc::PipeLinuxProcess::writePipe(str_ref aMessage, PypeType aType)
{
#ifdef PIPE_LOGS
    WRITE_LOG("Writing_from_pipe");
#endif // !PIPE_LOG_OUTPUT

    //aMessage.push_back('\n');
    write(mPipeA[1], aMessage.c_str(), aMessage.size());
    if (aType == ZERO)
    {
        write(mPipeA[1], "\n\0", 2);
    }
    //    write(mPipefd[1], aMessage.c_str(), aMessage.size());
    //    if (aType == ZERO)
    //    {
    //        write(mPipefd[1], "\n", 1);
    //    }
    //    close(mPipefd[1]);

#ifdef PIPE_LOGS
    WD_LOG("write " + std::to_string(bread) + " bites\n");
    WD_END_LOG;
#endif // !PIPE_LOG_OUTPUT
}