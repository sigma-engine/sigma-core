#version 330

in vec4 vertex_position;
in vec4 final;

void main()
{
    float width = .01;
    float size = 1 - width;

    float vx = vertex_position.x - (width / 2.0f);
    float vy = vertex_position.y - (width / 2.0f);

    float x = fract(vx);
    float y = fract(vy);

    float xx = fract(10 * vx);
    float yy = fract(10 * vy);
    //

    vec4 color = vec4(0.5f, 0.5f, 0.5f, 0.3f);
    if (ceil(vx) == 0 && x > size && vy > -width)
        color = vec4(0.0f, 1, 0, .5f);

    if (ceil(vy) == 0 && y > size && vx > -width)
        color = vec4(1.0f, 0, 0, .5f);

    if (x > size || y > size) {
        gl_FragColor = color;
        gl_FragColor.a *= min(1, 50.0f / length(final.xyz));
    } else if (xx > size || yy > size) {
        gl_FragColor = vec4(.05f, .05f, .05f, 0.185f);
        gl_FragColor.a *= min(1, 2.0f / length(final.xyz));
    } else {
        discard;
    }
}
