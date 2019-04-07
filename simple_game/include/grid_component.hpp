#ifndef GRID_COMPONENT_HPP
#define GRID_COMPONENT_HPP

#include <sigma/component.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/reflect.hpp>
#include <sigma/context.hpp>
#include <sigma/tools/json_conversion.hpp>

struct grid_component {
    int rows;
    int columns;
    float row_spacing;
    float column_spacing;
    sigma::handle<sigma::graphics::static_mesh> mesh;

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

namespace sigma {
namespace json {
    static bool from_json(std::shared_ptr<sigma::context> ctx, const Json::Value& source, grid_component& output)
    {
        if (source.isMember("rows"))
            sigma::json::from_json(ctx, source["rows"], output.rows);
        
        if (source.isMember("columns"))
            sigma::json::from_json(ctx, source["columns"], output.columns);

        if (source.isMember("row_spacing"))
            sigma::json::from_json(ctx, source["row_spacing"], output.row_spacing);

        if (source.isMember("column_spacing"))
            sigma::json::from_json(ctx, source["column_spacing"], output.column_spacing);

        if (source.isMember("mesh"))
            sigma::json::from_json(ctx, source["mesh"], output.mesh);
        return true;
    }   
}
}

REGISTER_COMPONENT(grid_component)

#endif // GRID_COMPONENT_HPP
