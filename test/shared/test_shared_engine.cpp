#include <boost/test/unit_test.hpp>
#include "../../src/shared/state.h"
#include "../../src/shared/engine.h"
#include "../../src/client/client.h"
#include <mutex>

using namespace state;
using namespace engine;
using namespace client;

BOOST_AUTO_TEST_CASE(TestEngine) {
    Board* board = new Board();
    Engine* engine = new Engine(board);

    engine->goToNextPlayer();
    BOOST_CHECK_EQUAL(engine->getCurrentPlayer().id, board->playerList[1]->id);
    board->playerList[2]->isAlive = false;
    engine->goToNextPlayer();

    engine->processOneCommand();
    engine->processOneCommandMT();
    RevealCommand* reveal = new RevealCommand(*engine, 2);
    engine->commands.push_back(reveal);
    engine->processOneCommand();
    BOOST_CHECK_EQUAL(board->playerList[2]->revealed, true);
    engine->commands.push_back(reveal);
    engine->processOneCommandMT();
    engine->update();

    AttackCommand* attack = new AttackCommand(*engine, 0);
    attack->execute();
    int id = -1;
    attack->receivePromptAnswer(&id);
    attack->execute();
    attack->isAttackCancelled = false;
    id = 1;
    attack->receivePromptAnswer(&id);
    attack->execute();

    Player* player2 = board->playerList[1].get();
    FranklinCapacityCommand* franklin = new FranklinCapacityCommand(*engine, player2);
    franklin->execute();
    franklin->receivePromptAnswer(&id);
    franklin->execute();

    GeorgesCapacityCommand* georges = new GeorgesCapacityCommand(*engine, player2);
    georges->execute();
    georges->receivePromptAnswer(&id);
    georges->execute();

    DarkSpiderCommand* spider = new DarkSpiderCommand(*engine, player2);
    spider->execute();
    board->equipCard(*player2, new CardClass(SPIDER1, DARK, INSTANT));
    spider->receivePromptAnswer(&id);
    spider->execute();

    DarkDollCommand* doll = new DarkDollCommand(*engine, player2);
    doll->execute();
    board->equipCard(*player2, new CardClass(DOLL1, DARK, INSTANT));
    doll->receivePromptAnswer(&id);
    doll->execute();

    DarkBatCommand* bat = new DarkBatCommand(*engine, player2);
    bat->execute();
    board->equipCard(*player2, new CardClass(BAT1, DARK, INSTANT));
    bat->receivePromptAnswer(&id);
    bat->execute();

    WhiteFlareCommand* flare = new WhiteFlareCommand(*engine, player2);
    board->equipCard(*player2, new CardClass(FLARE1, WHITE, INSTANT));
    flare->execute();

    WhiteWaterCommand* water = new WhiteWaterCommand(*engine, player2);
    board->equipCard(*player2, new CardClass(WATER1, WHITE, INSTANT));
    water->execute();

    WhiteAidCommand* aid = new WhiteAidCommand(*engine, player2);
    aid->execute();
    board->equipCard(*player2, new CardClass(AID1, WHITE, INSTANT));
    aid->receivePromptAnswer(&id);
    aid->execute();

    WhiteBlessingCommand* blessing = new WhiteBlessingCommand(*engine, player2);
    blessing->execute();
    board->equipCard(*player2, new CardClass(BLESSING1, WHITE, INSTANT));
    blessing->receivePromptAnswer(&id);
    blessing->execute();

    BlackDynamiteCommand* dynamite = new BlackDynamiteCommand(*engine, player2);
    board->equipCard(*player2, new CardClass(DYNAMITE, DARK, INSTANT));
    dynamite->execute();

    Player* customPlayer = new Player(board, 12, SHADOW, FRANKLIN);
    WhiteAdventCommand* advent = new WhiteAdventCommand(*engine, customPlayer);
    board->equipCard(*customPlayer, new CardClass(ADVENT1, WHITE, INSTANT));
    advent->execute();
    Player* customPlayer2 = new Player(board, 12, HUNTER, FRANKLIN);
    board->equipCard(*customPlayer2, new CardClass(ADVENT1, WHITE, INSTANT));
    customPlayer2->revealed = true;
    WhiteAdventCommand* advent2 = new WhiteAdventCommand(*engine, customPlayer2);
    advent2->execute();
    customPlayer2->revealed = false;
    board->equipCard(*customPlayer2, new CardClass(ADVENT1, WHITE, INSTANT));
    advent2->execute();
    bool yes = true;
    advent2->receivePromptAnswer(&yes);
    advent2->execute();

    Player* customPlayer3 = new Player(board, 12, HUNTER, VAMPIRE);
    BlackRitualCommand* ritual = new BlackRitualCommand(*engine, customPlayer3);
    board->equipCard(*customPlayer3, new CardClass(RITUAL1, DARK, INSTANT));
    ritual->execute();
    Player* customPlayer4 = new Player(board, 12, SHADOW, VAMPIRE);
    board->equipCard(*customPlayer4, new CardClass(RITUAL1, DARK, INSTANT));
    customPlayer4->revealed = true;
    BlackRitualCommand* ritual2 = new BlackRitualCommand(*engine, customPlayer4);
    ritual2->execute();
    customPlayer4->revealed = false;
    board->equipCard(*customPlayer4, new CardClass(RITUAL1, DARK, INSTANT));
    ritual2->execute();
    ritual2->receivePromptAnswer(&yes);
    ritual2->execute();

    DrawCardCommand* draw = new DrawCardCommand(*engine, DARK);
    DrawCardCommand* draw2 = new DrawCardCommand(*engine, DARK, true);
    draw->execute();
    draw->receivePromptAnswer(&yes);
    draw->execute();
    draw->cardType = WHITE;
    draw->execute();
    draw->cardType = HERMIT;
    draw->execute();

    GateCommand* gate = new GateCommand(*engine);
    gate->execute();
    int type = 3;
    gate->receivePromptAnswer(&type);
    gate->execute();
    type = 1;
    gate->isPromptCancelled = false;
    gate->receivePromptAnswer(&type);
    gate->execute();

    MoveCommand* move = new MoveCommand(*engine);
    move->execute();
    CellClass* cell = new CellClass(GRAVEYARD, {8}); 
    move->receivePromptAnswer(cell);
    move->execute();
    move->isWaitingForCell = true;
    move->execute();

    StealEquipCommand* steal = new StealEquipCommand(*engine, player2);
    steal->execute();
    steal->receivePromptAnswer(nullptr);
    steal->execute();
    steal->isPromptCancelled = false;
    CardClass* card = new CardClass(FLARE1, WHITE, INSTANT);
    board->equipCard(*board->playerList[0], card);
    steal->receivePromptAnswer(card);
    steal->execute();

    CardClass hCard(HERMIT1, HERMIT, INSTANT);
    hCard.effect = HEALORRECEIVE1;
    UseHermitCommand* useHermit = new UseHermitCommand(*engine, hCard);
    useHermit->execute();
    useHermit->receivePromptAnswer(&id);
    useHermit->execute();
    struct HermitGiveReceive hgr;
    hgr.choice = 1;
    hgr.card = card;
    hgr.receive = 1;
    useHermit->receivePromptAnswer(&hgr);
    useHermit->execute();
    useHermit->card.effect = GIVEORRECEIVE1;
    useHermit->isWaitingForInfo = true;
    useHermit->receivePromptAnswer(&hgr);
    useHermit->isWaitingForInfo = false;
    useHermit->isWaitingForTarget = false;
    useHermit->receivePromptAnswer(&id);

    WoodsCommand* woods = new WoodsCommand(*engine);
    woods->execute();
    int target = -1;
    woods->receivePromptAnswer(&target);
    woods->execute();
    woods->isPromptCancelled = false;
    target = 1;
    woods->receivePromptAnswer(&target);
    woods->execute();
    target = 2;
    woods->receivePromptAnswer(&target);

    engine->checkForVictory();
    board->playerList.clear();
    engine->checkForVictory();
    board->playerList.push_back(std::make_shared<Werewolf>(board));
    engine->checkForVictory();
    board->playerList.clear();
    board->playerList.push_back(std::make_shared<Franklin>(board));
    engine->checkForVictory();
}