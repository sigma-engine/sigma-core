from conans import ConanFile, CMake


class SigmaEngineConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = (
        "glm/0.9.7.6@dlarudgus20/stable"
        , "Boost/1.60.0@lasote/stable"
        , "jsoncpp/1.7.3@theirix/stable"
        , "libpng/1.6.21@lasote/stable"
        , "libtiff/4.0.6@bilke/stable"
        , "libjpeg/9a@Kaosumaru/stable"
        , "SDL2/2.0.5@lasote/stable"
        , "glad/master@siegelaaron94/develop"
        , "gtest/1.8.0@lasote/stable"
        , "google-benchmark/1.1.0@cpace6/stable"
        , "assimp/3.3.1@siegelaaron94/develop"
    )
    generators = "cmake", "txt"
    default_options = "gtest:shared=False" , "Boost:without_test=True"

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
