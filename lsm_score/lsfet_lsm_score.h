#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pwd.h>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <stdarg.h>
using namespace std;
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

static int muti_ans_array[MUTI_ANS_QUESTIONS_NUM][LSMs_NUM][MAX_OPS] = {
    // q1-Supports security models - DTE、FLASK、MLS、RBAC
    {{0, 0, 0, 0}, {0, 1, 1, 1}, {1, 0, 1, 0}, {0, 0, 0, 0}},
    // q2-Supports virtualization -libvirt、KVM、QEMU、Xen、sVirt、Docker
    {{1, 0, 0, 0, 1, 1}, {0, 1, 0, 1, 1, 1}, {0, 0, 0, 0, 0, 0}, {0, 1, 0, 0, 0, 0}},
    // q3-Security certifications - CAPP 、 EAL 、 LSPP
    {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}, {0, 0, 0}},
    // q4-Configuration type - ACL、C-like、CUI、DIT
    {{1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 1}, {0, 0, 1, 0}}

};

string beginning = "Hello! Welcome to use LSFET - Linux Security Framework Evaluation Tool\nWe will ask you some questions to help us init criteria, which will help you find the best LSM.\n";
string str_autopolicy = "Want LSM supports Automatic policy? [T/F]";
string caches_req = "Want LSM supports caches? [T/F]";
string str_log_support_policy = "Want LSM supports log edit policy? [T/F]";
string str_dynamically_load_policy = "Want LSM supports dynamically load policy? [T/F]";
string str_configurable_aduit = "Want LSM supports configurable audit? [T/F]";
string str_default_policy = "Want LSM has default policy? [T/F]";
string str_security_auditing = "Want LSM supports security auditing? [T/F]";
string str_support_tpe = "Want LSM supports trust path execution? [T/F]";
string str_whitelist = "Want LSM supports whitelist? [T/F]";
string str_file_system_support_xattr = "Does File System support extended attributes? [T/F]";

// complex questions string
string complex_rule = "Please key in your options continually then press Enter (e.g. if you want to select option 1, 3, 4, please key in 134 then press Enter\n)\n";
string str_support_security_models = "Which security models do you want? [1]DTE [2]FLASK [3]MLS [4]RBAC";
string str_support_virtualization = "Which virtualization technique do you want to support? [1]libvirt [2]KVM [3]QEMU [4]Xen [5]sVirt [6]Docker";
string str_fit_security_certifications = "What kind of certifications you want your LSM fit? [1]CAPP [2]EAL [3]LSPP";
string str_configuration_type = "What kind of configuration type you want to use? [1]ACL [2]C-like [3]CUI [4]DIT";

// environmental questions string
string does_insert_into_kerenl = "Checking LSMs hasn't been inserted into the kernel";
string does_distrib_support_lsm = "Checking your distribution Linux whether supports LSMs";
string better_lsm = "Your distribution does not support ";
string but_lsm = "But it can meet your needs better";
string change_dis = "So you can consider other distributions";

// init
void os_init();
void ques_init();
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