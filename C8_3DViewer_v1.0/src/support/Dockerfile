FROM quay.io/outtrip-kpsv/dvicreator:0.1

RUN rm -rf /project
COPY . /project

WORKDIR "/project"

ENTRYPOINT [ "/bin/bash", "support/entrypoint.sh"]
# ENTRYPOINT "/bin/sh"