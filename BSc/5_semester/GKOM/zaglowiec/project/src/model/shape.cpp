#include "model/shape.hpp"

#include <cmath>

// clang-format off

static const double PI = 3.14159265;

Shape shape::cube(GLfloat s) {
    return shape::cuboid(s, s, s);
}

Shape shape::cuboid(GLfloat w, GLfloat h, GLfloat d) {
    return Shape({
        w * -0.5f, h *  0.5f, d *  0.5f,   0.0f, 0.0f,    0.0f,  0.0f,  1.0f,
        w * -0.5f, h * -0.5f, d *  0.5f,   0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
        w *  0.5f, h *  0.5f, d *  0.5f,   1.0f, 0.0f,    0.0f,  0.0f,  1.0f,
        w *  0.5f, h *  0.5f, d *  0.5f,   1.0f, 0.0f,    0.0f,  0.0f,  1.0f,
        w * -0.5f, h * -0.5f, d *  0.5f,   0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
        w *  0.5f, h * -0.5f, d *  0.5f,   1.0f, 1.0f,    0.0f,  0.0f,  1.0f,

        w *  0.5f, h *  0.5f, d *  0.5f,   0.0f, 0.0f,    1.0f,  0.0f,  0.0f,
        w *  0.5f, h * -0.5f, d *  0.5f,   0.0f, 1.0f,    1.0f,  0.0f,  0.0f,
        w *  0.5f, h *  0.5f, d * -0.5f,   1.0f, 0.0f,    1.0f,  0.0f,  0.0f,
        w *  0.5f, h *  0.5f, d * -0.5f,   1.0f, 0.0f,    1.0f,  0.0f,  0.0f,
        w *  0.5f, h * -0.5f, d *  0.5f,   0.0f, 1.0f,    1.0f,  0.0f,  0.0f,
        w *  0.5f, h * -0.5f, d * -0.5f,   1.0f, 1.0f,    1.0f,  0.0f,  0.0f,

        w *  0.5f, h *  0.5f, d * -0.5f,   0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        w *  0.5f, h * -0.5f, d * -0.5f,   0.0f, 1.0f,    0.0f,  0.0f, -1.0f,
        w * -0.5f, h *  0.5f, d * -0.5f,   1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        w * -0.5f, h *  0.5f, d * -0.5f,   1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
        w *  0.5f, h * -0.5f, d * -0.5f,   0.0f, 1.0f,    0.0f,  0.0f, -1.0f,
        w * -0.5f, h * -0.5f, d * -0.5f,   1.0f, 1.0f,    0.0f,  0.0f, -1.0f,

        w * -0.5f, h *  0.5f, d * -0.5f,   0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
        w * -0.5f, h * -0.5f, d * -0.5f,   0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
        w * -0.5f, h *  0.5f, d *  0.5f,   1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
        w * -0.5f, h *  0.5f, d *  0.5f,   1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
        w * -0.5f, h * -0.5f, d * -0.5f,   0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
        w * -0.5f, h * -0.5f, d *  0.5f,   1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,

        w * -0.5f, h *  0.5f, d * -0.5f,   0.0f, 0.0f,    0.0f,  1.0f,  0.0f,
        w * -0.5f, h *  0.5f, d *  0.5f,   0.0f, 1.0f,    0.0f,  1.0f,  0.0f,
        w *  0.5f, h *  0.5f, d * -0.5f,   1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
        w *  0.5f, h *  0.5f, d * -0.5f,   1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
        w * -0.5f, h *  0.5f, d *  0.5f,   0.0f, 1.0f,    0.0f,  1.0f,  0.0f,
        w *  0.5f, h *  0.5f, d *  0.5f,   1.0f, 1.0f,    0.0f,  1.0f,  0.0f,

        w * -0.5f, h * -0.5f, d *  0.5f,   0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
        w * -0.5f, h * -0.5f, d * -0.5f,   0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
        w *  0.5f, h * -0.5f, d *  0.5f,   1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
        w *  0.5f, h * -0.5f, d *  0.5f,   1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
        w * -0.5f, h * -0.5f, d * -0.5f,   0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
        w *  0.5f, h * -0.5f, d * -0.5f,   1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
    });
}

