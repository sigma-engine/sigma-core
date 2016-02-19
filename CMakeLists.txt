set(SOURCES
    src/sigmafive/engine.cpp
	src/sigmafive/plugin_manager.cpp

    src/sigmafive/game/game.cpp
    src/sigmafive/game/static_mesh_component.cpp
    src/sigmafive/game/static_mesh_component_system.cpp
    src/sigmafive/game/transform_component.cpp

    src/sigmafive/graphics/context.cpp
    src/sigmafive/graphics/context_manager.cpp
    src/sigmafive/graphics/static_mesh.cpp

    src/sigmafive/resource/resource.cpp

    src/sigmafive/system/window.cpp
    src/sigmafive/system/window_manager.cpp

    src/sigmafive/resource2/identifier.cpp
    src/sigmafive/resource2/package_manager.cpp
    src/sigmafive/resource2/resource_manager.cpp
	src/sigmafive/resource2/filesystem_package.cpp

    src/sigmafive/entity/entity.cpp
	src/sigmafive/entity/entity_manager.cpp
	src/sigmafive/entity/component_manager.cpp

	src/sigmafive/entity/component_system.cpp
	src/sigmafive/entity/component_system_manager.cpp
)

set(HEADERS
    include/sigmafive/config.hpp
    include/sigmafive/engine.hpp
	include/sigmafive/plugin_manager.hpp

    include/sigmafive/game/game.hpp
    include/sigmafive/game/static_mesh_component.hpp
    include/sigmafive/game/static_mesh_component_system.hpp
    include/sigmafive/game/transform_component.hpp

    include/sigmafive/graphics/context.hpp
    include/sigmafive/graphics/context_manager.hpp
    include/sigmafive/graphics/static_mesh.hpp

    include/sigmafive/resource/resource.hpp

    include/sigmafive/system/window.hpp
    include/sigmafive/system/window_manager.hpp


    include/sigmafive/resource2/cache.hpp
    include/sigmafive/resource2/identifier.hpp
    include/sigmafive/resource2/package.hpp
    include/sigmafive/resource2/package_manager.hpp
    include/sigmafive/resource2/resource_manager.hpp
	include/sigmafive/resource2/filesystem_package.hpp

    include/sigmafive/entity/entity.hpp
	include/sigmafive/entity/entity_manager.hpp

	include/sigmafive/entity/component.hpp
	include/sigmafive/entity/component_cache.hpp
	include/sigmafive/entity/default_component_cache.hpp
	include/sigmafive/entity/component_cache_factory.hpp

	include/sigmafive/entity/component_manager.hpp

	include/sigmafive/entity/component_system.hpp
	include/sigmafive/entity/component_system_manager.hpp

)

add_library(sigma-five-engine SHARED ${SOURCES} ${HEADERS})

target_include_directories(sigma-five-engine PUBLIC include)

target_compile_definitions(sigma-five-engine
PUBLIC
    -D$<$<CONFIG:Debug>:SIGMA_FIVE_DEBUG>
PRIVATE
    -DSIGMA_FIVE_ENGINE_EXPORT
)

if(NOT MSVC)
	target_compile_options(sigma-five-engine PUBLIC -std=c++14 -pedantic -fvisibility=hidden)
endif()

target_link_libraries(sigma-five-engine cppbr-math cppbr-meta cppbr-cache ${Boost_system_LIBRARY} ${Boost_filsystem_LIBRARY})

if(UNIX)
	target_link_libraries(sigma-five-engine dl)
endif()

#add_subdirectory(tests)
