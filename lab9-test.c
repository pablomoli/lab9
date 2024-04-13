#include <stdio.h>
#include <stdlib.h>
#define HASH_SZ 23

// RecordType
struct RecordType
{
	int id;
	char name;
	int order;
};

// Fill out this structure
struct HashType
{
	struct RecordType *record;
	struct HashType *next;
};

// Compute the hash function
int hash(int x)
{
	return (x & HASH_SZ);
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
	FILE *inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	struct HashType *t = pHashArray;

	for (i = 0; i < hashSz; ++i)
	{
		// if index is occupied with any records, print all
		if (t == NULL)
		{
			return;
		}
		while (t != NULL)
		{
			struct RecordType *r = t->record;
			printf(" => {%d, %c, %d}", r->id, r->name, r->order);
			t = t->next;
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation

	struct HashType **hashtable = calloc(HASH_SZ, sizeof(struct HashType));

	for (int i = 0; i < recordSz; i++)
	{
		int index = hash((&pRecords[i])->id);
		// printf("HASH = %d\n", index);
		// printf("HASHTABLE[%d]= %p\n", index, hashtable[index]);

		if (hashtable[index] == NULL)
		{
			struct HashType *new = malloc(sizeof(struct HashType));
			new->record = &pRecords[i];
			new->next = NULL;
			hashtable[index] = new;
			// printf("\tADDED FIRST\n\n");
			continue;
		}
		else
		{
			struct HashType *t = hashtable[index];
			// printf("FILLED W/ %p\n",hashtable[index] );
			if (t->next == NULL)
			{
				struct HashType *new = malloc(sizeof(struct HashType));
				new->record = &pRecords[i];
				new->next = NULL;
				t->next = new;
				// printf("ADDED SECOND\n\n");
			}
			else
			{
				while (t->next != NULL)
				{
					t = t->next;
				}
				struct HashType *new = malloc(sizeof(struct HashType));
				new->record = &pRecords[i];
				new->next = NULL;
				t->next = new;
			}
		}
	}
	// printf("\n\nADDED ALL\n");

	for (int i = 0; i < HASH_SZ; i++)
	{
		if(hashtable[i]!= NULL){
			printf("\n");
			printf("hashtable[%d]", i);
		}
		displayRecordsInHash(hashtable[i], HASH_SZ);
	}
}
