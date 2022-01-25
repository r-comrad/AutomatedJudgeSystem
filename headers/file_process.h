#ifndef FILE_PROCESS_H
#define FILE_PROCESS_H

//--------------------------------------------------------------------------------

#include <string>

#include "my_process.h"

class FileProcess : public MyProcess
{
public:
	FileProcess(const std::vector<char*>& aParameters,
		std::string& aInputPath, std::string& aOutputPath);
	virtual ~FileProcess();

private:
	virtual void IORedirection();
	virtual void closeHandles();

	std::string& mInputPath;
	std::string& mOutputPath;
#ifdef BILL_WINDOWS
	HANDLE mInputFileHandle;
	HANDLE mOutputFileHandle;
#endif // BILL_WINDOWS
};

//--------------------------------------------------------------------------------

#endif // FILE_PROCESS_H