cmake_minimum_required(VERSION 3.10.0)
project(chess VERSION 0.1.0 LANGUAGES C CXX)

# 设置c++版本和强制执行
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 添加头文件
include_directories(include)

# qt相关
find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(SOURCES
    src/gui/main.cpp
    src/gui/BoardWidget.cpp
    src/gui/mainwindow.cpp
    src/game/chessgame.cpp
    src/ai/ChessAi.cpp
)
set(HEADERS
    src/gui/BoardWidget.h
    src/gui/mainwindow.h
    include/game/ChessGame.h
    include/ai/ChessAi.h
)

# 主可执行文件
add_executable(chess ${SOURCES} ${HEADERS})
target_link_libraries(chess Qt6::Widgets)

# 测试相关
include(CTest)
enable_testing()
add_executable(test_chess tests/test_game.cpp src/game/chessgame.cpp
    src/gui/mainwindow.h src/gui/mainwindow.cpp src/gui/mainwindow.ui
    src/gui/BoardWidget.cpp src/ai/ChessAi.cpp)
target_link_libraries(test_chess Qt6::Widgets) # 添加测试
add_test(NAME ChessTest COMMAND test_chess)
