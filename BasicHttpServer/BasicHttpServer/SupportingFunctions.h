#ifndef SUPPORTINGFUNCTIONS_H
#define SUPPORTINGFUNCTIONS_H

#include "Shlwapi.h"
#include "ErrorCodes.h"

int stringcopy(char* strDestination, int nNoOfCharacters, char* strSource);
int substringcopy(char* strDestination, int nStartIndex, int nNoOfCharacters, char *strSource);
int FileExists(char* strFile);
int ConvertStringToNumber(char* strNum, int* pResNum);

#endif
