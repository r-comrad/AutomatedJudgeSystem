# Automated Judge System

In this project, an automated judge system has been implemented, which is part of the [kussystem](https://kussystem.ru). It solves the following tasks: 
 - reading/writing data about problems and submissions from a database (SQLite or Postgresql)
 - compilation or interpretation participant source code file (currently only C++ and Python are available)
 - compilation  of checker file that uses testlib (author MikeMirzayanov: [testlib](https://github.com/MikeMirzayanov/testlib) or LapochnihIvan: [TestLibrary](https://github.com/LapochnihIvan/TestLibrary))
 - run processes (submission and checker) using WinAPI or LinuxAPI, data is transmitted to processes via pipes
 - multithreaded testing of participant submission
 - evaluation of testing results and recording them in the database.
 
The test system compiled for Windows or Linux by setting the BILL_WINDOWS or LINUS_LINUX preprocessor definitions depending on the version of the compiler that was found (the presence of MSVC version 1900 or higher trigger BILL_WINDOWS flag to be set). 

The SQLITE or POSTGRESQL preprocessor definitions specify the database to be used. 

The \_DBG\_ preprocessor definition turn on log and error output. LOGS_OUTPUT_TYPE and ERRORS_OUTPUT_TYPE define the way log and error messages are printed. Precise values for these definitions can be found in domain/error_message.cpp file. 

To start the testing process, you need to run a test system with a single argument - the number of the participant submission in the database. After testing is completed, the database will be updated.
