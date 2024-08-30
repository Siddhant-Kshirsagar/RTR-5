
#include <cmath>
#include <stdlib.h>
#include "Sphere.h"

Mesh::Mesh() {
  // code
}

Mesh::~Mesh() {
  // code

  if (vao) {
    glDeleteVertexArrays(1, &vao);
    vao = 0;
  }

  if (vbo_index) {
    glDeleteBuffers(1, &vbo_index);
    vbo_index = 0;
  }

  if (vbo_texture) {
    glDeleteBuffers(1, &vbo_texture);
    vbo_texture = 0;
  }

  if (vbo_normal) {
    glDeleteBuffers(1, &vbo_normal);
    vbo_normal = 0;
  }

  if (vbo_position) {
    glDeleteBuffers(1, &vbo_position);
    vbo_position = 0;
  }
}

void Mesh::allocate(int numIndices) {

  // first cleanup, if not initially empty
  cleanupMeshData();

  maxElements = numIndices;
  numElements = 0;
  numVertices = 0;

  iNumIndices = numIndices / 3;

  //   elements = (uint16_t *)malloc(iNumIndices * 3 * sizeof(uint16_t));
  //   verts = (float *)malloc(iNumIndices * 3 * sizeof(float));
  //   norms = (float *)malloc(iNumIndices * 3 * sizeof(float));
  //   texCoords = (float *)malloc(iNumIndices * 2 * sizeof(float));
  try {
    elements.resize(iNumIndices * 3, 0);
    verts.resize(iNumIndices * 3, 0.0f);
    norms.resize(iNumIndices * 3, 0.0f);
    texCoords.resize(iNumIndices * 2, 0.0f);
    fprintf(gpFILE, "\nMemory allocation succeeded.");
  } catch (const std::length_error &err) {
    fprintf(gpFILE, "\nLength error.");
  } catch (const std::exception &ex) {
    fprintf(gpFILE, "\nException occurred.");
  }
}

void Mesh::draw() {
  // code

  glBindVertexArray(vao);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index);
  glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

  glBindVertexArray(0);
}

void Mesh::prepareToDraw() {
  // code

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // vbo for position
  glGenBuffers(1, &vbo_position);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), verts.data(),
               GL_STATIC_DRAW);
  fprintf(gpFILE, "\nVertices size = %lu", sizeof(verts));
  glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // vbo for normals
  glGenBuffers(1, &vbo_normal);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * norms.size(), norms.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // vbo for texture
  glGenBuffers(1, &vbo_texture);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_texture);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size(),
               texCoords.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // vbo for index
  glGenBuffers(1, &vbo_index);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_index);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * elements.size(),
               elements.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  // cleanupMeshData();
}

int Mesh::getIndexCount() {
  // code
  return (numElements);
}

int Mesh::getVertexCount() {
  // code
  return (numVertices);
}

void Mesh::cleanupMeshData() {

  //   if (elements) {
  //     free(elements);
  //     elements = NULL;
  //   }

  //   if (verts) {
  //     free(verts);
  //     verts = NULL;
  //   }

  //   if (norms) {
  //     free(norms);
  //     norms = NULL;
  //   }

  //   if (texCoords) {
  //     free(texCoords);
  //     texCoords = NULL;
  //   }
  verts.clear();
  elements.clear();
  norms.clear();
  texCoords.clear();
}

void Mesh::normalizeVector(float *v) {
  // code

  // square the vector length
  float squaredVectorLength = (v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]);

  // get square root of above 'squared vector length'
  float squareRootOfSquaredVectorLength = std::sqrt(squaredVectorLength);

  // scale the vector with 1/squareRootOfSquaredVectorLength
  v[0] = v[0] * 1.0 / squareRootOfSquaredVectorLength;
  v[1] = v[1] * 1.0 / squareRootOfSquaredVectorLength;
  v[2] = v[2] * 1.0 / squareRootOfSquaredVectorLength;
}

void Mesh::addTriangle(float single_vertex[4][3], float single_normal[4][3],
                       float single_texture[4][2]) {
  float diff = 0.00001;
  int i, j;

  // normals should be of unit length
  normalizeVector(single_normal[0]);
  normalizeVector(single_normal[1]);
  normalizeVector(single_normal[2]);

  for (i = 0; i < 3; i++) {
    for (j = 0; j < numVertices;
         j++) // for the first ever iteration of 'j', numVertices will be 0
              // because of it's initialization in the parameterized constructor
    {
      if (isFoundIdentical(verts[j * 3], single_vertex[i][0], diff) &&
          isFoundIdentical(verts[(j * 3) + 1], single_vertex[i][1], diff) &&
          isFoundIdentical(verts[(j * 3) + 2], single_vertex[i][2], diff) &&

          isFoundIdentical(norms[j * 3], single_normal[i][0], diff) &&
          isFoundIdentical(norms[(j * 3) + 1], single_normal[i][1], diff) &&
          isFoundIdentical(norms[(j * 3) + 2], single_normal[i][2], diff) &&

          isFoundIdentical(texCoords[j * 2], single_texture[i][0], diff) &&
          isFoundIdentical(texCoords[(j * 2) + 1], single_texture[i][1],
                           diff)) {
        elements[numElements] = j;
        numElements++;
        break;
      }
    }

    // If the single vertex, normal and texture do not match with the given,
    // then add the corressponding triangle to the end of the list
    if (j == numVertices && numVertices < maxElements &&
        numElements < maxElements) {
      verts[numVertices * 3] = single_vertex[i][0];
      verts[(numVertices * 3) + 1] = single_vertex[i][1];
      verts[(numVertices * 3) + 2] = single_vertex[i][2];

      norms[numVertices * 3] = single_normal[i][0];
      norms[(numVertices * 3) + 1] = single_normal[i][1];
      norms[(numVertices * 3) + 2] = single_normal[i][2];

      texCoords[numVertices * 2] = single_texture[i][0];
      texCoords[(numVertices * 2) + 1] = single_texture[i][1];

      elements[numElements] = numVertices; // adding the index to the end of the
                                           // list of elements/indices
      numElements++;                       // incrementing the 'end' of the list
      numVertices++;                       // incrementing coun of vertices
    }
  }
}

