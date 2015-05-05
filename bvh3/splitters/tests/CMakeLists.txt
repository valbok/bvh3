set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(SplitterByCenterTest SplitterByCenterTest.cpp)
target_link_libraries(SplitterByCenterTest gtest KDop)
