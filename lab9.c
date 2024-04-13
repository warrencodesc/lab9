// Warren Daniel

#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 10

// Structure to represent a record with id, name, and order
struct RecordType
{
    int id;   // Record identifier
    char name; // Record name (single character)
    int order; // Order or sequence value
};

// Node structure for a hash table
struct HashNode
{
    struct RecordType data; // The data stored in the node
    struct HashNode *next;  // Pointer to the next node for handling collisions
};

// Structure to represent a hash table with an array of buckets
struct HashType
{
    struct HashNode *table[HASH_SIZE]; // Array of pointers to hash nodes (buckets)
};

// Function to compute the hash function based on an integer key
int hash(int key)
{
    return key % HASH_SIZE; // simple modulo hashing
}

// Function to parse input data from a file into an array of RecordType structures
int parseData(char *inputFileName, struct RecordType **recordsArray)
{
    FILE *inputFile = fopen(inputFileName, "r");
    int numRecords = 0; // Number of records in the file
    int i, n;
    char c;
    struct RecordType *recordPtr;
    *recordsArray = NULL;

    if (inputFile)
    {
        // Read the number of records from the file
        fscanf(inputFile, "%d\n", &numRecords);
        // Allocate memory for the records array
        *recordsArray = (struct RecordType *)malloc(sizeof(struct RecordType) * numRecords);
        if (*recordsArray == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        // Read each record from the file
        for (i = 0; i < numRecords; ++i)
        {
            recordPtr = *recordsArray + i;
            // Read id, name, and order for each record
            fscanf(inputFile, "%d ", &n);
            recordPtr->id = n;
            fscanf(inputFile, "%c ", &c);
            recordPtr->name = c;
            fscanf(inputFile, "%d ", &n);
            recordPtr->order = n;
        }

        // Close the file
        fclose(inputFile);
    }

    // Return the number of records read
    return numRecords;
}

// Function to print the records array
void printRecords(struct RecordType recordsArray[], int numRecords)
{
    printf("\nRecords:\n");
    // Iterate through each record and print its fields
    for (int i = 0; i < numRecords; ++i)
    {
        printf("\t%d %c %d\n", recordsArray[i].id, recordsArray[i].name, recordsArray[i].order);
    }
    printf("\n\n");
}


void displayRecordsInHash(struct HashType *hashTable, int hashSz)
{
    printf("\nRecords in Hash Table:\n");
    // Iterate through each bucket in the hash table
    for (int i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        struct HashNode *currentNode = hashTable->table[i];
        // Iterate through the linked list at each bucket
        while (currentNode != NULL)
        {
            printf("%d %c %d -> ", currentNode->data.id, currentNode->data.name, currentNode->data.order);
            // Move to the next node in the list
            currentNode = currentNode->next;
        }
        printf("NULL\n"); // Indicates the end of the linked list
    }
}

// Main function
int main(void)
{
    struct RecordType *recordsArray;
    int numRecords = 0;

    // Load records from the input file
    numRecords = parseData("input.txt", &recordsArray);
    printRecords(recordsArray, numRecords);

    // Initialize Hash Table
    struct HashType hashTable;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        hashTable.table[i] = NULL; // Initialize each bucket to NULL
    }

    // Insert records into the hash table
    for (int i = 0; i < numRecords; ++i)
    {
        // Calculate the index using the hash function
        int index = hash(recordsArray[i].id);
        // Create a new hash node for the record
        struct HashNode *newNode = (struct HashNode *)malloc(sizeof(struct HashNode));
        newNode->data = recordsArray[i];
        // Insert the new node at the beginning of the linked list at the calculated index
        newNode->next = hashTable.table[index];
        hashTable.table[index] = newNode;
    }

    // Display records stored in the hash table
    displayRecordsInHash(&hashTable, HASH_SIZE);

    return 0;
}