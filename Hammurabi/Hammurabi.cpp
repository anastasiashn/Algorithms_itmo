#include <iostream>
#include "Random.h"

#include "Hammurabi.h"

Hammurabi::Hammurabi()
{
    reset();
}

void Hammurabi::start()
{
    while (round < finishRound 
        && !gameLoosed
        && !gameClosed)
    {
        playRound();
    }

    if (!gameLoosed) {
        gameOver();
    }
}

void Hammurabi::reset()
{
    round = roundStart;
    population = startPopulation;
    wheat = startWheat;
    landArea = startLandAreat;
    gameLoosed = false;
    gameClosed = false;
    diedOfHungerPerYear = new double[finishRound];
}

void Hammurabi::playRound() {
    clearConsole();
    setAreaPriceForRound();
    setAmountOfWheatPerAcre();
    
    if (round == roundStart) {
        printFirstRound();
    }
    else {
        HammurabiRound hammurabiRound(
            population,
            wheat,
            amountOfWheatPerAcre,
            orderedToSowAcres,
            orderedToEatWheat,
            acresByOneWheat);
        double populationDiedPercent = hammurabiRound.diedOfHunger / (double)population * 100;
        if (populationDiedPercent >= losePopulationDiedPercent) {
            gameLose();
            return;
        }

        printRoundInfo(hammurabiRound);
        if (!askYesOrNo("Хотите ли вы продолжить?")) {
            gameClosed = true;
        }
    }

    handleUserInput();
    round++;
}

void Hammurabi::clearConsole()
{
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif
}

void Hammurabi::setAreaPriceForRound() {
    landPrice = Random::getRandomNumber(17, 26);
}

void Hammurabi::setAmountOfWheatPerAcre() {
    amountOfWheatPerAcre = Random::getRandomNumber(1, 6);
}

void Hammurabi::printFirstRound() {
    std::cout
        << "Здравствуй новый государь. Тебе передано в правление государство." << std::endl
        << "В твоем распоряжении:" << std::endl
        << tab << wheat << " бушелей пшеницы" << std::endl
        << tab << landArea << " акров земли" << std::endl
        << tab << "население " << population << " человек." << std::endl;
}

void Hammurabi::printRoundInfo(HammurabiRound roundInfo) {
    population += roundInfo.populationChange();
    wheat += roundInfo.wheatChange();

    std::cout
        << "Мой повелитель, соизволь поведать тебе"
        << std::endl;
    std::cout
        << tab
        << "в году "
        << round
        << " твоего высочайшего правления"
        << std::endl;
    std::cout
        << tab
        << roundInfo.diedOfHunger
        << " человек умерли с голоду, и "
        << roundInfo.newPeoplesCount
        << " человек прибыли в наш великий город."
        << std::endl;

    if (roundInfo.plagueHasOccurred) {
        std::cout
            << tab
            << "Чума уничтожила половину населения."
            << std::endl;
    }

    std::cout
        << tab
        << "Население города сейчас составляет "
        << population
        << " человек."
        << std::endl;
    std::cout
        << tab
        << "Мы собрали "
        << roundInfo.harvestedWheat
        << " бушелей пшеницы, по "
        << amountOfWheatPerAcre
        << " бушеля с акра."
        << std::endl;
    std::cout
        << tab
        << "Крысы истребили "
        << roundInfo.ratsEatedWheat
        << " бушелей пшеницы, оставив "
        << wheat
        << " бушеля в амбарах."
        << std::endl;
}

void Hammurabi::gameOver() {
    double sumOfdiedOfHungerPerYear = 0;
    for (int index = 0; index < finishRound; index++) {
        sumOfdiedOfHungerPerYear += diedOfHungerPerYear[index];
    }
    double diedOfHungerPerYear = sumOfdiedOfHungerPerYear / finishRound;
    int acresPerPerson = landArea / population;
    if (diedOfHungerPerYear > 33 && acresPerPerson < 7) {
        std::cout
            << "Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города. Теперь вы вынуждены влачить жалкое существование в изгнании";
        return;
    }

    if (diedOfHungerPerYear > 10 && acresPerPerson < 9) {
        std::cout
            << "Вы правили железной рукой, подобно Нерону и Ивану Грозному. Народ вздохнул с облегчением, и никто больше не желает видеть вас правителем";
        return;
    }

    if (diedOfHungerPerYear > 3 && acresPerPerson < 10) {
        std::cout
            << "Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы увидеть вас во главе города снова";
        return;
    }

    std::cout
        << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше";
}

void Hammurabi::gameLose() {
    std::cout
        << "В вашем государстве умерли почти все жители. Государство больше не может существовать."
        << std::endl;
    if (askYesOrNo("Начать заного?")) {
        reset();
        return;
    }

    gameLoosed = true;
}

#pragma region UserInput

void Hammurabi::handleUserInput() {
    std::cout << "Что пожелаешь, повелитель?" << std::endl;
    bool landBought = false;
    bool landSold = false;
    bool wheatEated = false;
    bool wheatIsSown = false;

    int beforeHandlePopulation = population;
    int beforeHandleWheat = wheat;
    int beforeHandleLandArea = landArea;
    while (!landBought
        || !landSold
        || !wheatEated
        || !wheatIsSown)
    {
        population = beforeHandlePopulation;
        wheat = beforeHandleWheat;
        landArea = beforeHandleLandArea;

        landBought = tryBuyLand();

        if (landBought) {
            landSold = trySellLand();
        }

        if (landSold) {
            wheatEated = tryEatWheat();
        }

        if (wheatEated) {
            wheatIsSown = trySowWheat();
        }
    }
}

