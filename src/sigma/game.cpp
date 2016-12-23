#include <sigma/game.hpp>

#include <sigma/context.hpp>

namespace sigma {
game::game(context* ctx)
    : textures(ctx->textures())
    , shaders(ctx->shaders())
    , materials(ctx->materials())
    , static_meshes(ctx->static_meshes())
    , static_mesh_instances(ctx->static_meshes())
    , ctx(ctx)
{
}
}
