#version 330
in TexCoords;

uniform sampler2D scene;
uniform vec2 offset[9];
uniform int edge_kernel[9];
uniform int blur_kernel[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main() {
    gl_FragColor = vec4(0.0f);
    vec3 samples[9];
    if (chaos || shake)
        for (int i = 0; i < 9; i++)
            samples[i] = vec3(texture(scene, TexCoords + offset[i]));
    if (chaos) {
        for (int i = 0; i < 9; i++)
            gl_FragColor += vec4(samples[i] * edge_kernel[i], 0.0);
        gl_FragColor.a = 1.0;
    }
    else if (confuse) {
        gl_FragColor = vec4(1.0 - texture(scene, TexCoords).rgb, 1.0);
    }
    else if (shake) {
        for (int i = 0; i < 9; i++)
            gl_FragColor += vec4(samples[i] * blur_kernel[i], 0.0);
        gl_FragColor.a = 1.0;
    }
    else {
        gl_FragColor = texture(scene, TexCoords);
    }
}
