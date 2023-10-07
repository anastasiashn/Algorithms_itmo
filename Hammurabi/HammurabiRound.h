#pragma once
class HammurabiRound
{
public:
    int diedOfHunger;
    int newPeoplesCount;
    int diedByPlague;
    bool plagueHasOccurred;
    int harvestedWheat;
    int ratsEatedWheat;
    int eatedWheat;
    int sowedWheat;

    HammurabiRound(
        int population,
        int wheat,
        int amountOfWheatPerAcre,
        int orderedToSowAcres,
        int orderedToEatWheat,
        int acresByOneWheat);
    int populationDied();
    int populationChange();
    int wheatChange();

private:
    const int wheatByManByRound = 20;
    const int plaguePercent = 15;
    int population;
    int wheat;
    int amountOfWheatPerAcre;
    int orderedToSowAcres;
    int orderedToEatWheat;
    int acresByOneWheat;

    void sowTheFields();
    void populationDiedOfHunger();
    void populationIncrease();
    void plague();
    void ratEated();
    void wheatHarvest();
};

