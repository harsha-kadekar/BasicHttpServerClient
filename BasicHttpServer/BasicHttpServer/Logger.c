#include"Logger.h"

char *szLogFile = 0;
int nCurrentDay = 0;

FILE *fpLogFile = 0;

CRITICAL_SECTION logCriticalSect;
int nCleanUpCount = 0;


int InitializeLogger()
{
	int nReturnValue = 0;
	time_t t = time(NULL);
	struct tm cuTime;
	localtime_s(&cuTime, &t);
	//char szDate[11] = { '\0' };
	char szFileName[25] = { '\0' };
	char szFullPath[260] = { '\0' };

	sprintf_s(szFileName, 25, "BHSLogFile%02d-%02d-%04d.log", cuTime.tm_mon + 1, cuTime.tm_mday, cuTime.tm_year + 1900);

	sprintf_s(szFullPath, 260, "%s\\%s", strLogFilePath, szLogFile);

	szLogFile = (char*)malloc(sizeof(char)*strnlen_s(szFullPath, 260)+1);
	memset(szLogFile, '\0', strnlen_s(szFullPath, 260)+1);
	stringcopy(szLogFile, strnlen_s(szFullPath, 260), szFullPath);

	fopen_s(&fpLogFile, szLogFile, "a+");

	nCurrentDay = cuTime.tm_mon + 1;
	nCurrentDay *= 100;
	nCurrentDay += cuTime.tm_mday;
	nCurrentDay *= 10000;
	nCurrentDay += (cuTime.tm_year + 1900);

	InitializeCriticalSectionAndSpinCount(&logCriticalSect, 0x00000400);

	return nReturnValue;
}

int UninitializeLogger()
{
	int nReturnValue = 0;

	DeleteCriticalSection(&logCriticalSect);

	if (fpLogFile != 0)
	{
		fclose(fpLogFile);
		fpLogFile = 0;
	}

	if (szLogFile != 0)
	{
		free(szLogFile);
		szLogFile = 0;
	}

	return nReturnValue;
}

int LogMessage(int msgLogLevel, char *szMessage)
{
	int nReturnValue = 0;
	char szCompleteMsg[1024] = {'\0'};
	time_t t = time(NULL);
	struct tm cuTime;
	localtime_s(&cuTime, &t);
	char szLevel[8] = { '\0' };
	int nThreadId = 0;

	if (msgLogLevel >= nLogLevel)
	{
		nThreadId = GetCurrentThreadId();

		switch (msgLogLevel)
		{
			case LOG_ALL:
			{
				sprintf_s(szLevel, 5, "ALL");
				break;
			}
			case LOG_DEBUG:
			{
				sprintf_s(szLevel, 5, "DEBUG");
				break;
			}
			case LOG_ERROR:
			{
				sprintf_s(szLevel, 5, "ERROR");
				break;
			}
			case LOG_INFO:
			{
				sprintf_s(szLevel, 5, "INFO");
				break;
			}
			case LOG_WARN:
			{
				sprintf_s(szLevel, 5, "WARN");
				break;
			}
			default:
			{
				sprintf_s(szLevel, 7, "UNKNOWN");
				break;

			}
		}

		sprintf_s(szCompleteMsg, 1024, "%02d-%02d-%04d %02d:%02d:%02d\t%s\t%d\t%s\n", cuTime.tm_mon + 1, cuTime.tm_mday, cuTime.tm_year + 1900, cuTime.tm_hour, cuTime.tm_min, cuTime.tm_sec, szLevel, nThreadId, szMessage);

		EnterCriticalSection(&logCriticalSect);
		nReturnValue = WriteToFile(szCompleteMsg);
		if (bOnlyFileLogging == 0)
		{
			printf_s("%s", szCompleteMsg);
		}
		LeaveCriticalSection(&logCriticalSect);
	}

	return nReturnValue;
}

int SetLogLevel(int nNewLogLevel)
{
	int nReturnValue = 0;

	nLogLevel = nNewLogLevel;

	return nReturnValue;
}

