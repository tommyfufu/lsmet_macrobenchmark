#include "lsfet_lsm_score.h"

vector<dis_struct> os_list;
// vector<ques_struct> tf_questions_list;
scoreboard_col score_lsm_array[4];
dis_struct *ur_os, alpine, centos, ubuntu, opensuse, default_os;
string distrib = "", lsm = "";
class questions
{
public:
    string str_question;
    int ans_input_count = num_apparmor;
    questions(string str_qu, int app, int se, int sm, int tomo);
    questions(string str_qu, int ops_num, int sequence);
    void muti_user_input_then_adjust_scoreboard();
    int tf_user_input_then_adjust_scoreboard(int seq);

private:
    int options_num, sequence_in_ques_list;
    int ans[4][MAX_OPS];
};
questions::questions(string str_qu, int app, int se, int sm, int tomo)
{
    str_question = str_qu;
    cout << "Input Questions: " << str_question << endl;
    ans[num_apparmor][0] = app;
    ans[num_selinux][0] = se;
    ans[num_smack][0] = sm;
    ans[num_tomoyo][0] = tomo;
}
questions::questions(string str_qu, int ops_num, int sequence)
{
    str_question = str_qu;
    cout << "Input Questions: " << str_question << endl;
    options_num = ops_num;
    sequence_in_ques_list = sequence - 1;
    for (int i = num_apparmor; i <= num_tomoyo; i++)
    {
        for (int j = 0; j < options_num; j++)
        {
            ans[i][j] = muti_ans_array[sequence_in_ques_list][i][j];
        }
    }
}
int questions::tf_user_input_then_adjust_scoreboard(int seq)
{

    char input = '\0';
    cout << seq << ". " << str_question << endl;
    cin >> input;
    cout << "Your input: " << input << endl
         << endl;
    if (input == 'T' || input == 't')
    {
        score_lsm_array[num_apparmor].score += ans[num_apparmor][0];
        score_lsm_array[num_selinux].score += ans[num_selinux][0];
        score_lsm_array[num_smack].score += ans[num_smack][0];
        score_lsm_array[num_tomoyo].score += ans[num_tomoyo][0];
        print_lsm_scoreboard();
        cout << endl;
    }
    else if (input == 'f' || input == 'F')
    {
        print_lsm_scoreboard();
        cout << endl;
    }
    else
    {
        cout << "Wrong intput! Please input [Y/y | N/n]" << endl;
        return 1;
    }
    return 0;
}
void questions::muti_user_input_then_adjust_scoreboard()
{
    string in = "0";
    int a;
    cin >> in;
    int j;

    while (in == " ")
        cin >> in;

    for (j = 0; j < in.size(); j++)
    {
        a = in[j] - '0';
        if (a == 0)
        {
            cout << "[none] "<< endl;
            j = in.size();
        }
        else if (a > options_num)
        {
            cout << "Wrong input! : " << a << endl
                 << "Scoreboard not adjusted" << endl;
        }
        else
        {
            a--;
            score_lsm_array[num_apparmor].score += (ans[num_apparmor][a] & 1);
            score_lsm_array[num_selinux].score += (ans[num_selinux][a] & 1);
            score_lsm_array[num_smack].score += (ans[num_smack][a] & 1);
            score_lsm_array[num_tomoyo].score += (ans[num_tomoyo][a] & 1);
        }
    }
    print_lsm_scoreboard();
}

vector<questions> muti_questions_list, tf_questions_list;

