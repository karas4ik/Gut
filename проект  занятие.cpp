#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
using namespace std;

HANDLE hConsole;

class Player
{
public:
    Player(const string& name = "");
    string GetName() const;
    Player* GetNext() const;
    void SetNext(Player* next);
    void SetScore(int score);
    int GetScore() const;
private:
    string m_Name;
    Player* m_pNext;
    int m_Score;
};

Player::Player(const string& name) :
    m_Name(name),
    m_pNext(NULL),
    m_Score(0)
{}

string Player::GetName() const {
    return m_Name;
}

Player* Player::GetNext() const {
    return m_pNext;
}

void Player::SetNext(Player* next) {
    m_pNext = next;
}

void Player::SetScore(int score) {
    m_Score = score;
}

int Player::GetScore() const {
    return m_Score;
}

class Lobby
{
    friend ostream& operator<<(ostream& os, const Lobby& aLobby);
public:
    Lobby(const string& playerName);
    ~Lobby();
    void AddPlayer();
    void RemovePlayer();
    void Clear();
    void DisplayPlayers() const;
    void SortPlayersByScore();
    void FindPlayerByName(const string& name) const;
    void AddScoreToPlayer(const string& name, int score);
    void AddPlayerByName(const string& name);
    void SaveData() const;
    void LoadData();
    void Instriction();
private:
    Player* m_pHead;
    string m_PlayerName;
};

Lobby::Lobby(const string& playerName) :
    m_pHead(0),
    m_PlayerName(playerName)
{
    LoadData();
}

Lobby::~Lobby() {
    SaveData();
    Clear();
}

void Lobby::AddPlayer() {
    cout << "Пожалуйста, назовите нового игрока:";
    string name;
    cin >> name;
    Player* pNewPlayer = new Player(name);
    if (m_pHead == 0) {
        m_pHead = pNewPlayer;
    }
    else {
        Player* pIter = m_pHead;
        while (pIter->GetNext() != 0) {
            pIter = pIter->GetNext();
        }
        pIter->SetNext(pNewPlayer);
    }
}

void Lobby::RemovePlayer() {
    if (m_pHead == 0) {
        cout << "Игровое лобби пустое!\n";
    }
    else {
        Player* pTemp = m_pHead;
        m_pHead = m_pHead->GetNext();
        delete pTemp;
    }
}

void Lobby::Clear() {
    while (m_pHead != 0) {
        RemovePlayer();
    }
}

void Lobby::Instriction() {
    cout << "Данная программа симулирует ожидание игрока в игровом лоббиn\n";
    cout << "user может добавлять игроков в своё лобби, отправлють их в игру или полностью очистить лобби\n admin может добавлять очки игрокам для бонуса в самой игре, находиить игрока в лобби или сортировать игроков по очкам\n";
    cout << "Что бы попасть на ветвь администратора в пароле введите admin\n";
    cout << "Для сохранения ваших данных в конце работы выходите вводя 0, а не просто закрывая так ваши данные не сохраняться\n";
    cout << "Функция сортировки ещё свежая и она может некорректно работать или вовсе не работать\n";
    cout << "Ваши данные сохраняются в файле с вашим ником в игре так что запомните свой ник\n";
    cout << "1 функция добавляет нового игрока снизу, а втроя отправляет игрока в игру самого первого\n";
    cout << "6 функция у админа начисляет игроку очки в формате: 1 ввод ник; 2 ввод количество очков\n";
    cout << "Приятной игры\n";
}

void Lobby::DisplayPlayers() const {
    Player* pIter = m_pHead;
    cout << "\nСейчас в игровом лобби\n";
    if (pIter == 0) {
        cout << "В лобби никого нет\n";
    }
    else {
        while (pIter != 0) {
            cout << pIter->GetName() << " - " << pIter->GetScore() << endl;
            pIter = pIter->GetNext();
        }
    }
}

void Lobby::SortPlayersByScore() {
    if (m_pHead == 0) return;
    Player* pIter = m_pHead;
    Player* pMin;
    int minScore;
    while (pIter != 0) {
        pMin = pIter;
        minScore = pIter->GetScore();
        Player* pNext = pIter->GetNext();
        while (pNext != 0) {
            if (pNext->GetScore() < minScore) {
                pMin = pNext;
                minScore = pNext->GetScore();
            }
            pNext = pNext->GetNext();
        }
        if (pMin != pIter) {
            int tempScore = pIter->GetScore();
            pIter->SetScore(pMin->GetScore());
            pMin->SetScore(tempScore);
        }
        pIter = pIter->GetNext();
    }
}

