#pragma once
#include <Application.hpp>
#include <Planet/TerrainConstructionStageBase.hpp>
using namespace evo_engine;
namespace planet {
class PerlinNoiseStage : public TerrainConstructionStageBase {
 public:
  void Process(glm::dvec3 point, double previous_result, double& elevation) override;
};

}  // namespace planet