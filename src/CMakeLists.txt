cmake_minimum_required(VERSION 3.14)
project(tanX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb3 -gdwarf-2")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra -O3 -g0")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-s")

set(HEADERS bullet.hxx
    client.hxx
    config.hxx
    display.hxx
    item.hxx
    tank.hxx
    terrain.hxx
    timer.hxx
)

add_executable(tanX bullet.cxx
    client.cxx
    display.cxx
    game.cxx
    item.cxx
    tank.cxx
    terrain.cxx
    timer.cxx
    ${HEADERS}
)
target_link_libraries(tanX pthread SDL SDL_mixer)

add_executable(lnX server.cxx ${HEADERS})
target_link_libraries(lnX pthread)
