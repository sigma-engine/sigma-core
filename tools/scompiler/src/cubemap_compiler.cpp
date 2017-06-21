#include <compiler_util.hpp>
#include <cubemap_compiler.hpp>

#include <sigma/graphics/cubemap.hpp>
#include <sigma/util/filesystem.hpp>

#include <json/json.h>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <fstream>
#include <iostream>
#include <string>

namespace sigma {
bool is_cubemap(boost::filesystem::path file)
{
    return file.extension() == ".cub";
}

bool compile_cubemaps(boost::filesystem::path outputdir, boost::filesystem::path sourcedir, std::vector<boost::filesystem::path> cubemaps)
{
    bool all_good = true;
    for (const auto& file_path : cubemaps) {
        try {
            std::ifstream file{ file_path.string(), std::ios::in };
            Json::Value cubemap_json;
            file >> cubemap_json;

            auto rid = boost::filesystem::path{ "cubemap" } / sigma::filesystem::make_relative(sourcedir, file_path).replace_extension("");
            auto final_path = outputdir / rid;

            sigma::graphics::cubemap cube;

            for (auto it = cubemap_json.begin(); it != cubemap_json.end(); ++it) {
                auto txt_id = boost::filesystem::path{ "texture" } / (*it).asString();
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

            auto output_folder = final_path.parent_path();
            if (!boost::filesystem::exists(output_folder))
                boost::filesystem::create_directories(output_folder);

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
