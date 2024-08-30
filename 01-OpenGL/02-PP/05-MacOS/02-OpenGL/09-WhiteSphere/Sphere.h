#pragma once

#include <vector>

#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

const double VDG_PI = 3.14159265358979323846;
enum { AMC_ATTRIBUTE_POSITION,
    AMC_ATTRIBUTE_COLOR,
    AMC_ATTRIBUTE_TEXCOORD,
    AMC_ATTRIBUTE_NORMAL };

extern FILE *gpFILE;

class Mesh {
public:
  Mesh();
  ~Mesh();
  void allocate(int numIndices);
  void draw();
  void prepareToDraw();
  int getIndexCount();
  int getVertexCount();
  void cleanupMeshData();
  void normalizeVector(float *v);
  void addTriangle(float single_vertex[4][3], float single_normal[4][3],
                   float single_texture[4][2]);
  bool isFoundIdentical(float val1, float val2, float diff);

private:
  // uint16_t *elements;
  //   float *verts;
  //   float *norms;
  //   float *texCoords;
  std::vector<uint16_t> elements;
  std::vector<float> verts;
  std::vector<float> norms;
  std::vector<float> texCoords;

  int numElements;
  int maxElements;
  int numVertices;
  int iNumIndices;

  GLuint vbo_position;
  GLuint vbo_normal;
  GLuint vbo_texture;
  GLuint vbo_index;
  GLuint vao;
};

void makeSphere(Mesh &meshSphere, float fRadius, int iSlices, int iStacks);
