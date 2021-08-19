#include "shaders.hpp"

std::string bloom_mask_str =
        "uniform sampler2D source;"
        "uniform vec2 resolution;"
        ""
        "void main() {"
        "     vec2 st = gl_FragCoord.st / resolution;"
        "     vec4 color = texture2D(source, vec2(st.x * 2.0, 1.0 - st.y * 2.0));"
        "     color.a = mod(floor(color.r * 255.0), 2.0) == 0.0 ? color.a : 0.0;"
        "     gl_FragColor = color;"
        "}";

std::string blur_str =
        "uniform sampler2D source;"
        "uniform vec2 offset;"
        "void main() {"
        "    vec2 textureCoordinates = gl_TexCoord[0].xy;"
        "    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);"
        "    color += texture2D(source, textureCoordinates - 10.0 * offset) * 0.0012;"
        "    color += texture2D(source, textureCoordinates - 9.0 * offset) * 0.0015;"
        "    color += texture2D(source, textureCoordinates - 8.0 * offset) * 0.0038;"
        "    color += texture2D(source, textureCoordinates - 7.0 * offset) * 0.0087;"
        "    color += texture2D(source, textureCoordinates - 6.0 * offset) * 0.0180;"
        "    color += texture2D(source, textureCoordinates - 5.0 * offset) * 0.0332;"
        "    color += texture2D(source, textureCoordinates - 4.0 * offset) * 0.0547;"
        "    color += texture2D(source, textureCoordinates - 3.0 * offset) * 0.0807;"
        "    color += texture2D(source, textureCoordinates - 2.0 * offset) * 0.1065;"
        "    color += texture2D(source, textureCoordinates - offset) * 0.1258;"
        "    color += texture2D(source, textureCoordinates) * 0.1330;"
        "    color += texture2D(source, textureCoordinates + offset) * 0.1258;"
        "    color += texture2D(source, textureCoordinates + 2.0 * offset) * 0.1065;"
        "    color += texture2D(source, textureCoordinates + 3.0 * offset) * 0.0807;"
        "    color += texture2D(source, textureCoordinates + 4.0 * offset) * 0.0547;"
        "    color += texture2D(source, textureCoordinates + 5.0 * offset) * 0.0332;"
        "    color += texture2D(source, textureCoordinates + 6.0 * offset) * 0.0180;"
        "    color += texture2D(source, textureCoordinates - 7.0 * offset) * 0.0087;"
        "    color += texture2D(source, textureCoordinates - 8.0 * offset) * 0.0038;"
        "    color += texture2D(source, textureCoordinates - 9.0 * offset) * 0.0015;"
        "    color += texture2D(source, textureCoordinates - 10.0 * offset) * 0.0012;"
        "    gl_FragColor = color;"
        "}";

void loadShader(){
    bloom_mask.loadFromMemory(bloom_mask_str, sf::Shader::Fragment);
    blur.loadFromMemory(blur_str, sf::Shader::Fragment);
}
