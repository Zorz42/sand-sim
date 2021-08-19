#include "shaders.hpp"



std::string bloom_shader_str =
        "#ifdef GL_ES\n"
        "precision mediump float;\n"
        "#endif\n"
        "uniform sampler2D u_scene_texture;\n"
        "uniform vec2 u_resolution;\n"
        "void main() {\n"
        "vec2 st = gl_FragCoord.st/u_resolution;\n"
        "if(mod(floor(texture2D(u_scene_texture, vec2(st)).r * 255), 2) == 0){;\n"
        "gl_FragColor = texture2D(u_scene_texture, vec2(st));\n"
        "}\n"
        "else{\n"
        "gl_FragColor = vec4(0,0,0.0,1);\n"
        "}\n"
        "}\n";


void loadShader(){
    bloom_shader.loadFromMemory(bloom_shader_str, sf::Shader::Fragment);
}


