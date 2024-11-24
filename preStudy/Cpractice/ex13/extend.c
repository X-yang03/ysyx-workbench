#include <stdio.h>

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("ERROR: You need one argument.\n");
        // this is how you abort a program
        return 1;
    }

    int i = 0;
    char letter;
    for(int j = 0; j < argc; j++){
    for(i = 0;letter = argv[j][i], argv[j][i] != '\0'; i++) {

        switch(letter) {
            case 'A':
            case 'a':
                printf("%d: 'a'\n", i);
                break;

            case 'E':
            case 'e':
                printf("%d: 'e'\n", i);
                break;

            case 'I':
            case 'i':
                printf("%d: 'i'\n", i);
                break;

            case 'O':
            case 'o':
                printf("%d: 'o'\n", i);
                break;

            case 'U':
            case 'u':
                printf("%d: 'u'\n", i);
                break;

            case 'Y':
            case 'y':
                if(i > 2) {
                    // it's only sometimes Y
                    printf("%d: 'y'\n", i);
                }
                break;

            default:
                printf("%d: %c is not a vowel\n", i, letter);
        }
    }

    } 

    return 0;
}
