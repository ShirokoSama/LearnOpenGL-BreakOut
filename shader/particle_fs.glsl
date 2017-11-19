#version 330

varying vec2 TexCoords;
varying vec4 ParticleColor;

uniform sampler2D sprite;

out vec4 color;

void main() {
    color = texture(sprite, TexCoords) * ParticleColor;
}
