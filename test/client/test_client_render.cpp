#include <boost/test/unit_test.hpp>
#include <SFML/Graphics.hpp>
#include "../../src/shared/state.h"
#include "../../src/client/render.h"
#include "../../src/client/client.h"
#include "../../src/shared/engine.h"
#include <thread>

using namespace state;
using namespace render;
using namespace client;
using namespace engine;

BOOST_AUTO_TEST_CASE(TestClientSubMethods)
{
    Board* board = new Board();
    Engine* engine = new Engine(board);
    RenderManager* renderMan = new RenderManager(board);
    Client* client = new Client(renderMan, engine);

    engine->isWaitingForHermitTargetPrompt = true;
    client->lookForPrompts();
    engine->isWaitingForHermitTargetPrompt = false;
    engine->isWaitingForHermitInfoPrompt = true;
    client->lookForPrompts();
    engine->isWaitingForHermitInfoPrompt = false;
    engine->isWaitingForAttackPrompt = true;
    CardClass* card = new CardClass(ADVENT1, WHITE, INSTANT);
    board->equipCard(engine->getCurrentPlayer(), card);
    renderMan->ui_render.init();
    engine->isWaitingForYesNoPrompt = true;
    engine->isWaitingForWoodsPrompt = true;
    engine->isWaitingForCellPrompt = true;
    engine->isWaitingForGatePrompt = true;
    Player* p3 = board->playerList[2].get();
    board->equipCard(*p3, card);
    engine->isWaitingForCardStealPrompt = true;
    engine->isWaitingForCardEffectTargetPrompt = true;
    client->lookForPrompts();

    engine->isBusy = false;
    client->moveClicked();
    client->revealedClicked();
    engine->getCurrentPlayer().name = FRANKLIN;
    client->capacityClicked();
    engine->getCurrentPlayer().name = GEORGES;
    client->capacityClicked();
    board->movePlayerTo(&engine->getCurrentPlayer(), board->cellList[0]);
    client->cellEffectClicked();
    client->damageClicked();
    DrawCardCommand* drawCmd = new DrawCardCommand(*engine, DARK);
    engine->waitingCommand = drawCmd;
    client->YesNoAnswer(true);
    AttackCommand* attackCmd = new AttackCommand(*engine, &engine->getCurrentPlayer());
    engine->waitingCommand = attackCmd;
    client->chosenAttackTarget(1);
    MoveCommand* moveCmd = new MoveCommand(*engine);
    engine->waitingCommand = moveCmd;
    client->cellChosen(2);
    GateCommand* gateCmd = new GateCommand(*engine);
    engine->waitingCommand = gateCmd;
    client->cardTypeChosen(static_cast<int>(DARK));
    WoodsCommand* woodsCmd = new WoodsCommand(*engine);
    engine->waitingCommand = woodsCmd;
    client->woodsAnswerClicked(0);
    StealEquipCommand* stealCmd = new StealEquipCommand(*engine, &engine->getCurrentPlayer());
    engine->waitingCommand = stealCmd;
    client->stealEquipAnswer(card);
    DarkBatCommand* darkBatCmd = new DarkBatCommand(*engine, &engine->getCurrentPlayer());
    engine->waitingCommand = darkBatCmd;
    client->chosenCardEffectTarget(1);
    CardClass hermitCard(HERMIT1, HERMIT, INSTANT);
    UseHermitCommand* hermitCmd = new UseHermitCommand(*engine, hermitCard);
    engine->waitingCommand = hermitCmd;
    client->chosenHermitTarget(2);
    HermitGiveReceive hermitInfo;
    hermitInfo.choice = 1;
    hermitInfo.card = card;
    hermitInfo.receive = 1;
    client->hermitEffect(hermitInfo);

}

