/*
This file will have all the supporting datastructures and algorithms that will be used by te web server.
Dictionary was developed based on "The C Programming Language" Section 6.6 By Brian W. Kernighan and Dennis M. Ritchie
*/

#include "SupportingDataStructures.h"

/*
Name: hash
Description: This function helps in finding the index for a string in a dictionary or hash table.
Parameters: nSizeOfTable - Size of the dictionary in which we are trying to find the index for the string.
			s - string for whom the index or place is being found in dictionary
ReturnValue: index of the hash table where this string can be placed.
*/
unsigned hash(int nSizeOfTable, char *s)
{
	unsigned hashval;
	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;
	return hashval % nSizeOfTable;
}


/*
Name: CreateDictionary
Description: This function will create a dictionary of size passed.
Parameters: nSizeOfDictionary : initial size of the dictionary. Size of the array
ReturnValue: Created dictionary.
*/
Dictionary* CreateDictionary(int nSizeOfDictory)
{
	Dictionary *dict = 0;
	Dictionary test;

	test.LoadFactor = 0.8;
	test.nCurrentSize = 0;
	test.nSizeOfDictionary = 10;
	test.dictionaryArray = 0;

	int i = 0;

	if (nSizeOfDictory == 0)
		nSizeOfDictory = 10;

	dict = (Dictionary*)malloc(sizeof(Dictionary));
	if (dict == 0)
	{
		//Error handle it
		return dict;
	}

	dict->LoadFactor = 0.8;
	dict->nSizeOfDictionary = nSizeOfDictory;
	dict->nCurrentSize = 0;
	dict->dictionaryArray = (DictionaryNode**)malloc(sizeof(DictionaryNode*)*dict->nSizeOfDictionary);

	for (int i = 0; i < dict->nSizeOfDictionary; i++)
		dict->dictionaryArray[i] = 0;


	return dict;
}

/*
Name: DeleteDictionaryNodeArray
Description: This function will delete an array of single list of dictionary nodes.
Parameters: dArray - Array of single list of dictionary nodes that needs to be deleted.
			nSizeOfArray - Arrays size
ReturnValue: 0 for error else erro code.
*/
int DeleteDictionaryNodeArray(DictionaryNode **dArray, int nSizeOfArray)
{
	int nReturnValue = 0;
	int i = 0, j = 0;
	DictionaryNode *pCurrent = 0, *pHead = 0;

	for (i = 0; i < nSizeOfArray; i++)
	{
		if (dArray[i] != 0)
		{
			pHead = dArray[i];
			while (pHead != 0)
			{
				if ((*pHead).strName != 0)
				{
					free((*pHead).strName);
					(*pHead).strName = 0;
				}

				if ((*pHead).strValue != 0)
				{
					free((*pHead).strValue);
					(*pHead).strValue = 0;
				}

				pCurrent = (*pHead).next;
				free(pHead);
				pHead = pCurrent;

			}

			dArray[i] = 0;
		}
	}

	free(dArray);
	dArray = 0;

	return nReturnValue;
}

/*
Name: DeleteDictionary
Description: This function deletes the given dictionary.
Parameters: dict - The dictionary that needs to be deleted.
ReturnValue: 0 for success else error code
*/
int DeleteDictionary(Dictionary *dict)
{
	int nReturnValue = 0;
	int i = 0, j = 0;
	DictionaryNode *pCurrent = 0, *pHead = 0;

	if (dict != 0)
	{
		if (dict->dictionaryArray != 0)
		{
			nReturnValue = DeleteDictionaryNodeArray(dict->dictionaryArray, dict->nSizeOfDictionary);
			dict->dictionaryArray = 0;
		}

		dict->LoadFactor = 0;
		dict->nCurrentSize = 0;
		dict->nSizeOfDictionary = 0;

		free(dict);
		dict = 0;
	}

	return nReturnValue;
}

/*
Name: CreateDictionaryNode
Description: Given a string name value pair, this function will return a DictionaryNode.
Parameter: strKey = Name field in the DictionaryNode
			strValue = Value field in the DictionaryNode.
*/
DictionaryNode* CreateADictionaryNode(char* strKey, char *strValue)
{
	DictionaryNode *node = 0;
	int i = 0;

	node = (DictionaryNode*)malloc(sizeof(DictionaryNode) * 1);

	(*node).next = 0;

	node->strName = strKey;
	node->strValue = strValue;

	/*(*node).strName = (char*)malloc(sizeof(char)*strnlen_s(strKey, MAXKEYSIZE) + 1);
	memset((*node).strName, '\0', strnlen_s(strKey, MAXKEYSIZE) + 1);
	while (strKey[i] != '\0')
	{
		(*node).strName[i] = strKey[i];
		i++;
	}

	i = 0;
	(*node).strValue = (char*)malloc(sizeof(char)*strnlen_s(strValue, MAXVALUESIZE) + 1);
	memset((*node).strValue, '\0', strnlen_s(strValue, MAXVALUESIZE) + 1);
	while (strValue[i] != '\0')
	{
		(*node).strValue[i] = strValue[i];
		i++;
	}*/

	return node;
}

