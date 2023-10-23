# LSMET working directory
```bash
lsmet
lsm_score
    lsmet_reference
    lsmet_report_dir
        lsmet_report.txt 
    lsmet.cpp
    lsmet_scoring.cpp
    lsmet_scoring.h
    Makefile
    score.conf 
performance_part
    refernece
        apparmor
            usr.sbin.apache2
        selinux
            enable_config
            disable_config
    lsmet_perf.sh
```
# LSMET Usage
## Step 1. Run lsmet evaluation criteria program
```bash=
$ cd lsmet/lsm_score/
$ make
$ cd ../
```
then result - lsmet_report will be put in ./lsmet/lsmet_report_dir/ as lsmet_report.txt
the lsmet_report.txt doesn't have performance testing result yet, so next step, you need to run lsmet performance testing
## Step 2. Run lsmet performance testing
```bash=
$ cd ../prformance
$ sudo ./lsmet_perf.sh #password rtesrtes
```
then result - overhead is caused by LSM of this environment, and it will be created as overhead.txt
## Step 3. Check the lsmet_report full version
see lsmet/lsm_score/lsmet_report_dir/lsmet_report.txt
its content include
I. Features of each LSMs
II. Features of each minor LSMs
III. Developer system environment
IV. Objective Criteria Evaluation
V. Performance impact

## Generate lsmet_report of specific LSM (Optional)
```bash=
$ cd lsmet/lsm_score/
$ ./lsmet lsm_name #lsm_name means AppArmor, SELinux, Smack, TOMOYO
```

