set(SOURCES
    main.cpp
    object_tests.cpp

    math/mat2_tests.cpp
    math/mat3_tests.cpp
    math/mat4_tests.cpp
    math/util_tests.cpp
    math/vec2_tests.cpp
    math/vec3_tests.cpp
    math/vec4_tests.cpp
    math/quaternion_tests.cpp

    system/resource_tests.cpp
    system/resource_manager_tests.cpp

    game/entity_tests.cpp
    game/component_tests.cpp
    game/fake_components.cpp
    game/component_system_tests.cpp
    game/component_manager_tests.cpp
    game/entity_manager_tests.cpp
    game/component_system_manager_tests.cpp
)
set(HEADERS
    game/fake_components.hpp
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sigmafive-tests ${HEADERS} ${SOURCES})
target_link_libraries(sigmafive-tests gmock gmock_main sigmafive)