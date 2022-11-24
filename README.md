# LSMET working directory
lsmet
> lsm_score
    > lsmet_reference
    > lsmet_report_dir
        - lsmet_report.txt 
    - lsmet.cpp
    - lsmet_scoring.cpp
    - lsmet_scoring.h
    - Makefile
    - score.conf 
> performance_part
    > refernece
        > apparmor
            - usr.sbin.apache2
        > selinux
            > 
            > 
    - lsmet_perf.sh
# Get program
```bash=
$ git clone https://github.com/tommyfufu/lsmet.git
# username: tommyfufuu
# password: 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
$ git checkout OOP
```
# LSMET Usage
## Step 1. Run lsmet evaluation criteria program
```bash=
$ cd lsmet/lsm_score/
$ make
$ ./lsmet
```
then result - lsmet_report will be put in ./lsmet_report_dir/ as lsmet_report.txt
the lsmet_report.txt doesn't have 
## Step 2. Run lsmet performance testing
```bash=
$ cd ../prformance
$ sudo ./lsmet_perf.sh #password rtesrtes
```
then result - overhead is caused by LSM of this environment, and it will be created as overhead.txt
## Step 3. Check the lsmet_report full version
see lsmet/lsm_score/lsmet_report_dir/lsmet_report.txt
its content includes 
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

