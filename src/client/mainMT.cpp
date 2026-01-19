#include <iostream>
#include "../shared/state.h"
#include "render.h"
#include "client.h"

#include <thread>
#include <chrono>
#include <atomic>

#include <SFML/Graphics.hpp>
#include <unistd.h>

void testSFML() {
    sf::Texture texture;
}
// end of test SFML

using namespace std;
using namespace state;
using namespace render;
using namespace client;
using namespace engine;

Board bd;

int nb_of_active_clients = 0;
std::atomic<bool> running(true);

void engine_process(engine::Engine* engine) {
    while (running) {
        engine->update();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void client_process(client::ClientMT* client) {
    nb_of_active_clients++;
    client->run();
    state::Player* myPlayer = client->engineGame->board->playerList[client->playerID].get();
    myPlayer->isAlive = false;
    myPlayer->wounds = myPlayer->getHP();
    myPlayer->revealed = true;
    client->renderMan->needsRedraw = true;
    if (client->playerID == client->engineGame->currentPlayerIndex) {
        client->engineGame->goToNextPlayer();
    }
    nb_of_active_clients--;
    if (nb_of_active_clients == 0) {
        running = false;
    }
}

int main(int argc,char* argv[])
{
    Engine eng(&bd);
    RenderManager rm(&bd);
    RenderManager rm2(&bd);
    RenderManager rm3(&bd);
    RenderManager rm4(&bd);
    ClientMT client(&rm, &eng, 0);
    ClientMT client2(&rm2, &eng, 1);
    ClientMT client3(&rm3, &eng, 2);
    ClientMT client4(&rm4, &eng, 3);    
    std::thread engineThread = std::thread([](engine::Engine* eng){ engine_process(eng); }, &eng);
    std::thread clientThread = std::thread([](client::ClientMT* cli){ client_process(cli); }, &client);
    sleep(1);
    std::thread clientThread2 = std::thread([](client::ClientMT* cli){ client_process(cli); }, &client2);
    sleep(1);
    std::thread clientThread3 = std::thread([](client::ClientMT* cli){ client_process(cli); }, &client3);
    sleep(1);
    std::thread clientThread4 = std::thread([](client::ClientMT* cli){ client_process(cli); }, &client4);
    clientThread.detach();
    clientThread2.detach();
    clientThread3.detach();
    clientThread4.detach();
    engineThread.join();
    return 0;
}