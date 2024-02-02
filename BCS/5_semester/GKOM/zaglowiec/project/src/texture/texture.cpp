#include "texture/texture.hpp"

const std::map<GLuint, const char*> Texture::texture_unit_shader_name_map{
    {GL_TEXTURE0, "Texture0"}, {GL_TEXTURE1, "Texture1"}};

const std::map<GLuint, int> Texture::texture_unit_shader_id_map{{GL_TEXTURE0, 0}, {GL_TEXTURE1, 1}};