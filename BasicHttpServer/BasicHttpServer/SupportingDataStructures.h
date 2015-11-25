#ifndef SUPPORTINGDATASTRUCTURES_H
#define SUPPORTINGDATASTRUCTURES_H

#include"ErrorCodes.h"
#include<string.h>
#include<stdlib.h>

#define MAXKEYSIZE 1024
#define MAXVALUESIZE 8092

typedef struct structDictionaryNode
{
	char* strName;
	char* strValue;
	struct structDictionaryNode* next;
}DictionaryNode;

typedef struct structDictionary
{
	float LoadFactor;
	int nSizeOfDictionary;
	int nCurrentSize;
	DictionaryNode** dictionaryArray;
}Dictionary;

unsigned hash(int nSizeOfTable, char *s);
Dictionary* CreateDictionary(int nSizeOfDictory);
int DeleteDictionaryNodeArray(DictionaryNode **dArray, int nSizeOfArray);
int DeleteDictionary(Dictionary *dict);
DictionaryNode* CreateADictionaryNode(char* strKey, char *strValue);
int AddNameValueToDictionary(Dictionary *dict, char *strKey, char *strValue);
int AddNodeToDictionary(Dictionary *dict, DictionaryNode *dictNode);
int IsKeyExistsInDictionary(Dictionary *dict, char *strKey);
int RebuildDictionary(Dictionary *dict, int nNewSize);
int DeleteKeyFromDictionary(Dictionary *dict, char *strKey);
int DeleteNodeOfDictionary(Dictionary *dict, DictionaryNode *dictNode);
DictionaryNode* GetNodeFromDictionary(Dictionary *dict, char *strKey);
char* GetValueFromDictionary(Dictionary *dict, char *strKey);

#endif
