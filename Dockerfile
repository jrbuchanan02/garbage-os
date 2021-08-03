# syntax=docker/dockerfile:1
# Note: I set the tag on my machine to garbageos-buildenv

FROM ubuntu
RUN apt update
RUN apt full-upgrade -y
# grab nasm directly?
RUN apt install -y nasm
#xorriso is a prerequisite for grub-pc-bin and grub-common
RUN apt install -y xorriso
RUN apt install -y grub-pc-bin
RUN apt install -y grub-common
#build-essential and ubuntu-dev-tools contain important compilers and stuff
RUN apt install -y build-essential
#RUN apt install -y ubuntu-dev-tools

VOLUME /root/env
WORKDIR /root/env