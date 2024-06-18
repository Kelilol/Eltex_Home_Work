#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#define _BSD_SOURCE
char* get_file_type(char* file_path) {
    char* file_type = malloc(16); // allocate a larger buffer for file_type
    struct stat file_stat;
    stat(file_path, &file_stat);

    if (S_ISREG(file_stat.st_mode)) {
        strcpy(file_type, "Regular");
    } else if (S_ISDIR(file_stat.st_mode)) {
        strcpy(file_type, "Directory");
    } else if (S_ISCHR(file_stat.st_mode)) {
        strcpy(file_type, "Character");
    } else if (S_ISBLK(file_stat.st_mode)) {
        strcpy(file_type, "Block");
    } else if (S_ISFIFO(file_stat.st_mode)) {
        strcpy(file_type, "FIFO");
    } else if (S_ISLNK(file_stat.st_mode)) {
        strcpy(file_type, "Symbolic Link");
    } else if (S_ISSOCK(file_stat.st_mode)) {
        strcpy(file_type, "Socket");
    } else {
        strcpy(file_type, "Unknown");
    }

    return file_type;
}

void view_directory(char* directory_path) {
    DIR* directory = opendir(directory_path);

    if (directory == NULL) {
        printf("Error: Unable to open directory %s\n", directory_path);
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s/%s", directory_path, entry->d_name);

        char* file_type = get_file_type(file_path);
        printf("%s - %s\n", entry->d_name, file_type);
        free(file_type);
    }

    closedir(directory);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    view_directory(argv[1]);

    return 0;
}
