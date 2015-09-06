#GoogleMock
set(gtest_force_shared_crt ON)
set(gtest_disable_pthreads ON)
add_subdirectory(${THIRDPARTY_DIR}/googletest/googlemock ${CMAKE_BINARY_DIR}/thirdparty)

set(TESTS_SRCS
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
    game/entity_world_tests.cpp
    game/fake_components.cpp
    game/component_system_tests.cpp
    game/component_manager_tests.cpp
    game/entity_manager_tests.cpp
    game/component_system_manager_tests.cpp
)
set(TESTS_HDRS
    game/fake_components.hpp
)

add_executable(sigmafive-tests ${TESTS_HDRS} ${TESTS_SRCS})
target_link_libraries(sigmafive-tests gmock gmock_main sigmafive)
target_include_directories(sigmafive-tests
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${THIRDPARTY_DIR}/googletest/googlemock/include
    ${THIRDPARTY_DIR}/googletest/googletest/include
)
