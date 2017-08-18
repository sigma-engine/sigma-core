#ifndef SIGMA_ENGINE_MATERIAL_HPP
#define SIGMA_ENGINE_MATERIAL_HPP

#include <sigma/graphics/material.hpp>
#include <sigma/graphics/opengl/technique.hpp>

#define MATERIAL_PTR(material_mgr, x) material_mgr.acquire(x)

namespace sigma {
namespace opengl {
    class material {
    public:
        material(technique_manager& technique_mgr, const graphics::material& data);

        material(material&&) = default;

        material& operator=(material&&) = default;

        resource::handle<graphics::technique> technique;
        const graphics::material& data;

    private:
        material(const material&) = delete;
        material& operator=(const material&) = delete;
    };

    class material_manager {
    public:
        // TODO remove the use of unique_ptr

        material_manager(technique_manager& techniques, resource::cache<graphics::material>& material_cache)
            : techniques_(techniques)
            , material_cache_(material_cache)
        {
        }

        resource::handle<graphics::material> get(const boost::filesystem::path& id)
        {
            return material_cache_.get(id);
        }

        opengl::material* acquire(const resource::handle<graphics::material>& hndl)
        {
            // TODO not thread safe
            if (hndl.index >= materials_.size())
                materials_.resize(hndl.index + 1);

            if (materials_[hndl.index] == nullptr)
                materials_[hndl.index] = std::make_unique<material>(techniques_, *material_cache_.acquire(hndl));

            return materials_.at(hndl.index).get();
        }

    private:
        technique_manager& techniques_;
        resource::cache<graphics::material>& material_cache_;
        std::vector<std::unique_ptr<material>> materials_;
    };
}
}

#endif // SIGMA_ENGINE_MATERIAL_HPP