bool Hammurabi::tryBuyLand() {
    int maxLandToBuy = wheat / landPrice;
    int landToBuy = INT_MAX;
    while (landToBuy > maxLandToBuy)
    {
        std::cout 
            << "Сколько акров земли повелеваешь купить? (Максимум "
            << maxLandToBuy
            << ") ";
        std::cin >> landToBuy;
        std::cout << std::endl;
        if (landToBuy > maxLandToBuy) {
            std::cout
                << "О, повелитель, пощади нас! У нас "
                << wheat << " бушелей пшеницы. Мы можем себе позволить только "
                << maxLandToBuy << " акров земли."
                << std::endl;
        }

        if (landToBuy < 0) {
            std::cout
                << "Повелитель, я не расслышал что вы сказали, немогли бы повторить."
                << std::endl;
            landToBuy = INT_MAX;
        }
    }

    landArea += landToBuy;
    wheat -= landToBuy * landPrice;
    return true;
}

bool Hammurabi::trySellLand() {
    int landToSell = -1;
    while (landToSell < 0)
    {
        std::cout 
            << "Сколько акров земли повелеваешь продать? (Максимум " 
            << landArea
            << ") ";
        std::cin >> landToSell;
        std::cout << std::endl;
        if (landToSell > landArea) {
            std::cout
                << "О, повелитель, пощади нас! У нас только "
                << landArea << " акров земли."
                << std::endl;
            landToSell = -1;
            if (askRestartUserInput()) {
                return false;
            }
        }

        if (landToSell < 0) {
            std::cout
                << "Повелитель, я не расслышал что вы сказали, немогли бы повторить."
                << std::endl;
            if (askRestartUserInput()) {
                return false;
            }
        }
    }

    landArea -= landToSell;
    wheat += landToSell * landPrice;
    return true;
}

bool Hammurabi::tryEatWheat() {
    int wheatToEat = INT_MAX;
    while (wheatToEat > wheat)
    {
        std::cout 
            << "Сколько бушелей пшеницы повелеваешь съесть? (Максимум "
            << wheat
            << ") ";
        std::cin >> wheatToEat;
        std::cout << std::endl;
        if (wheatToEat > wheat) {
            std::cout
                << "У нас недостаточно запасов пшеницы. Мы имеем только "
                << wheat
                << " бушелей пшеницы"
                << std::endl;
            if (askRestartUserInput()) {
                return false;
            }
        }

        if (wheatToEat < 0) {
            std::cout
                << "Повелитель, я не расслышал что вы сказали, немогли бы повторить."
                << std::endl;
            wheatToEat = INT_MAX;
            if (askRestartUserInput()) {
                return false;
            }
        }
    }

    orderedToEatWheat = wheatToEat;
    return true;
}

bool Hammurabi::trySowWheat() {
    int currentWheat = wheat - orderedToEatWheat;
    int maxAcresToSowByWheat = currentWheat * acresByOneWheat;
    int maxAcresToSowByPopulation = population * maxManCanHandleAcres;

    int maxAcresToSow = maxAcresToSowByWheat > maxAcresToSowByPopulation
        ? maxAcresToSowByPopulation
        : maxAcresToSowByWheat;
    maxAcresToSow = maxAcresToSow > landArea
        ? landArea
        : maxAcresToSow;
    int acresToSow = INT_MAX;
    while (acresToSow > maxAcresToSow)
    {
        std::cout 
            << "Сколько акров земли повелеваешь засеять? (Максимум "
            << maxAcresToSow
            << ") ";
        std::cin >> acresToSow;
        std::cout << std::endl;

        if (acresToSow > landArea) {
            std::cout
                << "Помилуйте государь. В нашем государстве только "
                << landArea
                << " акров земли."
                << std::endl;
            if (askRestartUserInput()) {
                return false;
            }
        }

        if (acresToSow > maxAcresToSowByWheat) {
            std::cout
                << "У нас недостаточно запасов пшеницы. Мы можем засеять только "
                << maxAcresToSow
                << " акров земли."
                << std::endl;
            if (askRestartUserInput()) {
                return false;
            }
        }

        if (acresToSow > maxAcresToSowByPopulation) {
            std::cout
                << "Население нашего государства недостаточно для засеевания такого количесва акров. Мы можем засеять только "
                << maxAcresToSow
                << " акров земли."
                << std::endl;
            if (askRestartUserInput()) {
                return false;
            }
        }

        if (acresToSow < 0) {
            std::cout
                << "Повелитель, я не расслышал что вы сказали, немогли бы повторить."
                << std::endl;
            acresToSow = INT_MAX;
            if (askRestartUserInput()) {
                return false;
            }
        }
    }

    orderedToSowAcres = acresToSow;
    return true;
}

#pragma endregion

bool Hammurabi::askRestartUserInput() {
    return askYesOrNo("Повелитель, может начнем планирование c самого начала?");
}

bool Hammurabi::askYesOrNo(const char* question) {
    std::string answer;
    std::string yesString = "да";
    std::string noString = "нет";
    while (answer.compare(yesString) != 0 && answer.compare(noString) != 0)
    {
        std::cout << question << " да / нет? ";
        std::cin >> answer;
        std::cout << std::endl;
    }
    return answer.compare("да") == 0;
}