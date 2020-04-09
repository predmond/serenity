#!/bin/bash ../.port_include.sh
port=llvm-project
version=10.0.0
workdir=llvm-project-${version}
files="https://github.com/llvm/llvm-project/releases/download/llvmorg-10.0.0/llvm-project-10.0.0.tar.xz llvm-project-${version}.tar.xz"

build() {
    run cmake \
        -G"Unix Makefiles" \
        -Bbuild \
        -Wno-dev \
        -DCMAKE_TOOLCHAIN_FILE=$SERENITY_ROOT/Toolchain/CMakeToolchain.txt \
        -DLLVM_TARGETS_TO_BUILD=X86 \
        -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" \
        -DCROSS_TOOLCHAIN_FLAGS_NATIVE="" \
        -DUNIX=1 \
        llvm
    run make -C build clangd
}

install() {
    run ls
}
