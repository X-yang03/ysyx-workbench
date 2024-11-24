#include <stdio.h>

int main(int argc, char *argv[])
{
    // go through each string in argv

    int i = 0;
    while(i < argc) {
        printf("arg %d: %s\n", i, argv[i]);
        i++;
    }

    // let's make our own array of strings
    char *states[] = {
        "California", "Oregon",
        "Washington", "Texas"
    };

    int num_states = 4;
    i = num_states - 1;  // watch for this
    while(i >= 0) {
        printf("before state %d: %s\n", i, states[i]);
        states[i] = argv[i];
        printf("after state %d: %s\n", i, states[i]);
        printf("addr of state %d: %p\n", i, &states[i]);
        printf("addr of argv %d: %p\n", i, &argv[i]);
        i--;
    }

    return 0;
}
