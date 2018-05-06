cmake_minimum_required(VERSION 3.10)
project(LibraryManagement)

set(CMAKE_CXX_STANDARD 17)

add_executable(LibraryManagement PRIVATE.h main.cpp user.cpp user.h menu.cpp menu.h admin.cpp admin.h return.cpp return.h loan.cpp loan.h)

find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)

target_include_directories(LibraryManagement PUBLIC ${LIBMONGOCXX_INCLUDE_DIRS})
target_include_directories(LibraryManagement PUBLIC ${LIBBSONCXX_INCLUDE_DIRS})
target_link_libraries(LibraryManagement ${LIBMONGOCXX_LIBRARIES} ${LIBBSONCXX_LIBRARIES})