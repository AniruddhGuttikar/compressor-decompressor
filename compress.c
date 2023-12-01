#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "heap.h"

#define MAX_TREE_HT 1000

typedef struct CharFreq {
    char character;
    int frequency;
} CharFreq;

typedef struct CharCode {
    char character;
    int* code;
    int size;
} CharCode;

void printArray(int arr[], int n) {
    for(int i = 0; i < n; i++) {
        printf("%d", arr[i]);
    }
    printf("\n");
}

//for every left push 0, every right push 1
void printCodes(Node* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top+1);
    }
    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top+1);
    }

    if (isLeaf(root)) {
        printf("%c: ", root->data);
        printArray(arr, top);
    }
}

void createCharCodeArr(CharCode* charCodeArr, int* index, Node* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        createCharCodeArr(charCodeArr, index, root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        createCharCodeArr(charCodeArr, index, root->right, arr, top + 1);
    }

    if (isLeaf(root)) {
       charCodeArr[*index].character = root->data;
       charCodeArr[*index].code = malloc(sizeof(int)*top);
       charCodeArr[*index].size = top;
       for (int i = 0; i < top; i++) {
            charCodeArr[*index].code[i] = arr[i];
       }
       (*index)++;
    }
}

Node* buildHuffmanTree(char data[], int freq[], int size) {
    Node *left, *right, *top;
    MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    //iterate till size of the heap becomes 1
    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        //create a new parent node for these two minimum nodes
        //with frequency equals to the sum of the left and right
        //tree formation is the separate process 
        //we are popping two least frequency elements and adding their 
        //parent in the heap and maintaining the order of heap

        top = newNode('$', left->frequency + right->frequency);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);

    }

    //the last remaining node will be the root of the tree 
    return extractMin(minHeap);
}

CharCode* huffmanCodes(char data[], int freq[], int size) {
    Node* root = buildHuffmanTree(data, freq, size);

    int arr[MAX_TREE_HT], top = 0;
    printCodes(root, arr, top);
    
    int index = 0;
    int arr2[MAX_TREE_HT];
    top = 0;
    CharCode* charCodeArr = malloc(sizeof(CharCode)*size);
    printf("size %d %d \n", sizeof(CharCode), sizeof(CharCode)*size);
    createCharCodeArr(charCodeArr, &index, root, arr2, top);
    return charCodeArr;
}

CharFreq* createCharFreq(char* inputStr, int* size) {
    int charCount[256] = {0};

    //FIND THE FREQUENCY OF EACH CHARACTER
    char* ptr = inputStr;
    while (*ptr != '\0') {
        charCount[(int)(*ptr)]++;
        ptr++;
    }

    //FIND THE SIZE OF THE UNIQUE CHARACTERS
    int uniqueCharCount = 0;
    for (int i = 0; i < 256; i++) {
        if (charCount[i]) {
            uniqueCharCount++;
        }
    }
    *size = uniqueCharCount;

    //CREATE AN ARRAY OF CHARFREQ STUCTS 
    CharFreq* charFreqArr = malloc(sizeof(CharFreq)*uniqueCharCount);
    int index = 0;
    for (int i = 0; i < 256; i++) {
        if(charCount[i]) {
            charFreqArr[index].character = (char)i;
            charFreqArr[index].frequency = charCount[i];
            index++;
        }
    }

    return charFreqArr;
}

char* createKeyArray(CharFreq* charFreqArr, int size) {
    char* keyArray = malloc(sizeof(char)*size);
    for (int i = 0; i < size; i++) {
        keyArray[i] = charFreqArr[i].character;
    }
    return keyArray;
}

int* createFreqArray(CharFreq* charFreqArr, int size) {
    int* freqArray = malloc(sizeof(int)*size);
    for (int i = 0; i < size; i++) {
        freqArray[i] = charFreqArr[i].frequency;
    }
    return freqArray;
}

