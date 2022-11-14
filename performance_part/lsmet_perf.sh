#!/bin/bash
# Program:
#	Let user to get security module info. and performance overhead
# History:
# 2022/07/29 Wen Hao
# 2022/07/15 Wen Hao

PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
export PATH

function apache_enable(){
    if [ "${1}" == "Ubuntu" ]; then
        echo "Ubuntu"
        apt update && sudo apt install apache2 -y
        systemctl restart apache2
        sleep 5
        ufw allow in "Apache Full"
        ufw allow ssh
        ufw enable
    elif [ "${1}" == "CentOS" ]; then
        yum install httpd -y; sleep 10; systemctl start httpd; sleep 1; systemctl enable httpd;
        sleep 5
        firewall-cmd --permanent --add-service=http
        sleep 5
        firewall-cmd --reload
    # elif [ "${1}" == "openSUSE" ]; then
    #     zypper refresh; zypper update; zypper install apache2 -y ;systemctl enable apache2;
    #     sleep 5
    #     firewall-cmd --permanent --add-service=http --add-service=https
    #     sleep 5
    #     firewall-cmd --reload
    elif [ "${1}" == "alpine" ]; then
        apk update; sleep 2; apk upgrade; sleep 2; apk add apache2 -y; sleep 2; rc-service apache2 start; sleep 2; rc-update add apache2;
        sleep 5
        firewall-cmd --permanent --add-service=http --add-service=https
        sleep 5
        firewall-cmd --reload
        apk add apache2-utils
    else
        echo "Other OSes"
    fi
    echo "1" > firsttime.txt
    sleep 5
    echo "apache is enabled, please run apache_bench"
}

function apache_bench(){
    local testtime
    totaltime=0
    for ((i=1; i<=2; i=i+1)); do
        ab -n 1000000 -c 20 http://127.0.0.1/ | grep "Time taken for tests:" | cut -d" " -f 7 > result.txt
        testtime=$(cat result.txt)
        totaltime=$(echo "${totaltime}+${testtime}" | bc -l)
    done
    ans=$(echo "${totaltime}/2" | bc -l)
    echo ${ans}
}

function tomoyo_disable(){
    echo "lsfet_perf.sh can't auto enable/disable TOMOYO, need Root manually configure"
    echo "==="
    echo "Please follow the steps below "
    echo "1. Run tomoyo-editpolicy (#tomoyo-editpolicy)"
    echo "2. Press key 'f' to find the absolute path of this program (aka lsfet_perf_sh.sh)"
    echo "   for example, key in /home/lsfet/lsfet_perf_sh.sh"
    echo "3. Then you will find this path under /bin/bash (or your shell name)"
    echo "   The number in front of this path, 0 means disabled, 1 means learning, 2 means permissive, 3 means enforcing"
    echo -e "   lsfet_perf_sh.sh will select the running time in disabled mode as the benchmark,"
    echo -e "   and then select the running time in permissive mode to caculate the overhead"
    echo "4. Please input 's' to set 0 first, and then input 'q' to quit tomoyo-edpolicy"
    read -p "If you have already understood and set tomoyo to disabled, please input [Y/N], Y to continue, N to exit" answer
    case $answer in
    Y | y)
        echo "Start caculating the benchmark"
        apache_bench > disable.txt
        sleep 2
        echo "The benchmark calculation is complete, please exit lsfet_perf_sh.sh and use tomoyo-editpolicy to set lsfet_perf_sh.sh in permissive mode"
        echo "The method is to find the same line and key in 's' to set 2"
        read -p "press any key to exit" ex
        ;;
    N | n)
      echo "exit";;
    *)
     echo "error input, exit";;
    esac
}

function tomoyo_enable(){
    echo "Start caculating the overhead"
    apache_bench > enable.txt
    sleep 2
}

declare -A ovarray
ovarray=([0]=0,[1]=0)
overhead=0;
echo "====="
echo "this is your OS distribution:"
cat /etc/os-release | grep PRETTY_NAME |  cut -d"\"" -f 2 | cut -d" " -f 1 >mydistrib.txt
firsttime=$(cat firsttime.txt)
mydistrib=$(cat mydistrib.txt)
echo ${mydistrib}
if [ firsttime != "1" ]; then
    apache_enable ${mydistrib}
fi
echo "====="
echo "====="
echo "Default Linux Security Module is "
ls /usr/sbin/ | grep 'sestatus\|apparmor_status\|tomoyo-editpolicy' >lsm.txt
mylsm=$(cat lsm.txt)
if [ ${mydistrib} == 'CentOS' ]; then
        getenforce > lsmstat.txt
        temp=$(cat lsmstat.txt)
        echo ${temp}
        if [ ${temp} == 'Disabled' ]; then
                tmp=$(grep -q tomoyo-editpolicy lsm.txt ; echo $?)
                if [ ${tmp} == 0 ]; then
                        echo "tomoyo-editpolicy">lsm.txt
                        cat lsm.txt
                        mylsm=$(cat lsm.txt)
                fi
        fi
fi

