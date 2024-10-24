FROM quay.io/school21/alpine:3.16

RUN apk --no-cache --upgrade add \
    bash \
    build-base \
    valgrind \
    check \
    gawk \
    check-dev \
    lcov --repository=http://dl-cdn.alpinelinux.org/alpine/edge/testing/

ARG GOAL=gcov_report
ARG G=
ARG TEST=

RUN mkdir /test 

COPY . /project

WORKDIR "/project"

ENTRYPOINT [ "/bin/bash", "shell_script/run.sh"]
# ENTRYPOINT "/bin/bash"