int main(int argc, char *argv[])
{
    os_init();
    ques_init();
    cout << beginning;
    cout << "-----------------------------------" << endl;
    printf("Start case 1 - Trivial Questions\n************************\n");
    trivial_q();
    cout << "end of case 1 questions\n************************\n";
    cout << "-----------------------------------" << endl;
    printf("Start case 2 - Complex Questions\n");
    complex_q();
    cout << "end of case 2 questions\n************************\n";
    cout << "-----------------------------------" << endl;
    printf("Start case 3 - Developer Request\n************************\n");
    if (envir_req())
        cout << "bad selection";
    cout << "end of case 3 questions\n************************\n";
    cout << "-----------------------------------" << endl;
    return 0;
}
void os_init()
{
    ur_os = new dis_struct;

    score_lsm_array[num_apparmor].lsm_name = "AppArmor";
    score_lsm_array[num_apparmor].score = 1;
    score_lsm_array[num_selinux].lsm_name = "SELinux";
    score_lsm_array[num_selinux].score = 1;
    score_lsm_array[num_smack].lsm_name = "SMACK";
    score_lsm_array[num_smack].score = 1;
    score_lsm_array[num_tomoyo].lsm_name = "TOMOYO Linux";
    score_lsm_array[num_tomoyo].score = 1;

    alpine.distribution = "Alpine";
    alpine.lsm = "none";
    alpine.de_fs = "ext4";
    alpine.distrib_kernel_lsm[num_apparmor] = 1;
    alpine.distrib_kernel_lsm[num_selinux] = -1;
    alpine.distrib_kernel_lsm[num_smack] = 0;
    alpine.distrib_kernel_lsm[num_tomoyo] = -1;

    centos.distribution = "centOS";
    centos.lsm = "SELinux";
    centos.de_fs = "xfs";
    centos.distrib_kernel_lsm[num_apparmor] = -1;
    centos.distrib_kernel_lsm[num_selinux] = 1;
    centos.distrib_kernel_lsm[num_smack] = 0;
    centos.distrib_kernel_lsm[num_tomoyo] = 0;

    opensuse.distribution = "openSUSE";
    opensuse.lsm = "AppArmor";
    opensuse.de_fs = "ext3";
    opensuse.distrib_kernel_lsm[num_apparmor] = 1;
    opensuse.distrib_kernel_lsm[num_selinux] = 0;
    opensuse.distrib_kernel_lsm[num_smack] = 0;
    opensuse.distrib_kernel_lsm[num_tomoyo] = 0;

    ubuntu.distribution = "Ubuntu";
    ubuntu.lsm = "AppArmor";
    ubuntu.de_fs = "ext4";
    ubuntu.distrib_kernel_lsm[num_apparmor] = 1;
    ubuntu.distrib_kernel_lsm[num_selinux] = 1;
    ubuntu.distrib_kernel_lsm[num_smack] = 1;
    ubuntu.distrib_kernel_lsm[num_tomoyo] = 1;

    default_os.distribution = "all";
    default_os.lsm = "all";
    default_os.de_fs = "all";
    default_os.distrib_kernel_lsm[num_apparmor] = 1;
    default_os.distrib_kernel_lsm[num_selinux] = 1;
    default_os.distrib_kernel_lsm[num_smack] = 1;
    default_os.distrib_kernel_lsm[num_tomoyo] = 1;

    os_list.push_back(default_os);
    os_list.push_back(alpine);
    os_list.push_back(centos);
    os_list.push_back(opensuse);
    os_list.push_back(ubuntu);
}