/*
Name: AddNameValueToDictionary
Description: This function will add a dictionary node to the dictionary given key and value pair
Parameters: dict - dictionary where node needs to be added.
			strKey - Name/Key which needs to be stored.
			strValue - Value which needs to be stored.
ReturnValue: 0 for success else error code.
*/
int AddNameValueToDictionary(Dictionary *dict, char *strKey, char *strValue)
{
	int nReturnValue = 0;
	DictionaryNode *pHead = 0, *pCurrent = 0;
	int i = 0;
	float fTemp = 0.0;
	int bFoundKey = 0;

	if (dict == 0 || strKey == 0 || strValue == 0)
	{
		nReturnValue = ERR_INVALID_PARAMETERS_DICT;
		return nReturnValue;
	}

	if (IsKeyExistsInDictionary(dict, strKey) == 1)
	{
		pHead = dict->dictionaryArray[hash(dict->nSizeOfDictionary, strKey)];
		if (pHead == 0)
		{
			//Actually Error
			dict->dictionaryArray[hash(dict->nSizeOfDictionary, strKey)] = CreateADictionaryNode(strKey, strValue);
			dict->nCurrentSize++;

		}
		else
		{
			while (pHead != 0)
			{
				if (strcmp((*pHead).strName, strKey) == 0)
				{
					//Update Value
					free((*pHead).strValue);
					(*pHead).strValue = (char*)malloc(sizeof(char)*strnlen_s(strValue, MAXVALUESIZE) + 1);
					memset((*pHead).strValue, '\0', strnlen_s(strValue, MAXVALUESIZE) + 1);

					i = 0;
					while (strValue[i] != '\0')
					{
						(*pHead).strValue[i] = strValue[i];
						i++;
					}

					bFoundKey = 1;
					break;
				}
				else
				{
					pHead = (*pHead).next;
				}
			}

			if (bFoundKey == 0)
			{
				//Actually Error
				pHead = dict->dictionaryArray[hash(dict->nSizeOfDictionary, strKey)];
				pCurrent = CreateADictionaryNode(strKey, strValue);
				(*pCurrent).next = pHead;
				dict->dictionaryArray[hash(dict->nSizeOfDictionary, strKey)] = pCurrent;
				dict->nCurrentSize++;

			}
		}



	}
	else
	{
		pHead = dict->dictionaryArray[hash(dict->nSizeOfDictionary, strKey)];

		if (pHead == 0)
		{
			dict->dictionaryArray[hash(dict->nSizeOfDictionary, strKey)] = CreateADictionaryNode(strKey, strValue);
			dict->nCurrentSize++;
		}
		else
		{
			pCurrent = CreateADictionaryNode(strKey, strValue);
			(*pCurrent).next = pHead;
			dict->dictionaryArray[hash(dict->nSizeOfDictionary, strKey)] = pCurrent;
			dict->nCurrentSize++;
			
		}

	}

	fTemp = dict->nCurrentSize;
	fTemp = fTemp / dict->nSizeOfDictionary;


	if (fTemp > dict->LoadFactor)
	{
		nReturnValue = RebuildDictionary(dict, dict->nSizeOfDictionary * 2);
	}

	return nReturnValue;
}

