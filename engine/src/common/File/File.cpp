#include "File.h"
#include <core/logger.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace FileUtils
{
    std::string getResourceFolderPath()
    {
        return "../resources/";
    };
    // todo Learn Streams, can be cool
    char *readFile(const char *filename)
    {
        FILE *file;
        long int fileSize = 0;
        char *filepath = (char *)calloc(1, 128 * sizeof(char));
        strcat(filepath, FileUtils::getResourceFolderPath().c_str());
        OUROTRACE("file: {}", filename);
        strcat(filepath, filename);
        file = fopen(filepath, "rb");
        if (file == NULL)
        {
          OUROERROR("file: {} on {} don't exist!", filename, filepath);
          exit(1);
        }
        fseek(file, 0, SEEK_END);
        fileSize = ftell(file);
        rewind(file);
        char *fileContent = (char *)calloc(1, (fileSize * sizeof(char)) + 1);
        if (fileContent == NULL)
        {
          OUROERROR("memory allocation for file failed");
          exit(1);
        }
        int freadResult = fread(fileContent, fileSize, 1, file);
        if (freadResult != 1)
        {
            if (ferror(file))
            {
                int error = ferror(file);
                OUROERROR("error id: {:d}, error: {}", error, strerror(errno));
            }
            if (feof(file))
            {
                int error = feof(file);
                OUROERROR("eof: {:d}", error);
            }
            fclose(file);
            free(fileContent);
            OUROFATAL("failed reading file content");
            exit(1);
        }
        fclose(file);
        return fileContent;
    }
}