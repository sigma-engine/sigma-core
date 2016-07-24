from conans import ConanFile,CMake

class sigmaEngineConan(ConanFile):
    name = "sigma-engine"
    version = "0.0.1"
    url = "https://github.com/siegelaaron94/sigma-engine"
    settings = "os", "compiler", "build_type", "arch"
    exports = "*"
    requires = "Boost/1.60.0@lasote/stable","glm/0.9.7.6@dlarudgus20/stable","jsoncpp/1.7.3@theirix/stable","assimp/3.3.1@siegelaaron94/stable","FreeImage/3.13.1@siegelaaron94/stable","LibGizmo/1.0.0@siegelaaron94/stable"
    default_options = "Boost:fPIC=True"
    generators = "cmake"

    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake %s %s' % (self.conanfile_directory, cmake.command_line))
        self.run("cmake --build . %s -- -j8" % cmake.build_config)

    def package(self):
        self.copy(pattern="*.hpp", dst="include", src="include")
        self.copy(pattern="*", dst="bin", src="bin")
        self.copy(pattern="*", dst="bin", src="bin")
        self.copy(pattern="*", dst="lib", src="lib")
        self.copy(pattern="*", dst="data", src="data")
        self.copy(pattern="sigma_setup.cmake")

    def package_info(self):
        self.cpp_info.libs = ["sigma-engine"]
