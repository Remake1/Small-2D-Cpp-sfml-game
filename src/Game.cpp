#include "Game.hpp"

Game::Game(const std::string &config) {
    init(config);
}

void Game::init(const std::string &config) {
    // TODO: read the config

    // Default windows parameters
    m_window.create(sf::VideoMode(W_WIDTH, W_HEIGHT), "Ball game");
    m_window.setFramerateLimit(60);

    if (!m_font.loadFromFile("Arial.ttf"))
    {
        std::cout << "[Font Error] Error loading font!\n";
    }

    spawnPlayer();
}

void Game::run() {
    // TODO: add pause functionality
    while (m_running){
        m_entities.update();

        if(!m_paused){
            sEnemySpawner();
            sMovement();
            sCollision();
            sLifespan();
            sUserInput();
        }

        sRender();

        m_currentFrame++;
    }
}

void Game::setPaused() {
    m_paused = !m_paused;
}

void Game::spawnPlayer() {
    auto entity = m_entities.addEntity("player");

    // Set components to player
    // TODO: config support

    // Move player to starting position and change velocity
    float mx = m_window.getSize().x / 2;
    float my = m_window.getSize().y / 2;
    entity->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(0.0f, 0.0f), 0.f);

    // Set player shape (Circle) and color
    entity->cCircleShape = std::make_shared<CCircleShape>(28.0f, 8, sf::Color::Cyan, sf::Color::Magenta, 4.0f);

    // Set circle collision
    // TODO: make the same radius as at Circle Shape
    entity->cCircleCollision = std::make_shared<CCircleCollision>(28.0f);

    // Set input state inside player by adding Input component
    entity->cInput = std::make_shared<CInput>();

    // TODO: Add Lifespan and Score
    entity->cLifespan = std::make_shared<CLifespan>(3, 10);

    // Set local player to player entity (against entity manager paradigm)
    m_player = entity;
}

// Spawns a bullet.
// entity - entity, that "shots".
// target - mouse position, for direction where bullet in shot.
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2 & target) {
    // Add bullet entity.
    auto bullet = m_entities.addEntity("bullet");
    // Set bullet position to the mouse, and velocity to 0.
    bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, entity->cTransform->pos.getNormalizedVelocity(target)*5, 0);
    // Add shape and color.
    bullet->cCircleShape = std::make_shared<CCircleShape>(10.0f, 8, sf::Color::Yellow, sf::Color(82, 58, 0), 2);
    // Add collision radius
    bullet->cCircleCollision = std::make_shared<CCircleCollision>(10.0f);
}

// Spawns an enemy
void Game::spawnEnemy() {
    auto entity = m_entities.addEntity("enemy");

    float ex = 10 + (rand() % (m_window.getSize().x - 10));
    float ey = 10 + (rand() % (m_window.getSize().y - 10));

    entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(0.0f, 0.0f), 0.0f);

    entity->cCircleShape = std::make_shared<CCircleShape>(24.0f, 20, sf::Color::Red, sf::Color::White, 3.0f);
    // TODO: make the same radius as at Circle Shape
    entity->cCircleCollision = std::make_shared<CCircleCollision>(24.0f);

    entity->cLifespan = std::make_shared<CLifespan>(1, 60);
}

void Game::spawnEnemyBullet(std::shared_ptr<Entity> entity, int bullet_velocity = 5) {
    // Spawn bullet to the player position.
    auto bullet = m_entities.addEntity("enemy_bullet");
    bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, entity->cTransform->pos.getNormalizedVelocity(m_player->cTransform->pos)*bullet_velocity, 0);
    bullet->cCircleShape = std::make_shared<CCircleShape>(10.0f, 8, sf::Color::Red, sf::Color(82, 58, 0), 2);
    bullet->cCircleCollision = std::make_shared<CCircleCollision>(10.0f);
}

bool Game::isInWindow(Vec2 &point) const {
    // TODO: Window x y from config
    if (point.x <= m_window.getSize().x && point.x >=0 && point.y <= m_window.getSize().y && point.y >= 0){
        return true;
    } else return false;
}

// -------------------
// SYSTEMS
// -------------------

void Game::sUserInput() {
    sf::Event event;

    while(m_window.pollEvent(event)){
        // Trigger event when window is closed.
        if(event.type == sf::Event::Closed){
            m_running = false;
        }

        // Triggers when key is pressed.
        if(event.type == sf::Event::KeyPressed){
            switch (event.key.code) {
                case sf::Keyboard::W:
                    m_player->cInput->up = true;
                    break;
                case sf::Keyboard::A:
                    m_player->cInput->left = true;
                    break;
                case sf::Keyboard::S:
                    m_player->cInput->down = true;
                    break;
                case sf::Keyboard::D:
                    m_player->cInput->right = true;
                    break;
                default:
                    break;
            }
        }

        // Triggers when key is released.
        if(event.type == sf::Event::KeyReleased){
            switch (event.key.code) {
                case sf::Keyboard::W:
                    m_player->cInput->up = false;
                    break;
                case sf::Keyboard::A:
                    m_player->cInput->left = false;
                    break;
                case sf::Keyboard::S:
                    m_player->cInput->down = false;
                    break;
                case sf::Keyboard::D:
                    m_player->cInput->right = false;
                    break;
                default:
                    break;
            }
        }

        if(event.type == sf::Event::MouseButtonPressed){
            if (event.mouseButton.button == sf::Mouse::Left) {
                // Call spawnBullet here.
                spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
            }
            if (event.mouseButton.button == sf::Mouse::Left) {
                // Call spawnSpecialWeapon here. In the future.
            }
        }
    }
}

