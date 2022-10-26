#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <pwd.h>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <stdarg.h>
#include <iosfwd>
#include <iomanip>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;
#define QUES_CLASS 4
#define LSMs_NUM 4
#define MAX_OPS 10
#define MUTI_ANS_QUESTIONS_NUM 4
enum
{
    num_apparmor = 0,
    num_selinux,
    num_smack,
    num_tomoyo,
};

enum
{
    num_default_os = 0,
    num_alpine,
    num_centos,
    num_opensuse,
    num_ubuntu,
};

enum
{
    tr_management = 0,
    tr_security,
    co_management,
    co_security
};

typedef struct distrib_struct
{
    string distribution;
    string lsm;
    string kernel;
    string de_fs;
    int distrib_kernel_lsm[4];
} dis_struct;

typedef struct questions_struct
{
    string questions = "";
    int weight[4];
} ques_struct;

struct scoreboard_col
{
    char *lsm_name;
    int score;
};

struct score_config
{
    int tr_m;
    int tr_s;
    int co_m;
    int co_s;
    int en_m;
    int en_un;
};

static int muti_ans_array[MUTI_ANS_QUESTIONS_NUM][LSMs_NUM][MAX_OPS] = {
    // q1-Supports security models - DTE、FLASK、MLS、RBAC
    {{0, 0, 0, 0}, {0, 1, 1, 1}, {1, 0, 1, 0}, {0, 0, 0, 0}},
    // q2-Supports virtualization -Docker、KVM、libvirt、QEMU、sVirt、Xen
    {{1, 1, 1, 1, 0, 0}, {0, 1, 1, 1, 1, 1}, {0, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}},
};

// init and declare
string beginning = "Hello! Welcome to use LSFET - Linux Security Framework Evaluation Tool\nWe will ask you some questions to help us init criteria, which will help you find the most suitable LSM.\n";
string str_apparmor = "AppArmor";
string str_selinux = "SELinux";
string str_smack = "SMACK";
string str_tomoyo = "TOMOYO Linux";
// class 1 -trivial questions string
string trviial_rule = "Please key in [T/t]ure or [F/f]alse then press enter";
string str_autopolicy = "Want LSM to support Automatically generate policies? [T/F]";
string caches_req = "Want LSM to support caches? [T/F]";
string str_log_support_policy = "Want LSM to support fine tuning policy based on log? [T/F]";
string str_dynamically_load_policy = "Want LSM to be able to support dynamically load policy? [T/F]";
string str_configurable_aduit = "Want LSM to supports configurable audit? [T/F]";
string str_default_policy = "Want LSM has default policy? [T/F]";
string str_security_auditing = "Want LSM to support security auditing? [T/F]";
string str_support_tpe = "Want LSM to support trust path execution? [T/F]";
string str_whitelist = "Want LSM to support whitelist? [T/F]";
string str_file_system_support_xattr = "Does File System support extended attributes? [T/F]";
string str_sandbox = "Want LSM to support sandbox? [T/F]";
string str_fit_security_certifications = "Want LSM to have security certifications? [T/F]";
string str_mem_protect = "Want LSM to support memory protection? [T/F]";

// class 2 - complex questions string
string complex_rule = "Please key in your options continually then press Enter (e.g. if you want to select option 1, 3, 4, please key in 134 then press Enter)";
string str_support_security_models = "Which security models do you want? [0]none [1]DTE [2]FLASK [3]MLS [4]RBAC";
string str_support_virtualization = "Which virtualization technique do you want to support? [0]none [1]Docker [2]KVM [3]libvirt [4]QEMU [5]sVirt [6]Xen";
// class 3 - environmental questions string
string does_insert_into_kerenl = "Checking LSMs hasn't been inserted into the kernel";
string does_distrib_support_lsm = "Checking your distribution Linux whether supports LSMs";
string better_lsm = "Your distribution does not support ";
string but_lsm = "But it can meet your needs better";
string change_dis = "So you can consider other distributions";

// init
void os_init();
void ques_init();
void weight_init();
void print_lsm_scoreboard();
void push_os_list(string distrib, string lsm, string de_fs, int app, int se, int sm, int tomo);
void push_trivial_quest_list(string question, int app, int se, int sm, int tomo);

//  Environmental request
int envir_req();
bool erq1();
bool erq2();
// Trivial questions
int trivial_q();
void tq_manage_plus1();
bool tq_security_plus2();
bool tq_lsm_request_minus1();
// Complex questions
int complex_q();
// Generate report
int find_the_most_suitable_lsm();
int gen_lsfet_report(string distrb_name, int dis_lsm_num);
