#!bin/bash

if [ -d ~/goinfre ] 
then
    mkdir -p ~/goinfre/Docker/Data
    ln -s ~/goinfre/Docker ~/Library/Containers/com.docker.docker
else
    echo start
fi

docker ps
if [ $? -eq 1 ]
then
    echo please RUN docker
else
    echo rr
    docker rm dvi_creator_container
    docker rmi -f dvi/creator
    docker   build . -t dvi/creator -f support/Dockerfile
    docker run --name dvi_creator_container -v $(pwd)/dvi:/project/dvi -it dvi/creator
    # docker run --name dvi/creator_container -v $(pwd)/lcov_report:/project/lcov_report -it -e GOAL=$1 -e G=$2   lib/docker_lcov_test
fi

    # docker rm docker_lcov_report
    # docker   build . -t lib/docker_lcov_test -f docker/Dockerfile
    # docker run --name docker_lcov_report -v $(pwd)/lcov_report:/project/lcov_report -it -e GOAL=$1 -e G=$2   lib/docker_lcov_test
    # docker rmi -f lib/docker_lcov_test
    # docker rm docker_lcov_report

