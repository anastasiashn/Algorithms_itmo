#include <iostream>
#include "Random.h"

#include "HammurabiRound.h"


HammurabiRound::HammurabiRound(
    int population,
    int wheat,
    int amountOfWheatPerAcre,
    int orderedToSowAcres,
    int orderedToEatWheat,
    int acresByOneWheat)
{
    this->population = population;
    this->wheat = wheat;
    this->amountOfWheatPerAcre = amountOfWheatPerAcre;
    this->orderedToSowAcres = orderedToSowAcres;
    this->orderedToEatWheat = orderedToEatWheat;
    this->acresByOneWheat = acresByOneWheat;

    sowTheFields();
    populationDiedOfHunger();
    populationIncrease();
    plague();
    wheatHarvest();
    ratEated();
}

int HammurabiRound::populationDied() {
    return diedOfHunger + diedByPlague;
}

int HammurabiRound::populationChange() {
    return newPeoplesCount - populationDied();
}

int HammurabiRound::wheatChange() {
    return harvestedWheat - ratsEatedWheat - eatedWheat - sowedWheat;
}

void HammurabiRound::sowTheFields() {
    sowedWheat = ceil((double)orderedToSowAcres / acresByOneWheat);
    wheat -= sowedWheat;
}


void HammurabiRound::populationDiedOfHunger() {
    int numberOfWellFedPeople = orderedToEatWheat / wheatByManByRound;
    if (population <= numberOfWellFedPeople) {
        eatedWheat = population * wheatByManByRound;
        wheat -= eatedWheat;
        diedOfHunger = 0;
        return;
    }

    eatedWheat = orderedToEatWheat;
    wheat -= orderedToEatWheat;
    diedOfHunger = population - numberOfWellFedPeople;
    population -= diedOfHunger;
}

void HammurabiRound::populationIncrease() {
    newPeoplesCount = diedOfHunger / 2 + (5 - amountOfWheatPerAcre) * wheat / 600 + 1;
    if (newPeoplesCount < 0) {
        newPeoplesCount = 0;
    }

    if (newPeoplesCount > 50) {
        newPeoplesCount = 50;
    }

    population += newPeoplesCount;
}

void HammurabiRound::plague() {
    plagueHasOccurred = Random::getPercent(plaguePercent);
    if (!plagueHasOccurred) {
        diedByPlague = 0;
        return;
    }
    diedByPlague = population / 2;
    population -= diedByPlague;
}

void HammurabiRound::ratEated() {
    int maxRatsCanEat = floor(wheat * 0.07);
    ratsEatedWheat = Random::getRandomNumber(0, maxRatsCanEat);
    wheat -= ratsEatedWheat;
}

void HammurabiRound::wheatHarvest() {
    harvestedWheat = orderedToSowAcres * amountOfWheatPerAcre;
    wheat += harvestedWheat;
}