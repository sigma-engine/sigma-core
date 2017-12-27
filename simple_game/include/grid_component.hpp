// TEMPLATE(generated.hpp)
#ifndef GRID_COMPONENT_HPP
#define GRID_COMPONENT_HPP

#include <sigma/component.hpp>
#include <sigma/reflect.hpp>

struct R_EXPORT() grid_component {
    BOOST_HANA_DEFINE_STRUCT(
        grid_component,
        (int, rows),
        (int, columns),
        (float, row_spacing),
        (float, column_spacing));

    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& rows;
        ar& columns;
        ar& row_spacing;
        ar& column_spacing;
    }
};

REGISTER_COMPONENT(grid_component)

#endif // GRID_COMPONENT_HPP
