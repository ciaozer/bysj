#include<iostream>
#include<io.h>

using namespace std;

//the file has to satisfy some rules
const char *to_search = "init_data/*.txt";
//const char *to_search = "D:/study/code/C++/bysj/init_data/*.txt";

/*
struct _finddata_t
{
    //文件属性的存储位置。它存储一个unsigned单元，用于表示文件的属性。
    //文件属性是用位表示的，主要有以下一些：
    //_A_ARCH（存档）、_A_HIDDEN（隐藏）、_A_NORMAL（正常）、
    //_A_RDONLY（只读）、_A_SUBDIR（文件夹）、_A_SYSTEM（系统）
    unsigned attrib;
    time_t time_create;//用来存储文件创建时间
    time_t time_access;//文件的最后一次被访问的时间
    time_t time_write;//文件最后一次被修改的时间
    _fsize_t size;
    char name[_MAX_FNAME];//文件的文件名
};
*/

void find_filename(){
    long handle;
    struct _finddata_t fileinfo;
/*
函数_findfirst()

函数原型：long _findfirst( char *filespec, struct _finddata_t *fileinfo);
返回值：如果查找成功的话，将返回一个long型的唯一的查找用的句柄（就是一个唯一编号）。这个句柄将在_findnext函数中被使用。若失败，则返回-1。
参数：
    filespec: 文件的完整路径，可支持通配符
    fileinfo: 存放文件信息的结构体的指针。
              这个结构体必须在调用此函数前声明，不过不用初始化，只要分配了内存空间就可以了。
              函数成功后，函数会把找到的文件的信息放入这个结构体中。
*/
    handle = _findfirst(to_search, &fileinfo);
    if( -1 == handle ){
        cout << "can't find any file" << endl;
        return ;
    }
    cout << fileinfo.name << endl;

/*
函数_findnext()

函数原型： int _findnext( long handle, struct _finddata_t *fileinfo );

返回值：若成功返回0，否则返回-1。

参数：
    handle：由_findfirst函数返回回来的句柄。
    fileinfo：文件信息结构体的指针。找到文件后，函数将该文件信息放入此结构体中。
*/   
    while( !_findnext(handle, &fileinfo) )
        cout << fileinfo.name << endl;

/*
函数_findclose()

函数原型： int _findclose( long handle );
返回值：成功返回0，失败返回-1。
参数：handle ：_findfirst函数返回回来的句柄。 
*/
    _findclose(handle);
}