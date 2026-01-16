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
    client->engineGame->board->playerList[client->playerID]->isAlive = false;
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
    ClientMT client(&rm, &eng, 0);
    ClientMT client2(&rm2, &eng, 1);
    std::thread engineThread = std::thread([](engine::Engine* eng){ engine_process(eng); }, &eng);
    std::thread clientThread = std::thread([](client::ClientMT* cli){ client_process(cli); }, &client);
    sleep(1);
    std::thread clientThread2 = std::thread([](client::ClientMT* cli){ client_process(cli); }, &client2);
    clientThread.detach();
    clientThread2.detach();
    engineThread.join();
    return 0;
}