Shape shape::cylinder(GLfloat diameter, GLfloat height, unsigned int sides) {
    if (sides < 3) sides = 3;
    Shape vertices;
    vertices.reserve(8 * 12 * sides);
    for (unsigned int i = 0; i < sides; ++i) {
        GLfloat r = i / static_cast<GLfloat>(sides);
        GLfloat nr = (i + 1) / static_cast<GLfloat>(sides);
        GLfloat norm_x = static_cast<GLfloat>(sin(r * 2 * PI));
        GLfloat nnorm_x = static_cast<GLfloat>(sin(nr * 2 * PI));
        GLfloat x = 0.5f * diameter * norm_x;
        GLfloat nx = 0.5f * diameter * nnorm_x;
        GLfloat ly = -0.5f * height;
        GLfloat hy = 0.5f * height;
        GLfloat norm_z = static_cast<GLfloat>(cos(r * 2 * PI));
        GLfloat nnorm_z = static_cast<GLfloat>(cos(nr * 2 * PI));
        GLfloat z = 0.5f * diameter * norm_z;
        GLfloat nz = 0.5f * diameter * nnorm_z;

        GLfloat u = r;
        GLfloat nu = nr;
        GLfloat tu = 0.25f + 0.25f * static_cast<GLfloat>(sin(2 * PI * r));
        GLfloat ntu = 0.25f + 0.25f * static_cast<GLfloat>(sin(2 * PI * nr));
        GLfloat bu = 0.5f + tu;
        GLfloat nbu = 0.5f + ntu;
        GLfloat v = 0.25f + 0.25f * static_cast<GLfloat>(cos(2 * PI * r));
        GLfloat nv = 0.25f + 0.25f * static_cast<GLfloat>(cos(2 * PI * nr));

        GLfloat slice[8 * 12] = {
            // side rectangle
               x, hy,   z,         u,  0.5f,      norm_x,  0.0f,  norm_z,
               x, ly,   z,         u,  1.0f,      norm_x,  0.0f,  norm_z,
              nx, hy,  nz,        nu,  0.5f,     nnorm_x,  0.0f, nnorm_z,
              nx, hy,  nz,        nu,  0.5f,     nnorm_x,  0.0f, nnorm_z,
               x, ly,   z,         u,  1.0f,      norm_x,  0.0f,  norm_z,
              nx, ly,  nz,        nu,  1.0f,     nnorm_x,  0.0f, nnorm_z,
            // top triangle
            0.0f, hy, 0.0f,    0.25f, 0.25f,        0.0f,  1.0f,    0.0f,
               x, hy,    z,       tu,     v,        0.0f,  1.0f,    0.0f,
              nx, hy,   nz,      ntu,    nv,        0.0f,  1.0f,    0.0f,
            // bottom triangle
               x, ly,    z,       bu,     v,        0.0f, -1.0f,    0.0f,
            0.0f, ly, 0.0f,    0.75f, 0.25f,        0.0f, -1.0f,    0.0f,
              nx, ly,   nz,      nbu,    nv,        0.0f, -1.0f,    0.0f,
        };
        vertices.insert(vertices.end(), slice, slice + 8 * 12);
    }
    return vertices;
}

