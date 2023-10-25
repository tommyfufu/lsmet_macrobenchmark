# lsmet_macrobenchmark working directory
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
# lsmet_macrobenchmark Usage
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
$ sudo ./lsmet_perf.sh
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

### Demo
#### AppArmor Performance Testing Result
![LSMET perf demojpg](https://github.com/tommyfufu/lsmet_macrobenchmark/assets/101622170/acdd581e-e141-4f3a-98f8-12a330317a76)

## Framework
![LSMET framework](https://github.com/tommyfufu/lsmet_macrobenchmark/assets/101622170/ac667225-c47b-432b-8468-5e174054db0e)

## Flowchart
![LSMET Flowchart](https://github.com/tommyfufu/lsmet_macrobenchmark/assets/101622170/48e9e2af-3717-4abb-b9d8-d1f2d9605405)


