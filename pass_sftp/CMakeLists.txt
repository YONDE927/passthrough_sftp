# CMakeのバージョンを設定
cmake_minimum_required(VERSION 3.0)
project(sftp C)

# プロジェクト名と使用する言語を設定
set(CMAKE_TOOLCHAIN_FILE "C:\\Users\\svyut\\AppData\\Local\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")
set(CMAKE_BUILD_TYPE Debug)


set(libssh_DIR "C:\\Users\\svyut\\AppData\\Local\\vcpkg\\installed\\x64-windows\\share\\libssh")
find_package(libssh CONFIG REQUIRED)

#link_directories("C:\\Program Files (x86)\\WinFsp\\lib")

add_executable(sftp sftp.c)

#target_include_directories(passsftp PRIVATE "C:\\Program Files (x86)\\WinFsp\\inc")
target_link_libraries(sftp PRIVATE ssh)
