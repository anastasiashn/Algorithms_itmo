#include "HammurabiRound.h"

class Hammurabi
{
public:
    Hammurabi();
    void start();
private:
    const int startPopulation = 100;
    const int startWheat = 2800;
    const int startLandAreat = 1000;
    const int losePopulationDiedPercent = 45;
    const int maxManCanHandleAcres = 10;
    const int acresByOneWheat = 2;
    const int finishRound = 10;
    const int roundStart = 0;
    const char* tab = "    ";

    int round;
    int population;
    int wheat;
    int landArea;
    int landPrice;
    int amountOfWheatPerAcre;
    int orderedToSowAcres;
    int orderedToEatWheat;

    bool gameLoosed;
    bool gameClosed;
    double* diedOfHungerPerYear;
    
    void reset();
    void playRound();
    void gameOver();
    void setAreaPriceForRound();
    void setAmountOfWheatPerAcre();
    void printFirstRound();
    void printRoundInfo(HammurabiRound roundInfo);
    void gameLose();
    void handleUserInput();
    bool tryBuyLand();
    bool trySellLand();
    bool tryEatWheat();
    bool trySowWheat();
    bool askRestartUserInput();
    bool askYesOrNo(const char* question);
    void clearConsole();
};

