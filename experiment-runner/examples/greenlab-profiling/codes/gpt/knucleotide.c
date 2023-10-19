#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


#define HASH_SIZE 100003

typedef struct Node {
    char *key;
    int count;
    struct Node *next;
} Node;

typedef struct HashTable {
    Node *table[HASH_SIZE];
} HashTable;

HashTable *createHashTable();
void insert(HashTable *ht, const char *key);
Node *search(HashTable *ht, const char *key);
unsigned int hash(const char *key);
int compareNodes(const void *a, const void *b);
void freeHashTable(HashTable *ht);
void extractKMerAndInsert(HashTable *ht, const char *sequence, int k);
void displayResults(HashTable *ht, const char *sequence, int k);
void displaySpecificCounts(HashTable *ht);


int readSequenceFromFile(const char *filename, char *sequence) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 0;  // failed to open file
    }

    char line[1000]; // assuming one line will not be longer than 1000 chars
    bool extractSequence = false;

    while (fgets(line, sizeof(line), file)) {
        // Remove newline characters
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Check for headers that start with ">"
        if (line[0] == '>') {
            extractSequence = strstr(line, "THREE") != NULL;
        } else if (extractSequence) {
            strcat(sequence, line);
        }
    }

    fclose(file);

    // Remove non-alphabet characters from the sequence
    size_t j = 0;
    for (size_t i = 0; sequence[i] != '\0'; i++) {
        if (sequence[i] == 'A' || sequence[i] == 'C' || sequence[i] == 'G' || sequence[i] == 'T' ||
            sequence[i] == 'a' || sequence[i] == 'c' || sequence[i] == 'g' || sequence[i] == 't') {
            sequence[j++] = sequence[i];
        }
    }
    sequence[j] = '\0';

    return 1;  // successfully read the sequence
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    char sequence[1000000] = {0}; // adjust this size as needed

    if (!readSequenceFromFile(argv[1], sequence)) {
        fprintf(stderr, "Failed to read the sequence from file.\n");
        return 1;
    }


    HashTable *ht = createHashTable();

    extractKMerAndInsert(ht, sequence, 1);
    extractKMerAndInsert(ht, sequence, 2);

    displayResults(ht, sequence, 1);
    displayResults(ht, sequence, 2);

    // Insert for 3-, 4-, 6-, 12-, and 18- nucleotide sequences
    extractKMerAndInsert(ht, sequence, 3);
    extractKMerAndInsert(ht, sequence, 4);
    extractKMerAndInsert(ht, sequence, 6);
    extractKMerAndInsert(ht, sequence, 12);
    extractKMerAndInsert(ht, sequence, 18);

    displaySpecificCounts(ht);

    freeHashTable(ht);
    return 0;
}

HashTable *createHashTable() {
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    for (int i = 0; i < HASH_SIZE; i++) {
        ht->table[i] = NULL;
    }
    return ht;
}

unsigned int hash(const char *key) {
    unsigned int value = 0;
    for (int i = 0; key[i]; i++) {
        value = (value * 31 + key[i]) % HASH_SIZE;
    }
    return value;
}

void insert(HashTable *ht, const char *key) {
    unsigned int index = hash(key);
    Node *node = search(ht, key);
    if (node) {
        node->count++;
    } else {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->key = strdup(key);
        newNode->count = 1;
        newNode->next = ht->table[index];
        ht->table[index] = newNode;
    }
}

Node *search(HashTable *ht, const char *key) {
    unsigned int index = hash(key);
    Node *node = ht->table[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

void freeHashTable(HashTable *ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        Node *node = ht->table[i];
        while (node) {
            Node *temp = node;
            node = node->next;
            free(temp->key);
            free(temp);
        }
    }
    free(ht);
}

int compareNodes(const void *a, const void *b) {
    Node *nodeA = *(Node **)a;
    Node *nodeB = *(Node **)b;
    if (nodeA->count == nodeB->count) {
        return strcmp(nodeA->key, nodeB->key);
    }
    return nodeB->count - nodeA->count;
}

void toUppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void extractKMerAndInsert(HashTable *ht, const char *sequence, int k) {
    int length = strlen(sequence);
    char buffer[k + 1];
    buffer[k] = '\0';
    for (int i = 0; i <= length - k; i++) {
        strncpy(buffer, sequence + i, k);
        toUppercase(buffer);  // Convert the k-mer to uppercase
        insert(ht, buffer);
    }
}


void displayResults(HashTable *ht, const char *sequence, int k) {
    Node *sortedNodes[HASH_SIZE] = {0};
    int nodeCount = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        Node *node = ht->table[i];
        while (node) {
            if (strlen(node->key) == k) {
                sortedNodes[nodeCount++] = node;
            }
            node = node->next;
        }
    }
    qsort(sortedNodes, nodeCount, sizeof(Node *), compareNodes);
    for (int i = 0; i < nodeCount; i++) {
        printf("%s\t%.3f\n", sortedNodes[i]->key, (float)sortedNodes[i]->count * 100.0 / strlen(sequence));
    }
    printf("\n");
}




void displaySpecificCounts(HashTable *ht) {
    const char *queries[] = {"GGT", "GGTA", "GGTATT", "GGTATTTTAATT", "GGTATTTTAATTTATAGT"};
    int n = sizeof(queries) / sizeof(queries[0]);
    for (int i = 0; i < n; i++) {
        Node *node = search(ht, queries[i]);
        int count = (node) ? node->count : 0;
        printf("%d\t%s\n", count, queries[i]);
    }
}
