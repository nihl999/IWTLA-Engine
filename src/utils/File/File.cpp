#include <cstdio>
#include <string.h>
#include <stdlib.h>
#include "File.h"

namespace FileUtils
{
    char *getResourceFolderPath()
    {
        return "../resources/";
    };
    // todo Erro logging
    char *readFile(const char *filename)
    {
        FILE *file;
        long int fileSize = 0;
        char *filepath = (char *)calloc(1, 128 * sizeof(char));
        strcat(filepath, FileUtils::getResourceFolderPath());
        printf("file: %s\n", filename);
        strcat(filepath, filename);
        file = fopen(filepath, "rb");
        if (file == NULL)
        {
            printf("file: %s on %s don't exist!", filename, filepath);
            exit(1);
        }
        fseek(file, 0, SEEK_END);
        fileSize = ftell(file);
        rewind(file);
        char *fileContent = (char *)calloc(1, (fileSize * sizeof(char)) + 1);
        if (fileContent == NULL)
        {
            printf("memory allocation for file failed");
            exit(1);
        }
        int freadResult = fread(fileContent, fileSize, 1, file);
        if (freadResult != 1)
        {
            if (ferror(file))
            {
                int error = ferror(file);
                printf("error id: %d, error: %s\n", error, strerror(errno));
            }
            if (feof(file))
            {
                int error = feof(file);
                printf("eof: %d\n", error);
            }
            fclose(file);
            free(fileContent);
            printf("failed reading file content");
            exit(1);
        }
        fclose(file);
        return fileContent;
    }
}