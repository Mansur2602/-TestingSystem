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


    static void CreateAdmin(const string& fio, const string& login, const string& password, string& file)
    {
        string salt = generateSalt(12);
        string LoginHash = Hash(login, salt);
        string PasswordHash = Hash(password, salt);
        ofstream outfile(file, ios::app);
        if (!outfile.is_open())
        {
            cout << "Не удалось открыть файл для записи" << endl;
            
        }
       outfile << fio << ";" << LoginHash << ";" << PasswordHash << ";" << salt << endl; // Запись ФИО, логина и пароля в файл
        outfile.close();
    }

    static User* LoginToAccount(const string& login, const string& password, const string& file)
    {
         static User foundUser; // Статический объект для хранения найденного пользователя
        ifstream infile(file);
        string line;
        while (getline(infile, line)) 
        {
        if (line.empty()) continue; // Пропускаем пустые строки;
        size_t pos1 = line.find(';');  // find приниммает первым аргументом символ, который ищем, а вторым аргументом позицию, с которой начинаем поиск
        size_t pos2 = line.find(';', pos1 + 1); 
        size_t pos3 = line.find(';', pos2 + 1);

        string fio = line.substr(0, pos1); // Извлекаем ФИО до первого ';' 
        string fileLogin  = line.substr(pos1 + 1, pos2 - pos1 - 1); // Извлекаем логин между первым и вторым ';'
        string filePassword = line.substr(pos2 + 1); // Извлекаем пароль после второго ';'
        string fileHash = line.substr(pos2 + 1, pos3 - pos2 - 1);
        string fileSalt = line.substr(pos3 + 1);

        string LoginHash = Hash(login, fileSalt);
        string PasswordHash = Hash(password, fileSalt);
        // substr принимает первым аргументом позицию, с которой начинаем извлечение, а вторым аргументом количество символов, которые нужно извлечь
        if (fileLogin == LoginHash && fileHash == PasswordHash) 
        {
            foundUser = User(fio, fileLogin, fileHash);
            return &foundUser;
        }
    }
    return nullptr;
    } 

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

    void ShowOneUser(const string& Userfile, const string& ResultsFile, const string& login)
{
    ifstream infile(Userfile);
    if (!infile.is_open())
    {
        cout << "Ошибка открытия файла пользователей!" << endl;
        return;
    }
    string line;
    bool found = false;
    string fio;
    while (getline(infile, line))
    {
        if (line.empty()) continue;
        size_t pos1 = line.find(';');
        size_t pos2 = line.find(';', pos1 + 1);
        fio = line.substr(0, pos1);
        string fileLogin = line.substr(pos1 + 1, pos2 - pos1 - 1);
        if (login == fileLogin)
        {
            cout << "ФИО: " << fio << ", Логин: " << login << endl;
            found = true;
            break;
        }
    }
    infile.close();

    if (!found)
    {
        cout << "Пользователь не найден!" << endl;
        return;
    }

    // Теперь выводим результаты пользователя
    ifstream resfile(ResultsFile);
    if (!resfile.is_open())
    {
        cout << "Ошибка открытия файла результатов!" << endl;
        return;
    }
    bool hasResults = false;
    while (getline(resfile, line))
    {
        if (line.empty()) continue;
        size_t pos1 = line.find(';');
        size_t pos2 = line.find(';', pos1 + 1);
        if (pos1 == string::npos || pos2 == string::npos) continue;
        string fileLogin = line.substr(0, pos1);
        string testName = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string result = line.substr(pos2 + 1);
        if (login == fileLogin)
        {
            cout << "Тест: " << testName << ", Результат: " << result << endl;
            hasResults = true;
        }
    }
    if (!hasResults)
    {
        cout << "Нет результатов тестирования для этого пользователя." << endl;
    }
    resfile.close();
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

    void Statistics(const string& resultsFile, const string& usersFile, const string& testsFile)
    {
         int userCount = 0, testCount = 0, attemptCount = 0;
        string line;
        ifstream ufile(usersFile);
        while (getline(ufile, line))
        if (!line.empty()) userCount++;
        ufile.close();

        // Считаем тесты
        ifstream tfile(testsFile);
        while (getline(tfile, line))
            if (line.find("TEST:") == 0) testCount++;
        tfile.close();

        ifstream rfile(resultsFile);
        while (getline(rfile, line))
        if (!line.empty()) attemptCount++;
        rfile.close();

        cout << "Всего пользователей: " << userCount << endl;
        cout << "Всего тестов: " << testCount << endl;
        cout << "Всего попыток прохождения тестов: " << attemptCount << endl;
    }
    

    // void ChangeTest(const string& file) const
    // {
    //     int choose;
    //     cout << "Что вы хотите изменить?" << endl;
    //     cout << "1. Название теста" << endl;
    //     cout << "2. Название раздела теста" << endl;
    //     cout << "3. Вопросы теста (не реализовано)" << endl;
    //     cout << "4. Ответы теста (не реализовано)" << endl;
    //     cout << "5. Вернуться в меню" << endl;
    //     cout << "Выберите действие: ";
    //     cin >> choose;
    //     cin.ignore();

    //     switch (choose)
    //     {
    //     case 1:
    //     {
    //         string oldName, newName;
    //         cout << "Введите старое название теста: ";
    //         getline(cin, oldName);
    //         cout << "Введите новое название теста: ";
    //         getline(cin, newName);

    //         ifstream infile(file);
    //         if (!infile.is_open())
    //         {
    //             cout << "Ошибка открытия файла!" << endl;
    //             return;
    //         }

    //         vector<string> lines;
    //         string line;
    //         while (getline(infile, line))
    //         {
    //             if (line.find("TEST:") == 0 && line.substr(5) == oldName)
    //             {
    //                 line = "TEST:" + newName;
    //             }
    //             lines.push_back(line);
    //         }
    //         infile.close();

    //         ofstream outfile(file);
    //         for (const auto& l : lines)
    //             outfile << l << endl;
    //         outfile.close();

    //         cout << "Название теста успешно изменено!" << endl;
    //         break;
    //     }

    //     case 2:
    //     {
    //         string testName, newSection;
    //         cout << "Введите название теста: ";
    //         getline(cin, testName);
    //         cout << "Введите новое название раздела: ";
    //         getline(cin, newSection);

    //         ifstream infile(file);
    //         if (!infile.is_open())
    //         {
    //             cout << "Ошибка открытия файла!" << endl;
    //             return;
    //         }

    //         vector<string> lines;
    //         string line;
    //         while (getline(infile, line))
    //         {
    //             if (line.find("TEST:") == 0 && line.substr(5) == testName)
    //             {
    //                 lines.push_back(line);
    //                 getline(infile, line); // SECTION
    //                 line = "SECTION:" + newSection;
    //             }
    //             lines.push_back(line);
    //         }
    //         infile.close();

    //         ofstream outfile(file);
    //         for (const auto& l : lines)
    //             outfile << l << endl;
    //         outfile.close();

    //         cout << "Раздел теста успешно изменён!" << endl;
    //         break;
    //     }

    //     case 3:
    //     case 4:
    //         cout << "Функция пока не реализована." << endl;
    //         break;

    //     case 5:
    //         cout << "Возврат в меню..." << endl;
    //         break;

    //     default:
    //         cout << "Неверный выбор." << endl;
    //         break;
    //     }
    // }

    void Menu() const
    {
        cout << "<---------------------------------------->" << endl;
        cout << "1. Просмотреть всех пользователей" << endl;
        cout << "2. Удалить пользователя" << endl;
        cout << "3. Найти пользователя по логину" << endl;
        cout << "4. Добавить тест" << endl;
        cout << "5. Просмотреть тесты" << endl;
        cout << "6. Удалить тест" << endl;
        cout << "7. Просмотреть статистику" << endl;
        cout << "8. Выйти" << endl;
        cout << "Выберите действие: ";
    }
};
