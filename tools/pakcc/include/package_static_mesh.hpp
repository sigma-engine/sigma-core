#ifndef SIGMA_PACKAGE_STATIC_MESH_HPP
#define SIGMA_PACKAGE_STATIC_MESH_HPP

#include <sigma/graphics/material.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/resource/database.hpp>

#include <boost/filesystem/path.hpp>

void package_static_meshes(
    const sigma::resource::database<sigma::graphics::material>& material_database,
    sigma::resource::database<sigma::graphics::static_mesh>& static_mesh_database,
    const boost::filesystem::path& source_directory);

#endif // SIGMA_PACKAGE_STATIC_MESH_HPP
