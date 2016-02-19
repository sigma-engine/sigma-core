add_package(googletest)

set(SOURCES
    main.cpp
    
    entity/component_manager_tests.cpp
    entity/component_system_manager_tests.cpp
    entity/component_system_tests.cpp
    entity/component_tests.cpp
	entity/default_component_cache_tests.cpp
    entity/entity_manager_tests.cpp
    entity/entity_tests.cpp
)

set(HEADERS
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sigma-five-engine-tests ${HEADERS} ${SOURCES})
target_link_libraries(sigma-five-engine-tests gmock gmock_main sigma-five-engine)
