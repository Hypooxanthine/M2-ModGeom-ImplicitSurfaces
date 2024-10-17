#include "Implicits/implicits.h"

#include "SceneGraph/NodeEditor/NodeEditor.h"

const float AnalyticScalarField::Epsilon = 1e-6f;

/*!
\brief Constructor.
*/
AnalyticScalarField::AnalyticScalarField()
{
}

std::unique_ptr<NodeEditor> AnalyticScalarField::instanciateEditor(SceneNode* node)
{
    return std::unique_ptr<NodeEditor>(new NodeEditor(node));
}

/*!
\brief Compute the value of the field.
\param p Point.
*/
float AnalyticScalarField::Value(const glm::vec3& p) const
{
  return glm::length(p) - 1.f;
}

/*!
\brief Compute the polygonal mesh approximating the implicit surface.

\param box %Box defining the region that will be polygonized.
\param n Discretization parameter.
\param g Returned geometry.
\param epsilon Epsilon value for computing vertices on straddling edges.
*/
void AnalyticScalarField::Polygonize(int n, vrm::MeshData& g, Box box, const float& epsilon) const
{
  for(glm::length_t i = 0; i < 3; i++)
    if (box[0][i] < box[1][i])
      std::swap(box[0][i], box[1][i]);


  std::vector<glm::vec3> vertex;
  std::vector<glm::vec3> normal;

  std::vector<int> triangle;

  vertex.reserve(20000);
  normal.reserve(20000);
  triangle.reserve(20000);

  int nv = 0;
  const int nx = n;
  const int ny = n;
  const int nz = n;

  Box clipped = box;

  // Clamped integer values
  const int nax = 0;
  const int nbx = nx;
  const int nay = 0;
  const int nby = ny;
  const int naz = 0;
  const int nbz = nz;

  const int size = nx * ny;

  // Intensities
  float* a = new float[size];
  float* b = new float[size];

  // Vertex
  glm::vec3* u = new glm::vec3[size];
  glm::vec3* v = new glm::vec3[size];

  // Edges
  int* eax = new int[size];
  int* eay = new int[size];
  int* ebx = new int[size];
  int* eby = new int[size];
  int* ez = new int[size];

  // Diagonal of a cell
  glm::vec3 d = clipped.Diagonal() / (float)(n - 1);

  float za = 0.0;

  // Compute field inside lower Oxy plane
  for (int i = nax; i < nbx; i++)
  {
    for (int j = nay; j < nby; j++)
    {
      u[i * ny + j] = clipped[0] + glm::vec3(i * d[0], j * d[1], za);
      a[i * ny + j] = Value(u[i * ny + j]);
    }
  }

  // Compute straddling edges inside lower Oxy plane
  for (int i = nax; i < nbx - 1; i++)
  {
    for (int j = nay; j < nby; j++)
    {
      // We need a xor b, which can be implemented a == !b 
      if (!((a[i * ny + j] < 0.0) == !(a[(i + 1) * ny + j] >= 0.0)))
      {
        vertex.push_back(Dichotomy(u[i * ny + j], u[(i + 1) * ny + j], a[i * ny + j], a[(i + 1) * ny + j], d[0], epsilon));
        normal.push_back(Normal(vertex.back()));
        eax[i * ny + j] = nv;
        nv++;
      }
    }
  }
  for (int i = nax; i < nbx; i++)
  {
    for (int j = nay; j < nby - 1; j++)
    {
      if (!((a[i * ny + j] < 0.0) == !(a[i * ny + (j + 1)] >= 0.0)))
      {
        vertex.push_back(Dichotomy(u[i * ny + j], u[i * ny + (j + 1)], a[i * ny + j], a[i * ny + (j + 1)], d[1], epsilon));
        normal.push_back(Normal(vertex.back()));
        eay[i * ny + j] = nv;
        nv++;
      }
    }
  }

  // Array for edge vertices
  int e[12];

  // For all layers
  for (int k = naz; k < nbz; k++)
  {
    float zb = za + d[2];
    for (int i = nax; i < nbx; i++)
    {
      for (int j = nay; j < nby; j++)
      {
        v[i * ny + j] = clipped[0] + glm::vec3(i * d[0], j * d[1], zb);
        b[i * ny + j] = Value(v[i * ny + j]);
      }
    }

    // Compute straddling edges inside lower Oxy plane
    for (int i = nax; i < nbx - 1; i++)
    {
      for (int j = nay; j < nby; j++)
      {
        //   if (((b[i*ny + j] < 0.0) && (b[(i + 1)*ny + j] >= 0.0)) || ((b[i*ny + j] >= 0.0) && (b[(i + 1)*ny + j] < 0.0)))
        if (!((b[i * ny + j] < 0.0) == !(b[(i + 1) * ny + j] >= 0.0)))
        {
          vertex.push_back(Dichotomy(v[i * ny + j], v[(i + 1) * ny + j], b[i * ny + j], b[(i + 1) * ny + j], d[0], epsilon));
          normal.push_back(Normal(vertex.back()));
          ebx[i * ny + j] = nv;
          nv++;
        }
      }
    }

    for (int i = nax; i < nbx; i++)
    {
      for (int j = nay; j < nby - 1; j++)
      {
        // if (((b[i*ny + j] < 0.0) && (b[i*ny + (j + 1)] >= 0.0)) || ((b[i*ny + j] >= 0.0) && (b[i*ny + (j + 1)] < 0.0)))
        if (!((b[i * ny + j] < 0.0) == !(b[i * ny + (j + 1)] >= 0.0)))
        {
          vertex.push_back(Dichotomy(v[i * ny + j], v[i * ny + (j + 1)], b[i * ny + j], b[i * ny + (j + 1)], d[1], epsilon));
          normal.push_back(Normal(vertex.back()));
          eby[i * ny + j] = nv;
          nv++;
        }
      }
    }

    // Create vertical straddling edges
    for (int i = nax; i < nbx; i++)
    {
      for (int j = nay; j < nby; j++)
      {
        // if ((a[i*ny + j] < 0.0) && (b[i*ny + j] >= 0.0) || (a[i*ny + j] >= 0.0) && (b[i*ny + j] < 0.0))
        if (!((a[i * ny + j] < 0.0) == !(b[i * ny + j] >= 0.0)))
        {
          vertex.push_back(Dichotomy(u[i * ny + j], v[i * ny + j], a[i * ny + j], b[i * ny + j], d[2], epsilon));
          normal.push_back(Normal(vertex.back()));
          ez[i * ny + j] = nv;
          nv++;
        }
      }
    }

    // Create mesh
    for (int i = nax; i < nbx - 1; i++)
    {
      for (int j = nay; j < nby - 1; j++)
      {
        int cubeindex = 0;
        if (a[i * ny + j] < 0.0)       cubeindex |= 1;
        if (a[(i + 1) * ny + j] < 0.0)   cubeindex |= 2;
        if (a[i * ny + j + 1] < 0.0)     cubeindex |= 4;
        if (a[(i + 1) * ny + j + 1] < 0.0) cubeindex |= 8;
        if (b[i * ny + j] < 0.0)       cubeindex |= 16;
        if (b[(i + 1) * ny + j] < 0.0)   cubeindex |= 32;
        if (b[i * ny + j + 1] < 0.0)     cubeindex |= 64;
        if (b[(i + 1) * ny + j + 1] < 0.0) cubeindex |= 128;

        // Cube is straddling the surface
        if ((cubeindex != 255) && (cubeindex != 0))
        {
          e[0] = eax[i * ny + j];
          e[1] = eax[i * ny + (j + 1)];
          e[2] = ebx[i * ny + j];
          e[3] = ebx[i * ny + (j + 1)];
          e[4] = eay[i * ny + j];
          e[5] = eay[(i + 1) * ny + j];
          e[6] = eby[i * ny + j];
          e[7] = eby[(i + 1) * ny + j];
          e[8] = ez[i * ny + j];
          e[9] = ez[(i + 1) * ny + j];
          e[10] = ez[i * ny + (j + 1)];
          e[11] = ez[(i + 1) * ny + (j + 1)];

          for (int h = 0; TriangleTable[cubeindex][h] != -1; h += 3)
          {
            triangle.push_back(e[TriangleTable[cubeindex][h + 0]]);
            triangle.push_back(e[TriangleTable[cubeindex][h + 1]]);
            triangle.push_back(e[TriangleTable[cubeindex][h + 2]]);
          }
        }
      }
    }

    std::swap(a, b);

    za = zb;
    std::swap(eax, ebx);
    std::swap(eay, eby);
    std::swap(u, v);
  }

  delete[]a;
  delete[]b;
  delete[]u;
  delete[]v;

  delete[]eax;
  delete[]eay;
  delete[]ebx;
  delete[]eby;
  delete[]ez;

  std::vector<int> normals = triangle;

  // Just to make it compatible with the mesh data structure...

  std::vector<vrm::Vertex> vertexData;
  vertexData.resize(vertex.size());

  std::vector<uint32_t> indexData;
  indexData.resize(triangle.size());

  for (size_t i = 0; i < vertex.size(); i++)
  {
    vertexData[i].position = vertex[i];
    vertexData[i].normal = normal[i];
  }

  for (size_t i = 0; i < triangle.size(); i++)
  {
    indexData[i] = triangle[i];
  }

  g = vrm::MeshData(std::move(vertexData), std::move(indexData));
}

