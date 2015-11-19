#ifndef SUPPORTINGDATASTRUCTURES_H
#define SUPPORTINGDATASTRUCTURES_H

#include"ErrorCodes.h"
#include<string.h>

#define MAXKEYSIZE 1024
#define MAXVALUESIZE 8092

typedef struct structDictionaryNode
{
	char* strName;
	char* strValue;
	struct structDictionaryNode* next;
}DictionaryNode;

typedef struct
{
	float LoadFactor;
	int nSizeOfDictionary;
	int nCurrentSize;
	DictionaryNode** dictionaryArray;
}Dictionary;



#endif
