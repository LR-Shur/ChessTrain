cmake_minimum_required(VERSION 3.10.0)
project(chess VERSION 0.1.0 LANGUAGES C CXX)

# 设置c++版本和强制执行
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 添加头文件
include_directories(include)

# qt相关
find_package(Qt6 REQUIRED COMPONENTS WIdgets)

# 主可执行文件
add_executable(chess main.cpp src/game/chessgame.cpp)
target_link_libraries(chess)

# 测试相关
include(CTest)
enable_testing()
add_executable(test_chess tests/test_game.cpp src/game/chessgame.cpp)

# 添加测试
add_test(NAME ChessTest COMMAND test_chess)