void ques_init()
{
    /***************Trivial Questions*******************/
    //+1 manage q
    // push_trivial_quest_list(str_autopolicy, 1, 0, 0, 1);
    // push_trivial_quest_list(caches_req, 0, 1, 0, 0);
    // push_trivial_quest_list(str_log_support_policy, 1, 1, 0, 0);
    // push_trivial_quest_list(str_dynamically_load_policy, 1, 1, 1, 1);
    // push_trivial_quest_list(str_configurable_aduit, 0, 1, 1, 0);
    // push_trivial_quest_list(str_default_policy, 0, 1, 1, 0);
    questions auto_policy(str_autopolicy, 1, 0, 0, 1);
    questions cach_req(caches_req, 0, 1, 0, 0);
    questions log_sup_policy(str_log_support_policy, 1, 1, 0, 0);
    questions dynmaic_load_policy(str_dynamically_load_policy, 1, 1, 1, 1);
    questions config_audit(str_configurable_aduit, 0, 1, 1, 0);
    questions default_policy(str_default_policy, 0, 1, 1, 0);

    // +2 security q
    questions secur_auditing(str_security_auditing, 2, 2, 2, 2);
    questions sup_type(str_support_tpe, 0, 2, 0, 2);
    questions sup_whitlist(str_whitelist, 2, 2, 0, 2);

    tf_questions_list.push_back(auto_policy);
    tf_questions_list.push_back(cach_req);
    tf_questions_list.push_back(log_sup_policy);
    tf_questions_list.push_back(dynmaic_load_policy);
    tf_questions_list.push_back(config_audit);
    tf_questions_list.push_back(default_policy);
    tf_questions_list.push_back(secur_auditing);
    tf_questions_list.push_back(sup_type);
    tf_questions_list.push_back(sup_whitlist);

    /***************Complex Questions*******************/
    questions secu_models(str_support_security_models, 4, 1);
    questions sup_virtual(str_support_virtualization, 6, 2);
    questions fit_secu_cert(str_fit_security_certifications, 3, 3);
    questions config_typ(str_configuration_type, 4, 4);

    muti_questions_list.push_back(secu_models);
    muti_questions_list.push_back(sup_virtual);
    muti_questions_list.push_back(fit_secu_cert);
    muti_questions_list.push_back(config_typ);

    // push_quest_list(str_file_system_support_xattr, 0, -1, -1, 0);
}

int complex_q()
{
    int seq = 0;
    cout << "/*****" << complex_rule << "*****/" << endl;
    for (int i = 0; i < muti_questions_list.size(); i++)
    {
        seq = i + 1;
        cout << seq << ". " << muti_questions_list[i].str_question << endl;
        muti_questions_list[i].muti_user_input_then_adjust_scoreboard();
    }
    return 0;
}

int trivial_q()
{
    int ret = 0;
    int seq = 0;
    cout << "/*****" << trviial_rule << "*****/" << endl
         << endl;
    for (int i = 0; i < tf_questions_list.size(); i++)
    {
        seq = i + 1;
        ret = tf_questions_list[i].tf_user_input_then_adjust_scoreboard(seq);
        if (ret)
            i--;
    }
    return 0;
}

