#ifndef SIGMA_PACKAGE_POST_PROCESS_EFFECT_HPP
#define SIGMA_PACKAGE_POST_PROCESS_EFFECT_HPP

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/resource/database.hpp>

#include <boost/filesystem/path.hpp>

void package_post_process_effects(
    const sigma::resource::database<sigma::graphics::texture>& texture_database,
    const sigma::resource::database<sigma::graphics::cubemap>& cubemap_database,
    const sigma::resource::database<sigma::graphics::technique>& technique_database,
    const sigma::resource::database<sigma::graphics::static_mesh>& static_mesh_database,
    sigma::resource::database<sigma::graphics::post_process_effect>& post_process_effect_database,
    const boost::filesystem::path& source_directory);

#endif // SIGMA_PACKAGE_POST_PROCESS_EFFECT_HPP
