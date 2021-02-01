FROM ubuntu:20.04

RUN apt-get update -y --fix-missing
RUN apt-get install -y

#Install utilities
RUN apt-get install -y git

#Install static analysis tools
RUN apt-get install -y cppcheck

CMD [ "/bin/bash" ]