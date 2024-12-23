#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <locale>
#include <windows.h>

using namespace std;

class Character {
protected:
    int x, y;

public:
    Character(int startX, int startY) : x(startX), y(startY) {}
    virtual ~Character() {}

    virtual void interact() const = 0;

    int getX() const { return x; }
    int getY() const { return y; }
};

class Player : public Character {
public:
    Player(int startX, int startY) : Character(startX, startY) {}

    void interact() const override {
        cout << "Oyuncu bir sey yapti!" << endl;
    }
};

class Enemy : public Character {
public:
    Enemy(int startX, int startY) : Character(startX, startY) {}

    void interact() const override {
        cout << "Bir dusmanla karsilastin! Savasa hazirlan!" << endl;
    }
};

class Treasure {
private:
    int x, y;

public:
    Treasure(int startX, int startY) : x(startX), y(startY) {}

    int getX() const { return x; }
    int getY() const { return y; }
};

class Dungeon {
private:
    vector<vector<char>> map;
    int size;
    Player* player;
    vector<Enemy> enemies;
    vector<Treasure> treasures;
    int collectedTreasures;

public:
    Dungeon(int size) : size(size), map(size, vector<char>(size, '.')), collectedTreasures(0) {
        player = new Player(0, 0);
        map[player->getX()][player->getY()] = 'P';
    }

    ~Dungeon() {
        delete player;
    }

    void generate() {
        srand(static_cast<unsigned>(time(0)));

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (i == 0 && j == 0) continue;
                int randValue = rand() % 10;

                if (randValue < 2) {
                    enemies.emplace_back(i, j);
                    map[i][j] = 'E';
                }
                else if (randValue < 4 && treasures.size() < 3) {
                    treasures.emplace_back(i, j);
                    map[i][j] = 'T';
                }
            }
        }
    }

    void print() const {
        for (const auto& row : map) {
            for (const auto& cell : row) {
                cout << cell << " ";
            }
            cout << endl;
        }
        cout << "Toplanan hazine: " << collectedTreasures << "/" << treasures.size() << endl;
    }

    void printFinalMap() const {
        vector<vector<char>> finalMap(size, vector<char>(size, '.'));
        for (const auto& enemy : enemies) {
            finalMap[enemy.getX()][enemy.getY()] = 'E';
        }
        finalMap[player->getX()][player->getY()] = 'P';

        for (const auto& row : finalMap) {
            for (const auto& cell : row) {
                cout << cell << " ";
            }
            cout << endl;
        }
    }

    bool movePlayer(char direction) {
        int newX = player->getX();
        int newY = player->getY();

        if (direction == 'W' || direction == 'w') newX--;
        else if (direction == 'S' || direction == 's') newX++;
        else if (direction == 'A' || direction == 'a') newY--;
        else if (direction == 'D' || direction == 'd') newY++;
        else {
            cout << "Gecersiz yon! Hareket etmek icin W/A/S/D kullanin." << endl;
            return false;
        }

        if (newX < 0 || newX >= size || newY < 0 || newY >= size) {
            cout << "Oraya tasinamazsin! Zindanin disinda." << endl;
            return false;
        }

        if (map[newX][newY] == 'E') {
            cout << "Bir dusmanla karsilastin! Oyunu kaybettin!" << endl;
            char choice;
            cout << "Yeniden baslamak ister misiniz? (E/H): ";
            cin >> choice;
            if (choice == 'E' || choice == 'e') {
                reset();
            }
            else {
                cout << "Oynadiginiz icin tesekkurler! Gule gule!" << endl;
                exit(0);
            }
        }
        else if (map[newX][newY] == 'T') {
            cout << "Bir hazine buldun! Envanterine eklendi." << endl;
            collectedTreasures++;
            if (collectedTreasures == treasures.size()) {
                cout << "Tebrikler! Tum hazineleri topladiniz!" << endl;
                cout << "Son harita:" << endl;
                printFinalMap();
                exit(0);
            }
        }
        else {
            cout << "Bos bir yere tasindin." << endl;
        }

        map[player->getX()][player->getY()] = '.';
        map[newX][newY] = 'P';
        player = new Player(newX, newY);

        return true;
    }

    void reset() {
        delete player;
        player = new Player(0, 0);
        map = vector<vector<char>>(size, vector<char>(size, '.'));
        enemies.clear();
        treasures.clear();
        collectedTreasures = 0;
        map[player->getX()][player->getY()] = 'P';
        generate();
    }
};

int main() {
    setlocale(LC_ALL, "Turkish");
    SetConsoleOutputCP(65001);

    cout << "Dungeon oyununa hos geldiniz!" << endl;
    Dungeon dungeon(5);
    dungeon.generate();

    while (true) {
        cout << "Dungeon haritasi:" << endl;
        dungeon.print();

        cout << "Hareketinizi girin (Hareket etmek icin W/A/S/D , cikmak icin Q): ";
        char move;
        cin >> move;

        if (move == 'Q' || move == 'q') {
            cout << "Oynadiginiz icin tesekkurler! Gule gule!" << endl;
            break;
        }

        dungeon.movePlayer(move);
    }

    return 0;
}
