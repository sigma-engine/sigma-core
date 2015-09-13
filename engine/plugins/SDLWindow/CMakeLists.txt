project(SDL2Window)

set(SOURCES
	src/window.cpp)
set(HEADERS
	include/window.hpp)

add_plugin(SDLWindow ${SOURCES} ${HEADERS})
target_include_directories(SDLWindow PUBLIC include)
target_link_libraries(SDLWindow engine SDL2)
