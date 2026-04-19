ARG VARIANT="focal"
FROM ubuntu:${VARIANT}

# Restate the variant to use it later on in the llvm and cmake installations
ARG VARIANT

# Install necessary packages available from standard repos
RUN apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
        software-properties-common wget apt-utils file zip \
        openssh-client gpg-agent socat rsync \
        make ninja-build git \
        python3 python3-pip

# Add gcc-${GCC_VER}
ARG GCC_VER="11"
RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
        gcc-${GCC_VER} g++-${GCC_VER} gdb

# Set gcc-${GCC_VER} as default gcc
RUN update-alternatives --install /usr/bin/gcc gcc $(which gcc-${GCC_VER}) 100
RUN update-alternatives --install /usr/bin/g++ g++ $(which g++-${GCC_VER}) 100

# Add clang-${LLVM_VER}
ARG LLVM_VER="14"
ARG LLVM_URL="http://apt.llvm.org/${VARIANT}/"
ARG LLVM_PKG="llvm-toolchain-${VARIANT}-${LLVM_VER}"
RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - 2>/dev/null && \
    add-apt-repository -y "deb ${LLVM_URL} ${LLVM_PKG} main" && \
    apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
        clang-${LLVM_VER} lldb-${LLVM_VER} lld-${LLVM_VER} clangd-${LLVM_VER} \
        llvm-${LLVM_VER}-dev libclang-${LLVM_VER}-dev clang-tidy-${LLVM_VER}

# Set the default clang-tidy, so CMake can find it
RUN update-alternatives --install /usr/bin/clang-tidy clang-tidy $(which clang-tidy-${LLVM_VER}) 1

# Set clang-${LLVM_VER} as default clang
RUN update-alternatives --install /usr/bin/clang clang $(which clang-${LLVM_VER}) 100
RUN update-alternatives --install /usr/bin/clang++ clang++ $(which clang++-${LLVM_VER}) 100

# Install cmake-${CMAKE_VER}
ARG CMAKE_VER=3.21.7
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VER}/cmake-${CMAKE_VER}-linux-x86_64.tar.gz && \
    tar -xzf cmake-${CMAKE_VER}-linux-x86_64.tar.gz && \
    mv cmake-${CMAKE_VER}-linux-x86_64 /opt/cmake && \
    update-alternatives --install /usr/bin/cmake cmake /opt/cmake/bin/cmake 100 && \
    update-alternatives --install /usr/bin/ctest ctest /opt/cmake/bin/ctest 100 && \
    update-alternatives --install /usr/bin/cpack cpack /opt/cmake/bin/cpack 100 && \
    update-alternatives --install /usr/bin/ccmake ccmake /opt/cmake/bin/ccmake 100

# Install editors
RUN apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
        neovim emacs nano

# Install optional dependecies
RUN apt-get update -qq && export DEBIAN_FRONTEND=noninteractive && \
    apt-get install -y --no-install-recommends \
        doxygen graphviz ccache cppcheck

## Cleanup cached apt data we don't need anymore
RUN apt-get autoremove -y && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Install conan
RUN python3 -m pip install --upgrade pip setuptools && \
    python3 -m pip install conan && \
    conan --version && \
    conan profile detect

ENV CONAN_SYSREQUIRES_SUDO 0
ENV CONAN_SYSREQUIRES_MODE enabled

# Include project
ADD . /workspace/
WORKDIR /workspace/

CMD ["/bin/bash"]
