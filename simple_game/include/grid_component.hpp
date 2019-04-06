#ifndef GRID_COMPONENT_HPP
#define GRID_COMPONENT_HPP

#include <sigma/component.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/reflect.hpp>

struct grid_component {
    BOOST_HANA_DEFINE_STRUCT(
        grid_component,
        (int, rows),
        (int, columns),
        (float, row_spacing),
        (float, column_spacing),
        (sigma::handle<sigma::graphics::static_mesh>, mesh));

    grid_component(int rows = 0, int columns = 0,
        float row_spacing = 0, float column_spacing = 0,
        sigma::handle<sigma::graphics::static_mesh> mesh = {})
        : rows(rows)
        , columns(columns)
        , row_spacing(row_spacing)
        , column_spacing(column_spacing)
    {
    }

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(rows,
        columns,
        row_spacing,
        column_spacing,
        mesh);
    }
};

REGISTER_COMPONENT(grid_component)

#endif // GRID_COMPONENT_HPP
