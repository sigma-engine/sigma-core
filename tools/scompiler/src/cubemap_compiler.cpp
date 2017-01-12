#include <compiler_util.hpp>
#include <cubemap_compiler.hpp>

#include <sigma/graphics/cubemap.hpp>

#include <json/json.h>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

namespace sigma {
bool is_cubemap(boost::filesystem::path file)
{
    return file.extension() == ".cub";
}

void load_texture(boost::filesystem::path data_dir, sigma::resource::identifier id, sigma::graphics::texture_data& texture)
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

            sigma::resource::development_identifier rid("cubemap", file_path);
            auto final_path = outputdir / std::to_string(rid.value());

            sigma::graphics::cubemap_data cube;
            load_texture(outputdir, sigma::resource::identifier{ "texture", cubemap_json["right"].asString() }, cube.right);
            load_texture(outputdir, sigma::resource::identifier{ "texture", cubemap_json["left"].asString() }, cube.left);
            load_texture(outputdir, sigma::resource::identifier{ "texture", cubemap_json["top"].asString() }, cube.top);
            load_texture(outputdir, sigma::resource::identifier{ "texture", cubemap_json["bottom"].asString() }, cube.bottom);
            load_texture(outputdir, sigma::resource::identifier{ "texture", cubemap_json["back"].asString() }, cube.back);
            load_texture(outputdir, sigma::resource::identifier{ "texture", cubemap_json["front"].asString() }, cube.front);

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