BOOST_AUTO_TEST_CASE(TestClientMTSubMethods) {
    Board* board = new Board();
    Engine* engine= new Engine(board);
    RenderManager* renderMan = new RenderManager(board);
    ClientMT* client = new ClientMT(renderMan, engine, 0);

    engine->isWaitingForHermitTargetPrompt = true;
    client->lookForPrompts();
    engine->isWaitingForHermitTargetPrompt = false;
    engine->isWaitingForHermitInfoPrompt = true;
    client->lookForPrompts();
    engine->isWaitingForHermitInfoPrompt = false;
    engine->isWaitingForAttackPrompt = true;
    CardClass* card = new CardClass(ADVENT1, WHITE, INSTANT);
    board->equipCard(engine->getCurrentPlayer(), card);
    renderMan->ui_render.init();
    engine->isWaitingForYesNoPrompt = true;
    engine->isWaitingForWoodsPrompt = true;
    engine->isWaitingForCellPrompt = true;
    engine->isWaitingForGatePrompt = true;
    Player* p3 = board->playerList[2].get();
    board->equipCard(*p3, card);
    engine->isWaitingForCardStealPrompt = true;
    engine->isWaitingForCardEffectTargetPrompt = true;
    client->lookForPrompts();

    engine->isBusy = false;
    client->moveClicked();
    client->revealedClicked();
    engine->getCurrentPlayer().name = FRANKLIN;
    client->capacityClicked();
    engine->getCurrentPlayer().name = GEORGES;
    client->capacityClicked();
    board->movePlayerTo(&engine->getCurrentPlayer(), board->cellList[0]);
    client->cellEffectClicked();
    client->damageClicked();
    DrawCardCommand* drawCmd = new DrawCardCommand(*engine, DARK);
    engine->waitingCommand = drawCmd;
    client->YesNoAnswer(true);
    AttackCommand* attackCmd = new AttackCommand(*engine, &engine->getCurrentPlayer());
    engine->waitingCommand = attackCmd;
    client->chosenAttackTarget(1);
    MoveCommand* moveCmd = new MoveCommand(*engine);
    engine->waitingCommand = moveCmd;
    client->cellChosen(2);
    GateCommand* gateCmd = new GateCommand(*engine);
    engine->waitingCommand = gateCmd;
    client->cardTypeChosen(static_cast<int>(DARK));
    WoodsCommand* woodsCmd = new WoodsCommand(*engine);
    engine->waitingCommand = woodsCmd;
    client->woodsAnswerClicked(0);
    StealEquipCommand* stealCmd = new StealEquipCommand(*engine, &engine->getCurrentPlayer());
    engine->waitingCommand = stealCmd;
    client->stealEquipAnswer(card);
    DarkBatCommand* darkBatCmd = new DarkBatCommand(*engine, &engine->getCurrentPlayer());
    engine->waitingCommand = darkBatCmd;
    client->chosenCardEffectTarget(1);
    CardClass hermitCard(HERMIT1, HERMIT, INSTANT);
    UseHermitCommand* hermitCmd = new UseHermitCommand(*engine, hermitCard);
    engine->waitingCommand = hermitCmd;
    client->chosenHermitTarget(2);
    HermitGiveReceive hermitInfo;
    hermitInfo.choice = 1;
    hermitInfo.card = card;
    hermitInfo.receive = 1;
    client->hermitEffect(hermitInfo);
}

BOOST_AUTO_TEST_CASE(TestClientRun) {
    Board* board = new Board();
    Engine* engine = new Engine(board);
    RenderManager* renderMan = new RenderManager(board);
    RenderManager* renderMan2 = new RenderManager(board);
    Client* client = new Client(renderMan, engine);
    ClientMT* clientMT = new ClientMT(renderMan2, engine, 0);

    std::thread clientThread(&Client::run, client);
    sleep(1);
    std::thread clientMTThread(&ClientMT::run, clientMT);
    std::this_thread::sleep_for(std::chrono::seconds(4));
    client->renderMan->window.close();
    clientMT->renderMan->window.close();
    clientThread.join();
    clientMTThread.join();
}