/*
Name: AddNodeToDictionary
Description: This function will add the given dictionary node to dictionary. Dictionary node intern will have the name value pair
Parameters: dict - Dictionary to which node needs to be added.
			dictNode - Dictionary Node, which has name/value pair , which needs to be added to dictionary
ReturnValue: 0 for success else error codes.
*/
int AddNodeToDictionary(Dictionary *dict, DictionaryNode *dictNode)
{
	int nReturnValue = 0;
	DictionaryNode *pHead = 0, *pCurrent = 0, *pPrev = 0;
	float fTemp = 0.0;
	int bFound = 0;
	int i = 0;

	if (dict == 0 || dictNode == 0)
	{
		//Handle the error
		nReturnValue = ERR_INVALID_PARAMETERS_DICT;
		return nReturnValue;
	}

	if (IsKeyExistsInDictionary(dict, (*dictNode).strName) == 1)
	{
		pHead = dict->dictionaryArray[hash(dict->nSizeOfDictionary, (*dictNode).strName)];
		if (pHead == 0)
		{
			//Actually Error
			dict->dictionaryArray[hash(dict->nSizeOfDictionary, (*dictNode).strName)] = dictNode;
			dict->nCurrentSize++;
		}
		else
		{
			pCurrent = pHead;
			pPrev = 0;
			while (pCurrent != 0)
			{
				if (strcmp((*pCurrent).strName, (*dictNode).strName) == 0)
				{
					//Update the Value
					if (pPrev == 0)
					{
						//1st element
						(*dictNode).next = (*pCurrent).next;
						free((*pCurrent).strName);
						free((*pCurrent).strValue);
						free(pCurrent);
					}
					else
					{
						(*dictNode).next = (*pCurrent).next;
						(*pPrev).next = dictNode;

						free((*pCurrent).strName);
						free((*pCurrent).strValue);
						free(pCurrent);
					}

					bFound = 1;
					break;
				}
				else
				{
					pPrev = pCurrent;
					pCurrent = (*pCurrent).next;

				}
			}

			if (bFound != 1)
			{
				//Actually error
				(*dictNode).next = pHead;
				dict->dictionaryArray[hash(dict->nSizeOfDictionary, (*dictNode).strName)] = dictNode;
				dict->nCurrentSize++;

			}
		}
	}
	else
	{
		pHead = dict->dictionaryArray[hash(dict->nSizeOfDictionary, (*dictNode).strName)];
		if (pHead == 0)
		{
			dict->dictionaryArray[hash(dict->nSizeOfDictionary, (*dictNode).strName)] = dictNode;
			dict->nCurrentSize++;
		}
		else
		{
			(*dictNode).next = pHead;
			dict->dictionaryArray[hash(dict->nSizeOfDictionary, (*dictNode).strName)] = dictNode;
			dict->nCurrentSize++;
		}
	}

	fTemp = dict->nCurrentSize;
	fTemp = fTemp / dict->nSizeOfDictionary;


	if (fTemp > dict->LoadFactor)
	{
		nReturnValue = RebuildDictionary(dict, dict->nSizeOfDictionary * 2);
	}

	return nReturnValue;
}

/*
Name: IsKeyExistsInDictionary
Description: This function will check if a given key/Name exists in the dictionary
Parameter: dict - Dictionary which needs to be searched for the key.
		   strKey - Key which needs to be searched in dictionary
ReturnValue: 1 if found else 0
*/
int IsKeyExistsInDictionary(Dictionary *dict, char *strKey)
{
	int bFound = 0;
	int i = 0;
	DictionaryNode *pHead = 0, *pCurrent = 0;

	for (i = 0; i < dict->nSizeOfDictionary; i++)
	{
		pHead = dict->dictionaryArray[i];
		if (pHead != 0)
		{
			pCurrent = pHead;
			while (pCurrent != 0)
			{
				if (strcmp(strKey, (*pCurrent).strName) == 0)
				{
					bFound = 1;
					break;
				}

				pCurrent = (*pCurrent).next;
			}
		}
	}

	return bFound;
}

/*
Name: IsValueExistsInDictionary
Description: This function will check if the given value exists in the given dictionary.
Parameters: dict - Dictionary where we need to search for the value
			strValue - Value that will be searched in the dictionary
ReturnValue: 0 if not found else 1
*/
int IsValueExistsInDictionary(Dictionary *dict, char *strValue)
{
	int nReturnValue = 0;

	return nReturnValue;
}

/*
Name: RebuildDictionary
Description: This function tries to rebuild the dictionary with the given new size.
Parameters: dict - Dictionary which needs to be rebuilt.
			nNewSize - This is the size dictionary has to be resized.
ReturnValue: 0 for success else error codes.
*/
int RebuildDictionary(Dictionary *dict, int nNewSize)
{
	int nReturnValue = 0;
	int i = 0;
	int nOldSize = 0;
	DictionaryNode *pHead = 0, *pCurrent = 0;

	DictionaryNode** pNewArray = (DictionaryNode**)malloc(sizeof(DictionaryNode*)*nNewSize);
	DictionaryNode** pOldArray = dict->dictionaryArray;
	nOldSize = dict->nSizeOfDictionary;
	dict->nSizeOfDictionary = nNewSize;
	dict->dictionaryArray = pNewArray;
	dict->nCurrentSize = 0;

	for (i = 0; i < nOldSize; i++)
	{
		pHead = pOldArray[i];

		while (pHead != 0)
		{
			pCurrent = pHead;
			pHead = (*pCurrent).next;

			(*pCurrent).next = 0;
			nReturnValue = AddNodeToDictionary(dict, pCurrent);
			if (nReturnValue != 0)
			{
				//Error handle it
			}
			
		}
	}

	free(pOldArray);
	pOldArray = 0;

	return nReturnValue;
}

