#version 330

in vec2 TexCoords;
in vec4 ParticleColor;

uniform sampler2D sprite;

void main() {
    gl_FragColor = texture(sprite, TexCoords) * ParticleColor;
}
