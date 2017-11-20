#version 330
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform bool shake;
uniform bool confuse;
uniform bool chaos;
uniform float time;

void main() {
    gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
    vec2 texture = vertex.zw;
    if (chaos) {
        float strength = 0.3;
        TexCoords = vec2(vertex.z + sin(time) * strength, vertex.w + cos(time) * strength);
    }
    else if (confuse) {
        TexCoords = vec2(1.0 - vertex.z, 1.0 - vertex.w);
    }
    else {
        TexCoords = vertex.zw;
    }
    if (shake) {
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;
        gl_Position.y += cos(time * 15) * strength;
    }
}
