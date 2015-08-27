// Copyright 2015, Aaron Ceross

#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <set>
#include <memory>
// #include <thread>
// #include <chrono>

#include "../include/level.hpp"

Level::Level(sf::RenderTarget& output_target, FontHolder& fonts)
: target_(output_target)
, scene_texture_()
, level_view_(output_target.getDefaultView())
, textures_()
, fonts_(fonts)
, scene_graph_()
, scene_layers_()
, level_bounds_(0.f, 0.f, level_view_.getSize().x, level_view_.getSize().y)
, start_position_(level_view_.getSize().x / 2.f, level_bounds_.height -
                                                 level_view_.getSize().y / 2.f)
, movement_speed_(2.5f)
, player_mario_(nullptr)
{
  scene_texture_.create(target_.getSize().x, target_.getSize().y);

  // init level instance
  LoadTextures();
  BuildScene();

  //


  // Prepare the main view and the mini-map
  level_view_.setCenter(player_mario_->getPosition());
  level_view_.zoom(0.57f);
  mini_map_.setViewport(sf::FloatRect(0.72f, 0, 0.23f, 0.23f));
}

void Level::Update(sf::Time delta_time) {
  if (!command_queue_.IsEmpty()) {
    scene_graph_.OnCommand(command_queue_.Pop(), delta_time);
    // // player_sprite_->move(0, 10);
    // sf::Vector2f position;
    //
    // if (!player_mario_->navigator_.start_gadget_actions_.empty()) {
    //   position = player_mario_->navigator_.start_gadget_actions_.front();
    //   player_mario_->move(position);
    //   player_mario_->navigator_.start_gadget_actions_.pop();
    //   std::cout << "Player update" << std::endl;
    //   // sf::sleep(sf::seconds(2));
    //   // waiting_ = true;
    // } else {
    //   // finish_ = true;
    //   std::cout << "Done" << std::endl;
    // }
    std::cout << "Command Executed" << std::endl;
  }

  // sf::Vector2f position;
  //
  // if (!player_mario_->navigator_.start_gadget_actions_.empty()) {
  //   position = player_mario_->navigator_.start_gadget_actions_.front();
  //   player_mario_->move(position);
  //   player_mario_->navigator_.start_gadget_actions_.pop();
  //   std::cout << "Player update" << std::endl;
  //   // sf::sleep(sf::seconds(2));
  //   // waiting_ = true;
  // } else {
  //   // finish_ = true;
  //   std::cout << "Done" << std::endl;
  // }

  // Collision detection and response (may destroy entities)
  // HandleCollisions();
  // Regular update step, adapt position (correct if outside view)
  scene_graph_.Update(delta_time, command_queue_);
}

void Level::draw() {
  // draw the main view level
  target_.setView(level_view_);
  target_.draw(scene_graph_);
  // target_.draw(player_mario_->sprite_);

  // draw the mini map view
  target_.setView(mini_map_);
  target_.draw(scene_graph_);
}

CommandQueue& Level::GetCommandQueue() {
  return command_queue_;
}

void Level::LoadTextures() {
  textures_.Load(Textures::Mario, "resources/gfx/mario_bros.png");
  textures_.Load(Textures::Koopa, "resources/gfx/enemies.png");
  textures_.Load(Textures::World, "resources/gfx/tile_set.png");
}

void Level::BuildScene() {
  // Initialise the different layers
  for (std::size_t i = 0; i < LayerCount; ++i) {
    Category::Type category = Category::None;
    SceneNode::Ptr layer(new SceneNode(category));
    scene_layers_[i] = layer.get();
    scene_graph_.AttachChild(std::move(layer));
  }

  // Start and load SAT solver
  zchaff_manager_.LoadInstance();
  // Read in the tile map
  tile_map_.InitialiseMap(zchaff_manager_);
  std::cout << "Tilemap Initialised in LEVEL" << std::endl;

  std::unique_ptr<MapNode> map_node(new MapNode());
  map_node->tile_map_.InitialiseMap(zchaff_manager_);
  scene_layers_[Background]->AttachChild(std::move(map_node));

  // Add player sprite
  std::unique_ptr<Mario> player(new Mario(Mario::SmallMario, textures_, fonts_));
  player_mario_ = player.get();
  // player_mario_->InitialiseLevelNavigator(tile_map_);


  player_mario_->setPosition(0, 0);
  scene_layers_[Foreground]->AttachChild(std::move(player));
}

void Level::AdaptPlayerPosition() {
  // Keep player's position inside the screen bounds,
  // at least border_distance units from the border
  // sf::FloatRect view_bounds(level_view_.getCenter() - level_view_.getSize() /
  //                                                   2.f, level_view_.getSize());
  // const float border_distance = 40.f;

  sf::Vector2f position = player_mario_->getPosition();
  // printf("First postion x = %d y = %d\n", position.x, position.y);
  // position.x = std::max(position.x, view_bounds.left + border_distance);
  // position.x = std::min(position.x, view_bounds.left + view_bounds.width - border_distance);
  // position.y = std::max(position.y, view_bounds.top + border_distance);
  // position.y = std::min(position.y, view_bounds.top + view_bounds.height - border_distance);
  player_mario_->setPosition(position);
  // sf::Vector2i location_update = static_cast<sf::Vector2i>(position);
  // printf("location_update x = %d y = %d\n", position.x, position.y);
  // player_sprite_->UpdateLocation(position);
}

void Level::HandleCollisions() {
  // std::set<SceneNode::Pair> collision_pairs;

  // scene_graph_.CheckSceneCollision(scene_graph_, collision_pairs);
  //
  // for (SceneNode::Pair pair : collision_pairs) {
  //   if (matchesCategories(pair, Category::Player, Category::EnemyAircraft))
  //   {
  //     auto& player = static_cast<Aircraft&>(*pair.first);
  //     auto& enemy = static_cast<Aircraft&>(*pair.second);
  //
  //     // Collision: Player damage = enemy's remaining HP
  //     player_sprite_.damage(enemy.getHitpoints());
  //     enemy.destroy();
  //   }
  //
  //   else if (matchesCategories(pair, Category::PlayerAircraft, Category::Pickup))
  //   {
  //     auto& player = static_cast<Aircraft&>(*pair.first);
  //     auto& pickup = static_cast<Pickup&>(*pair.second);
  //
  //     // Apply pickup effect to player, destroy projectile
  //     pickup.apply(player);
  //     pickup.destroy();
  //     player.playLocalSound(mCommandQueue, SoundEffect::CollectPickup);
  //   }
  //
  //   else if (matchesCategories(pair, Category::EnemyAircraft, Category::AlliedProjectile)
  //       || matchesCategories(pair, Category::PlayerAircraft, Category::EnemyProjectile))
  //   {
  //     auto& aircraft = static_cast<Aircraft&>(*pair.first);
  //     auto& projectile = static_cast<Projectile&>(*pair.second);
  //
  //     // Apply projectile damage to aircraft, destroy projectile
  //     aircraft.damage(projectile.getDamage());
  //     projectile.destroy();
  //   }
  // }

}

TileMap& Level::GetTileMap() {
  return tile_map_;
}
