FROM dlang2
FROM ubuntu


RUN apt update
RUN apt full-upgrade -y
RUN apt install -y nasm
RUN apt install -y xorriso
RUN apt install -y grub-pc-bin
RUN apt install -y grub-common
RUN apt install -y gdc
RUN apt install -y ldc
RUN apt install -y make


VOLUME /root/env
WORKDIR /root/env