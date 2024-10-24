#!bin/bash

FILE=~/goinfre/$(whoami)
SUDO=
rm -rf lcov_report/*
if [[ "$(uname)" == "Darwin" ]]; then
    if ! [ -x "$(command -v lcov)" ]; then
        if [ -L "${FILE}" ]; then
            echo "symlink was previously created"
        else
            ln -s /opt/goinfre/$(whoami) ~/goinfre    
        fi

        if [ -d "${FILE}"/homebrew ]; then
            echo "homebrew was previously installed"
        else
            git clone https://github.com/Homebrew/brew ~/goinfre/homebrew
        fi
    fi
    eval "$(~/goinfre/homebrew/bin/brew shellenv)"
    brew update --force --quiet
    chmod -R go-w "$(brew --prefix)/share/zsh"
    brew install lcov
    lcov --capture --directory . --output-file main_coverage.info
    genhtml main_coverage.info --output-directory lcov_report
fi
if [[ $(uname) == "Linux" ]]; then
    if [[ $(cat /etc/issue|grep -o Ubuntu) == "Ubuntu" ]]; then
        SUDO=sudo
    fi
     if ! [ -x "$(command -v lcov)" ]; then
        $SUDO apt update
        $SUDO apt install lcov -y
    fi
	$SUDO lcov --capture --directory . --output-file main_coverage.info
	$SUDO genhtml main_coverage.info --output-directory lcov_report
    $SUDO chmod 777 lcov_report/ -R
fi