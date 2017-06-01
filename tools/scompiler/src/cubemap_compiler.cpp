#include <compiler_util.hpp>
#include <cubemap_compiler.hpp>

#include <sigma/graphics/cubemap.hpp>

#include <json/json.h>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/path.hpp>

#include <fstream>
#include <iostream>
#include <sigma/resource/identifier.hpp>
#include <string>

namespace sigma {
bool is_cubemap(boost::filesystem::path file)
{
    return file.extension() == ".cub";
}

bool compile_cubemaps(boost::filesystem::path outputdir, std::vector<boost::filesystem::path> cubemaps)
{
    bool all_good = true;
    for (const auto& file_path : cubemaps) {
        try {
            std::ifstream file{ file_path.string(), std::ios::in };
            Json::Value cubemap_json;
            file >> cubemap_json;

            sigma::resource::identifier rid("cubemap", file_path);
            auto final_path = outputdir / std::to_string(rid.value());

            sigma::graphics::cubemap cube;

            for (auto it = cubemap_json.begin(); it != cubemap_json.end(); ++it) {
                sigma::resource::identifier txt_id{ "texture", (*it).asString() };
                if (it.key() == "right")
                    cube.right = txt_id;
                else if (it.key() == "left")
                    cube.left = txt_id;
                else if (it.key() == "top")
                    cube.top = txt_id;
                else if (it.key() == "bottom")
                    cube.bottom = txt_id;
                else if (it.key() == "back")
                    cube.back = txt_id;
                else if (it.key() == "front")
                    cube.front = txt_id;
            }

            std::ofstream stream{ final_path.string(), std::ios::binary };
            boost::archive::binary_oarchive oa(stream);
            oa << cube;

            touch_stamp_file(outputdir, file_path);
        } catch (...) { // TODO cacth the correct error types and get the messages
            all_good = false;
            std::cerr << "scompiler: error: could not compile cubemap '" << file_path << "'!\n";
        }
    }
    return all_good;
}
}