/*!
\brief Compute the intersection between a segment and an implicit surface.

\param a,b End vertices of the segment straddling the surface.
\param va,vb Field function value at those end vertices.
\param length Distance between vertices.
\param epsilon Precision.
\return Point on the implicit surface.
*/
glm::vec3 AnalyticScalarField::Dichotomy(glm::vec3 a, glm::vec3 b, float va, float vb, float length, const float& epsilon) const
{
  int ia = va > 0.0 ? 1 : -1;

  // Get an accurate first guess
  glm::vec3 c = (vb * a - va * b) / (vb - va);

  while (length > epsilon)
  {
    float vc = Value(c);
    int ic = vc > 0.0 ? 1 : -1;
    if (ia + ic == 0)
    {
      b = c;
    }
    else
    {
      ia = ic;
      a = c;
    }
    length *= 0.5;
    c = 0.5f * (a + b);
  }
  return c;
}


/*!
\brief Compute the gradient of the field.
\param p Point.
*/
glm::vec3 AnalyticScalarField::Gradient(const glm::vec3& p) const
{
  float x = Value(glm::vec3(p[0] + Epsilon, p[1], p[2])) - Value(glm::vec3(p[0] - Epsilon, p[1], p[2]));
  float y = Value(glm::vec3(p[0], p[1] + Epsilon, p[2])) - Value(glm::vec3(p[0], p[1] - Epsilon, p[2]));
  float z = Value(glm::vec3(p[0], p[1], p[2] + Epsilon)) - Value(glm::vec3(p[0], p[1], p[2] - Epsilon));

  return glm::vec3(x, y, z) * (0.5f / Epsilon);
}

