#ifndef PIPE_PROCESS_H
#define PIPE_PROCESS_H

//--------------------------------------------------------------------------------

#include <string>

#include "my_process.h"

class PipeProcess : public MyProcess
{
public:
	enum PypeType { ZERO = 0, NO_ZERO = 1 };

	PipeProcess(std::string& aName, std::string& aParameters);
	virtual ~PipeProcess();

	void readPipe(std::string& result);
	void writePipe(std::string& aMessage, PypeType aType = ZERO);

private:
	virtual void IORedirection();
	virtual void closeHandles();

	HANDLE mThisSTDIN;
	HANDLE mThisSTDOUT;
	HANDLE mChildSTDIN;
	HANDLE mChildSTDOUT;
};

//--------------------------------------------------------------------------------

#endif // PIPE_PROCESS_H