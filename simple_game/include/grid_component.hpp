#ifndef GRID_COMPONENT_HPP
#define GRID_COMPONENT_HPP

#include <sigma/component.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/reflect.hpp>

struct R_EXPORT() grid_component {
    BOOST_HANA_DEFINE_STRUCT(
        grid_component,
        (int, rows),
        (int, columns),
        (float, row_spacing),
        (float, column_spacing),
        (sigma::handle<sigma::graphics::static_mesh>, mesh));

    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& rows;
        ar& columns;
        ar& row_spacing;
        ar& column_spacing;
        ar& mesh;
    }
};

REGISTER_COMPONENT(grid_component)

#endif // GRID_COMPONENT_HPP
