#include <sigma/game.hpp>

#include <sigma/context.hpp>

namespace sigma {
game::game(context* ctx)
    : ctx(ctx)
    , textures(ctx->textures())
    , shaders(ctx->shaders())
    , effects(ctx->effects())
    , materials(ctx->materials())
    , static_meshes(ctx->static_meshes())
    , static_mesh_instances(static_meshes)
{
}
}