if [ ${mylsm} == 'apparmor_status' ]; then
    echo "AppArmor"
    echo "====="
    apt install apache2-utils libapache2-mod-apparmor -y
    a2enmod apparmor
    systemctl restart apparmor
    systemctl restart apache2
    systemctl is-active apparmor.service > lsmstat.txt
    lsmstat=$(cat lsmstat.txt)
    echo 'Default LSM on this environment: AppArmor' >> ../lsm_score/lsm_report_dir/lsmet_report.txt
    echo ${lsmstat}
    if [ ${lsmstat} == 'active' ]; then
        echo 'Fisrt, start testing active'
        aa-complain /etc/apparmor.d/usr.sbin.apache2
        apparmor_parser -r /etc/apparmor.d/usr.sbin.apache2
        apache_bench > enable.txt
        echo 'Time for LSM enabled: ' >> overhead.txt
        cat enable.txt >> overhead.txt
        aa-disable /etc/apparmor.d/usr.sbin.apache2
        #apparmor_parser -R /etc/apparmor.d/usr.sbin.apache2
        systemctl stop apparmor.service
        systemctl disable apparmor.service
        systemctl restart apache2
        #echo 'Please reboot and rerun lsfet_perf.sh to test the performance of apache without apparmor management'
        #exit 0
        # systemctl is-active apparmor.service
        # apache_bench > disable.txt
        # systemctl start apparmor.service
    #elif [ ${lsmstat} == 'inactive' ]; then
        #echo "After reboot please run lsfet_perf.sh again"
        #exit 0
        echo 'Start testing inactive'
        apache_bench > disable.txt
        echo 'Time for LSM disabled: ' >> overhead.txt
        cat disable.txt >> overhead.txt
        cat overhead.txt >> ../lsm_score/lsm_report_dir/lsmet_report.txt
        systemctl start apparmor.service
        systemctl enable apparmor.service
        rm /etc/apparmor.d/disable/usr.sbin.apache2
        aa-complain /etc/apparmor.d/usr.sbin.apache2
        systemctl restart apache2
        #apparmor_parser -r /etc/apparmor.d/usr.sbin.apache2
    else
        echo "AppArmor error"
    fi
    ovarray[0]=$(cat disable.txt |bc -l)
    ovarray[1]=$(cat enable.txt| bc -l)
    overhead=$(awk -v x=${ovarray[1]} -v y=${ovarray[0]} 'BEGIN{printf "%.2f\n", (x-y)/y}')
    echo 'Overhead is :'
    overhead=$(echo ${overhead}*100 |bc -l )
    echo "${overhead}%"
    echo "====="
elif [ ${mylsm} == 'sestatus' ]; then
    echo "SELinux"
    #apache_enable ${mydistrib}
    getenforce > lsmstat.txt
    lsmstat=$(cat lsmstat.txt)
    echo ${lsmstat}
    if [ ${lsmstat} == 'Disabled' ]; then
         apache_bench > disable.txt
         cp ./selinux_config/enable_config/config /etc/selinux/config
         sleep 3
         reboot
         exit 0
    elif [ ${lsmstat} == 'Enforcing' ] || [ ${lsmstat} == 'Permissive' ]; then
        #echo "Please manually set SELinux=disable in /etc/selinux/config and then reboot"
        #echo "After reboot please run lsfet_perf.sh again"
        #exit 0
        #setenforce 1
        apache_bench > enable.txt
    else
        echo "SELinux error"
    fi
    ovarray[0]=$(cat disable.txt |bc -l)
    ovarray[1]=$(cat enable.txt| bc -l)
    overhead=$(awk -v x=${ovarray[1]} -v y=${ovarray[0]} 'BEGIN{printf "%.2f\n", (x-y)/y}')
    echo 'Overhead is :'
    overhead=$(echo ${overhead}*100 |bc -l )
    echo "${overhead}%"
    echo "====="
elif [ ${mylsm} == 'tomoyo-editpolicy' ]; then
    echo "TOMOYO"
    echo "====="
    #apache_enable ${mydistrib}
    echo -e "If you are running lsfet_perf_sh.sh for the first time \nor you have not yet obtained the benchmark (i.e. do not disable tomoyo), please enter Y."
    read -p "Otherwise, if you have obtained the benchmark and want to further calculate the overhead, please enter N.[Y/N]" fisrttime
    case $fisrttime in
    Y | y)
      tomoyo_disable
      exit 0
      ;;
    N | n)
      tomoyo_enable
      ;;
    *)
        echo "error choice"
        exit 0
        ;;
    esac
    ovarray[0]=$(cat disable.txt| bc -l)
    echo 'disable LSM'
    ovarray[1]=$(cat enable.txt| bc -l)
    overhead=$(awk -v x=${ovarray[1]} -v y=${ovarray[0]} 'BEGIN{printf "%.4f\n", (x-y)/y}')
    echo 'Overhead is :' 
    overhead=$(echo ${overhead}*100|bc  )
    echo "${overhead}%" 
    echo "====="
else
    echo "No LSM install"
    echo "====="
fi
exit 0;