Shape shape::sphere(GLfloat diameter, unsigned int sides) {
    if (sides < 3) sides = 3;
    Shape vertices;
    vertices.reserve(8 * 6 * sides * sides);
    for (unsigned int i = 0; i < sides; ++i) {
        GLfloat r = i / static_cast<GLfloat>(sides);
        GLfloat nr = (i + 1) / static_cast<GLfloat>(sides);
        for (unsigned int u = 0; u < sides; ++u) {
            GLfloat d = u / static_cast<GLfloat>(sides);
            GLfloat nd = (u + 1) / static_cast<GLfloat>(sides);
            GLfloat hrad = 0.5f * diameter * static_cast<GLfloat>(sin(d * PI));
            GLfloat lrad = static_cast<GLfloat>(0.5) * diameter * static_cast<GLfloat>(sin(nd * PI));
            GLfloat hx = hrad * static_cast<GLfloat>(sin(r * 2 * PI));
            GLfloat lx = lrad * static_cast<GLfloat>(sin(r * 2 * PI));
            GLfloat nhx = hrad * static_cast<GLfloat>(sin(nr * 2 * PI));
            GLfloat nlx = lrad * static_cast<GLfloat>(sin(nr * 2 * PI));
            GLfloat y = 0.5f * diameter * static_cast<GLfloat>(cos(d * PI));
            GLfloat ny = 0.5f * diameter * static_cast<GLfloat>(cos(nd * PI));
            GLfloat hz = hrad * static_cast<GLfloat>(cos(r * 2 * PI));
            GLfloat lz = lrad * static_cast<GLfloat>(cos(r * 2 * PI));
            GLfloat nhz = hrad * static_cast<GLfloat>(cos(nr * 2 * PI));
            GLfloat nlz = lrad * static_cast<GLfloat>(cos(nr * 2 * PI));

            GLfloat lny = static_cast<GLfloat>(cos(d * PI));
            GLfloat nlny =  static_cast<GLfloat>(cos(nd * PI));
            GLfloat xzs = static_cast<GLfloat>(sin(d * PI));
            GLfloat nxzs = static_cast<GLfloat>(sin(nd * PI));
            GLfloat lnx = static_cast<GLfloat>(sin(r * 2 * PI));
            GLfloat nlnx = static_cast<GLfloat>(sin(nr * 2 * PI));
            GLfloat lnz = static_cast<GLfloat>(cos(r * 2 * PI));
            GLfloat nlnz = static_cast<GLfloat>(cos(nr * 2 * PI));

            GLfloat slice[8 * 6] = {
                 hx,  y,  hz,      r,  d,     lnx *  xzs,  lny,  lnz *  xzs,
                 lx, ny,  lz,      r, nd,     lnx * nxzs, nlny,  lnz * nxzs,
                nhx,  y, nhz,     nr,  d,    nlnx *  xzs,  lny, nlnz *  xzs,
                nhx,  y, nhz,     nr,  d,    nlnx *  xzs,  lny, nlnz *  xzs,
                 lx, ny,  lz,      r, nd,     lnx * nxzs, nlny,  lnz * nxzs,
                nlx, ny, nlz,     nr, nd,    nlnx * nxzs, nlny, nlnz * nxzs,
            };
            vertices.insert(vertices.end(), slice, slice + 8 * 6);

        }
    }
    return vertices;
}