char* readFile(const char* filename) {
    
    FILE *file;
    file = fopen("input_file.txt", "r");

    // Check if the file was opened successfully
    if (file == NULL) {
        perror("Error opening the input file");
        return NULL;
    }

    // Calculate the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // Allocate memory for the string
    char* fileContent = malloc(fileSize + 1);

    if (fileContent == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    // Read the entire content of the file into the string
    size_t bytesRead = fread(fileContent, 1, fileSize, file);

    // if (bytesRead != fileSize) {
    //     fprintf(stderr, "File read error: %s\n", strerror(errno));
    //     free(fileContent);
    //     fclose(file);
    //     return NULL;
    // }
    fileContent[fileSize] = '\0';
    fclose(file);

    printf("File content:\n%s\n", fileContent);
    return fileContent;

}
char* createString(int arr[], int size) {
    char* result = (char*)malloc(size + 1); // +1 for the null-terminator
    if (result == NULL) {
        return NULL;
    }
    result[0] = '\0';

    // Concatenate integers to the result string
    for (int i = 0; i < size; i++) {
        sprintf(result, "%s%d", result, arr[i]);
    }
    return result;
}

void writeEncodedFile(CharCode* charCodeArr, const char* inputFile, const char* outputFile, int size) {
    FILE* input = fopen(inputFile, "r");
    if (input == NULL) {
        perror("Error opening input file");
        return;
    }

    // Open the output file for writing in binary mode
    FILE* output = fopen(outputFile, "wb");
    if (output == NULL) {
        perror("Error opening output file");
        fclose(input);
        return;
    }

    int ch;
    char* bigString = NULL;

    size_t bufferSize = 1;
    bigString = malloc(bufferSize);
    bigString[0] = '\0';

    while ((ch = fgetc(input)) != EOF) {
        char character = (char) ch;
        CharCode charCode;
        for (int i = 0; i < size; i++) {
            if (character == charCodeArr[i].character ) {
                charCode = charCodeArr[i];
                break;
            }
        }

        while (bufferSize < (strlen(bigString) + charCode.size + 1)) {
            bufferSize *= 2;
            bigString = realloc(bigString, bufferSize);
        }

        //strcat(bigString, "\0\0");
        strcat(bigString, createString(charCode.code, charCode.size));
    }
    printf("bigstring original %s \n", bigString);


    //pad the string to make it multiple of 8 in length
    int padLength = 0;
    if (strlen(bigString) % 8 != 0) {
        padLength = 8 - strlen(bigString) % 8;

        char padString[10]; 
        memset(padString, '0', padLength);
        padString[padLength] = '\0';

        char* temp = malloc(strlen(bigString) + padLength + 1);
        temp[0] = '\0';

        strcat(temp, padString);
        strcat(temp, bigString);
        
        bigString = realloc(temp, strlen(temp));

        //printf("tempp: %s", bigString);
    }
    printf("new big string:%s", bigString);

    //for the worst case allocate 9*size + 1
    //write the codes in the file with a NULL character in between
    for (int i = 0; i < size; i++) {
        char* temp = malloc(2);

        char* codes = malloc(20); 
        codes[0] = '\0';

        temp[0] = charCodeArr[i].character;
        temp[1] = '\0';
        strcat(codes, temp);
        strcat(codes, createString(charCodeArr[i].code, charCodeArr[i].size));

        //write to the file
        fwrite(codes, strlen(codes) + 1, 1, output);
        free (temp);
        free(codes);
        printf(" CODESSS : %s", codes);
    }

    //write padding info with 2NULLs on each side
    //(helps in decoding)
    char pad = padLength + '0';
    char null = '\0';
    char padArr[2];
    padArr[0] = pad;
    padArr[1] = null;
    fwrite(&null, 1, 1, output);
    fwrite(&padArr, 1, 1, output);
    fwrite(&null, 1, 1, output);
    fwrite(&null, 1, 1, output);


    //write the bigString(inputfile) in binary form
    char tempBin[9];
    int dec, w;
    char buff[1];
    for (int j = 0; bigString[j]; j = j + 8)
    {
        strncpy(tempBin, &bigString[j], 8);
        // Convert to integer equivalent and then character
        dec = 0;
        int i = 7;
        while (i > -1)
        {
            w = tempBin[i] == '1' ? 1 : 0;
            dec += (1 << (7 - i)) * w;
            i--;
        }
        buff[0] = dec;
        fwrite(buff, 1, 1, output);
    }
    // Close the files
    printf("\nEEEEEEEE file compresssed YEEEEEE");
    fclose(input);
    fclose(output);
}


int main() {

    int size;  
    const char* inputFile = "input_file.txt";
    const char* outputFile = "output_file.bin";

    char* fileContent = readFile(inputFile);

    if (fileContent == NULL) {
        perror("Error opening the file");
        return 1;
    }

    CharFreq* charFreqArr = createCharFreq(fileContent, &size);
    
    char* arr = createKeyArray(charFreqArr, size);
    int* freq = createFreqArray(charFreqArr, size);

    CharCode* charCodeArr;
    charCodeArr = huffmanCodes(arr, freq, size);

    for(int i = 0; i < size; i++) {
        printf("%c: ", charCodeArr[i].character);
        printArray(charCodeArr[i].code, charCodeArr[i].size);
    }
    writeEncodedFile(charCodeArr, inputFile, outputFile, size);


    free(arr);
    free(freq);
    free(charFreqArr);
    free(charCodeArr);
    free(fileContent);
    return 0;
}