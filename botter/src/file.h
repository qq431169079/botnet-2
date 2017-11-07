#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED
#   define EXT ".ter"
    int initFile(FILE** file, const char* dest);
    int writeFile(char* dest,char* data,unsigned int size);
    int startBlock(FILE* file, char *url);
    int addData(FILE* file, char *key, char *value);
    int endBlock(FILE* file);
    unsigned int getFileContent(char* ip,char** content);
#endif // FILE_H_INCLUDED*/

/* FILE FORMAT : (la lecture se réfère aux lignes)
 *
 * i=XXX.XXX.XXX.XXX;u=www.test.io;d=[key=value;key=value;key=value;]
 *
 *# i = ip
 *# u = url
 *# d = data
 **************/
