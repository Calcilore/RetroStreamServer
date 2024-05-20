cmake_minimum_required(VERSION 3.28)
project(RetroServer)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(RetroServer src/Main.cpp
        src/Server.cpp
        src/Server.h
        src/Game/Game.h
        src/Game/Game.cpp
        src/Engine/Vec2.cpp
        src/Engine/Vec2.h
        src/Engine/Vec2I.cpp
        src/Engine/Vec2I.h
        src/Game/TestGame.cpp
        src/Game/TestGame.h
        src/Game/Snake.cpp
        src/Game/Snake.h
        src/PacketTypes.h
)

target_link_libraries(RetroServer ${Boost_LIBRARIES})
