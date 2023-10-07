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
        if (!askYesOrNo("������ �� �� ����������?")) {
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
        << "���������� ����� ��������. ���� �������� � ��������� �����������." << std::endl
        << "� ����� ������������:" << std::endl
        << tab << wheat << " ������� �������" << std::endl
        << tab << landArea << " ����� �����" << std::endl
        << tab << "��������� " << population << " �������." << std::endl;
}

void Hammurabi::printRoundInfo(HammurabiRound roundInfo) {
    population += roundInfo.populationChange();
    wheat += roundInfo.wheatChange();

    std::cout
        << "��� ����������, �������� �������� ����"
        << std::endl;
    std::cout
        << tab
        << "� ���� "
        << round
        << " ������ ����������� ���������"
        << std::endl;
    std::cout
        << tab
        << roundInfo.diedOfHunger
        << " ������� ������ � ������, � "
        << roundInfo.newPeoplesCount
        << " ������� ������� � ��� ������� �����."
        << std::endl;

    if (roundInfo.plagueHasOccurred) {
        std::cout
            << tab
            << "���� ���������� �������� ���������."
            << std::endl;
    }

    std::cout
        << tab
        << "��������� ������ ������ ���������� "
        << population
        << " �������."
        << std::endl;
    std::cout
        << tab
        << "�� ������� "
        << roundInfo.harvestedWheat
        << " ������� �������, �� "
        << amountOfWheatPerAcre
        << " ������ � ����."
        << std::endl;
    std::cout
        << tab
        << "����� ��������� "
        << roundInfo.ratsEatedWheat
        << " ������� �������, ������� "
        << wheat
        << " ������ � �������."
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
            << "��-�� ����� ���������������� � ����������, ����� ������� ����, � ������ ��� �� ������. ������ �� ��������� ������� ������ ������������� � ��������";
        return;
    }

    if (diedOfHungerPerYear > 10 && acresPerPerson < 9) {
        std::cout
            << "�� ������� �������� �����, ������� ������ � ����� ��������. ����� �������� � �����������, � ����� ������ �� ������ ������ ��� ����������";
        return;
    }

    if (diedOfHungerPerYear > 3 && acresPerPerson < 10) {
        std::cout
            << "�� ���������� ������ �������, � ���, �������, ���� ���������������, �� ������ ������ �� ������� ��� �� ����� ������ �����";
        return;
    }

    std::cout
        << "����������! ���� �������, �������� � ���������� ������ �� ���������� �� �����";
}

void Hammurabi::gameLose() {
    std::cout
        << "� ����� ����������� ������ ����� ��� ������. ����������� ������ �� ����� ������������."
        << std::endl;
    if (askYesOrNo("������ ������?")) {
        reset();
        return;
    }

    gameLoosed = true;
}

#pragma region UserInput

void Hammurabi::handleUserInput() {
    std::cout << "��� ���������, ����������?" << std::endl;
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
            << "������� ����� ����� ����������� ������? (�������� "
            << maxLandToBuy
            << ") ";
        std::cin >> landToBuy;
        std::cout << std::endl;
        if (landToBuy > maxLandToBuy) {
            std::cout
                << "�, ����������, ������ ���! � ��� "
                << wheat << " ������� �������. �� ����� ���� ��������� ������ "
                << maxLandToBuy << " ����� �����."
                << std::endl;
        }

        if (landToBuy < 0) {
            std::cout
                << "����������, � �� ��������� ��� �� �������, ������� �� ���������."
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
            << "������� ����� ����� ����������� �������? (�������� " 
            << landArea
            << ") ";
        std::cin >> landToSell;
        std::cout << std::endl;
        if (landToSell > landArea) {
            std::cout
                << "�, ����������, ������ ���! � ��� ������ "
                << landArea << " ����� �����."
                << std::endl;
            landToSell = -1;
            if (askRestartUserInput()) {
                return false;
            }
        }

        if (landToSell < 0) {
            std::cout
                << "����������, � �� ��������� ��� �� �������, ������� �� ���������."
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
            << "������� ������� ������� ����������� ������? (�������� "
            << wheat
            << ") ";
        std::cin >> wheatToEat;
        std::cout << std::endl;
        if (wheatToEat > wheat) {
            std::cout
                << "� ��� ������������ ������� �������. �� ����� ������ "
                << wheat
                << " ������� �������"
                << std::endl;
            if (askRestartUserInput()) {
                return false;
            }
        }

        if (wheatToEat < 0) {
            std::cout
                << "����������, � �� ��������� ��� �� �������, ������� �� ���������."
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
            << "������� ����� ����� ����������� �������? (�������� "
            << maxAcresToSow
            << ") ";
        std::cin >> acresToSow;
        std::cout << std::endl;

        if (acresToSow > landArea) {
            std::cout
                << "��������� ��������. � ����� ����������� ������ "
                << landArea
                << " ����� �����."
                << std::endl;
            if (askRestartUserInput()) {
                return false;
            }
        }

        if (acresToSow > maxAcresToSowByWheat) {
            std::cout
                << "� ��� ������������ ������� �������. �� ����� ������� ������ "
                << maxAcresToSow
                << " ����� �����."
                << std::endl;
            if (askRestartUserInput()) {
                return false;
            }
        }

        if (acresToSow > maxAcresToSowByPopulation) {
            std::cout
                << "��������� ������ ����������� ������������ ��� ���������� ������ ��������� �����. �� ����� ������� ������ "
                << maxAcresToSow
                << " ����� �����."
                << std::endl;
            if (askRestartUserInput()) {
                return false;
            }
        }

        if (acresToSow < 0) {
            std::cout
                << "����������, � �� ��������� ��� �� �������, ������� �� ���������."
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
    return askYesOrNo("����������, ����� ������ ������������ c ������ ������?");
}

bool Hammurabi::askYesOrNo(const char* question) {
    std::string answer;
    std::string yesString = "��";
    std::string noString = "���";
    while (answer.compare(yesString) != 0 && answer.compare(noString) != 0)
    {
        std::cout << question << " �� / ���? ";
        std::cin >> answer;
        std::cout << std::endl;
    }
    return answer.compare("��") == 0;
}