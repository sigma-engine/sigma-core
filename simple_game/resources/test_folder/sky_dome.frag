#version 330

in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec2 texcoord;
in vec3 binormal;

uniform float time;

const vec4 skytop = vec4(0.0f, 0.0f, 1.0f, 1.0f);
const vec4 skyhorizon = vec4(0.3294, 0.92157, 1.0, 1.0f);

void main()
{
    gl_FragColor = mix(skyhorizon, skytop, normalize(position.xyz).z);
}
