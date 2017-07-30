set(SOURCES
    src/main.cpp
)
set(HEADERS
    include/hdr_io.hpp
)

add_executable(texcc ${SOURCES} ${HEADERS})
target_include_directories(texcc PUBLIC include)
target_link_libraries(texcc sigma-core)