/*!
\brief Compute the normal to the surface.

\sa AnalyticScalarField::Gradient(const glm::vec3&) const

\param p Point (should be on the surface).
*/
glm::vec3 AnalyticScalarField::Normal(const glm::vec3& p) const
{
  glm::vec3 normal = glm::normalize(Gradient(p));

  return normal;
}


int AnalyticScalarField::edgeTable[256] = {
  0, 273, 545, 816, 1042, 1283, 1587, 1826, 2082, 2355, 2563, 2834, 3120, 3361, 3601, 3840,
  324, 85, 869, 628, 1366, 1095, 1911, 1638, 2406, 2167, 2887, 2646, 3444, 3173, 3925, 3652,
  644, 917, 165, 436, 1686, 1927, 1207, 1446, 2726, 2999, 2183, 2454, 3764, 4005, 3221, 3460,
  960, 721, 481, 240, 2002, 1731, 1523, 1250, 3042, 2803, 2499, 2258, 4080, 3809, 3537, 3264,
  1096, 1369, 1641, 1912, 90, 331, 635, 874, 3178, 3451, 3659, 3930, 2168, 2409, 2649, 2888,
  1292, 1053, 1837, 1596, 286, 15, 831, 558, 3374, 3135, 3855, 3614, 2364, 2093, 2845, 2572,
  1740, 2013, 1261, 1532, 734, 975, 255, 494, 3822, 4095, 3279, 3550, 2812, 3053, 2269, 2508,
  1928, 1689, 1449, 1208, 922, 651, 443, 170, 4010, 3771, 3467, 3226, 3000, 2729, 2457, 2184,
  2184, 2457, 2729, 3000, 3226, 3467, 3771, 4010, 170, 443, 651, 922, 1208, 1449, 1689, 1928,
  2508, 2269, 3053, 2812, 3550, 3279, 4095, 3822, 494, 255, 975, 734, 1532, 1261, 2013, 1740,
  2572, 2845, 2093, 2364, 3614, 3855, 3135, 3374, 558, 831, 15, 286, 1596, 1837, 1053, 1292,
  2888, 2649, 2409, 2168, 3930, 3659, 3451, 3178, 874, 635, 331, 90, 1912, 1641, 1369, 1096,
  3264, 3537, 3809, 4080, 2258, 2499, 2803, 3042, 1250, 1523, 1731, 2002, 240, 481, 721, 960,
  3460, 3221, 4005, 3764, 2454, 2183, 2999, 2726, 1446, 1207, 1927, 1686, 436, 165, 917, 644,
  3652, 3925, 3173, 3444, 2646, 2887, 2167, 2406, 1638, 1911, 1095, 1366, 628, 869, 85, 324,
  3840, 3601, 3361, 3120, 2834, 2563, 2355, 2082, 1826, 1587, 1283, 1042, 816, 545, 273, 0
};

