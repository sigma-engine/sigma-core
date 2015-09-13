#GoogleMock
set(gtest_force_shared_crt ON)
set(gtest_disable_pthreads ON)
add_subdirectory(googletest/googlemock)

#SDL2
set(LIBC ON)
set(SDL_SHARED ON)
set(SDL_STATIC OFF)
set(VIDEO_OPENGLES OFF)

if (MSVC)
	set(EXTRA_CFLAGS /arch:SSE)
else ()
	set(RENDER_D3D OFF)
endif ()

add_subdirectory(SDL)