int DateHasChanged()
{
	int bHasChanged = 0;
	int nDate = 0;

	time_t t = time(NULL);
	struct tm cuTime;
	localtime_s(&cuTime, &t);

	nDate = cuTime.tm_mon + 1;
	nDate *= 100;
	nDate += cuTime.tm_mday;
	nDate *= 10000;
	nDate += (cuTime.tm_year + 1900 );

	if (nDate != nCurrentDay)
		bHasChanged = 1;

	return bHasChanged;
}

int RoleOverLogFile()
{
	int nReturnValue = 0;
	time_t t = time(NULL);
	struct tm cuTime;
	localtime_s(&cuTime, &t);
	char szFileName[25] = { '\0' };
	char szFullPath[260] = { '\0' };

	if (fpLogFile != 0)
	{
		fclose(fpLogFile);
		fpLogFile = 0;
	}

	if (szLogFile != 0)
	{
		free(szLogFile);
		szLogFile = 0;
	}

	sprintf_s(szFileName, 25, "BHSLogFile%02d-%02d-%04d.log", cuTime.tm_mon + 1, cuTime.tm_mday, cuTime.tm_year + 1900);

	sprintf_s(szFullPath, 260, "%s\\%s", strLogFilePath, szLogFile);

	szLogFile = (char*)malloc(sizeof(char)*strnlen_s(szFullPath, 260) + 1);
	memset(szLogFile, '\0', strnlen_s(szFullPath, 260) + 1);
	stringcopy(szLogFile, strnlen_s(szFullPath, 260), szFullPath);

	fopen_s(&fpLogFile, szLogFile, "a+");

	nCurrentDay = cuTime.tm_mon + 1;
	nCurrentDay *= 100;
	nCurrentDay += cuTime.tm_mday;
	nCurrentDay *= 10000;
	nCurrentDay += (cuTime.tm_year + 1900);

	nCleanUpCount++;

	if (nCleanUpCount > 31)
	{
		CleanUpOldLogFiles();
		nCleanUpCount = 0;
	}

	return nReturnValue;
}

int WriteToFile(char *strMessage)
{
	int nReturnValue = 0;

	if (DateHasChanged() == 1)
	{
		RoleOverLogFile();
	}

	if (fpLogFile != NULL)
	{
		fwprintf_s(fpLogFile, "%s", strMessage);
	}

	return nReturnValue;
}


char* GetCurrentLogFile()
{
	return szLogFile;
}

int CleanUpOldLogFiles()
{
	int nReturnValue = 0;
	WIN32_FIND_DATA files;
	HANDLE hFind = 0;
	char szSearch[260] = {'\0'};
	SYSTEMTIME st;
	FILETIME ft;
	ULARGE_INTEGER v_ui;
	//long timeDiff = 0;
	__int64 v_right, v_left, v_res;

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
	v_ui.LowPart = ft.dwLowDateTime;
	v_ui.HighPart = ft.dwHighDateTime;
	v_right = v_ui.QuadPart;

	sprintf_s(szSearch, 260, "%s\\*.log", strLogFilePath);

	if ((hFind = FindFirstFileA(szSearch, &files)) == INVALID_HANDLE_VALUE)
	{
		//ERROR values;
		return nReturnValue;
	}

	do
	{
		memset(szSearch, '\0', 260);
		sprintf_s(szSearch, 260, "%s\\%s", strLogFilePath, files.cFileName);

		v_ui.LowPart = files.ftCreationTime.dwLowDateTime;
		v_ui.HighPart = files.ftCreationTime.dwHighDateTime;
		v_left = v_ui.QuadPart;

		v_res = v_right - v_left;
		if (v_res > (NoOfDaysBackUpLogFile * 24 * 60 * 60 * 10 ^ 7))
		{
			DeleteFileA(szSearch);
		}

	} while (FindNextFileA(hFind, &files));

	FindClose(hFind);

	return nReturnValue;
}