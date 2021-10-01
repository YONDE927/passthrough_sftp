# CMakeのバージョンを設定
cmake_minimum_required(VERSION 2.8)
# プロジェクト名と使用する言語を設定

set(CMAKE_TOOLCHAIN_FILE "C:/Users/svyut/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(sftp_cmake C)

find_path(LIBSSH_INCLUDE_DIR libssh2.h)
include_directories(${LIBSSH_INCLUDE_DIR})

# To find and use azure-storage-cpp
find_library(LIBSSH_LIB_DIR libssh2)

# a.outという実行ファイルをmain.cppとhello.cppから作成
add_executable(mysftp mysftp.c)
link_libraries(${WASTORAGE_LIBRARY})
project(sftp_cmake C)