void Game::sRender() {
    m_window.clear();

    // TODO: Update all Entities
    // TODO: HUD: score, etc...
    // TODO: Rotations

    /// HUD
    // m_player->cLifespan->remaining
    // Draw Health heart texture
    for (int i = 0; i < m_player->cLifespan->remaining; i++) {
        sf::Texture heart;
        if (!heart.loadFromFile("heart_texture.png"))
        {
            std::cout << "[Texture Error] Error loading heart texture!\n";
        }
        sf::Sprite sprite;
        sprite.setTexture(heart);
        sprite.setPosition(10 + i*30, 35);
        m_window.draw(sprite);
    }
    // Set text properties
    m_text.setFont(m_font);
    m_text.setCharacterSize(25);
    m_text.setFillColor(sf::Color::White);
    // Set text string (Entities size)
    m_text.setString("Entities: " + std::to_string( m_entities.getEntities().size() ) + "; Score: " + std::to_string( m_score ));

    // Draw HUD text
    m_window.draw(m_text);

    /// Circles
    // Handle new entity properties:
    // Set player shape actual position to Transform position value
    m_player->cCircleShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);
    m_player->cCircleShape->circle.setRotation(m_player->cTransform->angle);
    // Draw player entity
    m_window.draw(m_player->cCircleShape->circle);

    // Render all circle entities
    for (auto e : m_entities.getEntities()) {
        // Handle new entity properties:
        e->cCircleShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
        // Draw entity
        m_window.draw(e->cCircleShape->circle);
    }

    // Render changes (kinda like swapbuffers).
    m_window.display();
}

void Game::sMovement() {
    // -------------------------
    // Player
    // -------------------------
    // Reset the velocity, so it's 'handles' when you release key.
    m_player->cTransform->velocity = {0.0, 0.0};
    // Adjust the velocity if keys pressed.
    if(m_player->cInput->up){
        m_player->cTransform->velocity.y = -5;
    }
    if(m_player->cInput->down){
        m_player->cTransform->velocity.y = 5;
    }
    if(m_player->cInput->right){
        m_player->cTransform->velocity.x = 5;
    }
    if(m_player->cInput->left){
        m_player->cTransform->velocity.x = -5;
    }

    // Change position depend on velocity.
    m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
    m_player->cTransform->pos.y += m_player->cTransform->velocity.y;
    // -------------------------
    // Bullets
    // -------------------------
    for ( auto e : m_entities.getEntities("bullet")) {
        if (isInWindow(e->cTransform->pos))
        {
            e->cTransform->pos.x += e->cTransform->velocity.x;
            e->cTransform->pos.y += e->cTransform->velocity.y;
        } else {
            e->destroy();
        }

    }
    for ( auto e : m_entities.getEntities("enemy_bullet")) {
        if (isInWindow(e->cTransform->pos))
        {
            e->cTransform->pos.x += e->cTransform->velocity.x;
            e->cTransform->pos.y += e->cTransform->velocity.y;
        } else {
            e->destroy();
        }

    }
}

void Game::sCollision() {
    // Implements bullet collisions.
    for (auto b : m_entities.getEntities("bullet")){
        // Bullet - Enemy collision:
        for (auto e : m_entities.getEntities("enemy")) {
            // Bullet and entity collision condition.
            if (e->cTransform->pos.distNoSqrt(b->cTransform->pos) <
                    (b->cCircleCollision->radius + e->cCircleCollision->radius)*(b->cCircleCollision->radius + e->cCircleCollision->radius) )
            {
                // Collision occurred.
                e->destroy();
                b->destroy();

                m_score++;
                goto end;
            }
        }
    }
    // Implements enemy collisions.
    for (auto e : m_entities.getEntities("enemy")){
        // Enemy and player collision condition.
        if (e->cTransform->pos.distNoSqrt(m_player->cTransform->pos) <
            (m_player->cCircleCollision->radius + e->cCircleCollision->radius)*(m_player->cCircleCollision->radius + e->cCircleCollision->radius) )
        {
            // Collision occurred.
            e->destroy();
            m_player->cLifespan->remaining--;
        }
    }
    // Implements enemy bullet collisions.
    for (auto b : m_entities.getEntities("enemy_bullet")){
        // Enemy bullet and player collision condition.
        if (b->cTransform->pos.distNoSqrt(m_player->cTransform->pos) <
            (m_player->cCircleCollision->radius + b->cCircleCollision->radius)*(m_player->cCircleCollision->radius + b->cCircleCollision->radius) )
        {
            // Collision occurred.
            b->destroy();
            m_player->cLifespan->remaining--;
        }
    }
    end:{}
}

void Game::sEnemySpawner() {
    // TODO
    // Spawn enemy
    if ((m_currentFrame - enemySpawnInterval) > lastEnemySpawnTime){
        spawnEnemy();
        lastEnemySpawnTime = m_currentFrame;
        if(enemySpawnInterval > 60) enemySpawnInterval -= 7;
    }
    // Spawn enemy bullet
    int bullet_velocity = 5;
    for (auto e : m_entities.getEntities("enemy")) {
        if ((m_currentFrame - 60) > e->cLifespan->bullet){
            spawnEnemyBullet(e, bullet_velocity);
            if(bullet_velocity < 20) bullet_velocity++;
            e->cLifespan->bullet = m_currentFrame;
        }
    }
}

void Game::sLifespan() {
    if(m_player->cLifespan->remaining <= 0){
        m_player->destroy();
        m_running = false;
    }
}