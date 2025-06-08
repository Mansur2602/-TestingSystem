#pragma once
#include "user.h"
#include "test.h"
#include <string>
using namespace std;
#include <iostream>
#include <vector>
#include <fstream>


struct TestResult {
    string testName;
    int score;
    int total;
};

class Guest : public User //Класс Гость наследуется от класса User
{
private:
    vector<TestResult> results; 
public:
    Guest() : User() {};
    Guest(string fio, string login, string password) : User(fio, login, password) {};



    void showResults() const
    {
        if (results.empty())
        {
            cout << "Нет результатов тестирования" << endl;
            return;
        }
        cout << "<---------------------------------------->" << endl;
        cout << "Результаты тестирования:" << endl;
        for (const auto& result : results)
        {
            cout << "Тест: " << result.testName << ", Баллы: " << result.score << "/" << result.total << endl;
        }
    }

    void saveResultsToFile(const string& file) const
    {
    ofstream outfile(file, ios::app);
    if (!outfile.is_open())
    {
        cout << "Ошибка открытия файла!" << endl;
        return;
    }
    for (const auto& result : results)
    {
        outfile << Login << ";" << result.testName << ";" << result.score << "/" << result.total << endl;
    }
    outfile.close();
    }

    int StartTest(const string& file, const string& testName)
{
    // cout << "Введите название теста: ";
    // string testName;
    // getline(cin, testName);

    ifstream infile(file);
    if (!infile.is_open())
    {
        cout << "Ошибка открытия файла тестов!" << endl;
        return 0;
    }

    string line;
    int score = 0, total = 0;
    // Поиск теста
    while (getline(infile, line))
    {
        if (line.find("TEST:") == 0 && line.substr(5) == testName)
        {
            getline(infile, line);
            // Чтение вопросов и ответов
            while (getline(infile, line) && line != "ENDTEST")
            {
                if (line.find("Q:") == 0)
                {
                    cout << line.substr(2) << endl;
                    vector<string> answers;
                    vector<int> corrects;
                    int num = 1;
                    // Читаем варианты ответов
                    while (getline(infile, line) && line.find("A:") == 0)
                    {
                        size_t sep = line.find(';');
                        string answerText = line.substr(2, sep - 2); // Извлекаем текст ответа, начиная с 2-го символа отнимвем 2 символа, чтобы убрать "A:"
                        int isCorrect = (line[sep + 1] == '1'); // Проверяем, правильный ли ответ
                        cout << num << ") " << answerText << endl;
                        answers.push_back(answerText);
                        corrects.push_back(isCorrect);
                        num++;
                    }
                    cout << "Ваш ответ (номер): ";
                    int userAnswer;
                    cin >> userAnswer;
                    cin.ignore();
                    if (userAnswer >= 1 && userAnswer <= (int)corrects.size() && corrects[userAnswer - 1])
                    {
                        cout << "Правильно!" << endl;
                        score++;
                    }
                    else
                    {
                        cout << "Неправильно" << endl;
                    }
                    total++;
                    if (line == "ENDTEST") break;
                }
            }
            break; // тест найден и пройден, выходим
        }
    }
    infile.close();
    cout << "Ваш результат: " << score << "/" << total << endl;
    TestResult result;
    result.testName = testName;
    result.score = score;
    result.total = total;
    results.push_back(result);
    saveResultsToFile(file);
    return score;
}

void ShowAllResults(const string& file)
    {
        ifstream infile(file);
        string line;
        bool found = false;
        cout << "Список всех результатов тестирования: " << endl;
        while(getline(infile, line))
        {
            size_t pos1 = line.find(';');
            size_t pos2 = line.find(';', pos1 + 1);
            string fileLogin = line.substr(0, pos1);
            string testName = line.substr(pos1 + 1,pos2 - pos1 - 1 );
            string result = line.substr(pos2 + 1);
            if (Login == fileLogin )
            {
                cout << "Тест: " << testName << ", Результат: " << result;
                found = true;
            }
        }
        if (!found)
        {
            cout << "У вас пока нет результатов" << endl;
        }
        infile.close();
    }
   

    void Menu() const
    {
        cout << "<---------------------------------------->" << endl;
        cout << "1. Пройти тестирование" << endl;
        cout << "2. Просмотреть результаты" << endl;
        cout << "3. Изменить данные" << endl;
        cout << "4. Удалить аккаунт" << endl;
        cout << "5. Выйти" << endl;
        cout << "Выберите действие: ";
    }

    ~Guest() {};
};