/*
Name: DeleteKeyFromDictionary
Description: This function given a key, finds that node having the key and deletes that node from dictionary.
Parameters: dict - Dictionary from where we need to delete the node having key
			strKey - Key which needs to deleted. used to find the node having the key.
ReturnValue: 0 for success else error codes.
*/
int DeleteKeyFromDictionary(Dictionary *dict, char *strKey)
{
	int nReturnValue = 0;

	int i = 0;

	DictionaryNode *pHead = 0, *pCurrent = 0, *pPrev = 0;

	pHead = dict->dictionaryArray[hash(dict->nSizeOfDictionary, strKey)];
	if (pHead != 0)
	{
		pCurrent = pHead;
		while (pCurrent != 0)
		{
			if (strcmp((*pCurrent).strName, strKey) == 0)
			{
				if (pPrev == 0)
				{
					dict->dictionaryArray[hash(dict->nSizeOfDictionary, strKey)] = (*pCurrent).next;

				}
				else
				{
					(*pPrev).next = (*pCurrent).next;

				}
				free((*pCurrent).strName);
				free((*pCurrent).strValue);
				free(pCurrent);
				dict->nCurrentSize--;
				break;
			}

			pPrev = pCurrent;
			pCurrent = (*pCurrent).next;
		}
	}
	else
	{
		//Should I have to intimate user??
	}
	
	return nReturnValue;
}

/*
Name: DeleteNodeOfDictionary
Description: This function will delete the given node from the dictionary.
Parameter: dict - Dictionary in which node needs to be deleted.
		   dictNode - Node which needs to be deleted from the dictionary.
ReturnValue: 0 for success else error code.
*/
int DeleteNodeOfDictionary(Dictionary *dict, DictionaryNode *dictNode)
{
	int nReturnValue = 0;
	DictionaryNode *pCurrent = 0, *pPrev = 0, *pHead = 0;

	pHead = dict->dictionaryArray[hash(dict->nSizeOfDictionary, (*dictNode).strName)];
	if (pHead != 0)
	{
		
			pCurrent = pHead;
			while (pCurrent != 0)
			{
				if (pCurrent == dictNode)
				{
					//Delete the node
					if (pPrev == 0)
					{
						dict->dictionaryArray[hash(dict->nSizeOfDictionary, (*dictNode).strName)] = pCurrent->next;
					}
					else
					{
						pPrev->next = pCurrent->next;
					}

					free(pCurrent->strName);
					free(pCurrent->strValue);
					free(pCurrent);
					dict->nCurrentSize--;
					break;
				}

				pPrev = pCurrent;
				pCurrent = pCurrent->next;
			}
		
	}

	return nReturnValue;
}

/*
Name: GetNodeFromDictionary
Description: This function will get the Node having the Key as its Name.
Parameters: dict - Dictionary in which key will be searched
			strKey - Name used to identify the Node.
ReturnValue: Node containing the strKey as Name. If not found then Null.
*/
DictionaryNode* GetNodeFromDictionary(Dictionary *dict, char *strKey)
{
	DictionaryNode *pNode = 0;

	pNode = dict->dictionaryArray[hash(dict->nSizeOfDictionary, strKey)];
	
	while (pNode != 0)
	{
		if (strcmp(pNode->strName, strKey) == 0)
		{
			break;
		}
		
		pNode = pNode->next;
	}

	return pNode;
}

/*
Name: GetValueFromDictionary
Description: Given a key, this function will get the value from the dictionary.
Parameters: dict - Dictionary in which we will be searching from the strKey
			strKey - string which will be serached in dictionary.
ReturnValue: Value of the corresponding Key. If not found then null.
*/
char* GetValueFromDictionary(Dictionary *dict, char *strKey)
{
	char *strValue = 0;

	DictionaryNode *pNode = GetNodeFromDictionary(dict, strKey);
	if (pNode != 0)
	{
		strValue = pNode->strValue;
	}

	return strValue;
}

