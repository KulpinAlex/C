#!bin/bash

arg=LOGFILE=save

if [ "$G" == "-g" ] 
then
    arg+=' DEBUG=-g'
fi

make $GOAL $arg

[ -e full_leaks.LOG ] && cp full_leaks.LOG lcov_report/full_leaks.LOG