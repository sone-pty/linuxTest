#include "IOHeader.h"

static int count = -1;

void print_file_info(const char* name, int len)
{
    struct stat st;
    int res = stat(name, &st);
    if(res == -1)
    {
        perror("error:");
        exit(1);
    }
    printf("File name:%s,File size:%d\n",name,st.st_size);
}

void deep(const char* path)
{
    count++;
    //open dir
    DIR* pDir = opendir(path);
    struct dirent* pDirent;
    unsigned char type;
    char* name;
    int len;
    ino_t inode;
    char newpath[1024]={0};
    //enumerate cur path
    while((pDirent = readdir(pDir)) != 0)
    {
        //get data from dirent
        type = pDirent->d_type;
        name = pDirent->d_name;
        inode = pDirent->d_ino;
        len = strlen(pDirent->d_name);

        //skip . and ..
        if(strcmp(".",name) == 0 || strcmp("..",name) == 0)
            continue;

        //if it is dir,deep enumerate
        if(type == DT_DIR)
        {
            memset(newpath,0,sizeof(newpath));
            strcat(newpath,path);
            strcat(newpath,"/");
            strcat(newpath,name);
            for(int i = 0; i < count; i++)
                printf("-");
            printf("Dir:%s\n",newpath);
            deep(newpath);
            count--;
        }
        else
        {
            memset(newpath,0,sizeof(newpath));
            strcat(newpath,path);
            strcat(newpath,"/");
            strcat(newpath,name);
            for(int i = 0; i < count; i++)
                printf("-");
            print_file_info(newpath, len);
        }
    }
}

int main(int argc, char const *argv[])
{
    if(argc < 2)
        deep(".");
    else
        deep(argv[1]);
    return 0;
}
