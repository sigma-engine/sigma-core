if(PACK_CPP_CAHCE_DIR)
    return()
else()
    find_package(Git)
    if(GIT_FOUND)
        set(PACK_CPP_CAHCE_DIR "${CMAKE_SOURCE_DIR}/.pack-cpp")
        set(PACK_CPP_PACKAGE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/.pack-cpp/packages")
        set(PACK_CPP_PACKAGE_BUILD_DIR "${CMAKE_BINARY_DIR}" )
        if(EXISTS "${PACK_CPP_CAHCE_DIR}" AND IS_DIRECTORY "${PACK_CPP_CAHCE_DIR}")
            execute_process(COMMAND "${GIT_EXECUTABLE}" pull WORKING_DIRECTORY "${PACK_CPP_CAHCE_DIR}")
        else()
            execute_process(COMMAND "${GIT_EXECUTABLE}" clone https://github.com/siegelaaron94/pack-cpp.git .pack-cpp WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
        endif()
        include("${PACK_CPP_CAHCE_DIR}/setup.cmake")
    else()
      message(SEND_ERROR "git not found")
    endif()
endif()
