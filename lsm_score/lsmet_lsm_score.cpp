#include "lsmet_lsm_score.h"
int weight_arr[QUES_CLASS];
vector<dis_struct> os_list;
scoreboard_col score_lsm_array[4];
dis_struct *ur_os, alpine, centos, ubuntu, opensuse, any_os;
string distrib = "", lsm = "";
class questions
{
public:
    string str_question, str_ques_description, user_input;
    int ans[4][MAX_OPS];
    int sequence_in_ques_list;
    int ans_input_count = num_apparmor;
    questions(string str_qu, string str_qu_de, int sequence, int app, int se, int sm, int tomo, int ad_weigh);
    questions(string str_qu, string str_qu_de, int ops_num, int sequence, int ad_weigh);
    int muti_user_input_then_adjust_scoreboard();
    int tf_user_input_then_adjust_scoreboard(int seq);
    int options_num, weight;
};
questions::questions(string str_qu, string str_qu_de, int sequence, int app, int se, int sm, int tomo, int ad_weight)
{
    str_question = str_qu;
    str_ques_description = str_qu_de;
    sequence_in_ques_list = sequence;
    // cout << "Input Questions: " << str_question << endl;
    ans[num_apparmor][0] = app;
    ans[num_selinux][0] = se;
    ans[num_smack][0] = sm;
    ans[num_tomoyo][0] = tomo;
    weight = ad_weight;
};
questions::questions(string str_qu, string str_qu_de, int ops_num, int sequence, int ad_weight)
{
    str_question = str_qu;
    str_ques_description = str_qu_de;
    // cout << "Input Questions: " << str_question << endl;
    options_num = ops_num;
    sequence_in_ques_list = sequence - 1;
    weight = ad_weight;
    for (int i = num_apparmor; i <= num_tomoyo; i++)
    {
        for (int j = 0; j < options_num; j++)
        {
            ans[i][j] = muti_ans_array[sequence_in_ques_list][i][j];
        }
    }
};
int questions::tf_user_input_then_adjust_scoreboard(int seq)
{

    user_input = '\0';
    cout << seq << ". " << str_question << endl;
    cin >> user_input;

    cout << "Your input: " << user_input << endl
         << endl;
    if (user_input == "t\0" || user_input == "T\0")
    {
        score_lsm_array[num_apparmor].score += (ans[num_apparmor][0] * weight);
        score_lsm_array[num_selinux].score += (ans[num_selinux][0] * weight);
        score_lsm_array[num_smack].score += (ans[num_smack][0] * weight);
        score_lsm_array[num_tomoyo].score += (ans[num_tomoyo][0] * weight);
        print_lsm_scoreboard();
        cout << endl;
    }
    else if (user_input == "f\0" || user_input == "F\0")
    {
        print_lsm_scoreboard();
        cout << endl;
    }
    else
    {
        cout << "Wrong intput! Please input [T/t | F/f]" << endl;
        return 1;
    }
    return 0;
}
int questions::muti_user_input_then_adjust_scoreboard()
{
    user_input = "0";
    int a;
    cin >> user_input;
    int j;

    while (user_input == " ")
        cin >> user_input;

    for (j = 0; j < user_input.size(); j++)
    {
        a = user_input[j] - '0';
        if (a == 0)
        {
            cout << "[none] " << endl;
            j = user_input.size();
        }
        else if (a > options_num)
        {
            cout << "Wrong input! : " << a << endl
                 << "Scoreboard not adjusted" << endl;
            return 1;
        }
        else
        {
            a--;
            score_lsm_array[num_apparmor].score += ((ans[num_apparmor][a] & 1) * weight);
            score_lsm_array[num_selinux].score += ((ans[num_selinux][a] & 1) * weight);
            score_lsm_array[num_smack].score += ((ans[num_smack][a] & 1) * weight);
            score_lsm_array[num_tomoyo].score += ((ans[num_tomoyo][a] & 1) * weight);
        }
    }
    print_lsm_scoreboard();
    return 0;
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
    int dis_lsm_num = envir_req();
    cout << "-----------------------------------" << endl;
    cout << "Input questionnaire analysis to lsmet_report" << endl;
    gen_lsmet_report(ur_os->distribution, dis_lsm_num);

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

    any_os.distribution = "all";
    any_os.lsm = "all";
    any_os.de_fs = "all";
    any_os.distrib_kernel_lsm[num_apparmor] = 1;
    any_os.distrib_kernel_lsm[num_selinux] = 1;
    any_os.distrib_kernel_lsm[num_smack] = 1;
    any_os.distrib_kernel_lsm[num_tomoyo] = 1;

    os_list.push_back(any_os);
    os_list.push_back(alpine);
    os_list.push_back(centos);
    os_list.push_back(opensuse);
    os_list.push_back(ubuntu);
}
void ques_init()
{
    // Including ./score.conf, we will use your/default setting weight to cacluate the LSMs' score
    weight_init();
    /***************Trivial Questions*******************/
    // management q, weight is tr_management
    questions default_policy(str_default_policy, "Description: LSM provides default policy", 1, 0, 1, 1, 0, weight_arr[tr_management]);
    questions auto_policy(str_autopolicy, "Description: LSM provides the ability to automatically generate policies (i.e. learning mode and enforcing mode)?", 2, 1, 0, 0, 1, weight_arr[tr_management]);
    questions fine_tuning_policy(str_log_support_policy, "Description: LSM provides features that allow users to fine-tune access permissions (e.g. aa-logprof from AppArmor, audit2allow form selinux)", 3, 1, 1, 0, 0, weight_arr[tr_management]);
    questions dynmaic_load_policy(str_dynamically_load_policy, "Description: Load or use new policy without reboot", 4, 1, 1, 1, 1, weight_arr[tr_management]);
    questions cach_req(caches_req, "Description: Recording the access behavior through the cache can speed up the confirmation of access rights. In addition, the access behavior of the same object and subject \nusually occurs multiple times in a short period of time, so the caching mechanism can greatly speed up the confirmation speed of access rights.", 5, 0, 1, 0, 0, weight_arr[tr_management]);
    // security q, weight is tr_security
    questions secur_auditing(str_security_auditing, "Description: Provides additional auditing capabilities", 6, 1, 1, 1, 1, weight_arr[tr_security]);
    questions config_audit(str_configurable_aduit, "Description: Auditing can be tuned for more fine-grained record access behavior", 7, 0, 1, 1, 0, weight_arr[tr_security]);
    questions str_whitlist(str_whitelist, "Description: Provides a whitelist function to comply with the principle of least privilege as much as possible", 8, 1, 1, 0, 1, weight_arr[tr_security]);
    questions sup_tpe(str_support_tpe, "Description: Provides Path Trust Execution (PTE) functionality to revoke a program's permission to use system calls (e.g. exec())", 9, 0, 1, 0, 1, weight_arr[tr_security]);
    questions sand_box(str_sandbox, "Description: LSM can assist in establishing a sandbox environment", 10, 0, 1, 0, 0, weight_arr[tr_security]);
    questions secur_cert(str_fit_security_certifications, "Description: Such as passing CAPP, EAL, LSPP and other security verification", 11, 0, 1, 0, 0, weight_arr[tr_security]);
    questions memory_protect(str_mem_protect, "Description: LSM can shield important program and/or kernel structures from tampering via memory interfaces such as mmap(2), /dev/kmem, or even via stack overruns", 12, 0, 1, 1, 0, weight_arr[tr_security]);
    // tf means true/fasle, trivial questions will insert in tf_questions_list
    tf_questions_list.push_back(default_policy);
    tf_questions_list.push_back(auto_policy);
    tf_questions_list.push_back(fine_tuning_policy);
    tf_questions_list.push_back(dynmaic_load_policy);
    tf_questions_list.push_back(cach_req);
    tf_questions_list.push_back(secur_auditing);
    tf_questions_list.push_back(config_audit);
    tf_questions_list.push_back(str_whitlist);
    tf_questions_list.push_back(sup_tpe);
    tf_questions_list.push_back(sand_box);
    tf_questions_list.push_back(secur_cert);
    tf_questions_list.push_back(memory_protect);

    /***************Complex Questions*******************/
    questions secu_models(str_support_security_models, "Description: Domain Type Enforcement (DTE), The Flux Advanced Security Kernel (FLASK), Multi-level Security (MLS), Role-based access control (RBAC)", 4, 1, weight_arr[co_management]);
    questions sup_virtual(str_support_virtualization, "Description: LSM official website provides security support for specific virtualization technologies", 6, 2, weight_arr[co_management]);

    // muti means multiple choices, complex questions will insert in muti_questions_list
    muti_questions_list.push_back(secu_models);
    muti_questions_list.push_back(sup_virtual);

    // push_quest_list(str_file_system_support_xattr, 0, -1, -1, 0);
}
void weight_init()
{
    string config_line;
    struct score_config score_weight_conf;
    ifstream fin;

    fin.open("./score.conf");
    if (!fin.fail())
    {
        stringstream istr;
        int pos = 0;
        while (getline(fin, config_line))
        {
            istr.str(config_line);
            if (config_line[0] == '#')
                continue;
            else
            {
                // cout << config_line;

                if (config_line.find("tr_management") != std::string::npos)
                {
                    pos = config_line.rfind("=");
                    pos++;
                    istr << config_line.at(pos);
                    istr >> weight_arr[tr_management];
                }
                if (config_line.find("tr_security") != string::npos)
                {
                    pos = config_line.rfind("=");
                    pos++;
                    istr << config_line.at(pos);
                    istr >> weight_arr[tr_security];
                }
                if (config_line.find("co_management") != string::npos)
                {
                    pos = config_line.rfind("=");
                    pos++;
                    istr << config_line.at(pos);
                    istr >> weight_arr[co_management];
                }
                if (config_line.find("co_security") != string::npos)
                {
                    pos = config_line.rfind("=");
                    pos++;
                    istr << config_line.at(pos);
                    istr >> weight_arr[co_security];
                }
            }
        }

        // cout << "weight_arr = ";
        // for (int a = 0; a < 4; a++)
        //     cout << weight_arr[a] << endl;
    }
    else
    {
        cout << "open score.conf failed!" << endl;
        exit(1);
    }
    fin.close();
}
int complex_q()
{
    int seq = 0;
    cout << "/*****" << complex_rule << "*****/" << endl;
    for (int i = 0; i < muti_questions_list.size(); i++)
    {
        seq = i + 1;
        cout << seq << ". " << muti_questions_list[i].str_question << endl;
        int ret = muti_questions_list[i].muti_user_input_then_adjust_scoreboard();
        if (ret == 1)
            i--;
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
    int input;
    cout << "Choose your distribution Linux: [0]any way [1]Alpine [2]CentOS [3]openSUSE [4]Ubuntu [5]others " << endl;
    cin >> input;
    cout << "Your input : " << input << endl;
    if (input < 5 && input > 0)
    {
        ur_os = &os_list[input];
    }
    else if (input = 0)
    {
    }
    else
    {
        cout << "Others OS not support yet!";
        return false;
    }
    cout << "************************\n";
    cout << "This is your OS's informations" << endl;
    cout << "Distribution linux : " << ur_os->distribution << "\nDefault LSM : " << ur_os->lsm << "\nDefault File System : " << ur_os->de_fs << endl;
    if (ur_os->lsm == str_apparmor)
        return num_apparmor;
    else if (ur_os->lsm == str_selinux)
        return num_selinux;
    else if (ur_os->lsm == str_smack)
        return num_smack;
    else if (ur_os->lsm == str_tomoyo)
        return num_tomoyo;
    else
        return -1;
}
/*bool erq2()
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
         << "the_most_suitable LSM is " << score_lsm_array[pos].lsm_name << endl
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
}*/
void print_lsm_scoreboard()
{
    printf("LSM      %-20s%-21s%-16s%s\n", score_lsm_array[0].lsm_name, score_lsm_array[1].lsm_name, score_lsm_array[2].lsm_name, score_lsm_array[3].lsm_name);
    printf("score       %-20d%-20d%-20d%d\n", score_lsm_array[0].score, score_lsm_array[1].score, score_lsm_array[2].score, score_lsm_array[3].score);
}
int find_the_most_suitable_lsm()
{
    int max = -1, pos = -1;
    for (int i = 0; i < 4; i++)
    {
        if (score_lsm_array[i].score > max)
        {
            pos = i;
            max = score_lsm_array[i].score;
        }
    }
    return pos;
}
int gen_lsmet_report(string distrb_name, int dis_lsm_num)
{
    ofstream of("./lsmet_report_dir/lsmet_report.txt", ofstream::app);
    stringstream ofstr;
    int flag_dont_need_to_show_default_lsm = 0;
    int the_most_suitable_lsm_num = find_the_most_suitable_lsm();
    of << "After lsmet_lsm_score testing, the_most_suitable LSM is "
       << score_lsm_array[the_most_suitable_lsm_num].lsm_name << endl
       << "It's score is "
       << score_lsm_array[the_most_suitable_lsm_num].score << endl;
    if (dis_lsm_num == the_most_suitable_lsm_num)
    {
        of << "And the_most_suitable LSM is also the default LSM in your Distribution " << distrb_name << endl;
        flag_dont_need_to_show_default_lsm = 1;
    }
    else
    {
        of << "And your Distribution " << distrb_name << ", the default LSM is " << ur_os->lsm << endl
           << "It's score is " << score_lsm_array[dis_lsm_num].score << endl;
        flag_dont_need_to_show_default_lsm = 0;
    }

    if (ur_os->distrib_kernel_lsm[the_most_suitable_lsm_num] == 0)
    {
        of << "Unfortunately, your Distribution " << distrb_name << " doesn't not insert " << score_lsm_array[the_most_suitable_lsm_num].lsm_name
           << " into the kernel" << endl
           << "So you need to remake a new kernel that include " << score_lsm_array[the_most_suitable_lsm_num].lsm_name << endl;
    }
    else
    {
        of << "Fortunately, your Distribution " << distrb_name << " does insert " << score_lsm_array[the_most_suitable_lsm_num].lsm_name
           << " into the kernel" << endl
           << "So you don't need to remake a new kenerl" << endl;
    }
    of << "************************************************************************************" << endl;
    of << "************************************************************************************" << endl;
    of << "IV. Questionnaire analysis" << endl;
    of << "Listing each test and your answer, and comparison of the differences between the most appropriate LSM and the default LSM based on your responses to \nthe lsmet_lsm_score test" << endl
       << endl;
    of << "/----------------------------------------/" << endl
       << "Class 1 - Trivial questions " << endl
       << "/----------------------------------------/" << endl;
    for (int i = 0; i < tf_questions_list.size(); i++)
    {
        of << "----------------------------------------" << endl;
        of << tf_questions_list[i].sequence_in_ques_list << ". " << tf_questions_list[i].str_question << endl
           << tf_questions_list[i].str_ques_description << endl
           << "Your answer is: " << tf_questions_list[i].user_input << endl;
        if ((tf_questions_list[i].user_input == "t") || (tf_questions_list[i].user_input == "T"))
        {
            of << "----------" << endl
               << "the_most_suitable LSM " << score_lsm_array[the_most_suitable_lsm_num].lsm_name << " does " << endl;
            if (tf_questions_list[i].ans[the_most_suitable_lsm_num][0])
                of << "support and get " << tf_questions_list[i].weight << " point";
            else
                of << "not support";
            if (!flag_dont_need_to_show_default_lsm)
            {
                of << endl
                   << "----------" << endl
                   << "The default LSM " << score_lsm_array[dis_lsm_num].lsm_name << " does " << endl;
                if (tf_questions_list[i].ans[dis_lsm_num][0])
                    of << "support and get " << tf_questions_list[i].weight << " point";
                else
                    of << "not support";
            }
        }
        of << endl;
    }
    of << "************************************************" << endl
       << "/----------------------------------------/" << endl
       << "Class 2 - Complex questions " << endl
       << "/----------------------------------------/" << endl;
    for (int i = 0; i < muti_questions_list.size(); i++)
    {
        of << "----------------------------------------" << endl
           << ++muti_questions_list[i].sequence_in_ques_list << ". " << muti_questions_list[i].str_question << endl
           << muti_questions_list[i].str_ques_description << endl
           << "Your answer is: " << muti_questions_list[i].user_input << endl
           << "----------" << endl;
        string o1 = "", o2 = ""; // output, output1 for the most suitable, output2 for default lsm

        for (int k = 0; k < muti_questions_list[i].user_input.size(); k++)
        {

            int a = 0;
            a = muti_questions_list[i].user_input[k] - '0'; // get the value of user_input, ex. user_input=23, get 2 and transfer to int,
                                                            // then check ans correspond to 2 option
            if (a > 0)
            {

                if (muti_questions_list[i].ans[the_most_suitable_lsm_num][a - 1] == 1)
                {
                    o1 += muti_questions_list[i].user_input[k]; // o1 to record userinput that fit answer of the most suitable lsm
                    muti_questions_list[i].ans[the_most_suitable_lsm_num][a - 1] = -1;
                }
                if (!flag_dont_need_to_show_default_lsm)
                {
                    if (muti_questions_list[i].ans[dis_lsm_num][a - 1] == 1)
                    {
                        o2 += muti_questions_list[i].user_input[k];
                        muti_questions_list[i].ans[dis_lsm_num][a - 1] = -1;
                    }
                }
            }
        }
        for (int j = 0; j < muti_questions_list[i].options_num; j++)
        {
            if (muti_questions_list[i].ans[the_most_suitable_lsm_num][j] == -1)
                muti_questions_list[i].ans[the_most_suitable_lsm_num][j] = 1;
            if (muti_questions_list[i].ans[dis_lsm_num][j] == -1)
                muti_questions_list[i].ans[dis_lsm_num][j] = 1;
        }
        if (o1 != "")
            of << "the_most_suitable LSM " << endl
               << score_lsm_array[the_most_suitable_lsm_num].lsm_name << endl
               << "supports " << o1 << endl
               << "----------" << endl;
        else
            of << "the_most_suitable LSM " << endl
               << score_lsm_array[the_most_suitable_lsm_num].lsm_name << endl
               << "does not support any of your requirements"
               << endl
               << "----------" << endl;
        if (!flag_dont_need_to_show_default_lsm)
        {
            if (o2 != "")
                of << "The default LSM " << endl
                   << score_lsm_array[dis_lsm_num].lsm_name << endl
                   << "supports " << o2 << endl
                   << "----------" << endl;
            else
                of << "The default LSM " << endl
                   << score_lsm_array[dis_lsm_num].lsm_name << endl
                   << "does not support any of your requirements"
                   << endl
                   << "----------" << endl;
        }
    }
    of << "************************************************************************************" << endl;
    of << "************************************************************************************" << endl;
    of.close();
    return 1;
}