# Board library
add_library(board STATIC
    cpp/board.cpp
)

# Game Client executable
add_executable(game_client
    cpp/game_client.cpp
)
target_link_libraries(game_client
    board
    ${CMAKE_THREAD_LIBS_INIT}
)

# Game Server executable
add_executable(game_server
    cpp/game_server.cpp
)
target_link_libraries(game_server
    board
    ${CMAKE_THREAD_LIBS_INIT}
)

# Game Bot executable
add_executable(game_bot
    cpp/game_bot.cpp
)
target_link_libraries(game_bot
    board
    ${CMAKE_THREAD_LIBS_INIT}
)

# Genetic Optimizer executable
add_executable(genetic_optimizer
    cpp/genetic.cpp
)
target_link_libraries(genetic_optimizer
    board
    ${CMAKE_THREAD_LIBS_INIT}
)