set(TESTS_SRCS
    math/mat2_tests.cpp
    math/mat3_tests.cpp
    math/mat4_tests.cpp
    math/vec2_tests.cpp
    math/vec3_tests.cpp
    math/vec4_tests.cpp
    math/quaternion_tests.cpp
    math/util_tests.cpp
    main.cpp
)
set(TESTS_HDRS
)

add_executable(sigma-five-tests ${TESTS_SRCS} ${TESTS_HDRS})
target_link_libraries(sigma-five-tests gmock gmock_main sigma-five)