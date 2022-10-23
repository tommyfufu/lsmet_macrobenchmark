#include "lsfet_lsm_score.h"
class questions
{
public:
    string str_question, str_ques_description, user_input;
    int ans[4][MAX_OPS];
    int sequence_in_ques_list;
    int ans_input_count = num_apparmor;
    questions(string str_qu, string str_qu_de, int sequence, int app, int se, int sm, int tomo, int ad_weigh);
    questions(string str_qu, string str_qu_de, int ops_num, int sequence, int ad_weigh);
    void muti_user_input_then_adjust_scoreboard();
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
void questions::muti_user_input_then_adjust_scoreboard()
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
}
