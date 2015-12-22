#include "SupportingFunctions.h"

int stringcopy(char* strDestination, int nNoOfCharacters, char* strSource)
{
	int i = 0;

	while (i < nNoOfCharacters)
	{
		strDestination[i] = strSource[i];
		i++;
	}

	return i;
}

int substringcopy(char* strDestination, int nStartIndex, int nNoOfCharacters, char *strSource)
{
	int i = 0;
	int j = 0;

	for (i = nStartIndex, j = 0; j < nNoOfCharacters; j++, i++)
	{
		strDestination[j] = strSource[i];
	}

	return j;
}

int FileExists(char* strFile)
{
	int nReturnValue = 0;
	nReturnValue = PathFileExistsA(strFile);
	return nReturnValue;
}