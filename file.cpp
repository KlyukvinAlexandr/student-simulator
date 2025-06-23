#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

class Student {
private:
    string name;
    int energy;
    int knowledge;
    int happiness;
    int day;
    int money;
    int daysWithoutStudy;
    vector<string> inventory;
    bool gameOver;

public:
    Student(string n) : name(n), energy(100), knowledge(0), happiness(50), day(1), money(1000), daysWithoutStudy(0), gameOver(false) {}

    void sleep() {
        daysWithoutStudy++;
        checkStudyNeglect();

        cout << "Вы поспали и восстановили энергию.\n";
        energy = min(100, energy + 60);
        happiness = min(100, happiness + 10);
        nextDay();
    }

    void study() {
        if (energy < 20) {
            cout << "Слишком устал для учебы!\n";
            return;
        }
        daysWithoutStudy = 0;
        int gained = 10 + rand() % 11;
        knowledge = min(100, knowledge + gained);
        energy = max(0, energy - 20);
        happiness = max(0, happiness - 5);
        cout << "Вы учились и получили " << gained << " знаний. Текущий уровень: " << knowledge << "/100\n";
    }

    void relax() {
        int cost = 50 + rand() % 101;
        if (money < cost) {
            cout << "Не хватает денег на развлечения!\n";
            return;
        }
        money -= cost;
        happiness = min(100, happiness + 30);
        energy = min(100, energy + 10);
        cout << "Вы развлеклись за " << cost << " рублей. Счастье: " << happiness << "/100\n";
    }

    void work() {
        if (energy < 30) {
            cout << "Слишком устал для работы!\n";
            return;
        }
        int earned = 100 + rand() % 201;
        money += earned;
        energy = max(0, energy - 30);
        happiness = max(0, happiness - 10);
        cout << "Вы подработали и заработали " << earned << " рублей. Баланс: " << money << " руб.\n";
    }

    void buyFood() {
        int cost = 100;
        if (money < cost) {
            cout << "Не хватает денег на еду!\n";
            return;
        }
        money -= cost;
        inventory.push_back("Еда");
        cout << "Вы купили еду. Теперь у вас " << inventory.size() << " порций.\n";
    }

    void eat() {
        if (find(inventory.begin(), inventory.end(), "Еда") == inventory.end()) {
            cout << "У вас нет еды!\n";
            return;
        }
        auto it = find(inventory.begin(), inventory.end(), "Еда");
        inventory.erase(it);
        energy = min(100, energy + 30);
        happiness = min(100, happiness + 5);
        cout << "Вы поели. Энергия: " << energy << "/100\n";
    }

    void buyCoffee() {
        int cost = 50;
        if (money < cost) {
            cout << "Не хватает денег на кофе!\n";
            return;
        }
        money -= cost;
        inventory.push_back("Кофе");
        cout << "Вы купили кофе. Теперь у вас " << inventory.size() << " предметов.\n";
    }

    void drinkCoffee() {
        if (find(inventory.begin(), inventory.end(), "Кофе") == inventory.end()) {
            cout << "У вас нет кофе!\n";
            return;
        }
        auto it = find(inventory.begin(), inventory.end(), "Кофе");
        inventory.erase(it);
        energy = min(150, energy + 40);
        cout << "Вы выпили кофе. Энергия: " << energy << "/150\n";
    }

    void checkStats() const {
        cout << "\n--- Статус ---\n";
        cout << "День: " << day << "/30\n";
        cout << "Энергия: " << energy << "/100\n";
        cout << "Знания: " << knowledge << "/100\n";
        cout << "Счастье: " << happiness << "/100\n";
        cout << "Деньги: " << money << " руб.\n";
        cout << "Инвентарь: ";
        if (inventory.empty()) {
            cout << "пусто\n";
        }
        else {
            for (const auto& item : inventory) {
                cout << item << ", ";
            }
            cout << "\n";
        }
    }

    void checkStudyNeglect() {
        if (daysWithoutStudy >= 3) {
            cout << "\n=== ВНИМАНИЕ ===\n";
            cout << "Вы слишком долго не учились! Преподаватели начали замечать ваше отсутствие.\n";
            happiness -= 15;

            if (daysWithoutStudy >= 5) {
                gameOver = true;
                cout << "\n=== ВЫ ПРОИГРАЛИ ===\n";
                cout << "Вас отчислили за прогулы!\n";
            }
        }
    }