int AnalyticScalarField::TriangleTable[256][16] = {
  { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 8, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 5, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 8, 4, 9, 8, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 10, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 10, 1, 8, 10, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 9, 0, 1, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 10, 1, 5, 9, 10, 9, 8, 10, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 1, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 8, 4, 5, 1, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 1, 11, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 1, 8, 4, 1, 11, 8, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 11, 5, 10, 11, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 11, 5, 0, 8, 11, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 9, 0, 4, 10, 9, 10, 11, 9, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 8, 11, 11, 8, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 2, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 2, 4, 0, 6, 4, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 5, 9, 8, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 2, 5, 9, 2, 6, 5, 6, 4, 5, -1, -1, -1, -1, -1, -1, -1 },
  { 8, 2, 6, 4, 10, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 10, 2, 6, 10, 1, 2, 1, 0, 2, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 0, 5, 8, 2, 6, 1, 4, 10, -1, -1, -1, -1, -1, -1, -1 },
  { 2, 6, 10, 9, 2, 10, 9, 10, 1, 9, 1, 5, -1, -1, -1, -1 },
  { 5, 1, 11, 8, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 2, 6, 4, 0, 2, 5, 1, 11, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 1, 11, 9, 0, 1, 8, 2, 6, -1, -1, -1, -1, -1, -1, -1 },
  { 1, 11, 9, 1, 9, 6, 1, 6, 4, 6, 9, 2, -1, -1, -1, -1 },
  { 4, 11, 5, 4, 10, 11, 6, 8, 2, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 10, 11, 5, 2, 10, 5, 0, 2, 6, 10, 2, -1, -1, -1, -1 },
  { 2, 6, 8, 9, 0, 10, 9, 10, 11, 10, 0, 4, -1, -1, -1, -1 },
  { 2, 6, 10, 2, 10, 9, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 7, 2, 0, 8, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 7, 2, 5, 7, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 8, 7, 2, 8, 4, 7, 4, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 7, 2, 1, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 10, 1, 0, 8, 10, 2, 9, 7, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 7, 2, 0, 5, 7, 1, 4, 10, -1, -1, -1, -1, -1, -1, -1 },
  { 1, 5, 7, 1, 7, 8, 1, 8, 10, 2, 8, 7, -1, -1, -1, -1 },
  { 5, 1, 11, 9, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 0, 8, 5, 1, 11, 2, 9, 7, -1, -1, -1, -1, -1, -1, -1 },
  { 7, 1, 11, 7, 2, 1, 2, 0, 1, -1, -1, -1, -1, -1, -1, -1 },
  { 1, 11, 7, 4, 1, 7, 4, 7, 2, 4, 2, 8, -1, -1, -1, -1 },
  { 11, 4, 10, 11, 5, 4, 9, 7, 2, -1, -1, -1, -1, -1, -1, -1 },
  { 2, 9, 7, 0, 8, 5, 8, 11, 5, 8, 10, 11, -1, -1, -1, -1 },
  { 7, 2, 0, 7, 0, 10, 7, 10, 11, 10, 0, 4, -1, -1, -1, -1 },
  { 7, 2, 8, 7, 8, 11, 11, 8, 10, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 6, 8, 7, 6, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 4, 0, 9, 7, 4, 7, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 6, 8, 0, 5, 6, 5, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 7, 4, 4, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 6, 9, 7, 6, 8, 9, 4, 10, 1, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 7, 6, 9, 6, 1, 9, 1, 0, 1, 6, 10, -1, -1, -1, -1 },
  { 1, 4, 10, 0, 5, 8, 5, 6, 8, 5, 7, 6, -1, -1, -1, -1 },
  { 10, 1, 5, 10, 5, 6, 6, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 6, 8, 9, 7, 6, 11, 5, 1, -1, -1, -1, -1, -1, -1, -1 },
  { 11, 5, 1, 9, 7, 0, 7, 4, 0, 7, 6, 4, -1, -1, -1, -1 },
  { 8, 0, 1, 8, 1, 7, 8, 7, 6, 11, 7, 1, -1, -1, -1, -1 },
  { 1, 11, 7, 1, 7, 4, 4, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 7, 8, 8, 7, 6, 11, 5, 4, 11, 4, 10, -1, -1, -1, -1 },
  { 7, 6, 0, 7, 0, 9, 6, 10, 0, 5, 0, 11, 10, 11, 0, -1 },
  { 10, 11, 0, 10, 0, 4, 11, 7, 0, 8, 0, 6, 7, 6, 0, -1 },
  { 10, 11, 7, 6, 10, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 6, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 0, 8, 10, 6, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 5, 9, 10, 6, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 8, 5, 9, 8, 4, 5, 10, 6, 3, -1, -1, -1, -1, -1, -1, -1 },
  { 6, 1, 4, 3, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 6, 0, 8, 6, 3, 0, 3, 1, 0, -1, -1, -1, -1, -1, -1, -1 },
  { 1, 6, 3, 1, 4, 6, 0, 5, 9, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 3, 1, 5, 8, 3, 5, 9, 8, 8, 6, 3, -1, -1, -1, -1 },
  { 11, 5, 1, 3, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 1, 11, 4, 0, 8, 3, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
  { 1, 9, 0, 1, 11, 9, 3, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
  { 3, 10, 6, 1, 11, 4, 11, 8, 4, 11, 9, 8, -1, -1, -1, -1 },
  { 11, 6, 3, 11, 5, 6, 5, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
  { 11, 6, 3, 5, 6, 11, 5, 8, 6, 5, 0, 8, -1, -1, -1, -1 },
  { 0, 4, 6, 0, 6, 11, 0, 11, 9, 3, 11, 6, -1, -1, -1, -1 },
  { 6, 3, 11, 6, 11, 8, 8, 11, 9, -1, -1, -1, -1, -1, -1, -1 },
  { 3, 8, 2, 10, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 3, 10, 4, 0, 3, 0, 2, 3, -1, -1, -1, -1, -1, -1, -1 },
  { 8, 3, 10, 8, 2, 3, 9, 0, 5, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 2, 3, 9, 3, 4, 9, 4, 5, 10, 4, 3, -1, -1, -1, -1 },
  { 8, 1, 4, 8, 2, 1, 2, 3, 1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 2, 1, 2, 3, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 9, 0, 1, 4, 2, 1, 2, 3, 2, 4, 8, -1, -1, -1, -1 },
  { 5, 9, 2, 5, 2, 1, 1, 2, 3, -1, -1, -1, -1, -1, -1, -1 },
  { 3, 8, 2, 3, 10, 8, 1, 11, 5, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 1, 11, 4, 0, 10, 0, 3, 10, 0, 2, 3, -1, -1, -1, -1 },
  { 2, 10, 8, 2, 3, 10, 0, 1, 9, 1, 11, 9, -1, -1, -1, -1 },
  { 11, 9, 4, 11, 4, 1, 9, 2, 4, 10, 4, 3, 2, 3, 4, -1 },
  { 8, 5, 4, 8, 3, 5, 8, 2, 3, 3, 11, 5, -1, -1, -1, -1 },
  { 11, 5, 0, 11, 0, 3, 3, 0, 2, -1, -1, -1, -1, -1, -1, -1 },
  { 2, 3, 4, 2, 4, 8, 3, 11, 4, 0, 4, 9, 11, 9, 4, -1 },
  { 11, 9, 2, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 2, 9, 7, 6, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 8, 4, 2, 9, 7, 10, 6, 3, -1, -1, -1, -1, -1, -1, -1 },
  { 7, 0, 5, 7, 2, 0, 6, 3, 10, -1, -1, -1, -1, -1, -1, -1 },
  { 10, 6, 3, 8, 4, 2, 4, 7, 2, 4, 5, 7, -1, -1, -1, -1 },
  { 6, 1, 4, 6, 3, 1, 7, 2, 9, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 7, 2, 0, 8, 3, 0, 3, 1, 3, 8, 6, -1, -1, -1, -1 },
  { 4, 3, 1, 4, 6, 3, 5, 7, 0, 7, 2, 0, -1, -1, -1, -1 },
  { 3, 1, 8, 3, 8, 6, 1, 5, 8, 2, 8, 7, 5, 7, 8, -1 },
  { 9, 7, 2, 11, 5, 1, 6, 3, 10, -1, -1, -1, -1, -1, -1, -1 },
  { 3, 10, 6, 5, 1, 11, 0, 8, 4, 2, 9, 7, -1, -1, -1, -1 },
  { 6, 3, 10, 7, 2, 11, 2, 1, 11, 2, 0, 1, -1, -1, -1, -1 },
  { 4, 2, 8, 4, 7, 2, 4, 1, 7, 11, 7, 1, 10, 6, 3, -1 },
  { 9, 7, 2, 11, 5, 3, 5, 6, 3, 5, 4, 6, -1, -1, -1, -1 },
  { 5, 3, 11, 5, 6, 3, 5, 0, 6, 8, 6, 0, 9, 7, 2, -1 },
  { 2, 0, 11, 2, 11, 7, 0, 4, 11, 3, 11, 6, 4, 6, 11, -1 },
  { 6, 3, 11, 6, 11, 8, 7, 2, 11, 2, 8, 11, -1, -1, -1, -1 },
  { 3, 9, 7, 3, 10, 9, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 3, 10, 0, 3, 4, 0, 7, 3, 0, 9, 7, -1, -1, -1, -1 },
  { 0, 10, 8, 0, 7, 10, 0, 5, 7, 7, 3, 10, -1, -1, -1, -1 },
  { 3, 10, 4, 3, 4, 7, 7, 4, 5, -1, -1, -1, -1, -1, -1, -1 },
  { 7, 8, 9, 7, 1, 8, 7, 3, 1, 4, 8, 1, -1, -1, -1, -1 },
  { 9, 7, 3, 9, 3, 0, 0, 3, 1, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 7, 8, 5, 8, 0, 7, 3, 8, 4, 8, 1, 3, 1, 8, -1 },
  { 5, 7, 3, 1, 5, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 1, 11, 9, 7, 10, 9, 10, 8, 10, 7, 3, -1, -1, -1, -1 },
  { 0, 10, 4, 0, 3, 10, 0, 9, 3, 7, 3, 9, 5, 1, 11, -1 },
  { 10, 8, 7, 10, 7, 3, 8, 0, 7, 11, 7, 1, 0, 1, 7, -1 },
  { 3, 10, 4, 3, 4, 7, 1, 11, 4, 11, 7, 4, -1, -1, -1, -1 },
  { 5, 4, 3, 5, 3, 11, 4, 8, 3, 7, 3, 9, 8, 9, 3, -1 },
  { 11, 5, 0, 11, 0, 3, 9, 7, 0, 7, 3, 0, -1, -1, -1, -1 },
  { 0, 4, 8, 7, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 11, 7, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 11, 3, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 8, 4, 7, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 0, 5, 7, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 8, 4, 5, 9, 8, 7, 11, 3, -1, -1, -1, -1, -1, -1, -1 },
  { 1, 4, 10, 11, 3, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 10, 0, 8, 10, 1, 0, 11, 3, 7, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 5, 9, 1, 4, 10, 7, 11, 3, -1, -1, -1, -1, -1, -1, -1 },
  { 7, 11, 3, 5, 9, 1, 9, 10, 1, 9, 8, 10, -1, -1, -1, -1 },
  { 5, 3, 7, 1, 3, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 3, 7, 5, 1, 3, 4, 0, 8, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 3, 7, 9, 0, 3, 0, 1, 3, -1, -1, -1, -1, -1, -1, -1 },
  { 7, 9, 8, 7, 8, 1, 7, 1, 3, 4, 1, 8, -1, -1, -1, -1 },
  { 3, 4, 10, 3, 7, 4, 7, 5, 4, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 8, 10, 0, 10, 7, 0, 7, 5, 7, 10, 3, -1, -1, -1, -1 },
  { 4, 10, 3, 0, 4, 3, 0, 3, 7, 0, 7, 9, -1, -1, -1, -1 },
  { 3, 7, 9, 3, 9, 10, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1 },
  { 7, 11, 3, 2, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 2, 4, 0, 2, 6, 4, 3, 7, 11, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 9, 0, 7, 11, 3, 8, 2, 6, -1, -1, -1, -1, -1, -1, -1 },
  { 11, 3, 7, 5, 9, 6, 5, 6, 4, 6, 9, 2, -1, -1, -1, -1 },
  { 4, 10, 1, 6, 8, 2, 11, 3, 7, -1, -1, -1, -1, -1, -1, -1 },
  { 7, 11, 3, 2, 6, 1, 2, 1, 0, 1, 6, 10, -1, -1, -1, -1 },
  { 0, 5, 9, 2, 6, 8, 1, 4, 10, 7, 11, 3, -1, -1, -1, -1 },
  { 9, 1, 5, 9, 10, 1, 9, 2, 10, 6, 10, 2, 7, 11, 3, -1 },
  { 3, 5, 1, 3, 7, 5, 2, 6, 8, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 1, 7, 7, 1, 3, 4, 0, 2, 4, 2, 6, -1, -1, -1, -1 },
  { 8, 2, 6, 9, 0, 7, 0, 3, 7, 0, 1, 3, -1, -1, -1, -1 },
  { 6, 4, 9, 6, 9, 2, 4, 1, 9, 7, 9, 3, 1, 3, 9, -1 },
  { 8, 2, 6, 4, 10, 7, 4, 7, 5, 7, 10, 3, -1, -1, -1, -1 },
  { 7, 5, 10, 7, 10, 3, 5, 0, 10, 6, 10, 2, 0, 2, 10, -1 },
  { 0, 7, 9, 0, 3, 7, 0, 4, 3, 10, 3, 4, 8, 2, 6, -1 },
  { 3, 7, 9, 3, 9, 10, 2, 6, 9, 6, 10, 9, -1, -1, -1, -1 },
  { 11, 2, 9, 3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 2, 11, 3, 2, 9, 11, 0, 8, 4, -1, -1, -1, -1, -1, -1, -1 },
  { 11, 0, 5, 11, 3, 0, 3, 2, 0, -1, -1, -1, -1, -1, -1, -1 },
  { 8, 4, 5, 8, 5, 3, 8, 3, 2, 3, 5, 11, -1, -1, -1, -1 },
  { 11, 2, 9, 11, 3, 2, 10, 1, 4, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 8, 1, 1, 8, 10, 2, 9, 11, 2, 11, 3, -1, -1, -1, -1 },
  { 4, 10, 1, 0, 5, 3, 0, 3, 2, 3, 5, 11, -1, -1, -1, -1 },
  { 3, 2, 5, 3, 5, 11, 2, 8, 5, 1, 5, 10, 8, 10, 5, -1 },
  { 5, 2, 9, 5, 1, 2, 1, 3, 2, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 0, 8, 5, 1, 9, 1, 2, 9, 1, 3, 2, -1, -1, -1, -1 },
  { 0, 1, 2, 2, 1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 8, 4, 1, 8, 1, 2, 2, 1, 3, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 3, 2, 9, 4, 3, 9, 5, 4, 10, 3, 4, -1, -1, -1, -1 },
  { 8, 10, 5, 8, 5, 0, 10, 3, 5, 9, 5, 2, 3, 2, 5, -1 },
  { 4, 10, 3, 4, 3, 0, 0, 3, 2, -1, -1, -1, -1, -1, -1, -1 },
  { 3, 2, 8, 10, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 6, 11, 3, 6, 8, 11, 8, 9, 11, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 6, 4, 0, 11, 6, 0, 9, 11, 3, 6, 11, -1, -1, -1, -1 },
  { 11, 3, 6, 5, 11, 6, 5, 6, 8, 5, 8, 0, -1, -1, -1, -1 },
  { 11, 3, 6, 11, 6, 5, 5, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
  { 1, 4, 10, 11, 3, 8, 11, 8, 9, 8, 3, 6, -1, -1, -1, -1 },
  { 1, 0, 6, 1, 6, 10, 0, 9, 6, 3, 6, 11, 9, 11, 6, -1 },
  { 5, 8, 0, 5, 6, 8, 5, 11, 6, 3, 6, 11, 1, 4, 10, -1 },
  { 10, 1, 5, 10, 5, 6, 11, 3, 5, 3, 6, 5, -1, -1, -1, -1 },
  { 5, 1, 3, 5, 3, 8, 5, 8, 9, 8, 3, 6, -1, -1, -1, -1 },
  { 1, 3, 9, 1, 9, 5, 3, 6, 9, 0, 9, 4, 6, 4, 9, -1 },
  { 6, 8, 0, 6, 0, 3, 3, 0, 1, -1, -1, -1, -1, -1, -1, -1 },
  { 6, 4, 1, 3, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 8, 9, 3, 8, 3, 6, 9, 5, 3, 10, 3, 4, 5, 4, 3, -1 },
  { 0, 9, 5, 10, 3, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 6, 8, 0, 6, 0, 3, 4, 10, 0, 10, 3, 0, -1, -1, -1, -1 },
  { 6, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 10, 7, 11, 6, 7, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 10, 7, 11, 10, 6, 7, 8, 4, 0, -1, -1, -1, -1, -1, -1, -1 },
  { 7, 10, 6, 7, 11, 10, 5, 9, 0, -1, -1, -1, -1, -1, -1, -1 },
  { 11, 6, 7, 11, 10, 6, 9, 8, 5, 8, 4, 5, -1, -1, -1, -1 },
  { 1, 7, 11, 1, 4, 7, 4, 6, 7, -1, -1, -1, -1, -1, -1, -1 },
  { 8, 1, 0, 8, 7, 1, 8, 6, 7, 11, 1, 7, -1, -1, -1, -1 },
  { 9, 0, 5, 7, 11, 4, 7, 4, 6, 4, 11, 1, -1, -1, -1, -1 },
  { 9, 8, 1, 9, 1, 5, 8, 6, 1, 11, 1, 7, 6, 7, 1, -1 },
  { 10, 5, 1, 10, 6, 5, 6, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 8, 4, 5, 1, 6, 5, 6, 7, 6, 1, 10, -1, -1, -1, -1 },
  { 9, 6, 7, 9, 1, 6, 9, 0, 1, 1, 10, 6, -1, -1, -1, -1 },
  { 6, 7, 1, 6, 1, 10, 7, 9, 1, 4, 1, 8, 9, 8, 1, -1 },
  { 5, 4, 7, 4, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 8, 6, 0, 6, 5, 5, 6, 7, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 0, 4, 9, 4, 7, 7, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 8, 6, 7, 9, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 7, 8, 2, 7, 11, 8, 11, 10, 8, -1, -1, -1, -1, -1, -1, -1 },
  { 7, 0, 2, 7, 10, 0, 7, 11, 10, 10, 4, 0, -1, -1, -1, -1 },
  { 0, 5, 9, 8, 2, 11, 8, 11, 10, 11, 2, 7, -1, -1, -1, -1 },
  { 11, 10, 2, 11, 2, 7, 10, 4, 2, 9, 2, 5, 4, 5, 2, -1 },
  { 1, 7, 11, 4, 7, 1, 4, 2, 7, 4, 8, 2, -1, -1, -1, -1 },
  { 7, 11, 1, 7, 1, 2, 2, 1, 0, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 11, 1, 4, 7, 11, 4, 8, 7, 2, 7, 8, 0, 5, 9, -1 },
  { 7, 11, 1, 7, 1, 2, 5, 9, 1, 9, 2, 1, -1, -1, -1, -1 },
  { 1, 7, 5, 1, 8, 7, 1, 10, 8, 2, 7, 8, -1, -1, -1, -1 },
  { 0, 2, 10, 0, 10, 4, 2, 7, 10, 1, 10, 5, 7, 5, 10, -1 },
  { 0, 1, 7, 0, 7, 9, 1, 10, 7, 2, 7, 8, 10, 8, 7, -1 },
  { 9, 2, 7, 1, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 8, 2, 7, 8, 7, 4, 4, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 2, 7, 5, 0, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 8, 2, 7, 8, 7, 4, 9, 0, 7, 0, 4, 7, -1, -1, -1, -1 },
  { 9, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 2, 10, 6, 2, 9, 10, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 8, 4, 2, 9, 6, 9, 10, 6, 9, 11, 10, -1, -1, -1, -1 },
  { 5, 11, 10, 5, 10, 2, 5, 2, 0, 6, 2, 10, -1, -1, -1, -1 },
  { 4, 5, 2, 4, 2, 8, 5, 11, 2, 6, 2, 10, 11, 10, 2, -1 },
  { 1, 9, 11, 1, 6, 9, 1, 4, 6, 6, 2, 9, -1, -1, -1, -1 },
  { 9, 11, 6, 9, 6, 2, 11, 1, 6, 8, 6, 0, 1, 0, 6, -1 },
  { 4, 6, 11, 4, 11, 1, 6, 2, 11, 5, 11, 0, 2, 0, 11, -1 },
  { 5, 11, 1, 8, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 2, 10, 6, 9, 10, 2, 9, 1, 10, 9, 5, 1, -1, -1, -1, -1 },
  { 9, 6, 2, 9, 10, 6, 9, 5, 10, 1, 10, 5, 0, 8, 4, -1 },
  { 10, 6, 2, 10, 2, 1, 1, 2, 0, -1, -1, -1, -1, -1, -1, -1 },
  { 10, 6, 2, 10, 2, 1, 8, 4, 2, 4, 1, 2, -1, -1, -1, -1 },
  { 2, 9, 5, 2, 5, 6, 6, 5, 4, -1, -1, -1, -1, -1, -1, -1 },
  { 2, 9, 5, 2, 5, 6, 0, 8, 5, 8, 6, 5, -1, -1, -1, -1 },
  { 2, 0, 4, 6, 2, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 2, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 11, 8, 11, 10, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 0, 9, 4, 9, 10, 10, 9, 11, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 5, 11, 0, 11, 8, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 5, 11, 10, 4, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 1, 4, 8, 1, 8, 11, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1 },
  { 9, 11, 1, 0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 1, 4, 8, 1, 8, 11, 0, 5, 8, 5, 11, 8, -1, -1, -1, -1 },
  { 5, 11, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 1, 10, 5, 10, 9, 9, 10, 8, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 0, 9, 4, 9, 10, 5, 1, 9, 1, 10, 9, -1, -1, -1, -1 },
  { 0, 1, 10, 8, 0, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 4, 1, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 5, 4, 8, 9, 5, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 9, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { 0, 4, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
  { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 } };
