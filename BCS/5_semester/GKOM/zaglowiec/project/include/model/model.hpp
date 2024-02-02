#pragma once

#include <memory>
#include <vector>

#include "opengl_all.hpp"

class Model {
public:
    virtual ~Model();
    void load(GLfloat vertices[], int v_count);
    void load(std::vector<GLfloat>& shape);
    void load(std::vector<GLfloat>&& shape);
    GLuint getVao() const { return VAO; }
    int getVertexCount() const { return vertex_count; }
    virtual void draw() const { glDrawArrays(GL_TRIANGLES, 0, vertex_count); }

protected:
    void storeDataInAttribList(GLuint attrib_number, GLuint vector_len, std::vector<GLfloat>& data);

    int vertex_count;
    GLuint VAO;
};

class IndexedModel : public Model {
public:
    virtual void draw() const override {
        glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, (void*)0);
    }
    void load(std::vector<GLfloat>& vertices, std::vector<GLfloat>& normals,
              std::vector<GLfloat>& tex_coords, std::vector<GLuint>& indices);

protected:
    int index_count;
};

class StripedModel : public IndexedModel {
public:
    void draw() const override {
        glDrawElements(GL_TRIANGLE_STRIP, index_count, GL_UNSIGNED_INT, (void*)0);
    }
};

using ModelPtr = std::unique_ptr<Model>;
using IndexedModelPtr = std::unique_ptr<IndexedModel>;
using StripedModelPtr = std::unique_ptr<StripedModel>;