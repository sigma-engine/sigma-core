project(simple-sample)

if(NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pedantic")
endif()

set(SOURCES
	main.cpp)
set(HEADERS)

add_executable(simple-sample ${SOURCES} ${HEADERS})
target_link_libraries(simple-sample engine)
