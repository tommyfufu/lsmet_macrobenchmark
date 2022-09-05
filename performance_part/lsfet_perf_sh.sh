#!/bin/bash
# Program:
#	Let user to get security module info. and performance overhead
# History:
# 2022/07/15 Wen Hao
PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
export PATH

function apache_enable(){
    if [ "${1}" == "Ubuntu" ]; then
        echo "Ubuntu"
    elif [ "${1}" == "CentOS" ]; then
        yum install httpd -y; sleep 10; systemctl start httpd; systemctl enable httpd;
        sleep 10
        firewall-cmd --permanent --add-service=http
        sleep 10
        sudo firewall-cmd --reload
    else
        echo "Other OSes"
    fi
    sleep 5
    echo "apache is enabled, please run apache_bench"
}

function apache_bench(){
    local testtime
    totaltime=0
    for ((i=1; i<=5; i=i+1)); do
        ab -n 1000000 -c 20 http://127.0.0.1/ | grep "Time taken for tests:" | cut -d" " -f 7 > result.txt
        testtime=$(cat result.txt)
        #echo "turn before testtime: "
        #echo $testtime
        echo testtime|testtime=$(bc -l)
        #echo ${testtime}
        echo "testtime is "
        echo ${testtime}
        totaltime=$(echo "${totaltime}+${testtime}" | bc -l)
    done
    ans=$(echo "${totaltime} / 5" | bc -l)
    echo "total time is: "
    echo ${totaltime}
    echo "and avg time is: "

    echo ${ans}
}


overhead = 0;
echo "====="
echo "this is your OS distribution:"
cat /etc/os-release | grep PRETTY_NAME |  cut -d"\"" -f 2 | cut -d" " -f 1 >mydistrib.txt
mydistrib=$(cat mydistrib.txt)
echo ${mydistrib}
echo "====="
echo "====="
echo "Default Linux Security Module is "
ls /usr/sbin/ | grep 'sestatus\|apparmor_status\|tomoyo-editpolicy' >lsm.txt
mylsm=$(cat lsm.txt)
if [ ${mylsm} == 'apparmor_status' ]; then
    echo "AppArmor"
    echo "====="

elif [ ${mylsm} == 'sestatus' ]; then
    echo "SELinux"
    getenforce > lsmstat.txt
    lsmstat=$(cat lsmstat.txt)
    echo ${lsmstat}
    apache_enable ${mydistrib}
    apache_bench
    echo "====="
elif [ ${mylsm} == 'tomoyo-editpolicy' ]; then
    echo "TOMOYO"
    echo "====="

else
    echo "No LSM install"
    echo "====="
fi
exit 0;
