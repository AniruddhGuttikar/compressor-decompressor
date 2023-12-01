#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct CharCode {
    char character;
    char* code;
    int size;
} CharCode;

char* createString(char arr[], int size) {
    char* result = malloc(size + 1); // +1 for the null-terminator
    if (result == NULL) {
        return NULL;
    }
    result[0] = '\0';

    // Concatenate integers to the result string
    for (int i = 0; i < size; i++) {
        sprintf(result, "%s%c", result, arr[i]);
    }
    return result;
}

char* dectobin(int decimal) {
    char* binary = malloc(9);
    if (binary == NULL) {
        perror("failed to allocate memory for binary");
        return NULL;
    }

    for (int i = 7; i >= 0; i--) {
        if (decimal & (1 << i)) {
            binary[7 - i] = '1';
        } else {
            binary[7 - i] = '0';
        }
    }
    binary[8] = '\0';
    return binary;
}

char isMatching(CharCode* charCodeArr, const char* str, int size) {
    for (int i = 0; i < size-1; i++) {
        if (strcmp(charCodeArr[i].code, str) == 0) {
            //printf(" %s and and %s ", charCodeArr[i].code, str);
            return charCodeArr[i].character;
        }
    }
    return '\0';
}

void printCharCodeArr(CharCode* charCodeArr, int size) {
    for(int i = 0; i < size-1; i++) {
        printf("char: %c\t", charCodeArr[i].character);
        printf("code: %s\t", charCodeArr[i].code);
        printf("size: %d\n", charCodeArr[i].size);
    }
}

int main() {
    FILE* input = fopen("output_file.bin", "rb");

    if(input == NULL) {
        perror("Error opening the compressed binary file file\n");
        return 1;
    }


    FILE *output = fopen("file-decompressed.txt", "w");
    if(output == NULL) {
        perror("Error creating output file\n");
        return 1;
    }

    char buff[1];
    int flag = 0;
    //find the number of unique characters to allocate memory
    int numOfUniqueChars = 0;
    while (fread(buff, 1, 1, input)) {
        if (buff[0] == '\0') {
            flag++;
            numOfUniqueChars++;
            if (flag == 2) {
                break;
            }
        } else {
            flag = 0;
        }
    }
    printf("number of unique characters %d\n", numOfUniqueChars);
    rewind(input);

    //read the codes and store in the charCodeArr
    CharCode* charCodeArr = malloc(sizeof(CharCode)*numOfUniqueChars);
    flag = 0;
    char* str = malloc(20);
    str[0] = '\0';
    int i = 0;
    while (fread(buff, 1, 1, input)) {
        if (buff[0] != '\0') {
            flag = 0;
            char temp[2];
            temp[0] = buff[0];
            temp[1] = '\0';
            strcat(str, temp);
        } else {
            flag++;
            if (flag == 2) {
                free(str);
                break;
            }
            printf("%s\n", str);
            charCodeArr[i].character =  str[0];
            charCodeArr[i].code = createString(str+1, strlen(str));
            charCodeArr[i].size = strlen(charCodeArr[i].code);
            i++;
            str[0]='\0';
        }
    }

    printCharCodeArr(charCodeArr, numOfUniqueChars);
    

    //read and ignore the padding
    fread(buff, 1, 1, input);
    int padding = buff[0] - '0';
    fread(buff, 1, 1, input);
    fread(buff, 1, 1, input);

    printf("padding: %d\n", padding);

    //read the rest and store it as a string of binary characters
    //actual binary code starts from here

    int decimal;
    int binaryStringCapacity = 50;
    int binaryStringLength = 0; //initialize to a random size
    char* binaryString = malloc(binaryStringCapacity);
    binaryString[0] = '\0';
    char* tempBinary;
    
    while (fread(buff, 1, 1, input)) {
        decimal = buff[0];
        tempBinary = dectobin(decimal);
        int tempBinaryLength = strlen(tempBinary);

        if (binaryStringLength + tempBinaryLength + 1 > binaryStringCapacity) {
            binaryStringCapacity *= 2;
            binaryString = realloc(binaryString, binaryStringCapacity);
            if (binaryString == NULL) {
                perror("memory allocation failed for binaryString");
                free(tempBinary);
                break;
            }
        }
            memcpy(binaryString + binaryStringLength, tempBinary, tempBinaryLength);
            binaryStringLength += tempBinaryLength;
            free(tempBinary);
            //binaryStringLength = strlen(binaryString);
            binaryString[binaryStringLength] = '\0';
    }

    printf("%s\n", binaryString);

    printf("formed the string\n");


    int start;

    if (padding == 0) {
        memmove(binaryString + 1, binaryString, strlen(binaryString) + 1);
        binaryString[0] = '0';
        start = 1;
    } else {
        start = padding;
    }
    printf("start: %d\n", start);
    printf("string: %s\n", binaryString);
    printf("capacity: %d and size: %d\n", binaryStringCapacity, strlen(binaryString));

    while (binaryString[start] != '\0') {
        char matchingChar;
        
        for (int i = 1; binaryString[start + i -1] != '\0'; i++) {
            char substring[32]; 

            strncpy(substring, binaryString + start, i);
            substring[i] = '\0';
            //printf("substring: %s size: %d", substring, strlen(substring));
            matchingChar = isMatching(charCodeArr, substring, numOfUniqueChars);

            if (matchingChar != '\0') {
                printf("current character: %c\n", matchingChar);
                fwrite(&matchingChar, 1, 1, output);
                start += i;
                i = 0;
            }
        }
    }

    free(charCodeArr);
    fclose(input);
    fclose(output);
    return 0;
}