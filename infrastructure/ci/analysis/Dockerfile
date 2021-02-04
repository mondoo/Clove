FROM ubuntu:20.04

RUN apt-get update -y --fix-missing
RUN apt-get install -y

#Install utilities
RUN apt-get install -y git
RUN apt-get install -y python3

#Install static analysis tools
RUN apt-get install -y cppcheck
RUN apt-get install -y clang-tidy

CMD [ "/bin/bash" ]