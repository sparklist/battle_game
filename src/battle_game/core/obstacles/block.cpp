#include "battle_game/core/obstacles/block.h"

namespace battle_game::obstacle {

Block::Block(GameCore *game_core,
             uint32_t id,
             glm::vec2 position,
             float rotation,
             glm::vec2 scale)
    : Obstacle(game_core, id, position, rotation) {
}

bool Block::IsBlocked(glm::vec2 p) const {
  p = WorldToLocal(p);
  return p.x <= scale_.x && p.x >= -scale_.x && p.y <= scale_.y &&
         p.y >= -scale_.y;
}

void Block::Render() {
  battle_game::SetColor(glm::vec4{0.0f, 0.0f, 0.0f, 1.0f});
  battle_game::SetTexture(0);
  battle_game::SetTransformation(position_, rotation_, scale_);
  battle_game::DrawModel(0);
}
direction Block::bouncedirection (glm::vec2 p, glm::vec2 velocity) const{
  auto nextposition = WorldToLocal( p + velocity * kSecondPerTick);
  p = WorldToLocal(p);
  if (nextposition.x >= -scale_.x && p.x <= -scale_.x && p.y >= -scale_.y &&
      p.y <= scale_.y)
    return left;
  if (nextposition.x <= scale_.x && p.x > scale_.x && p.y >= -scale_.y &&
      p.y <= scale_.y)
    return right;
  if (nextposition.y <= scale_.y && p.y >= scale_.y &&
      p.x >= -scale_.x && p.x <= scale_.x)
    return up;
  if (nextposition.y >= -scale_.y && p.y <= -scale_.y && p.x >= -scale_.x &&
      p.x <= scale_.x)
    return down;
  return no;
}
}  // namespace battle_game::obstacle
