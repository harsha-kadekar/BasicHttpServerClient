#ifndef LOGGER_H
#define LOGGER_H

#include<time.h>
#include<string.h>
#include<stdio.h>
#include<Windows.h>
#include"ErrorCodes.h"
#include"SupportingFunctions.h"


#define LOG_DEBUG 3
#define LOG_WARN 4
#define LOG_ERROR 5
#define LOG_INFO 2
#define LOG_ALL 1


extern int nLogLevel;
extern char *strLogFilePath;
extern int bOnlyFileLogging;
extern int NoOfDaysBackUpLogFile;
extern char *szLogFile;
extern int nCurrentDay;

int CleanUpOldLogFiles();
char* GetCurrentLogFile();
int WriteToFile(char *strMessage);
int RoleOverLogFile();
int DateHasChanged();
int SetLogLevel(int nNewLogLevel);
int LogMessage(int msgLogLevel, char *szMessage);
int UninitializeLogger();
int InitializeLogger();




#endif
