#pragma once
#include "battle_game/core/object.h"
#include "glm/glm.hpp"

namespace battle_game {
enum direction { no, up, down, left, right };
class Obstacle : public Object {
 public:
  Obstacle(GameCore *game_core,
           uint32_t id,
           glm::vec2 position,
           float rotation = 0.0f);
  [[nodiscard]] virtual bool IsBlocked(glm::vec2 p) const = 0;
  [[nodiscard]] virtual direction bouncedirection(glm::vec2 p,
                                                  glm::vec2 velocity) const {
    return no;
  };
  void Update() override;
  void Render() override;

 protected:
};
}  // namespace battle_game
