#!/usr/bin/env python3


def ABS_PATH(path):
    from os.path import abspath
    return abspath(path)


def JOIN_PATH(left, *right):
    from os.path import join
    return join(left, *right)


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

def RM_FILES_RECUR(path, *masks):
    from glob import glob
    for mask in masks:
        files = glob(path + '/**/' + mask, recursive=False)
        for file in files:
            RM_FILE(file)

def FIND_FILES(path, *masks, recur = False):
    from glob import glob
    files = []
    for mask in masks:
        files += glob(path + '/**/' + mask, recursive=recur)
    return files

# ========== SCRIPT ========== #


build_dir = ABS_PATH("./build")


def welcome():
    print("Now we install all dependencies for your project! \n")
    if not EXISTS(build_dir):
        MK_DIRS(build_dir)


def conan():
    print("\n\nConan install")
    print("========================")
    cmd = "conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan"
    EXECUTE(cmd)

    conan_dir = JOIN_PATH(build_dir, 'conan')
    RM_DIRS(conan_dir)
    MK_DIRS(conan_dir)
    cmd = str.format("conan install ./conanfile.txt -if {} --build=missing", conan_dir)
    EXECUTE(cmd)
    print("========== Conan installed ==========")


def gtest():
    print("\n\nGtest install")
    print("========================")
    gtest_dir = JOIN_PATH(build_dir, 'gtest')
    RM_DIRS(gtest_dir)
    MK_DIRS(gtest_dir)
    cmd = str.format(
        "git clone https://github.com/google/googletest.git {}", gtest_dir)
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
    github_url = 'https://www.lua.org/ftp/lua-5.3.5.tar.gz' 
    cmd = str.format(
        "wget {} -O {}", github_url, lua_tar)
    EXECUTE(cmd)

    cmd = str.format("tar xzf {} -C {}", lua_tar, build_dir)
    EXECUTE(cmd)
    RM_FILE(lua_tar)

    cmd = str.format("make linux local -C {}", lua_dir)
    EXECUTE(cmd)

    lua_install_dir = JOIN_PATH(lua_dir, 'install')
    lua_include_dir = JOIN_PATH(lua_install_dir, 'include')
    lua_include_lua_dir = JOIN_PATH(lua_include_dir, 'lua')
    MK_DIRS(lua_include_lua_dir)
    cmd = str.format("mv {}/*.h* {}", lua_include_dir, lua_include_lua_dir)
    EXECUTE(cmd)
    print("========== lua installed ==========")


def sol2():
    print("\n\nSol2 install")
    print("========================")
    sol2_dir = JOIN_PATH(build_dir, 'sol2')
    RM_DIRS(sol2_dir)
    MK_DIRS(sol2_dir)
    github_url = 'https://github.com/ThePhD/sol2.git'
    cmd = str.format("git clone {} {}", github_url, sol2_dir)
    EXECUTE(cmd)

    sol2_single_build_dir = JOIN_PATH(sol2_dir, 'single')
    cmd = str.format('cd {0} && python3 {0}/single.py', sol2_single_build_dir)
    EXECUTE(cmd)
    print("========== Sol2 installed ==========")


if __name__ == "__main__":
    welcome()
    conan()
    gtest()
    lua()
    sol2()












