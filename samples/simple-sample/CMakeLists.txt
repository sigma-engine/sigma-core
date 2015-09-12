project(simple-sample)

set(SOURCES
    main.cpp
)
set(HEADERS
)

add_executable(simple-sample ${SOURCES} ${HEADERS})
target_link_libraries(simple-sample engine)
