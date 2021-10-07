# CMakeのバージョンを設定
cmake_minimum_required(VERSION 3.0)
project(sftp C)

# プロジェクト名と使用する言語を設定
set(CMAKE_TOOLCHAIN_FILE "C:\\Users\\svyut\\AppData\\Local\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")


set(libssh_DIR "C:\\Users\\svyut\\AppData\\Local\\vcpkg\\installed\\x64-windows\\share\\libssh")
find_package(libssh CONFIG REQUIRED)

# a.outという実行ファイルをmain.cppとhello.cppから作成
add_executable(myssh myssh.c)

target_link_libraries(myssh PRIVATE ssh)
