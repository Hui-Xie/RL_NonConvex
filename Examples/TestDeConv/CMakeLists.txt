set(TestDeConvNetSRC DeConvNet.cpp TestDeConvNet.cpp)
add_executable(TestDeConvNet ${TestDeConvNetSRC})
target_link_libraries(TestDeConvNet ${CDLFLib} )
