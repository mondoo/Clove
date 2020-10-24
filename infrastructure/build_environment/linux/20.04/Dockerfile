FROM gcc:10 as builder

RUN apt-get install -y git

#Get cmake build requirements
RUN apt-get install -y make
RUN apt-get install -y libssl-dev

#Build Cmake
RUN git clone --branch v3.18.3 --depth 1 https://gitlab.kitware.com/cmake/cmake.git
WORKDIR /cmake/
RUN ./bootstrap --prefix=/cmake_install
RUN make 
RUN make install

FROM ubuntu:20.04

RUN apt-get update -y
RUN apt-get install -y

#Install any utilities
RUN apt-get install -y git
RUN apt-get install -y make
RUN apt-get install -y wget

#Install the compilers
RUN apt-get install -y g++-10
RUN apt-get install -y clang-10

#Install dotnet
RUN wget https://packages.microsoft.com/config/ubuntu/20.04/packages-microsoft-prod.deb -O packages-microsoft-prod.deb
RUN dpkg -i packages-microsoft-prod.deb
RUN apt-get update -y

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get install -y dotnet-sdk-3.1

#Install cmake
COPY --from=builder /cmake_install /cmake_install
ENV PATH "/cmake_install/bin:$PATH"

#Install X11
RUN apt-get install -y libx11-dev

#Install OpenGL
RUN apt-get install -y libgl1-mesa-glx
RUN apt-get install -y libgl1-mesa-dev

CMD [ "/bin/bash" ]