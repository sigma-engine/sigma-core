#version 400

// clang-format off
#include <pbr/deffered/post_process_effect.glsl>
#include <pbr/brdf.glsl>
#include <math.glsl>
// clang-format on

uniform samplerCube environment_map;
uniform uvec2 environment_map_size = uvec2(2048, 2048);

// http://www.trentreed.net/blog/physically-based-shading-and-image-based-lighting/
// http://blog.tobias-franke.eu/2014/03/30/notes_on_importance_sampling.html
// http://http.developer.nvidia.com/GPUGems3/gpugems3_ch20.html
// http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
// https://placeholderart.wordpress.com/2015/07/28/implementation-notes-runtime-environment-map-filtering-for-image-based-lighting/

#define SAMPLES 32

float environment_map_lod(uvec2 size, uint N, float pdf)
{
    return max(.5 * (log2(size.x * size.y / N) - log2(pdf)), 0);
}

vec3 importance_sample_ggx(vec2 xi, float alpha2)
{

    float cos_theta = sqrt((1.0f - xi.y) / ((alpha2 - 1.0f) * xi.y + 1.0f));
    float sin_theta = sqrt(1 - cos_theta * cos_theta);
    float phi = PI_2 * xi.x;

    return vec3(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);
}

vec3 radiance(vec3 N, vec3 V, vec3 F0, float roughness)
{
    vec3 up_vec = abs(N.z) < 0.999 ? vec3(0, 0, 1) : vec3(1, 0, 0);
    vec3 tangent_x = normalize(cross(up_vec, N));
    vec3 tangent_y = cross(N, tangent_x);

    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;

    float NdotV = clamp(dot(N, V), 0.0, 1.0);

    vec3 sum = vec3(0);
    for (uint i = 0; i < SAMPLES; i++) {
        // Generate two independent random variables in the range [0-1]
        vec2 Xi = hammersley(i, SAMPLES);

        // Skew the two random variables to towards the specular lobe
        vec3 X = importance_sample_ggx(Xi, alpha2);

        vec3 H = X.x * tangent_x + X.y * tangent_y + X.z * N; // Convert to world space
        vec3 L = 2 * dot(V, H) * H - V;

        float NdotL = clamp(dot(N, L), 0.0, 1.0);
        float NdotH = clamp(dot(N, H), 0.0, 1.0);
        float VdotH = clamp(dot(V, H), 0.0, 1.0);

        // Calculate the based on roughness
        float pdf = D_ggx(NdotH, alpha2) * NdotH / (4 * VdotH);
        float lod = environment_map_lod(environment_map_size, SAMPLES, pdf);

        // Sample the environment map
        vec3 environment_color = textureLod(environment_map, L, lod).rgb;
        environment_color = pow(environment_color, vec3(2.2)); // TODO gamma conversion should be done on loading

        sum += environment_color * F_schlick(VdotH, F0) * G_schlick(NdotL, NdotV, roughness) * VdotH / (NdotH * NdotV + NO_DIV_BY_ZERO);
    }

    return sum / SAMPLES;
}

void main()
{

    surface s = read_geometry_buffer();

    vec3 V = normalize((inverse_view_matrix * vec4(-s.position, 0.0)).xyz);

    if (s.depth == 1.0)
        out_image = pow(texture(environment_map, -V, 0.0).rgb, vec3(3));
    else {
        vec3 N = normalize((inverse_view_matrix * vec4(s.normal, 0.0)).xyz);

        vec3 F0 = mix(vec3(0.04f), s.diffuse, s.metalness);

        out_image = radiance(N, V, F0, s.roughness);
    }
}
