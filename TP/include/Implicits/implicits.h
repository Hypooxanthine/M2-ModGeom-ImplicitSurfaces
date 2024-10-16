/**
 * @file implicits.h
 * @author Eric Galin
 * @brief Adapted from https://github.com/eric-galin/TinyMesh
 * 
 */

#pragma once

#include <iostream>
#include <memory>

#include <Vroom/Asset/AssetData/MeshData.h>
#include <glm/glm.hpp>

#include "box.h"
#include "SceneGraph/NodeType.h"

class SceneNode;
class NodeEditor;

class AnalyticScalarField
{
protected:
public:
  AnalyticScalarField();
  AnalyticScalarField(const AnalyticScalarField&) = default;
  virtual ~AnalyticScalarField() = default;
  virtual float Value(const glm::vec3&) const;
  virtual glm::vec3 Gradient(const glm::vec3&) const;

  // Normal
  virtual glm::vec3 Normal(const glm::vec3&) const;

  // Dichotomy
  glm::vec3 Dichotomy(glm::vec3, glm::vec3, float, float, float, const float& = 1.0e-4) const;

  virtual void Polygonize(int, vrm::MeshData&, Box, const float& = 1e-4) const;

  // Only for ASF that hold children.
  virtual void setField(size_t field, const AnalyticScalarField* asf) { throw std::runtime_error("Unauthorized operation"); }
  virtual void addField(const AnalyticScalarField* asf) { throw std::runtime_error("Unauthorized operation"); }
  virtual void removeField(size_t field) { throw std::runtime_error("Unauthorized operation"); }

  virtual std::unique_ptr<NodeEditor> instanciateEditor(SceneNode* node);
protected:
  static const float Epsilon; //!< Epsilon value for partial derivatives
protected:
  static int TriangleTable[256][16]; //!< Two dimensionnal array storing the straddling edges for every marching cubes configuration.
  static int edgeTable[256];    //!< Array storing straddling edges for every marching cubes configuration.
};