int envir_req()
{
    bool stat = true;
    stat = erq1() & erq2();
    if (stat)
        return 0;
    else
        return 1;
}
bool erq1()
{
    int input;
    cout << "Choose your distribution Linux: [0]any way [1]Alpine [2]CentOS [3]openSUSE [4]Ubuntu [5]others " << endl;
    cin >> input;
    cout << "Your input : " << input << endl;
    if (input < 5)
    {
        ur_os = &os_list[input];
    }
    else
    {
        cout << "Others OS not support yet!";
        return false;
    }
    cout << "end of case 3 questions\n************************\n";
    cout << "This is your OS's informations" << endl;
    cout << "Distribution linux : " << ur_os->distribution << "\nDefault LSM : " << ur_os->lsm << "\nDefault File System : " << ur_os->de_fs << endl;

    return true;
}
bool erq2()
{
    cout << does_insert_into_kerenl << endl
         << endl;
    for (int i = 0; i < 4; i++)
    {
        if (ur_os->distrib_kernel_lsm[i] <= 0)
        {
            if (i == num_apparmor)
            {
                cout << ur_os->distribution << " doesn't insert AppArmor into the kernel" << endl;
                cout << "So minus 1 point because your need to remake a new kernel with AppArmor" << endl;
                score_lsm_array[num_apparmor].score--;
                cout << endl;
            }
            if (i == num_selinux)
            {
                cout << ur_os->distribution << " doesn't insert SELinux into the kernel" << endl;
                cout << "So SELinux minus 1 point because your need to remake a new kernel with SELinux" << endl;
                score_lsm_array[num_selinux].score--;
                cout << endl;
            }
            if (i == num_smack)
            {
                cout << ur_os->distribution << " doesn't insert SMACK into the kernel" << endl;
                cout << "So SMACK minus 1 point because your need to remake a new kernel with SMACK" << endl;
                score_lsm_array[num_smack].score--;
                cout << endl;
            }
            if (i == num_tomoyo)
            {
                cout << ur_os->distribution << " doesn't insert TOMOYO Linux into the kernel" << endl;
                cout << "So TOMOYO Linux minus 1 point because your need to remake a new kernel with TOMOYO Linux" << endl;
                score_lsm_array[num_tomoyo].score--;
                cout << endl;
            }
        }
    }
    printf("LSM      %-20s%-21s%-16s%s\n", score_lsm_array[0].lsm_name, score_lsm_array[1].lsm_name, score_lsm_array[2].lsm_name, score_lsm_array[3].lsm_name);
    printf("score       %-20d%-20d%-20d%d\n", score_lsm_array[0].score, score_lsm_array[1].score, score_lsm_array[2].score, score_lsm_array[3].score);

    cout << does_distrib_support_lsm << endl;
    for (int i = 0; i < 4; i++)
    {
        if (ur_os->distrib_kernel_lsm[i] < 0)
        {
            if (i == num_apparmor)
            {
                cout << ur_os->distribution << " doesn't support AppArmor " << endl;
                cout << "So multiple -1" << endl;
                score_lsm_array[num_apparmor].score = score_lsm_array[num_apparmor].score * (-1);
            }
            if (i == num_selinux)
            {
                cout << ur_os->distribution << " doesn't support SELinux " << endl;
                cout << "So multiple -1" << endl;
                score_lsm_array[num_selinux].score = score_lsm_array[num_selinux].score * (-1);
            }
            if (i == num_smack)
            {
                cout << ur_os->distribution << " doesn't support SMACK" << endl;
                cout << "So multiple -1" << endl;
                score_lsm_array[num_smack].score = score_lsm_array[num_smack].score * (-1);
            }
            if (i == num_tomoyo)
            {
                cout << ur_os->distribution << " doesn't support TOMOYO" << endl;
                cout << "So multiple -1" << endl;
                score_lsm_array[num_tomoyo].score = score_lsm_array[num_tomoyo].score * (-1);
            }
        }
    }
    printf("LSM      %-20s%-21s%-16s%s\n", score_lsm_array[0].lsm_name, score_lsm_array[1].lsm_name, score_lsm_array[2].lsm_name, score_lsm_array[3].lsm_name);
    printf("score       %-20d%-20d%-20d%d\n", score_lsm_array[0].score, score_lsm_array[1].score, score_lsm_array[2].score, score_lsm_array[3].score);
    int max = -1, pos = -1;
    for (int i = 0; i < 4; i++)
    {
        if (score_lsm_array[i].score > max)
        {
            pos = i;
            max = score_lsm_array[i].score;
        }
    }

    cout << endl
         << "The most suitable LSM is " << score_lsm_array[pos].lsm_name << endl
         << endl;
    cout << "NOTE:" << endl;
    if ((score_lsm_array[num_apparmor].score < 0) && (abs(score_lsm_array[num_apparmor].score) > max))
    {
        cout << better_lsm << "AppArmor " << endl
             << but_lsm << endl
             << change_dis << endl;
    }
    if ((score_lsm_array[num_selinux].score < 0) && (abs(score_lsm_array[num_selinux].score) > max))
    {
        cout << better_lsm << "SELinux " << endl
             << but_lsm << endl
             << change_dis << endl;
    }
    if ((score_lsm_array[num_smack].score < 0) && (abs(score_lsm_array[num_smack].score) > max))
    {
        cout << better_lsm << "SMACK " << endl
             << but_lsm << endl
             << change_dis << endl;
    }
    if ((score_lsm_array[num_tomoyo].score < 0) && (abs(score_lsm_array[num_tomoyo].score) > max))
    {
        cout << better_lsm << "TOMOYO Linux " << endl
             << but_lsm << endl
             << change_dis << endl;
    }

    return true;
}
void print_lsm_scoreboard()
{
    printf("LSM      %-20s%-21s%-16s%s\n", score_lsm_array[0].lsm_name, score_lsm_array[1].lsm_name, score_lsm_array[2].lsm_name, score_lsm_array[3].lsm_name);
    printf("score       %-20d%-20d%-20d%d\n", score_lsm_array[0].score, score_lsm_array[1].score, score_lsm_array[2].score, score_lsm_array[3].score);
};