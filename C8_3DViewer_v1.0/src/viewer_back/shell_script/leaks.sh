#!bin/bash

if [ -e leaks.LOG ]; then
    if [[ "$(uname)" == "Darwin" ]]; then
        grep 'leaks for' leaks.LOG
    fi

    if [[ $(uname) == "Linux" ]]; then
        grep 'ERROR SUMMARY' leaks.LOG
    fi

    if [ "save" == "$1" ]; then
        cat leaks.LOG>>full_leaks.LOG
    fi
    rm -f leaks.LOG
fi