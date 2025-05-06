cmake_minimum_required(VERSION 3.10)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

if(POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

# Указываем проект
project(ComputerClub)

# Указываем стандарт
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})

# Добавляем исполняемый файл
add_executable(computer_club main.cpp MyTime.cpp ClubState.cpp)
