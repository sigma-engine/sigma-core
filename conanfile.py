from conans import ConanFile, CMake


class SigmaEngineConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "glm/0.9.7.6@dlarudgus20/stable", "Boost/1.61.0@siegelaaron94/develop", "assimp/3.3.1@siegelaaron94/develop", "jsoncpp/1.8.0@siegelaaron94/develop", "libpng/1.6.21@lasote/stable", "libtiff/4.0.6@bilke/stable", "libjpeg/9a@Kaosumaru/stable", "SDL2/2.0.5@lasote/stable", "gtest/master@smspillaz/gtest", "glad/master@siegelaaron94/develop"
    generators = "cmake", "txt"
    default_options = "Boost:fPIC=True", "jsoncpp:fPIC=True"

    def imports(self):
        if self.settings.os == "Windows":
            self.copy("*.dll", dst="bin", src="bin")
        elif self.settings.os == "Macos":
            self.copy("*.dylib*", dst="bin", src="lib")

    def build(self):
        cmake = CMake(self.settings)
        cmake.configure(self, build_dir=".")
        cmake.configure(self, build_dir=".")
        cmake.build(self)
