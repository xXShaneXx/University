cmake_minimum_required(VERSION 3.10)
project(TicTacToeBot)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/include)

# Main executable
add_executable(TicTacToeBot
    src/main/main.cpp
    src/cpp/Bot.cpp
)

# Enable testing
enable_testing()

# Bot tests
add_executable(test_bot 
    src/tests/test_bot.cpp
    src/cpp/Bot.cpp
)

# Register test with CTest
add_test(NAME BotWinLoseTests COMMAND test_bot)