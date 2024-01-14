cmake_minimum_required(VERSION 3.0)

project(TicTacToeAPP)

add_executable(TicTacToeAPP
    source/main.cpp
    source/Application/GamesContainer.cpp
    source/Application/Game.cpp
    source/Application/Board.cpp
    source/Application/Entanglement.cpp
    source/Application/Tile.cpp
    source/API/GameResourceFactory.cpp
    source/API/CreateGameResourceFactory.cpp
    source/API/MakeMoveResourceFactory.cpp
    source/API/GameServiceSettingsFactory.cpp
    source/API/GameService.cpp
    )


target_link_libraries(TicTacToeAPP restbed)

target_include_directories(TicTacToeAPP PUBLIC ${CMAKE_SOURCE_DIR}/headers/Application ${CMAKE_SOURCE_DIR}/headers/API)

set_property(TARGET TicTacToeAPP PROPERTY CXX_STANDARD 20)


Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests
    tests/all_tests.cpp
    source/Application/GamesContainer.cpp
    source/Application/Game.cpp
    source/Application/Board.cpp
    source/Application/Entanglement.cpp
    source/Application/Tile.cpp
    )

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/headers/Application)


target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

