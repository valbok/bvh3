set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(KDopTest KDopTest.cpp)
target_link_libraries(KDopTest gtest KDop)
