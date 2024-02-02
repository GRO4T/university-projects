#pragma once

#include "model/shape.hpp"

// TBD if this stays here
#include <vector>

#include "cubemap.hpp"
#include "entity/entity.hpp"
#include "model/material.hpp"
#include "opengl_all.hpp"

class ShapeFactory {
public:
    static EntitySPtr createCube(const Transform& t, const std::string& tex_path,
                                 MaterialSPtr material);
    static EntitySPtr createCuboid(const Transform& t, const std::string& tex_path,
                                   MaterialSPtr material, GLfloat width, GLfloat height,
                                   GLfloat depth);
    static EntitySPtr createCylinder(const Transform& t, const std::string& tex_path,
                                     MaterialSPtr material, GLfloat diameter, GLfloat height,
                                     uint32_t sides);
    static EntitySPtr createCone(const Transform& t, const std::string& tex_path,
                                 MaterialSPtr material, GLfloat diameter, GLfloat height,
                                 uint32_t sides);
    static EntitySPtr createSphere(const Transform& t, const std::string& tex_path,
                                   MaterialSPtr material, GLfloat diameter, uint32_t sides);

    static CubemapSharedPtr createCubemap(const std::vector<std::string>& tex_paths);
    static EntitySPtr createTorus(const Transform& t, const std::string& tex_path,
                                  MaterialSPtr material, GLfloat radius, GLfloat cs_radius,
                                  unsigned int sides, unsigned int cs_sides);

private:
    static EntitySPtr createShape(ModelPtr model, const Transform& t,
                                  const std::string& texture_path, MaterialSPtr material);
};