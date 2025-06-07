#pragma once
#include "user.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Admin : public User
{
public:
    Admin() : User() {}
    Admin(const string& fio, const string& login, const string& password) : User(fio, login, password) {}
    ~Admin() {}

    void showAllUsers(const string& file)
    {
        ifstream infile(file);
        string line;
        cout << "<---------------------------------------->" << endl;
        cout << "Список всех пользователей:" << endl;
        while (getline(infile, line))
        {
            if (line.empty()) continue;
            size_t pos1 = line.find(';');
            size_t pos2 = line.find(';', pos1 + 1);
            string fio = line.substr(0, pos1);
            string login = line.substr(pos1 + 1, pos2 - pos1 - 1);
            cout << "ФИО: " << fio << ", Логин: " << login << endl;
        }
    }

    void DeleteUser(const string& login, const string& file) const
    {
        int choose;
        cout << "Вы уверены, что хотите удалить пользователя с логином " << login << "?" << endl;
        cout << "1. Да" << endl;
        cout << "2. Нет" << endl;
        cout << "Выберите действие: ";
        cin >> choose;
        cin.ignore();

        switch (choose)
        {
        case 1:
        {
            ifstream infile(file);
            if (!infile.is_open())
            {
                cout << "Ошибка открытия файла для чтения!" << endl;
                return;
            }

            vector<string> lines;
            string line;
            while (getline(infile, line))
            {
                if (line.empty()) continue;
                size_t pos1 = line.find(';');
                size_t pos2 = line.find(';', pos1 + 1);
                string fileLogin = line.substr(pos1 + 1, pos2 - pos1 - 1);
                if (fileLogin != login)
                {
                    lines.push_back(line);
                }
            }
            infile.close();

            ofstream outfile(file);
            if (outfile.is_open())
            {
                for (const auto& l : lines)
                    outfile << l << endl;
                outfile.close();
                cout << "Пользователь с логином " << login << " успешно удалён." << endl;
            }
            else
            {
                cout << "Ошибка открытия файла для записи!" << endl;
            }
            break;
        }
        case 2:
            cout << "Удаление пользователя отменено." << endl;
            break;
        default:
            cout << "Неверный выбор, попробуйте снова." << endl;
            break;
        }
    }

    void AddTest(const string& file) const
    {
        string testName, testSection;
        cout << "Введите название теста: ";
        getline(cin, testName);
        cout << "Введите раздел теста: ";
        getline(cin, testSection);

        ofstream outfile(file, ios::app);
        if (!outfile.is_open())
        {
            cout << "Ошибка открытия файла для записи!" << endl;
            return;
        }

        outfile << "TEST:" << testName << endl;
        outfile << "SECTION:" << testSection << endl;

        cout << "Введите количество вопросов в тесте: ";
        int questionCount;
        cin >> questionCount;
        cin.ignore();

        for (int q = 0; q < questionCount; ++q)
        {
            string questionText;
            cout << "Введите текст вопроса " << q + 1 << ": ";
            getline(cin, questionText);
            outfile << "Q:" << questionText << endl;

            int answerCount;
            while (true)
            {
                cout << "Введите количество вариантов ответа (2-6): ";
                cin >> answerCount;
                cin.ignore();
                if (answerCount >= 2 && answerCount <= 6)
                    break;
                cout << "Ошибка: допустимо от 2 до 6!" << endl;
            }

            for (int a = 0; a < answerCount; ++a)
            {
                string answerText;
                cout << "Ответ " << a + 1 << ": ";
                getline(cin, answerText);
                int isCorrect;
                cout << "Это правильный ответ? (1 - да, 0 - нет): ";
                cin >> isCorrect;
                cin.ignore();
                outfile << "A:" << answerText << ";" << (isCorrect ? "1" : "0") << endl;
            }
        }

        outfile << "ENDTEST" << endl;
        outfile.close();
        cout << "Тест успешно добавлен!" << endl;
    }

    void DeleteTest(const string& file) const
    {
        cout << "Введите название теста для удаления: ";
        string testName;
        getline(cin, testName);

        if (testName.empty())
        {
            cout << "Название теста не может быть пустым." << endl;
            return;
        }

        ifstream infile(file);
        if (!infile.is_open())
        {
            cout << "Ошибка открытия файла!" << endl;
            return;
        }

        vector<string> lines;
        string line;
        while (getline(infile, line))
        {
            if (line.find("TEST:") == 0 && line.substr(5) == testName)
            {
                while (getline(infile, line) && line != "ENDTEST") {}
                continue;
            }
            lines.push_back(line);
        }
        infile.close();

        ofstream outfile(file, ios::trunc);
        if (!outfile.is_open())
        {
            cout << "Ошибка записи файла!" << endl;
            return;
        }

        for (const auto& l : lines)
            outfile << l << endl;

        outfile.close();
        cout << "Тест \"" << testName << "\" успешно удалён!" << endl;
    }

    void ChangeTest(const string& file) const
    {
        int choose;
        cout << "Что вы хотите изменить?" << endl;
        cout << "1. Название теста" << endl;
        cout << "2. Название раздела теста" << endl;
        cout << "3. Вопросы теста (не реализовано)" << endl;
        cout << "4. Ответы теста (не реализовано)" << endl;
        cout << "5. Вернуться в меню" << endl;
        cout << "Выберите действие: ";
        cin >> choose;
        cin.ignore();

        switch (choose)
        {
        case 1:
        {
            string oldName, newName;
            cout << "Введите старое название теста: ";
            getline(cin, oldName);
            cout << "Введите новое название теста: ";
            getline(cin, newName);

            ifstream infile(file);
            if (!infile.is_open())
            {
                cout << "Ошибка открытия файла!" << endl;
                return;
            }

            vector<string> lines;
            string line;
            while (getline(infile, line))
            {
                if (line.find("TEST:") == 0 && line.substr(5) == oldName)
                {
                    line = "TEST:" + newName;
                }
                lines.push_back(line);
            }
            infile.close();

            ofstream outfile(file);
            for (const auto& l : lines)
                outfile << l << endl;
            outfile.close();

            cout << "Название теста успешно изменено!" << endl;
            break;
        }

        case 2:
        {
            string testName, newSection;
            cout << "Введите название теста: ";
            getline(cin, testName);
            cout << "Введите новое название раздела: ";
            getline(cin, newSection);

            ifstream infile(file);
            if (!infile.is_open())
            {
                cout << "Ошибка открытия файла!" << endl;
                return;
            }

            vector<string> lines;
            string line;
            while (getline(infile, line))
            {
                if (line.find("TEST:") == 0 && line.substr(5) == testName)
                {
                    lines.push_back(line);
                    getline(infile, line); // SECTION
                    line = "SECTION:" + newSection;
                }
                lines.push_back(line);
            }
            infile.close();

            ofstream outfile(file);
            for (const auto& l : lines)
                outfile << l << endl;
            outfile.close();

            cout << "Раздел теста успешно изменён!" << endl;
            break;
        }

        case 3:
        case 4:
            cout << "Функция пока не реализована." << endl;
            break;

        case 5:
            cout << "Возврат в меню..." << endl;
            break;

        default:
            cout << "Неверный выбор." << endl;
            break;
        }
    }

    void Menu() const
    {
        cout << "<---------------------------------------->" << endl;
        cout << "1. Просмотреть всех пользователей" << endl;
        cout << "2. Удалить пользователя" << endl;
        cout << "3. Найти пользователя по логину" << endl;
        cout << "4. Добавить тест" << endl;
        cout << "5. Просмотреть тесты" << endl;
        cout << "6. Удалить тест" << endl;
        cout << "7. Изменить тест" << endl;
        cout << "8. Выйти" << endl;
        cout << "Выберите действие: ";
    }
};
