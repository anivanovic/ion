#version 410

in vec2 texCoord;

out vec4 color;

uniform sampler2D imageTexture;

void main() {
    // gl_FragColor = texture2D(diffuse, texCoord0); // vec4(1.0, 1.0, 0.8, 1.0);
    color = texture(imageTexture, texCoord); // vec4(1.0, 1.0, 0.8, 1.0);
}