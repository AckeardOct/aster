#!/usr/bin/env python

def ABS_PATH(path):
    from os.path import abspath
    return abspath(path)

def JOIN_PATH(left, right):
    from os.path import join
    return join(left, right)

def EXISTS(path):
    from os.path import exists
    return exists(path)

def MK_DIRS(path):
    from os import makedirs
    return makedirs(path)

def RM_DIRS(path):
    from shutil import rmtree
    if EXISTS(path):
        rmtree(path)

def RM_FILE(path):
    from os import remove
    if EXISTS(path):
        remove(path)

def EXECUTE(cmd):
    from os import system
    print('========== EXECUTE COMMAND: ')
    print(cmd)
    print('====================')
    return system(cmd)

# ========== SCRIPT ========== #

build_dir = ABS_PATH("./build")


def welcome():
    print("Now we install all dependencies for your project! \n")
    if not EXISTS(build_dir):
        MK_DIRS(build_dir)


def conan():
    print("\n\nConan install")
    print("========================")
    conan_dir = JOIN_PATH(build_dir, 'conan')
    RM_DIRS(conan_dir)
    MK_DIRS(conan_dir)
    cmd = str.format("conan install ./conanfile.txt -if {}", conan_dir)
    EXECUTE(cmd)
    print("========== Conan installed ==========")


def gtest():
    print("\n\nGtest install")
    print("========================")
    gtest_dir = JOIN_PATH(build_dir, 'gtest')
    RM_DIRS(gtest_dir)
    MK_DIRS(gtest_dir)
    cmd = str.format("git clone https://github.com/google/googletest.git {}", gtest_dir)
    EXECUTE(cmd)

    gtest_build_dir = JOIN_PATH(gtest_dir, 'build')
    MK_DIRS(gtest_build_dir)
    cmd = str.format("cmake -S {} -B {}", gtest_dir, gtest_build_dir)
    EXECUTE(cmd)

    cmd = str.format("make -C {}", gtest_build_dir)
    EXECUTE(cmd)
    print("========== gtest installed ==========")


def lua():
    print("\n\nLua install")
    print("========================")
    lua_dir = JOIN_PATH(build_dir, 'lua-5.3.5')
    RM_DIRS(lua_dir)

    lua_tar = lua_dir + ".tar.gz"
    cmd = str.format("wget 'https://www.lua.org/ftp/lua-5.3.5.tar.gz' -O {}", lua_tar)
    EXECUTE(cmd)

    cmd = str.format("tar xzf {} -C {}", lua_tar, build_dir)
    EXECUTE(cmd)
    RM_FILE(lua_tar)

    cmd = str.format("make linux local -C {}", lua_dir)
    EXECUTE(cmd)
    print("========== lua installed ==========")


if __name__ == "__main__":
    welcome()
    conan()
    gtest()
    lua()