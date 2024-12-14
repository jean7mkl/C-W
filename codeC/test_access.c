#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    if (access(argv[1], F_OK) == 0) {
        printf("File %s exists and is accessible.\n", argv[1]);
    } else {
        perror("File access error");
    }

    return 0;
}
