#version 330

in vec3 normal;

void main() { gl_FragColor = vec4(normal * 0.5 + 0.5, 1); }
