set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(NodeTest NodeTest.cpp)
target_link_libraries(NodeTest gtest KDop)
