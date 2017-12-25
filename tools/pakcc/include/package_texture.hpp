#ifndef SIGMA_PACKAGE_TEXTURE_HPP
#define SIGMA_PACKAGE_TEXTURE_HPP

#include <sigma/graphics/texture.hpp>
#include <sigma/resource/database.hpp>

#include <boost/filesystem/path.hpp>

void package_textures(
    sigma::resource::database<sigma::graphics::texture>& texture_database,
    const boost::filesystem::path& source_directory);

#endif // SIGMA_PACKAGE_TEXTURE_HPP
