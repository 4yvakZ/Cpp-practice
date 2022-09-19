// Lab1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

constexpr int kLastRound = 10;

constexpr int kMinLandPrice = 17;
constexpr int kMaxLandPrice = 26;

constexpr int kMinWheatFormLand = 1;
constexpr int kMaxWheatFormLand = 6;

constexpr float kMinWheatEatenByRats = 0;
constexpr float kMaxWheatEatenByRats = 0.07;

constexpr int kWheatToFeadCitizen = 20;
constexpr int kNumberOfLandFarmedByCitizen = 10;

constexpr int kPercentProbobilityOfPlague = 15;

constexpr int kStartNumberOfCitisens = 100;
constexpr int kStartNumberOfWheat = 2800;
constexpr int kStartNumberOfLand = 1000;

struct City {
    int citizens;
    int wheat;
    int land;
    int hungryCitizens;
    int plantedLand;
};

void buyLand(City &city, int landPrice) {
    while (true) {
        int numberOfLandToBuy;
        std::cout << "Сколько акров земли повелеваешь купить? ";
        std::cin >> numberOfLandToBuy;
        if (numberOfLandToBuy < 0 || numberOfLandToBuy * landPrice > city.wheat) {
            std::cout << "О, повелитель, пощади нас! У нас только "<< city.wheat << " бушелей пшеницы!\n";
        } else {
            city.land += numberOfLandToBuy;
            city.wheat -= numberOfLandToBuy * landPrice;
            return;
        }
    }
}

void sellLand(City &city, int landPrice) {
    while (true) {
        int numberOfLandToSell;
        std::cout << "Сколько акров земли повелеваешь продать? ";
        std::cin >> numberOfLandToSell;
        if (numberOfLandToSell < 0 || numberOfLandToSell > city.land) {
            std::cout << "О, повелитель, пощади нас! У нас только " << city.land << " акров земли!\n";
        } else {
            city.land -= numberOfLandToSell;
            city.wheat += numberOfLandToSell * landPrice;
            return;
        }
    }
}


void feedCity(City &city) {
    while (true) {
        int numberOfWheatToEat;
        std::cout << "Сколько бушелей пшеницы повелеваешь съесть? ";
        std::cin >> numberOfWheatToEat;
        if (numberOfWheatToEat < 0 || numberOfWheatToEat > city.wheat) {
            std::cout << "О, повелитель, пощади нас! У нас только " << city.wheat << " бушелей пшеницы!\n";
        } else {
            city.wheat -= numberOfWheatToEat;
            city.hungryCitizens = city.citizens - numberOfWheatToEat / kWheatToFeadCitizen;
            if (city.hungryCitizens < 0) {
                city.hungryCitizens = 0;
            }
            return;
        }
    }
}

void plantLand(City &city) {
    while (true) {
        int numberOfLandToPlant;
        std::cout << "Сколько акров земли повелеваешь засеять? ";
        std::cin >> numberOfLandToPlant;
        if (numberOfLandToPlant < 0 || numberOfLandToPlant > city.land) {
            std::cout << "О, повелитель, пощади нас! У нас только " << city.land << " акров земли !\n";
        } else if (numberOfLandToPlant / kNumberOfLandFarmedByCitizen > city.citizens) {
            std::cout << "О, повелитель, пощади нас! У нас только " << city.citizens << " человек!\n";
        } else {
            city.plantedLand = numberOfLandToPlant;
            return;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::srand(std::time(nullptr));

    City city;
    city.citizens = kStartNumberOfCitisens;
    city.wheat = kStartNumberOfWheat;
    city.land = kStartNumberOfLand;
    city.hungryCitizens = 0;
    city.plantedLand = 0;
    int round = 0;
    while (true) {
        int round = 0;
        while (round < kLastRound) {
            int landPrice = rand() % (kMaxLandPrice - kMinLandPrice + 1) + kMinLandPrice;
            if (round == 0) {
                std::cout << "Мой повелитель, соизволь поведать тебе о твоём городе перед началом твоего велечайшего правления\n";
                std::cout << "\tНаселение города сейчас составляет " << city.citizens << " человек;\n";
                std::cout << "\tВ амбарах " << city.wheat << " бушеля пшеницы;\n";
                std::cout << "\tГород сейчас занимает " << city.land << " акров;\n";
                std::cout << "\t1 акр земли стоит " << landPrice << " бушель.\n";
            } else {
                int wheatFromOneLand = rand() % (kMaxWheatFormLand - kMinWheatFormLand + 1) + kMinWheatFormLand;
                int wheatGrown = wheatFromOneLand * city.plantedLand;
                city.wheat += wheatGrown;
                int wheatEatedByRats = rand() % (int)(city.wheat * kMaxWheatEatenByRats - city.wheat * kMinWheatEatenByRats + 1) + city.wheat * kMinWheatEatenByRats;
                city.wheat -= wheatEatedByRats;

                int newSitizens = city.hungryCitizens / 2 + (5 - wheatFromOneLand) * city.wheat / 600 + 1;
                if (newSitizens < 0) {
                    newSitizens = 0;
                } else if (newSitizens > 50) {
                    newSitizens = 50;
                }

                city.citizens += newSitizens - city.hungryCitizens;

                std::cout << "Мой повелитель, соизволь поведать тебе\n";
                std::cout << "\tв году " << round << " твоего высочайшего правления\n";
                std::cout << "\t";
                if (city.hungryCitizens != 0) {
                    std::cout << city.hungryCitizens << " человек умерли с голоду, и ";
                }
                std::cout << newSitizens << " человек прибыли в наш великий город;\n";
                if (rand() % 100 < kPercentProbobilityOfPlague) {
                    std::cout << "\tЧума уничтожила половину населения;\n";
                    city.citizens /= 2;
                }
                std::cout << "\tНаселение города сейчас составляет " << city.citizens << " человек;\n";
                std::cout << "\tМы собрали " << wheatGrown << " бушеля пшеницы, по " << wheatFromOneLand << " бушеля с акра;\n";
                std::cout << "\t";
                if (wheatEatedByRats > 0) {
                    std::cout << "Крысы истребили " << wheatEatedByRats << " бушеля пшеницы, оставив ";
                }
                std::cout << city.wheat << " бушеля пщеницы в амбарах;\n";
                std::cout << "\tГород сейчас занимает " << city.land << " акров;\n";
                std::cout << "\t1 акр земли стоит " << landPrice << " бушель.\n";
            }

            std::cout << "Что пожелаешь, повелитель?\n";

            buyLand(city, landPrice);

            sellLand(city, landPrice);

            feedCity(city);

            plantLand(city);

            if (city.hungryCitizens * 1.0 / city.citizens > 0.45) {
                std::cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города.Теперь вы вынуждены влачить жалкое существование в изгнании\n";
                break;
            }

            round++;
        }
    }
}