void Lobby::FindPlayerByName(const string& name) const {
    Player* pIter = m_pHead;
    while (pIter != 0) {
        if (pIter->GetName() == name) {
            cout << "Игрок " << name << " найден!\n";
            return;
        }
        pIter = pIter->GetNext();
    }
    cout << "Игрок " << name << " не найден\n";
}

void Lobby::AddScoreToPlayer(const string& name, int score) {
    Player* pIter = m_pHead;
    while (pIter != 0) {
        if (pIter->GetName() == name) {
            pIter->SetScore(pIter->GetScore() + score);
            cout << "Очки игрока " << name << " обновлены до " << pIter->GetScore() << endl;
            return;
        }
        pIter = pIter->GetNext();
    }
    cout << "Игрок " << name << " не найден\n";
}

void Lobby::SaveData() const {
    ofstream file(m_PlayerName + ".txt");
    if (file.is_open()) {
        Player* pIter = m_pHead;
        while (pIter != 0) {
            file << pIter->GetName() << " " << pIter->GetScore() << endl;
            pIter = pIter->GetNext();
        }
        file.close();
    }
    else {
        cout << "Ошибка пожалуйства перезайдите в игру\n";
    }
}

void Lobby::AddPlayerByName(const string& name) {
    Player* pNewPlayer = new Player(name);
    if (m_pHead == 0) {
        m_pHead = pNewPlayer;
    }
    else {
        Player* pIter = m_pHead;
        while (pIter->GetNext() != 0) {
            pIter = pIter->GetNext();
        }
        pIter->SetNext(pNewPlayer);
    }
}

void Lobby::LoadData() {
    ifstream file(m_PlayerName + ".txt");
    if (file.is_open()) {
        string name;
        int score;
        while (file >> name >> score) {
            AddPlayerByName(name);
            Player* pIter = m_pHead;
            while (pIter->GetNext() != 0) {
                pIter = pIter->GetNext();
            }
            pIter->SetScore(score);
        }
        file.close();
    }
    else {
        cout << "Ошибка такой аккаунт не найден. Создаем новый аккаунт с таким именем\n";
    }
}

ostream& operator<<(ostream& os, const Lobby& aLobby) {
    aLobby.DisplayPlayers();
    return os;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 2);

    cout << "Введите свое игровое имя: ";
    string playerName;
    cin >> playerName;

    Lobby myLobby(playerName);
    int choice;
    string name;
    int score;
    cout << "Приветствую тебя " << playerName << " в игре  //Лобби ожидания// \n";
    cout << "Введите пароль администратора: ";
    string adminPassword;
    cin >> adminPassword;
    bool isAdmin = (adminPassword == "admin");
    do {
        cout << myLobby;
        cout << "\nЛобби ожидания\n";
        cout << "-1 - Краткая Инструкция\n";
        cout << "0 - Выйти из программы\n";
        cout << "1 - Добавить игрока в очередь\n";
        cout << "2 - Отправить игрока в игру\n";
        cout << "3 - Полностью очистить лобби\n";
        if (isAdmin) {
            cout << "4 - Сортировка игроков по очкам\n";
            cout << "5 - Найти игрока по имени\n";
            cout << "6 - Начислить очки игроку\n";
        }
        cout << endl << "Выберите: ";
        cin >> choice;
        switch (choice) {
        case -1: myLobby.Instriction(); break;
        case 0: cout << "До новых встреч\n"; break;
        case 1: myLobby.AddPlayer(); break;
        case 2: myLobby.RemovePlayer(); break;
        case 3: myLobby.Clear(); break;
        case 4: myLobby.SortPlayersByScore(); break;
        case 5:
            cout << "Введите имя игрока: ";
            cin >> name;
            myLobby.FindPlayerByName(name);
            break;
        case 6:
            cout << "Введите имя игрока и количество очков: ";
            cin >> name >> score;
            myLobby.AddScoreToPlayer(name, score);
            break;
        default: cout << "Ошибка. Такого номера нет\n";
        }
    } while (choice != 0);
    return 0;
}