project(sdl_window)

set(SOURCES
	src/window.cpp)
set(HEADERS
	include/window.hpp)

add_plugin(sdl_window ${SOURCES} ${HEADERS})
target_include_directories(sdl_window PUBLIC include)
target_link_libraries(sdl_window engine SDL2)
