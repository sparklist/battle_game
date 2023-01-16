#include "battle_game/core/bullets/cannon_ball.h"

#include "battle_game/core/game_core.h"
#include "battle_game/core/particles/particles.h"
#include "battle_game/core/obstacle.h"
#include "battle_game/core/obstacles/obstacles.h"
#include <iostream>
namespace battle_game::bullet {
CannonBall::CannonBall(GameCore *core,
                       uint32_t id,
                       uint32_t unit_id,
                       uint32_t player_id,
                       glm::vec2 position,
                       float rotation,
                       float damage_scale,
                       glm::vec2 velocity)
    : Bullet(core, id, unit_id, player_id, position, rotation, damage_scale),
      velocity_(velocity) {
}

void CannonBall::Render() {
  SetTransformation(position_, rotation_, glm::vec2{0.1f});
  SetColor(game_core_->GetPlayerColor(player_id_));
  SetTexture("../../textures/particle3.png");
  DrawModel(0);
}

void CannonBall::Update() {
  position_ += velocity_ * kSecondPerTick;
  bool should_die = false;
  /*if (game_core_->IsBlockedByObstacles(position_)) {
    should_die = true;
  }*/ 
  auto nextposition = position_ + velocity_ * kSecondPerTick;
  if (game_core_->GetUnit(unit_id_)) {
    if (nextposition.y >= game_core_->boundary_high_.y &&
        position_.y <= game_core_->boundary_high_.y) {
      game_core_->PushEventRemoveBullet(id_);
      game_core_->GetUnit(unit_id_)->GenerateBullet<bullet::CannonBall>(
          position_, glm::radians(0.0f) + rotation_, damage_scale_,
          glm::vec2(velocity_.x, -velocity_.y));
    }
    if (nextposition.y <= game_core_->boundary_low_.y &&
        position_.y >= game_core_->boundary_low_.y) {
      game_core_->PushEventRemoveBullet(id_);
      game_core_->GetUnit(unit_id_)->GenerateBullet<bullet::CannonBall>(
          position_, glm::radians(0.0f) + rotation_, damage_scale_,
          glm::vec2(velocity_.x, -velocity_.y));
    }
    if (nextposition.x <= game_core_->boundary_low_.x &&
        position_.x >= game_core_->boundary_low_.x) {
      game_core_->PushEventRemoveBullet(id_);
      game_core_->GetUnit(unit_id_)->GenerateBullet<bullet::CannonBall>(
          position_, glm::radians(0.0f) + rotation_, damage_scale_,
          glm::vec2(-velocity_.x, velocity_.y));
    }
    if (nextposition.x >= game_core_->boundary_high_.x &&
        position_.x <= game_core_->boundary_high_.x) {
      game_core_->PushEventRemoveBullet(id_);
      game_core_->GetUnit(unit_id_)->GenerateBullet<bullet::CannonBall>(
          position_, glm::radians(0.0f) + rotation_, damage_scale_,
          glm::vec2(-velocity_.x, velocity_.y));
    }
  }
  for (auto &obstacle : game_core_->obstacles_) {
    if (obstacle.second->bouncedirection(position_, velocity_) == up) {
      game_core_->PushEventRemoveBullet(id_);
      /* game_core_->PushEventGenerateBullet<CannonBall>(
          id_, player_id_, position_, glm::radians(0.0f)- rotation_,
          damage_scale_, velocity_);*/
      game_core_->GetUnit(unit_id_)->GenerateBullet<bullet::CannonBall>(
          position_, glm::radians(0.0f)+rotation_, damage_scale_, glm::vec2(velocity_.x,-velocity_.y));
    }
    if (obstacle.second->bouncedirection(position_, velocity_) == down) {
      game_core_->PushEventRemoveBullet(id_);
      game_core_->GetUnit(unit_id_)->GenerateBullet<bullet::CannonBall>(
          position_, glm::radians(0.0f) + rotation_, damage_scale_,
          glm::vec2(velocity_.x, -velocity_.y));
    }
    if (obstacle.second->bouncedirection(position_, velocity_) == left) {
      game_core_->PushEventRemoveBullet(id_);
      game_core_->GetUnit(unit_id_)->GenerateBullet<bullet::CannonBall>(
          position_, glm::radians(0.0f) + rotation_, damage_scale_,
          glm::vec2(-velocity_.x, velocity_.y));
    }
    if (obstacle.second->bouncedirection(position_, velocity_) == right) {
      game_core_->PushEventRemoveBullet(id_);
      game_core_->GetUnit(unit_id_)->GenerateBullet<bullet::CannonBall>(
          position_, glm::radians(0.0f) + rotation_, damage_scale_,
          glm::vec2(-velocity_.x, velocity_.y));
    }
  }
  auto &units = game_core_->GetUnits();
  for (auto &unit : units) {
    if (unit.first == unit_id_) {
      continue;
    }
    if (unit.second->IsHit(position_)) {
      game_core_->PushEventDealDamage(unit.first, id_, damage_scale_ * 10.0f);
      should_die = true;
    }
  }

  if (should_die) {
    game_core_->PushEventRemoveBullet(id_);
  }
}

CannonBall::~CannonBall() {
  for (int i = 0; i < 5; i++) {
    game_core_->PushEventGenerateParticle<particle::Smoke>(
        position_, rotation_, game_core_->RandomInCircle() * 2.0f, 0.2f,
        glm::vec4{0.0f, 0.0f, 0.0f, 1.0f}, 3.0f);
  }
}
}  // namespace battle_game::bullet
