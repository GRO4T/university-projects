#include "model/model.hpp"

#include "model/shape.hpp"

Model::~Model() { glDeleteVertexArrays(1, &VAO); }

void Model::load(GLfloat vertices[], int v_count) {
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * v_count, vertices, GL_STATIC_DRAW);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and
    // attribute pointer(s).
    glBindVertexArray(VAO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);  // Unbind VAO (it's always a good thing to unbind any
                           // buffer/array to prevent strange bugs)
    vertex_count = v_count;
}

void Model::load(Shape &shape) { load(shape.data(), static_cast<int>(shape.size())); }

void Model::load(Shape &&shape) { load(shape.data(), static_cast<int>(shape.size())); }

void IndexedModel::load(std::vector<GLfloat> &vertices, std::vector<GLfloat> &normals,
                        std::vector<GLfloat> &tex_coords, std::vector<GLuint> &indices) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(),
                 GL_STATIC_DRAW);
    storeDataInAttribList(0, 3, vertices);
    storeDataInAttribList(1, 2, tex_coords);
    storeDataInAttribList(2, 3, normals);
    glBindVertexArray(0);
    vertex_count = static_cast<int>(vertices.size()) / 3;
    index_count = static_cast<int>(indices.size());
}

void Model::storeDataInAttribList(GLuint attrib_number, GLuint vector_len,
                                  std::vector<GLfloat> &data) {
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    GLuint data_size = sizeof(GLfloat) * static_cast<GLuint>(data.size());
    glBufferData(GL_ARRAY_BUFFER, data_size, data.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(attrib_number, vector_len, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(attrib_number);
}
