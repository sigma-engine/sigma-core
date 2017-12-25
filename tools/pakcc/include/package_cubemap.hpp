#ifndef SIGMA_PACKAGE_CUBEMAP_HPP
#define SIGMA_PACKAGE_CUBEMAP_HPP

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/resource/database.hpp>

#include <boost/filesystem/path.hpp>

void package_cubemaps(
    const sigma::resource::database<sigma::graphics::texture>& texture_database,
    sigma::resource::database<sigma::graphics::cubemap>& cubemap_database,
    const boost::filesystem::path& source_directory);

#endif // SIGMA_PACKAGE_CUBEMAP_HPP