Shape shape::cone(GLfloat diameter, GLfloat height, unsigned int sides) {
    if (sides < 3) sides = 3;
    Shape vertices;

    GLfloat clen = static_cast<GLfloat>(sqrt(diameter * diameter / 4.0 + height * height));
    GLfloat lny = static_cast<GLfloat>((diameter / 2.0) / clen);
    GLfloat xzs = static_cast<GLfloat>(height / clen);

    vertices.reserve(8 * 6 * sides);
    for (unsigned int i = 0; i < sides; ++i) {
        GLfloat r = i / static_cast<GLfloat>(sides);
        GLfloat nr = (i + 1) / static_cast<GLfloat>(sides);
        GLfloat norm_x = static_cast<GLfloat>(sin(r * 2 * PI));
        GLfloat nnorm_x = static_cast<GLfloat>(sin(nr * 2 * PI));
        GLfloat x = 0.5f * diameter * norm_x;
        GLfloat nx = 0.5f * diameter * nnorm_x;
        GLfloat ly = -0.5f * height;
        GLfloat hy = 0.5f * height;
        GLfloat norm_z = static_cast<GLfloat>(cos(r * 2 * PI));
        GLfloat nnorm_z = static_cast<GLfloat>(cos(nr * 2 * PI));
        GLfloat z = 0.5f * diameter * norm_z;
        GLfloat nz = 0.5f * diameter * nnorm_z;

        GLfloat u = 0.5f + 0.25f * static_cast<GLfloat>(sin(2 * PI * r));
        GLfloat nu = 0.5f + 0.25f * static_cast<GLfloat>(sin(2 * PI * nr));
        GLfloat tv = 0.25f + 0.25f * static_cast<GLfloat>(cos(2 * PI * r));
        GLfloat ntv = 0.25f + 0.25f * static_cast<GLfloat>(cos(2 * PI * nr));
        GLfloat bv = 0.5f + tv;
        GLfloat nbv = 0.5f + ntv;

        GLfloat lnx = static_cast<GLfloat>(sin(r * 2 * PI));
        GLfloat mlnx = static_cast<GLfloat>(sin(((r + nr) / 2.0f) * 2 * PI));
        GLfloat nlnx = static_cast<GLfloat>(sin(nr * 2 * PI));
        GLfloat lnz = static_cast<GLfloat>(cos(r * 2 * PI));
        GLfloat mlnz = static_cast<GLfloat>(cos(((r + nr) / 2.0f) * 2 * PI));
        GLfloat nlnz = static_cast<GLfloat>(cos(nr * 2 * PI));

        GLfloat slice[8 * 6] = {
            0.0f, hy, 0.0f,    0.5f, 0.25f,    mlnx * xzs,   lny, mlnz * xzs,
               x, ly,    z,       u,    tv,     lnx * xzs,   lny,  lnz * xzs,
              nx, ly,   nz,      nu,   ntv,    nlnx * xzs,   lny, nlnz * xzs,

               x, ly,    z,       u,    bv,          0.0f, -1.0f,       0.0f,
            0.0f, ly, 0.0f,    0.5f, 0.75f,          0.0f, -1.0f,       0.0f,
              nx, ly,   nz,      nu,   nbv,          0.0f, -1.0f,       0.0f,
        };
        vertices.insert(vertices.end(), slice, slice + 8 * 6);
    }
    return vertices;
}

Shape shape::plane(GLfloat w, GLfloat h, unsigned int w_segments, unsigned int h_segments) {
    if (w_segments < 1) w_segments = 1;
    if (h_segments < 1) h_segments = 1;
    Shape vertices;
    vertices.reserve(8 * 6 * w_segments * h_segments);
    for (unsigned int i = 0; i < w_segments; ++i) {
        for (unsigned int u = 0; u < h_segments; ++u) {
            GLfloat x = i / static_cast<GLfloat>(w_segments);
            GLfloat nx = (i + 1) / static_cast<GLfloat>(w_segments);
            GLfloat y = u / static_cast<GLfloat>(h_segments);
            GLfloat ny = (u + 1) / static_cast<GLfloat>(h_segments);
            x = (-1.0f + 2 * x);
            nx = (-1.0f + 2 * nx);
            y = (-1.0f + 2 * y);
            ny = (-1.0f + 2 * ny);

            GLfloat slice[8 * 6] = {
                w *  x, h *  y, 0.0f,   0.0f, 0.0f,    0.0f,  0.0f,  1.0f,
                w *  x, h * ny, 0.0f,   0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
                w * nx, h *  y, 0.0f,   1.0f, 0.0f,    0.0f,  0.0f,  1.0f,
                w * nx, h *  y, 0.0f,   1.0f, 0.0f,    0.0f,  0.0f,  1.0f,
                w *  x, h * ny, 0.0f,   0.0f, 1.0f,    0.0f,  0.0f,  1.0f,
                w * nx, h * ny, 0.0f,   1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
            };
            vertices.insert(vertices.end(), slice, slice + 8 * 6);
        }
    }
    return vertices;
}

Shape shape::cubemap() {
    return shape::cuboid(2.0f, 2.0f, 2.0f);
}

// clang-format on

void shape::invert_normals(Shape &shape) {
    for (int i = 0; i < shape.size(); ++i) {
        if (i % 8 >= 5) {
            shape[i] = -shape[i];
        }
    }
}