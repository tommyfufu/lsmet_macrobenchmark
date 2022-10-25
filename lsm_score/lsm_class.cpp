#include "lsfet_lsm_score.h"

class LSMs
{
public:
    LSMs();
    void input_lsm_info(string, string, string);
    string output_off_doc(void) { return official_doc; };
    string output_off_paper(void) { return official_paper; };

private:
    string official_doc = "https://www.kernel.org/doc/html/v4.16/admin-guide/LSM/index.html";
    string official_paper = "https://www.usenix.org/conference/11th-usenix-security-symposium/linux-security-modules-general-security-support-linux";

protected:
    string lsm_name;
    string official_website;
    string official_url_install;
    string official_url_policy;
};

class AppArmor : public LSMs
{
public:
    AppArmor(void)
    {  
        official_website = official_web;
        official_url_install = official_install;
        official_url_policy = official_policy;
    }
    
};