bool Mesh::isFoundIdentical(float val1, float val2, float diff) {
  // code
  if (std::abs(val1 - val2) < diff)
    return (true);
  else
    return (false);
}

void makeSphere(Mesh &sphereMesh, float fRadius, int iSlices, int iStacks) {
  // code
  float drho = (float)VDG_PI / (float)iStacks;
  float dtheta = 2.0f * (float)VDG_PI / (float)iSlices;
  float ds = 1.0f / (float)iSlices;
  float dt = 1.0f / (float)iStacks;
  float t = 1.0f;
  float s = 0.0f;
  int i = 0;
  int j = 0;

  sphereMesh.allocate(iSlices * iStacks * 6);

  for (i = 0; i < iStacks; i++) {
    float rho = (float)(i * drho);
    float srho = (float)std::sin(rho);
    float crho = (float)std::cos(rho);
    float srhodrho = (float)std::sin(rho + drho);
    float crhodrho = (float)std::cos(rho + drho);

    // Many sources of OpenGL sphere drawing code uses a triangle fan
    // for the caps of the sphere. This however introduces texturing
    // artifacts at the poles on some OpenGL implementations
    s = 0.0f;

    // initialization of three 2-D arrays, two are 4 x 3 and one is 4 x 2
    float vertex[4][3];
    float normal[4][3];
    float texture[4][2];

    for (j = 0; j < iSlices; j++) {
      float theta = (j == iSlices) ? 0.0f : j * dtheta;
      float stheta = (float)-std::sin(theta);
      float ctheta = (float)std::cos(theta);

      float x = stheta * srho;
      float y = ctheta * srho;
      float z = crho;

      texture[0][0] = s;
      texture[0][1] = t;
      normal[0][0] = x;
      normal[0][1] = y;
      normal[0][2] = z;
      vertex[0][0] = x * fRadius;
      vertex[0][1] = y * fRadius;
      vertex[0][2] = z * fRadius;

      x = stheta * srhodrho;
      y = ctheta * srhodrho;
      z = crhodrho;

      texture[1][0] = s;
      texture[1][1] = t - dt;
      normal[1][0] = x;
      normal[1][1] = y;
      normal[1][2] = z;
      vertex[1][0] = x * fRadius;
      vertex[1][1] = y * fRadius;
      vertex[1][2] = z * fRadius;

      theta = ((j + 1) == iSlices) ? 0.0 : (j + 1) * dtheta;
      stheta = (float)-std::sin(theta);
      ctheta = (float)std::cos(theta);

      x = stheta * srho;
      y = ctheta * srho;
      z = crho;

      s += ds;
      texture[2][0] = s;
      texture[2][1] = t;
      normal[2][0] = x;
      normal[2][1] = y;
      normal[2][2] = z;
      vertex[2][0] = x * fRadius;
      vertex[2][1] = y * fRadius;
      vertex[2][2] = z * fRadius;

      x = stheta * srhodrho;
      y = ctheta * srhodrho;
      z = crhodrho;

      texture[3][0] = s;
      texture[3][1] = t - dt;
      normal[3][0] = x;
      normal[3][1] = y;
      normal[3][2] = z;
      vertex[3][0] = x * fRadius;
      vertex[3][1] = y * fRadius;
      vertex[3][2] = z * fRadius;

      sphereMesh.addTriangle(vertex, normal, texture);

      // Rearrange for next triangle
      vertex[0][0] = vertex[1][0];
      vertex[0][1] = vertex[1][1];
      vertex[0][2] = vertex[1][2];
      normal[0][0] = normal[1][0];
      normal[0][1] = normal[1][1];
      normal[0][2] = normal[1][2];
      texture[0][0] = texture[1][0];
      texture[0][1] = texture[1][1];

      vertex[1][0] = vertex[3][0];
      vertex[1][1] = vertex[3][1];
      vertex[1][2] = vertex[3][2];
      normal[1][0] = normal[3][0];
      normal[1][1] = normal[3][1];
      normal[1][2] = normal[3][2];
      texture[1][0] = texture[3][0];
      texture[1][1] = texture[3][1];

      sphereMesh.addTriangle(vertex, normal, texture);
    }
    t -= dt;
  }

  sphereMesh.prepareToDraw();
}
