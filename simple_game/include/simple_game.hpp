#ifndef SIMPLE_GAME_HPP
#define SIMPLE_GAME_HPP

#include <simple_world.hpp>

#include <sigma/game.hpp>

#include <glm/vec3.hpp>

#include <random>

struct uniform_vec3_distribution {
    uniform_vec3_distribution(glm::vec3 min, glm::vec3 max)
        : x_distribution_(min.x, max.x)
        , y_distribution_(min.y, max.y)
        , z_distribution_(min.z, max.z)
    {
    }

    template <class Generator>
    glm::vec3 operator()(Generator& gen)
    {
        return { x_distribution_(gen), y_distribution_(gen), z_distribution_(gen) };
    }

private:
    std::uniform_real_distribution<float> x_distribution_;
    std::uniform_real_distribution<float> y_distribution_;
    std::uniform_real_distribution<float> z_distribution_;
};

class simple_game : public sigma::game<simple_world> {
public:
    simple_game(const boost::filesystem::path& cache_path);

    sigma::transform& random_transform(sigma::entity e);

    virtual void update(std::chrono::duration<float> dt) override;

private:
    std::default_random_engine generator_;
    uniform_vec3_distribution position_distribution_;
    uniform_vec3_distribution rotation_distribution_;
    std::uniform_real_distribution<float> scale_distribution_;
    uniform_vec3_distribution color_distribution_;
};

#endif // SIMPLE_GAME_HPP
