#include<iostream>
#include<dirent.h>
#include<sys/types.h>
#include<vector>

using namespace std;

/*
struct dirent {   
    ino_t  d_ino; //表示存放的该文件的结点数目  
    off_t  d_off; //表示文件在目录中的偏移   
    unsigned short d_reclen; //length of this record    
    unsigned char  d_type; //表示文件类型    
    char d_name[256]; //表示文件名字
};
*/

vector<string> find_filename_linux(const char* path){
    vector<string> result;
    DIR *dirp = NULL;
    struct dirent *dir_entry = NULL;
    char namebuf[100] = {0};

/*
函数opendir()
函数原型：DIR *opendir(const char *path);
函数返回：   DIR*，表示打开的目录句柄；
            NULL，表示打开目录失败。
*/
    if( (dirp = opendir(path)) == NULL ){
        cout << "open dir " << path << " fail" << endl;
        return result;
    }

/*
函数readdir()
函数原型：struct dirent *readdir(DIR *dirp);
函数返回结构体dirent指针。
若返回值为NULL，则获取目录内容失败或目录内容已经全部读取完成。
*/

    while( (dir_entry = readdir(dirp)) != NULL ){
        //remove the . ..
        if( dir_entry->d_name[0] != '.' ){
            result.push_back(dir_entry->d_name);
        }
    }

/*
函数closedir()
函数原型：int closedir(DIR *dirp);
返回值：0正常 -1错误
*/

    closedir(dirp);

    return result;
}