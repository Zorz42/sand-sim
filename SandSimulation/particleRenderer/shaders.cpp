#include "shaders.hpp"



std::string bloom_mask_str =
        "#ifdef GL_ES\n"
        "precision mediump float;\n"
        "#endif\n"
        "uniform sampler2D u_scene_texture;\n"
        "uniform vec2 u_resolution;\n"
        "void main() {\n"
        "vec2 st = gl_FragCoord.st/u_resolution;\n"
        "if(mod(floor(texture2D(u_scene_texture, vec2(st.x * 2.0, 1.0 - st.y * 2.0)).r * 255), 2.0) == 0.0){;\n"
        "gl_FragColor = texture2D(u_scene_texture, vec2(st.x * 2.0, 1.0 - st.y * 2.0));\n"
        "}\n"
        "else{\n"
        "gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);\n"
        "}\n"
        "}";

std::string combine_str =
        "uniform sampler2D u_scene_texture;\n"
        "uniform vec2 u_resolution;\n"
        "void main(){\n"
        "vec2 st = gl_FragCoord.st/u_resolution;\n"
        "vec4 blurtext = gl_FragColor;\n"
        "gl_FragColor = texture2D(u_scene_texture, vec2(st.x, 1.0 - st.y)) * (1.0 - blurtext.a) + blurtext;\n"
        "}";

std::string blur_str =
        "uniform sampler2D source;\n"
        "uniform vec2 offset;\n"
        "void main() {\n"
        "    vec2 textureCoordinates = gl_TexCoord[0].xy;\n"
        "    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);\n"
        "    color += texture2D(source, textureCoordinates - 10.0 * offset) * 0.0012;\n"
        "    color += texture2D(source, textureCoordinates - 9.0 * offset) * 0.0015;\n"
        "    color += texture2D(source, textureCoordinates - 8.0 * offset) * 0.0038;\n"
        "    color += texture2D(source, textureCoordinates - 7.0 * offset) * 0.0087;\n"
        "    color += texture2D(source, textureCoordinates - 6.0 * offset) * 0.0180;\n"
        "    color += texture2D(source, textureCoordinates - 5.0 * offset) * 0.0332;\n"
        "    color += texture2D(source, textureCoordinates - 4.0 * offset) * 0.0547;\n"
        "    color += texture2D(source, textureCoordinates - 3.0 * offset) * 0.0807;\n"
        "    color += texture2D(source, textureCoordinates - 2.0 * offset) * 0.1065;\n"
        "    color += texture2D(source, textureCoordinates - offset) * 0.1258;\n"
        "    color += texture2D(source, textureCoordinates) * 0.1330;\n"
        "    color += texture2D(source, textureCoordinates + offset) * 0.1258;\n"
        "    color += texture2D(source, textureCoordinates + 2.0 * offset) * 0.1065;\n"
        "    color += texture2D(source, textureCoordinates + 3.0 * offset) * 0.0807;\n"
        "    color += texture2D(source, textureCoordinates + 4.0 * offset) * 0.0547;\n"
        "    color += texture2D(source, textureCoordinates + 5.0 * offset) * 0.0332;\n"
        "    color += texture2D(source, textureCoordinates + 6.0 * offset) * 0.0180;\n"
        "    color += texture2D(source, textureCoordinates - 7.0 * offset) * 0.0087;\n"
        "    color += texture2D(source, textureCoordinates - 8.0 * offset) * 0.0038;\n"
        "    color += texture2D(source, textureCoordinates - 9.0 * offset) * 0.0015;\n"
        "    color += texture2D(source, textureCoordinates - 10.0 * offset) * 0.0012;\n"
        "    gl_FragColor = color;\n"
        "}";




void loadShader(){
    bloom_mask.loadFromMemory(bloom_mask_str, sf::Shader::Fragment);
    blur.loadFromMemory(blur_str, sf::Shader::Fragment);
    combine.loadFromMemory(combine_str, sf::Shader::Fragment);
}


