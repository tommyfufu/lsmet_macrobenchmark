#include "lsfet_lsm_score.h"
#include <filesystem>
int read_file_to_another_file_without_append_without_append(string inf, string outf)
{
    ifstream in;
    ofstream out;
    in.open(inf, ifstream::in);
    out.open(outf, ofstream::out);
    if (in.fail())
    {
        cout << inf << " open failed" << endl;
        return 1;
    }
    if (out.fail())
    {
        cout << outf << " open failed" << endl;
        return 2;
    }
    out << in.rdbuf();
    in.close();
    out.close();
    return 0;
}

int append_overhead_behind_lsfet_report(string inf, string outf)
{
    ifstream in;
    ofstream out;
    in.open(inf, ifstream::in);
    out.open(outf, ofstream::app);
    if (in.fail())
    {
        cout << inf << " open failed" << endl;
        return 1;
    }
    if (out.fail())
    {
        cout << outf << " open failed" << endl;
        return 2;
    }

    out << in.rdbuf();
    in.close();
    out.close();
    return 0;
}

int main(int argc, char *argv[])
{
    short stage = 0;
    if (argc == 1)
    {
        stage = 1;
        read_file_to_another_file_without_append_without_append("./lsfet_reference/lsm_overview", "./lsfet_report_dir/lsfet_report.txt");
        pid_t lsm_score_testing;
        lsm_score_testing = fork();
        if (lsm_score_testing == 0)
        {
            stage = 2;
            execl("lsfet_lsm_score", "lsfet_lsm_score", NULL);
        }
        else if (lsm_score_testing > 0)
        {
            stage = 3;
            int status;
            waitpid(lsm_score_testing, &status, 0);
            //append_overhead_behind_lsfet_report("../performance_part/overhead", "./lsfet_report.txt");
            stage = 4;
        }
        else
        {
            perror("fork failed\n");
            exit(1);
        }
    }
    else if (argc == 2)
    {
        string input_lsm = argv[1];
        if (!strcasecmp(input_lsm.c_str(), "AppArmor"))
        {
            read_file_to_another_file_without_append_without_append("./apparmor_report", "./lsfet_report_dir/lsfet_report.txt");
        }
        else if (!strcasecmp(input_lsm.c_str(), "SELinux"))
        {
            read_file_to_another_file_without_append_without_append("./selinux_report", "./lsfet_report_dir/lsfet_report.txt");
        }
        else if (!strcasecmp(input_lsm.c_str(), "Smack"))
        {
            read_file_to_another_file_without_append_without_append("./smack_report", "./lsfet_report_dir/lsfet_report.txt");
        }
        else if (!strcasecmp(input_lsm.c_str(), "TOMOYO"))
        {
            read_file_to_another_file_without_append_without_append("./tomoyo_report", "./lsfet_report_dir/lsfet_report.txt");
        }
        else
            cout << "No LSM named " << input_lsm << " , please input apparmor/selinux/smack/tomoyo" << endl;
    }
    else
    {
        cout << "Usage: ./LSFET [lsm_name]" << endl;
    }
    if (stage == 4)
    {
        int input_overhead = append_overhead_behind_lsfet_report("../performance_part/overhead", "./lsfet_report_dir/lsfet_report.txt");
        if (input_overhead == 1)
        {
            cout << "Please run lsfet_perf_sh.sh first, then you can get overhead that casued by the lsm." << endl;
        }
        else if (input_overhead == 2)
        {
            cout << "Could not find lsfet_report, please check if this file exists." << endl;
        }
        else
        {
            cout << "overhead input successed" << endl << "LSFET final report completed, you can read lsfet_report now" << endl;
        }
    }
    return 0;
}