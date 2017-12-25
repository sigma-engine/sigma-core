#ifndef SIGMA_PACKAGE_MATERIAL_HPP
#define SIGMA_PACKAGE_MATERIAL_HPP

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/resource/database.hpp>

#include <boost/filesystem/path.hpp>

void package_materials(
    const sigma::resource::database<sigma::graphics::texture>& texture_database,
    const sigma::resource::database<sigma::graphics::cubemap>& cubemap_database,
    const sigma::resource::database<sigma::graphics::technique>& technique_database,
    sigma::resource::database<sigma::graphics::material>& material_database,
    const boost::filesystem::path& source_directory);

#endif // SIGMA_PACKAGE_MATERIAL_HPP
