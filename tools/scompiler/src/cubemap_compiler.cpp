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

void load_texture(boost::filesystem::path data_dir, sigma::resource::identifier id, sigma::graphics::texture& texture)
{
    auto input_path = data_dir / std::to_string(id.value());
    std::ifstream stream{ input_path.string(), std::ios::binary };
    boost::archive::binary_iarchive oa(stream);
    oa >> texture;
}

bool compile_cubemaps(boost::filesystem::path outputdir, std::vector<boost::filesystem::path> cubemaps)
{
    bool all_good = true;
    for (const auto& file_path : cubemaps) {
        if (!resource_has_changes(outputdir, file_path))
            continue;

        std::cout << file_path.filename().string() << std::endl;
        try {
            std::ifstream file{ file_path.string(), std::ios::in };
            Json::Value cubemap_json;
            file >> cubemap_json;

            sigma::resource::identifier rid("cubemap", file_path);
            auto final_path = outputdir / std::to_string(rid.value());

            sigma::graphics::cubemap cube;
            std::vector<boost::filesystem::path> dependencies;

            for (auto it = cubemap_json.begin(); it != cubemap_json.end(); ++it) {
                sigma::resource::identifier txt_id{ "texture", (*it).asString() };
                dependencies.push_back(outputdir / std::to_string(txt_id.value()));
                if (it.key() == "right")
                    load_texture(outputdir, txt_id, cube.right);
                else if (it.key() == "left")
                    load_texture(outputdir, txt_id, cube.left);
                else if (it.key() == "top")
                    load_texture(outputdir, txt_id, cube.top);
                else if (it.key() == "bottom")
                    load_texture(outputdir, txt_id, cube.bottom);
                else if (it.key() == "back")
                    load_texture(outputdir, txt_id, cube.back);
                else if (it.key() == "front")
                    load_texture(outputdir, txt_id, cube.front);
            }

            std::ofstream stream{ final_path.string(), std::ios::binary };
            boost::archive::binary_oarchive oa(stream);
            oa << cube;

            touch_stamp_file(outputdir, file_path, dependencies);
        } catch (...) { // TODO cacth the correct error types and get the messages
            all_good = false;
            std::cerr << "scompiler: error: could not compile cubemap '" << file_path << "'!\n";
        }
    }
    return all_good;
}
}
