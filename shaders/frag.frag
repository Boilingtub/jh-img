R"(
#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D unifTexture;

void main() {
    FragColor = texture(unifTexture, TexCoord);
}
)"
