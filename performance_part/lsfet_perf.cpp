#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pwd.h>
#include <string>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <regex>
#include <sys/utsname.h>
#define BUFSIZE 4096
using namespace std;

const char *distrib_info_path = "/etc/os-release";
const char *lsm_info_path = "/sys/kernel/security/lsm";

struct sys_info
{
    string distrib;
    string lsm;
};

void clean_char_array(char *chArrPtr)
{
    for (long unsigned int i = 0; i < sizeof(chArrPtr); i++)
        chArrPtr[i] = '\0';
}
string read_file(const char *path)
{
    ifstream ifs;
    char *content = new char[BUFSIZE];
    clean_char_array(content);
    ifs.open(path);
    if (!ifs.is_open())
    {
        fprintf(stderr, "open failure: %s", path);
        perror("");
    }
    else
    {
        ifs.read(content, BUFSIZE);
        ifs.close();
    }
    string str(content);

    return str;
}
string get_distrbi_info()
{
    string distrib = read_file(distrib_info_path);
    regex reg("(NAME=)+(\")+([a-zA-Z]+)+(\")");
    smatch m;
    ssub_match sm;
    if (regex_search(distrib, m, reg))
        ;
    else
        cout << "Err: NOT find Distrib: " << endl;
    //cout << "regex_search" << endl;
    string str(m[3]);
    return str;
}
string get_lsm()
{
    string lsm = read_file(lsm_info_path);
    return lsm;
}
int main()
{

    struct utsname myutsname;
    struct sys_info my_sys_info;

    my_sys_info.distrib = get_distrbi_info();
    //cout << my_sys_info.distrib << endl;

    if(my_sys_info.distrib == "Ubuntu")
    {
        my_sys_info.lsm = get_lsm();

    }
    else if(my_sys_info.distrib == "CentOS")
    {
        

    }
    else
    {

    }
    cout << my_sys_info.distrib << "with Linux Security Module: " << my_sys_info.lsm << endl;
    return 0;
}
