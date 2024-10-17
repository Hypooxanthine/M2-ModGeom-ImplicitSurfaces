/**
 * @file box.h
 * @author Eric Galin
 * @brief Adapted from https://github.com/eric-galin/TinyMesh
 * 
 */

#pragma once

#include <vector>
#include <iostream>

#include <glm/glm.hpp>

class Box
{
protected:
  glm::vec3 a,b; //!< Lower and upper vertex.
public:
  //! Empty.
  Box() {}
  explicit Box(float);
  explicit Box(const glm::vec3&, const glm::vec3&);
  explicit Box(const glm::vec3&, float);
  explicit Box(const std::vector<glm::vec3>&);
  explicit Box(const Box&, const Box&);

  //! Empty.
  ~Box() {}

  // Access vertexes
  glm::vec3& operator[] (int);
  const glm::vec3& operator[] (int) const;

  // Comparison
  friend int operator==(const Box&, const Box&);
  friend int operator!=(const Box&, const Box&);

  // Acces to vertices
  glm::vec3 Center() const;
  glm::vec3 Vertex(int) const;

  glm::vec3 Size() const;
  glm::vec3 Diagonal() const;
  float Radius() const;

  bool Inside(const Box&) const;
  bool Inside(const glm::vec3&) const;

  float Volume() const;
  float Area() const;

  // Compute sub-box
  Box Sub(int) const;

  // Translation, scale
  void Translate(const glm::vec3&);
  void Scale(float);

  friend std::ostream& operator<<(std::ostream&, const Box&);

public:
  static const float epsilon; //!< Internal \htmlonly\epsilon;\endhtmlonly for ray intersection tests.
  static const Box Null; //!< Empty box.
  static const int edge[24]; //!< Edge vertices.
  static const glm::vec3 normal[6]; //!< Face normals.
};

//! Returns either end vertex of the box.
inline glm::vec3& Box::operator[] (int i)
{
  if (i == 0) return a;
  else return b;
}

//! Overloaded.
inline const glm::vec3& Box::operator[] (int i) const
{
  if (i == 0) return a;
  else return b;
}

//! Returns the center of the box.
inline glm::vec3 Box::Center() const
{
  return 0.5f * (a + b);
}

/*!
\brief Returns the diagonal of the box.
*/
inline glm::vec3 Box::Diagonal() const
{
  return (b - a);
}

/*!
\brief Compute the size (width, length and height) of a box.
\sa Box::Diagonal()
*/
inline glm::vec3 Box::Size() const
{
  return b - a;
}

/*!
\brief Returns the radius of the box, i.e. the length of the half diagonal of the box.
*/
inline float Box::Radius() const
{
  return 0.5f * glm::length(b - a);
}

/*!
\brief Returns the k-th vertex of the box.

The returned vector is computed by analysing the first three bits of k as follows:
\code
glm::vec3 vertex=glm::vec3((k&1)?b[0]:a[0],(k&2)?b[1]:a[1],(k&4)?b[2]:a[2]);
\endcode
*/
inline glm::vec3 Box::Vertex(int k) const
{
  return glm::vec3((k & 1) ? b[0] : a[0], (k & 2) ? b[1] : a[1], (k & 4) ? b[2] : a[2]);
}

//! Compute the volume of a box.
inline float Box::Volume() const
{
  glm::vec3 side = b - a;
  return side[0] * side[1] * side[2];
}

/*!
\brief Compute the surface area of a box.
*/
inline float Box::Area() const
{
  glm::vec3 side = b - a;
  return 2.f * (side[0] * side[1] + side[0] * side[2] + side[1] * side[2]);
}

/*!
\brief Check if an argument box is inside the box.
\param box The box.
*/
inline bool Box::Inside(const Box& box) const
{
  return (
    (
        a.x < box.a.x &&
        a.y < box.a.y &&
        a.z < box.a.z
    ) &&
    (
        b.x > box.b.x &&
        b.y > box.b.y &&
        b.z > box.b.z
    )
  );
}

/*!
\brief Check if a point is inside the box.
\param p Point.
*/
inline bool Box::Inside(const glm::vec3& p) const
{
  return (
    (
        a.x < p.x &&
        a.y < p.y &&
        a.z < p.z
    ) &&
    (
        b.x > p.x &&
        b.y > p.y &&
        b.z > p.z
    )
  );
}

/*!
\brief Check if two boxes are (strictly) equal.
\param a, b Boxes.
*/
inline int operator==(const Box& a, const Box& b)
{
  return (a.a == b.a) && (a.b == b.b);
}

/*!
\brief Check if two boxes are (strictly) different.
\param a, b Boxes.
*/
inline int operator!=(const Box& a, const Box& b)
{
  return !(a == b);
}