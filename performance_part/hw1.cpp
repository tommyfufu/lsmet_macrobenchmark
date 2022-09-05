#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pwd.h>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <regex>
#define BUFSIZE 4096
using namespace std;
extern int errno;
enum types
{
    DIR_type,
    REG,
    CHR,
    FIFO,
    SOCK,
    unknown,
    EMPTY
};
vector<string> check_type_str = {"DIR", "REG", "CHR", "FIFO", "SOCK", "unknown", ""};

struct filter_struct
{
    string cmd;
    types type;
    string name;
};
filter_struct filter;
struct info_struct
{
    string cmd;
    unsigned int pid;
    string user;
    string fd;
    types type;
    unsigned long node;
    string name;
};
vector<info_struct> showlist;

/*****functions*****/
void visit_dir(char *dir);
bool is_pid_digital(char *d_name);
void proc_info(struct dirent *dir_entry);
string get_proc_cmd(char *proc_id);
void clean_char_array(char *chArrPtr);
string read_file(char *path);
string get_proc_user(char *proc_id);
void get_proc_info(char *pid, char *fd_type, info_struct proc_info);
bool link_can_read(char *link, string &name);
void set_proc_type_and_node(char *path, types &type, unsigned long &node);
void add_proc_info_in_showlist(info_struct &proc_info);
void get_proc_fd(char *pid, char *fd_type, info_struct tmp);
void set_fd(char *fdinfo_path, string &fd, char *fd_no);
void get_proc_maps(char *pid, char *fd_type, info_struct tmp);
void print_showlist();
/*****main*****/
int main(int argc, char *argv[])
{
    filter.cmd = "";
    filter.name = "";
    filter.type = EMPTY;
    for (int i = 1; i < argc; i += 2)
    {
        if (!strcmp(argv[i], "-c"))
            filter.cmd = string(argv[i + 1]);
        if (!strcmp(argv[i], "-f"))
            filter.name = string(argv[i + 1]);
        if (!strcmp(argv[i], "-t"))
        {
            for (int j = 0; j < check_type_str.size(); j++)
                if (check_type_str[j] == string(argv[i + 1]))
                    filter.type = (types)j;
            if (filter.type == EMPTY)
            {
                cout << "Invalid TYPE option." << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    visit_dir("/proc/");
    print_showlist();
    return 0;
}
void visit_dir(char *dir)
{
    DIR *dp;
    struct dirent *dir_entry;
    if ((dp = opendir(dir)) == NULL)
    {
        fprintf(stderr, "Can't open directory %s\n", dir);
        exit(EXIT_FAILURE);
    }
    while ((dir_entry = readdir(dp)) != NULL)
    {
        if (is_pid_digital(dir_entry->d_name)) // choose right dir
            proc_info(dir_entry);
    }

    closedir(dp);
}
bool is_pid_digital(char *d_name)
{
    for (int i = 0; i < strlen(d_name); i++)
        if (d_name[i] < '0' || d_name[i] > '9')
            return false;

    return true;
}
void proc_info(struct dirent *dir_entry) // ready to catch info from dir
{
    string cmd, user;
    unsigned int pid;
    pid = atoi(dir_entry->d_name);
    cmd = get_proc_cmd(dir_entry->d_name);
    user = get_proc_user(dir_entry->d_name);
    info_struct info;
    info.cmd = cmd;
    info.pid = pid;
    info.user = user;
    info.fd = "";
    info.type = EMPTY;
    info.node = 0;
    info.name = "";

    get_proc_info(dir_entry->d_name, "cwd", info);
    get_proc_info(dir_entry->d_name, "root", info);
    get_proc_info(dir_entry->d_name, "exe", info);
    get_proc_fd(dir_entry->d_name, "fd", info);
    get_proc_maps(dir_entry->d_name, "maps", info);
}
string get_proc_cmd(char *proc_id) // catch cmd
{
    char path[64] = {'\0'};
    sprintf(path, "/proc/%s/comm", proc_id);
    string cmd = read_file(path);
    regex reg("\\S+");
    smatch match;
    if (regex_search(cmd, match, reg))
        return match[0];

    else
        cout << "Err: NOT find cmd: " << proc_id << endl;
}
string read_file(char *path)
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
void clean_char_array(char *chArrPtr)
{
    for (long unsigned int i = 0; i < sizeof(chArrPtr); i++)
        chArrPtr[i] = '\0';
}
string get_proc_user(char *proc_id) // catch user
{
    char path[64] = {'\0'};
    sprintf(path, "/proc/%s/status", proc_id);
    string status = read_file(path);
    regex reg("(Uid:\\s*)(\\d+)");
    smatch match;
    if (regex_search(status, match, reg))
        ;
    else
        cout << "Err: NOT find uid: " << proc_id << endl;

    struct passwd pw;
    struct passwd *result;
    char buf[1024] = "\0";

    if (getpwuid_r(stoi(match[2]), &pw, buf, sizeof(buf), &result))
    {
        perror("getpwuid_r: not found pw and uid match");
    }
    else
    {
        string name(result->pw_name);
        return name;
    }
}
void get_proc_info(char *pid, char *fd_type, info_struct proc_info)
{
    char *path = new char[100]{'\0'};
    sprintf(path, "/proc/%s/%s", pid, fd_type);
    proc_info.fd = string(fd_type);

    if (link_can_read(path, proc_info.name))
        set_proc_type_and_node(path, proc_info.type, proc_info.node);
    else
    {
        proc_info.node = 0;
        proc_info.type = unknown;
    }
    add_proc_info_in_showlist(proc_info);
}
bool link_can_read(char *link, string &proc_name)
{
    char real_name[4096] = "\0";
    ssize_t len = readlink(link, real_name, sizeof(real_name) - 1);
    if (len == -1 && errno == EACCES)
    {
        proc_name = string(link) + " (Permission denied)";
        return false;
    }
    else
    {
        proc_name = string(real_name);
        return true;
    }
}
void set_proc_type_and_node(char *path, types &type, unsigned long &node)
{
    struct stat sb;
    if (stat(path, &sb) == -1)
        type = unknown;
    else
    {
        node = sb.st_ino;
        if (S_ISDIR(sb.st_mode))
            type = DIR_type;
        else if (S_ISREG(sb.st_mode))
            type = REG;
        else if (S_ISCHR(sb.st_mode))
            type = CHR;
        else if (S_ISFIFO(sb.st_mode))
            type = FIFO;
        else if (S_ISSOCK(sb.st_mode))
            type = SOCK;
        else
            type = unknown;
    }
}
void add_proc_info_in_showlist(info_struct &proc_info)
{
    if (proc_info.name.find("(deleted)") != string::npos)
    {
        proc_info.type = unknown;
    }
    if (filter.cmd != "")
        if (proc_info.cmd.find(filter.cmd) == string::npos)
            return;
    if (filter.type != EMPTY)
        if (proc_info.type != filter.type)
            return;
    if (filter.name != "")
    {
        regex reg("(.*)( \\(.*)|(.*\\S)");

        regex f(filter.name);
        smatch match;
        if (regex_search(proc_info.name, match, reg))
        {
            string name = "";
            if (match[1] != "")
                name = match[1];
            else
                name = match[3];

            if (name.find(filter.name) == string::npos)
                return;
        }
    }
    showlist.push_back(proc_info);
}
void get_proc_fd(char *pid, char *fd_type, info_struct tmp)
{
    char path[64] = {'\0'};
    sprintf(path, "/proc/%s/fd", pid);
    DIR *dp = opendir(path);
    if (dp == NULL || errno == EACCES)
    {
        tmp.fd = "NOFD";
        tmp.type = EMPTY;
        tmp.name = string(path) + " (opendir: Permission denied)";
        tmp.node = 0;
        add_proc_info_in_showlist(tmp);
        return;
    }

    struct dirent *ent = NULL;
    while ((ent = readdir(dp)) != NULL)
    {
        if (is_pid_digital(ent->d_name))
        {
            char fd_path[4096] = {'\0'};
            char fdinfo_path[4096] = {'\0'};
            sprintf(fd_path, "%s/%s", path, ent->d_name);         //  /proc/[pid]/fd/[fd]
            sprintf(fdinfo_path, "%sinfo/%s", path, ent->d_name); //  /proc/[pid]/fdinfo/[fd]
            if (link_can_read(fd_path, tmp.name))
            {
                set_proc_type_and_node(fd_path, tmp.type, tmp.node);
                set_fd(fdinfo_path, tmp.fd, ent->d_name);
                add_proc_info_in_showlist(tmp);
            }
            else
            {
                perror("fd readlink");
            }
        }
    }
    closedir(dp);
}
void set_fd(char *fdinfo_path, string &fd, char *fd_no)
{
    string fdinfo = read_file(fdinfo_path);
    regex reg("(flags:\\s*)(\\d*)(\\d{2})");
    smatch match;
    /*  O_RDONLY    00
        O_WRONLY    01
        O_RDWR      02  */
    if (regex_search(fdinfo, match, reg))
    {
        if (match[3] == "00")
            fd = "r" + string(fd_no);
        else if (match[3] == "01")
            fd = "w" + string(fd_no);
        else if (match[3] == "02")
            fd = "u" + string(fd_no);
    }
}
void get_proc_maps(char *pid, char *fd_type, info_struct tmp)
{
    char path[64] = {'\0'};
    sprintf(path, "/proc/%s/maps", pid);
    vector<string> maps;
    ifstream ifs(path, ios::in);

    if (!ifs.is_open() && errno == EACCES)
        return;

    string s = "";
    while (getline(ifs, s))
        maps.push_back(s);

    ifs.close();

    regex reg("(\\d+)(\\s*)(\\/\\S+)");
    smatch match;
    for (int i = 0; i < maps.size(); i++)
    {
        if (regex_search(maps[i], match, reg))
        {
            if (showlist.empty() || (showlist.back().node != stoul(match[1])))
            {
                tmp.node = stoul(match[1]);
                tmp.name = match[3];
                tmp.type = REG;
                tmp.fd = "mem";
                add_proc_info_in_showlist(tmp);
            }
        }
    }
}
void print_showlist()
{
    printf("%-40s%-10s%-15s%-10s%-10s%-15s%s\n", "COMMAND", "PID", "USER", "FD", "TYPE", "NODE", "NAME");
    for (int i = 0; i < showlist.size(); i++)
    {
        if (showlist[i].node == 0)
            printf("%-40s%-10u%-15s%-10s%-10s%-15s%s\n",
                   showlist[i].cmd.c_str(), showlist[i].pid, showlist[i].user.c_str(), showlist[i].fd.c_str(), check_type_str[showlist[i].type].c_str(), "", showlist[i].name.c_str());

        else
            printf("%-40s%-10u%-15s%-10s%-10s%-15lu%s\n",
                   showlist[i].cmd.c_str(), showlist[i].pid, showlist[i].user.c_str(), showlist[i].fd.c_str(), check_type_str[showlist[i].type].c_str(), showlist[i].node, showlist[i].name.c_str());
    }
}
