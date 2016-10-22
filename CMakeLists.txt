cmake_minimum_required(VERSION 3.6)
project(ProjetTutore1)

add_definitions(-std=c89 -pedantic-errors)
set_property(TARGET PROPERTY C_STANDARD 89)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES dev/mainTest.c dev/MyString.c dev/EncryptDecrypt.c dev/DocumentUtil.c dev/Operatortable.c dev/Operatortable.h)
add_executable(ProjetTutore1 ${SOURCE_FILES})