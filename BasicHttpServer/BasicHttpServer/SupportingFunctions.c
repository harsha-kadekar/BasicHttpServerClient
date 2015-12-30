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

/*
Name: ConvertStringToNumber
Description: As the name suggests this function is responsible for converting the read string into number.
Parameter: strNum = Number in the format of strings
pResNum = after conversion, number will be placed here.
ReturnValue: 0 for success, else -ve error code.
*/
int ConvertStringToNumber(char* strNum, int* pResNum)
{
	int nReturnValue = 0;
	int i = 0;
	int nNumber = 0;
	int nTemp = 0;

	while (strNum[i] != '\0' && i < 1024)
	{
		nTemp = strNum[i++] - '0';
		if (nTemp > 9)
		{
			//INVALID NUMBER CHARACTER
			nReturnValue = ERR_INVALID_VALUE_IN_CONFIG;
			return nReturnValue;
		}

		nNumber = nNumber * 10 + nTemp;

	}

	(*pResNum) = nNumber;

	return nReturnValue;
}