    void nextDay() {
        day++;

        // Проверка условий проигрыша
        if (energy <= 0) {
            gameOver = true;
            cout << "\n=== ВЫ ПРОИГРАЛИ ===\n";
            cout << "Вы полностью истощены и больше не можете продолжать учебу.\n";
            return;
        }

        if (happiness <= 0) {
            gameOver = true;
            cout << "\n=== ВЫ ПРОИГРАЛИ ===\n";
            cout << "Вы впали в депрессию и бросили университет.\n";
            return;
        }

        if (money < 0) {
            gameOver = true;
            cout << "\n=== ВЫ ПРОИГРАЛИ ===\n";
            cout << "Вы накопили слишком много долгов и были отчислены.\n";
            return;
        }

        // Обязательные события (каждые 5 дней)
        if (day % 5 == 0 && day <= 30) {
            cout << "\n=== ОБЯЗАТЕЛЬНОЕ МЕРОПРИЯТИЕ ===\n";
            cout << "Сегодня контрольная работа! Без учебы в предыдущие дни вы не сможете ее написать.\n";

            if (knowledge < day) {
                cout << "Вы не справились с контрольной! -20 счастья.\n";
                happiness -= 20;

                if (happiness <= 0) {
                    gameOver = true;
                    cout << "\n=== ВЫ ПРОИГРАЛИ ===\n";
                    cout << "Вы не выдержали стресса от неудач и бросили учебу.\n";
                    return;
                }
            }
            else {
                cout << "Вы успешно написали контрольную! +10 счастья и +5 знаний.\n";
                happiness = min(100, happiness + 10);
                knowledge = min(100, knowledge + 5);
            }
        }

        // Случайные события
        if (day <= 30) {
            int event = rand() % 10;
            switch (event) {
            case 0:
                cout << "\nСегодня лекция у строгого преподавателя! Придется идти.\n";
                energy = max(0, energy - 15);
                knowledge = min(100, knowledge + 15);
                break;
            case 1:
                cout << "\nВаш друг одолжил вам конспект. +10 знаний!\n";
                knowledge = min(100, knowledge + 10);
                break;
            case 2:
            {
                int foundMoney = 50 + rand() % 151;
                cout << "\nВы нашли " << foundMoney << " рублей на улице!\n";
                money += foundMoney;
                break;
            }
            case 3:
                cout << "\nВы простудились. -20 энергии.\n";
                energy = max(0, energy - 20);
                break;
            }
        }
    }

    bool isGameOver() const {
        return gameOver || day > 30;
    }

    void checkExamResults() {
        if (day <= 30) return;

        cout << "\n=== РЕЗУЛЬТАТЫ СЕССИИ ===\n";
        cout << "Ваш уровень знаний: " << knowledge << "/100\n";

        if (knowledge >= 80) {
            cout << "ОТЛИЧНО! Вы сдали все экзамены на 5 и получаете повышенную стипендию!\n";
        }
        else if (knowledge >= 60) {
            cout << "ХОРОШО. В основном четверки, есть несколько пятерок. Вы успешно переведены на следующий курс.\n";
        }
        else if (knowledge >= 40) {
            cout << "УДОВЛЕТВОРИТЕЛЬНО. Вы сдали сессию, но есть пересдачи. Нужно подтянуть знания.\n";
        }
        else {
            cout << "НЕУДОВЛЕТВОРИТЕЛЬНО. Вы не смогли сдать сессию и отчислены.\n";
            gameOver = true;
        }
    }

    int getDay() const { return day; }
};

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void showMenu() {
    cout << "\n1. Учиться\n";
    cout << "2. Работать\n";
    cout << "3. Развлечься\n";
    cout << "4. Спать\n";
    cout << "5. Купить еду\n";
    cout << "6. Поесть\n";
    cout << "7. Купить кофе\n";
    cout << "8. Выпить кофе\n";
    cout << "9. Проверить статус\n";
    cout << "0. Выйти из игры\n";
    cout << "Выберите действие: ";
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(timeв(nullptr));
    clearScreen();

    cout << "=== СТУДЕНТ-СИМУЛЯТОР ===\n";
    cout << "Ваша цель - прожить 30 дней и успешно сдать сессию.\n";
    cout << "Следите за энергией, знаниями, счастьем и деньгами.\n";
    cout << "Важные правила:\n";
    cout << "- Нельзя пропускать учебу более 5 дней\n";
    cout << "- Нужно готовиться к контрольным (каждые 5 дней)\n";
    cout << "- К сессии нужно иметь минимум 40 знаний\n\n";

    string name;
    cout << "Введите имя вашего студента: ";
    getline(cin, name);

    Student student(name);
    int choice;

    do {
        cout << "\n=== День " << student.getDay() << " ===\n";
        showMenu();

        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод. Попробуйте снова: ";
        }

        clearScreen();

        switch (choice) {
        case 1: student.study(); break;
        case 2: student.work(); break;
        case 3: student.relax(); break;
        case 4: student.sleep(); break;
        case 5: student.buyFood(); break;
        case 6: student.eat(); break;
        case 7: student.buyCoffee(); break;
        case 8: student.drinkCoffee(); break;
        case 9: student.checkStats(); break;
        case 0: cout << "Выход из игры...\n"; return 0;
        default: cout << "Неверный выбор!\n";
        }

        if (student.isGameOver()) {
            student.checkExamResults();
            cout << "\n=== ИГРА ОКОНЧЕНА ===\n";
            cout << "Нажмите Enter чтобы выйти...";
            cin.ignore();
            cin.get();
            return 0;
        }

    } while (true);

    return 0;
}