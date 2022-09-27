#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>

#define HEAP_SIZE 100
#define HEAP_ARRAY_SIZE 101
#define HASH_TABLE_SIZE 500000
#define MAX_WORD_SIZE 99

//double time = 0;
void measure_time()
{
    clock_t t = clock();
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Time %f seconds\n", time_taken);
}

// Counter for the nonempty slots of the heapArray
int heapCount = 0;

// Function for hashing a string
unsigned hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c;

    return hash;
}

// Function for giving an index to a string for the hash array
int getIndex(const char *str)
{
    unsigned long hashVal = hash(str);

    hashVal = hashVal%HASH_TABLE_SIZE;

    return (int) hashVal;
}

typedef struct HashNode HashNode;

struct HashNode
{
    unsigned int freq;
    char word[MAX_WORD_SIZE];
};

struct HashNode* hashArray[HASH_TABLE_SIZE];
struct HashNode* item;

// Funciton for inserting a string into the hash table
int insert(const char *str)
{
    // Allocate memory
    struct HashNode *item = (struct HashNode*) malloc(sizeof(struct HashNode));

    int hashIndex = getIndex(str);
    int initialIndex = hashIndex;
    //unsigned long hashValue = hash(str);
    item->freq = 1;
    strcpy(item->word, str);

    // If the calculated index is already occupied
    while(hashArray[hashIndex] != NULL)
    {
        // If word is already hashed, increase frequency
        if (strcmp(hashArray[hashIndex]->word,str) == 0)
        {
            ++hashArray[hashIndex]->freq;
            return 0;
        }

        // Get a new index
        ++hashIndex;
        hashIndex %= HASH_TABLE_SIZE;

        // If the hash table is full
        if (hashIndex == initialIndex)
        {
            printf("Hash full\n");
            return -1;
        }
    }

    hashArray[hashIndex] = item;

    return 0;
}

// Function for swapping two integers
void swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

int getRchild(int index)
{
    if ( (((2*index) + 1) < HEAP_ARRAY_SIZE) && (index >= 1) )
        return (2*index) + 1;
    return -1;
}

int getLchild(int index)
{
    if ( ((2*index) < HEAP_ARRAY_SIZE) && (index >= 1) )
        return (2*index);
    return -1;
}

int getParent(int index)
{
    if ( (index > 1) && (index < HEAP_ARRAY_SIZE))
        return index/2;
    return -1;
}

void minHeapify(int *heapArray, int index)
{
    int leftChildIndex = getLchild(index);
    int rightChildIndex = getRchild(index);

    int smallest = index;
    if ( (leftChildIndex <= HEAP_ARRAY_SIZE) && (leftChildIndex > 0))
    {
        if (hashArray[heapArray[leftChildIndex]]->freq < hashArray[heapArray[smallest]]->freq)
        {
            smallest = leftChildIndex;
        }
    }

    if ( (rightChildIndex <= HEAP_ARRAY_SIZE) && (rightChildIndex > 0))
    {
        if (hashArray[heapArray[rightChildIndex]]->freq < hashArray[heapArray[smallest]]->freq)
        {
            smallest = rightChildIndex;
        }
    }

    if (smallest != index)
    {
        swap(&heapArray[index], &heapArray[smallest]);
        minHeapify(heapArray, smallest);
    }
}

// Function for building the min heap after the array has been filled
void buildMinHeap(int *heapArray)
{
    for (int i = HEAP_SIZE/2; i >= 1; --i)
    {
        minHeapify(heapArray, i);
    }
}

// Function for displaying the contents of the heap (used after sorting)
void display(int *heapArray)
{
    for (int i = 1; i < HEAP_SIZE; ++i)
        printf("%s:   %u\n", hashArray[heapArray[i]]->word, hashArray[heapArray[i]]->freq);
}

// Function for sorting (insertion sort) the heap to descending order after it contains the indexes
// of 100 most frequent words
void sort(int *heapArray)
{
    for (int i = 2; i < HEAP_ARRAY_SIZE; ++i)
    {
        int key = heapArray[i];
        int j = i - 1;
        while (j >= 1 && (hashArray[heapArray[j]]->freq < hashArray[key]->freq))
        {
            heapArray[j + 1] = heapArray[j];
            j = j - 1;
        }
        heapArray[j + 1] = key;
    }
}

// Function for reading all of the strings into the min heap,
// sorting it and displaying its contents at the end
void initialize(int* heapArray)
{
    int heapCount = 0;
    int i = 0;
    int j = 1;
    // First store 100 first words from the hash table
    while (heapCount <= HEAP_SIZE)
    {
        if (hashArray[i] != NULL)
        {
            heapArray[j] = i;   // Store the index of a word
            ++j;
            ++i;
            ++heapCount;
        }
        else
            ++i;
    }
    // Build the heap
    buildMinHeap(heapArray);

    // Go through the rest of the hash table an compare the strings to
    // the root of the min heap. If a more frequent string is found
    // replace the root and heapify.
    while (i < HASH_TABLE_SIZE)
    {
        if (hashArray[i] != NULL)
        {
            if (hashArray[heapArray[1]]->freq < hashArray[i]->freq)
            {
                heapArray[1] = i;
                minHeapify(heapArray,1);
            }
            ++i;
        }
        ++i;
    }

    // After 100 most frequent words have been stored, sort the heap
    // using insertion sort
    sort(heapArray);
    // Display the contents of the heap (100 most frequent words in descending order)
    display(heapArray);
}

int main(int argc, char ** argv)
{
    measure_time();
    int i,j;

    FILE *fp = fopen ("bulk.txt", "r");

    if (fp == NULL){
        printf ("Invalid filename\n");
        exit(-1);
    }
    else
    {
        char buffer[MAX_WORD_SIZE];
        while (fscanf(fp, "%s", buffer) != EOF)
        {

            for(i = 0; buffer[i] != '\0'; ++i)
            {
                while (!( (buffer[i] >= 'a' && buffer[i] <= 'z') || (buffer[i] >= 'A' && buffer[i] <= 'Z') || buffer[i] == '\0') )
                {

                }
            }

            // upper case the buffer
            for(int l = 0; buffer[l]; ++l)
            {
                buffer[l] = toupper(buffer[l]);
            }

            // If the buffer is an actual word after the process,
            // insert into the has table
            if(strcmp("",buffer) != 0)
            {
                insert(buffer);
            }


        }
        fclose(fp);
    }

    // The heap is implemented by an array of integers containing
    // the indexes of the strings in the hash table
    int heapArray[HEAP_ARRAY_SIZE];
    initialize(heapArray);

    measure_time();
    return 0;
}
