#ifndef CPPGAME_GAMEENGINE_HPP
#define CPPGAME_GAMEENGINE_HPP

#include "Common.hpp"
#include "Scene.hpp"

using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

class GameEngine {
private:
    SceneMap            m_sceneMap;
    sf::RenderWindow    m_window;
    std::string         m_currentScene;
    bool                m_running=true;
    size_t              m_simulationSpeed = 1;

    void init(const std::string & path);
    void update();
    void sUserInput();
    std::shared_ptr<Scene> currentScene();

public:
    explicit GameEngine(const std::string &path);

    void changeScene(const std::string & sceneName,std::shared_ptr<Scene> scene, bool endCurrentScene = false);
    sf ::RenderWindow &window();
    void run(); // main loop
    void quit();
    bool isRunning() const;
};


#endif //CPPGAME_GAMEENGINE_HPP