#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LineLength 60

struct Frequency
{
    char c;
    double p;
};

struct Frequency IUB[] = {
    {'a', 0.27},
    {'c', 0.12},
    {'g', 0.12},
    {'t', 0.27},
    {'B', 0.02},
    {'D', 0.02},
    {'H', 0.02},
    {'K', 0.02},
    {'M', 0.02},
    {'N', 0.02},
    {'R', 0.02},
    {'S', 0.02},
    {'V', 0.02},
    {'W', 0.02},
    {'Y', 0.02}
};

struct Frequency HomoSapiens[] = {
    {'a', 0.3029549426680},
    {'c', 0.1979883004921},
    {'g', 0.1975473066391},
    {'t', 0.3015094502008}
};

int LineCounter = 0;

double Random()
{
    return (double)rand() / RAND_MAX;
}

char* GenerateRepeatSequence(const char* pattern, int length) {
    char* sequence = (char*)malloc(length + 1);
    if (!sequence) {
        perror("Failed to allocate memory");
        exit(1);
    }
    
    int patternLength = strlen(pattern);
    for (int i = 0; i < length; i++) {
        sequence[i] = pattern[i % patternLength];
    }
    sequence[length] = '\0';
    return sequence;
}


char* GenerateIUBAmbiguityCodes(int length)
{
    char* sequence = (char*)malloc(length + 1);
    sequence[0] = '\0';

    while (strlen(sequence) < length)
    {
        char baseChar = ' ';
        double randomValue = Random();

        for (int i = 0; i < sizeof(IUB) / sizeof(IUB[0]); i++)
        {
            randomValue -= IUB[i].p;
            if (randomValue < 0)
            {
                baseChar = IUB[i].c;
                break;
            }
        }

        char temp[2] = {baseChar, '\0'};
        strcat(sequence, temp);
    }

    sequence[length] = '\0';
    return sequence;
}

char* GenerateHomoSapiensFrequency(int length)
{
    char* sequence = (char*)malloc(length + 1);
    sequence[0] = '\0';

    while (strlen(sequence) < length)
    {
        char baseChar = ' ';
        double randomValue = Random();

        for (int i = 0; i < sizeof(HomoSapiens) / sizeof(HomoSapiens[0]); i++)
        {
            randomValue -= HomoSapiens[i].p;
            if (randomValue < 0)
            {
                baseChar = HomoSapiens[i].c;
                break;
            }
        }

        char temp[2] = {baseChar, '\0'};
        strcat(sequence, temp);
    }

    sequence[length] = '\0';
    return sequence;
}

void InsertNewLineIfNeeded()
{
    LineCounter++;
    if (LineCounter >= LineLength)
    {
        printf("\n");
        LineCounter = 0;
    }
}

void PrintWithLineBreaks(const char* sequence)
{
    for (int i = 0; i < strlen(sequence); i++)
    {
        printf("%c", sequence[i]);
        InsertNewLineIfNeeded();
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <sequence_length>\n", argv[0]);
        return 1;
    }

    int sequenceLength = atoi(argv[1]);
    srand(time(NULL));

    char* aluSequence = GenerateRepeatSequence("GGCCGGGCGCGGTGGCTCACGCCTGTAATCCCAGCACTTTGG", sequenceLength * 2);
    printf(">ONE Homo sapiens alu\n");
    PrintWithLineBreaks(aluSequence);
    free(aluSequence);

    char* iubSequence = GenerateIUBAmbiguityCodes(sequenceLength * 3);
    printf(">TWO IUB ambiguity codes\n");
    PrintWithLineBreaks(iubSequence);
    free(iubSequence);

    char* humanFrequencySequence = GenerateHomoSapiensFrequency(sequenceLength * 5);
    printf(">THREE Homo sapiens frequency\n");
    PrintWithLineBreaks(humanFrequencySequence);
    free(humanFrequencySequence);

    return 0;
}
