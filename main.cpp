#include "guest.h"
#include "admin.h"
#include "test.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;



int main()
{
    string usersFile = "users.txt";
    ofstream ofile(usersFile, ios::app);
    
    if (!ofile.is_open()) {
        cout << "Ошибка создания файла пользователей!" << endl;
        return 1;
    }
    ofile.close();

    string adminsFile = "admins.txt";
    ofile.open(adminsFile, ios::app);
    if (!ofile.is_open()) {
        cout << "Ошибка создания файла администраторов!" << endl;
        return 1;
    }
    ofile.close();

    string testsFile = "tests.txt";
    ofile.open(testsFile, ios::app);
    if (!ofile.is_open()) {
        cout << "Ошибка создания файла тестов!" << endl;
        return 1;
    }
    ofile.close();

    string resultsFile = "results.txt";
    ofile.open(resultsFile, ios::app);
    if (!ofile.is_open()) {
        cout << "Ошибка создания файла результатов!" << endl;
        return 1;
    }
    ofile.close();

    User* currentUser = nullptr;
    bool isGuest = false;
    bool isAdmin = false;




    ifstream adminCheck(adminsFile);
    bool adminExists = false;
    string line;
    while (getline(adminCheck, line))
    {
        if (!line.empty()) {
        adminExists = true;
        break;
    }
    }
adminCheck.close();

if (!adminExists) {
    cout << "Первый запуск: создайте администратора системы." << endl;
    string fio, login, password;
    cout << "Введите ФИО администратора: ";
    getline(cin, fio);
    cout << "Введите логин администратора: ";
    getline(cin, login);
    while (true) {
        cout << "Введите пароль администратора: ";
        getline(cin, password);
        if (!User::isPasswordValid(password)) {
            cout << "Пароль слишком короткий, должен быть больше 6 символов" << endl;
        } else break;
    }
    Admin::CreateAdmin(fio, login, password, adminsFile);
    cout << "Администратор успешно создан!" << endl;
}




    cout << "Добро пожаловать в систему тестирования!" << endl;
    while (true)
    {
        int choose;
        cout << "<---------------------------------------->" << endl;
        cout << "1. Войти как гость" << endl;
        cout << "2. Войти как администратор" << endl;
        cout << "3. Зарегистрироваться" << endl;
        cout << "4. Выйти" << endl;
        cout << "Выберите действие: ";
        cin >> choose;
        cin.ignore();
        if (cin.fail()) {
        cin.clear(); // сбрасываем флаг ошибки
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ошибка ввода! Введите число!" << endl;
        continue; 
        }       
        
        switch (choose)
        {
        case 1: // Вход как гость
    {   
        string login, password;
        cout << "Введите логин: ";
        getline(cin, login);
        cout << "Введите пароль: ";
        getline(cin, password);

        User* user = User::LoginToAccount(login, password, usersFile);
        if (user)
        {
            currentUser = new Guest(user->getFio(), user->getLogin(), user->getPassword());
            isGuest = true;
        }
        else
        {
            cout << "Неверный логин или пароль!" << endl;
        }
        break;
    }
        case 2: // Вход как администратор
    {
        string login, password;
        cout << "Введите логин администратора: ";
        getline(cin, login);
        cout << "Введите пароль администратора: ";
        getline(cin, password);

        User* user = Admin::LoginToAccount(login, password, "admins.txt");
        if (user)
        {
            currentUser = new Admin(user->getFio(), user->getLogin(), user->getPassword());
            isAdmin = true;
        }
        else
        {
            cout << "Неверный логин или пароль!" << endl;
        }
    break;
    }
        case 3: // Регистрация
        {
            string fio, login, password;
            cout << "Введите ФИО: ";
            getline(cin, fio);
            while (true)
            {
                cout << "Введите логин: ";
                getline(cin, login);
                if (User::isLoginTaken(login, usersFile ) || User::isLoginTaken(login, adminsFile))
                {
                    cout << "Этот логин уже занят" << endl;
                }
                else { break; }
            }

            while (true)
            {
                cout << "Введите пароль: ";
                getline(cin, password);
                if (!User::isPasswordValid(password))
                {
                    cout << "Пароль слишком короткий, должен быть больше 6 символов" << endl;
                }
                else  break; 
            }
         
            ofile.open(usersFile, ios::app);
            if (!ofile.is_open())
             {
                cout << "Ошибка открытия файла для записи!" << endl;
                return 1;
            }
            User::CreateUser(fio, login, password, usersFile);
            ofile.close();
            cout << "Регистрация прошла успешно!" << endl;
            cout << endl;
            // После регистрации сразу авторизуем пользователя как гостя
            currentUser = new Guest(fio, login, password);
            isGuest = true;
            break;
        }
        case 4: // Выход
        {
            cout << "Выход из системы..." << endl;
            return 0; 
        }
        default:
            cout << "Неверный выбор, попробуйте снова." << endl;
            break;
        }

        // Если пользователь вошёл или зарегистрировался — выходим из цикла
        if (currentUser) break;
    }

    // Работаем с пользователем после выхода из цикла
    if (isGuest && currentUser)
     {
        Guest guest(currentUser->getFio(), currentUser->getLogin(), currentUser->getPassword());
        cout << "Добро пожаловать, " << guest.getFio() << endl;
        while (true)
        {
        int choose;
        guest.Menu();
        cin >> choose;
        cin.ignore();
        if (cin.fail()) 
        {
        cin.clear(); // сбрасываем флаг ошибки
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ошибка ввода! Введите число!" << endl;
        continue; 
        }    
        switch(choose)
        {
           case 1: // Пройти тестирование
{
    string sectionName;
    while (true)
    {
        // Показываем только уникальные разделы
        ifstream infile(testsFile);
        if (!infile.is_open())
        {
            cout << "Ошибка открытия файла тестов!" << endl;
            break;
        }
        string line;
        vector<string> shownSections;
        cout << "<---------------------------------------->" << endl;
        cout << "Список разделов тестов:" << endl;
        while (getline(infile, line))
        {
            if (line.find("SECTION:") == 0)
            {
                string sec = line.substr(8);
                bool already = false;
                for (size_t i = 0; i < shownSections.size(); ++i)
                    if (shownSections[i] == sec) { already = true; break; }
                if (!already)
                {
                    cout << "Раздел: " << sec << endl;
                    shownSections.push_back(sec);
                }
            }
        }
        infile.close();

        cout << "Введите название раздела тестов: ";
        getline(cin, sectionName);

        // Проверяем, существует ли раздел
        bool sectionFound = false;
        for (size_t i = 0; i < shownSections.size(); ++i)
        {
            if (shownSections[i] == sectionName)
            {
                sectionFound = true;
                break;
            }
        }
        if (sectionFound) break;
        cout << "Такого раздела не существует. Попробуйте снова." << endl;
    }

    // 2. Выбор теста с защитой от ошибочного ввода
    string testName;
    while (true)
    {
        // Показываем тесты только выбранного раздела
        ifstream infile(testsFile);
        if (!infile.is_open())
        {
            cout << "Ошибка открытия файла тестов!" << endl;
            break;
        }
        string line, lastTestName;
        vector<string> testsInSection;
        cout << "<---------------------------------------->" << endl;
        cout << "Тесты в разделе " << sectionName << ":" << endl;
        while (getline(infile, line))
        {
            if (line.empty()) continue;
            if (line.find("TEST:") == 0)
            {
                lastTestName = line.substr(5);
            }
            if (line.find("SECTION:") == 0)
            {
                string currentSection = line.substr(8);
                if (currentSection == sectionName && !lastTestName.empty())
                {
                    cout << "Тест: " << lastTestName << endl;
                    testsInSection.push_back(lastTestName);
                }
            }
            if (line == "ENDTEST")
            {
                lastTestName.clear();
            }
        }
        infile.close();

        cout << "Введите название теста: ";
        getline(cin, testName);

        // Проверяем, существует ли тест в этом разделе
        bool testFound = false;
        for (size_t i = 0; i < testsInSection.size(); ++i)
        {
            if (testsInSection[i] == testName)
            {
                testFound = true;
                break;
            }
        }
        if (testFound) break;
        cout << "Такого теста в этом разделе нет. Попробуйте снова." << endl;
    }

    guest.StartTest(testsFile, testName);
    guest.saveResultsToFile(resultsFile);
    break;
    }
            case 2: // просмотреть результатыв
            {
                guest.ShowAllResults("results.txt");
                break;
            }
            case 3: // Изменить данные
            {
                guest.ChangeData(usersFile);
                break;
            }
            case 4: //Удалить аккаунт
            {
                guest.DeleteAccount(usersFile);
                delete currentUser; 
                currentUser = nullptr; 
                isGuest = false; 
                cout << "Аккаунт успешно удалён!" << endl;
                return 0; 
            }
            case 5: // Выйти
            {
                cout << "Выход из системы..." << endl;
                return 0; 
            }
            default:
                cout << "Неверный выбор, попробуйте снова." << endl;
                break;

            
    }
    }
    }
    if (isAdmin && currentUser) 
    {
        Admin admin(currentUser->getFio(), currentUser->getLogin(), currentUser->getPassword());
        cout << "Добро пожаловать, администратор " << admin.getFio() << endl;
        while (true)
        {

        admin.Menu();
        int choose;
        cin >> choose;
        cin.ignore();
        if (cin.fail()) {
        cin.clear(); // сбрасываем флаг ошибки
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Ошибка ввода! Введите число!" << endl;
        continue; 
        }    
        switch (choose)
        {
            case 1: //Просмотреть всех пользователей
            {
                admin.showAllUsers(usersFile);
                break;
            }
            case 2: // Удалить пользователя
            {
                string login;
                admin.showAllUsers(usersFile);
                cout << "Введите логин пользователя для удаления: ";
                getline(cin, login);
                admin.DeleteUser(login, usersFile);
                break;
            }
            case 3: // Найти пользователя по логину
            {
                cout << "Введите логин пользователя: ";
                string login;
                getline(cin, login);
                admin.ShowOneUser("users.txt", "results.txt", login );
                break;
            }
            case 4: // Добавить тест
            {
                admin.AddTest(testsFile);
                break;
            }
            case 5: // Просмотреть тесты
            {
                Test::ShowTests(testsFile);
                break;
            }
            case 6: // Удалить тест
            {
                Test::ShowTests(testsFile);
                admin.DeleteTest(testsFile);
                break;
            }
            case 7: //Просмотреть статистику
            {
                admin.Statistics(resultsFile, usersFile, testsFile);
                break;
            }
            case 8: // Выйти
            {   
                cout << "Выход из системы..." << endl;
                delete currentUser; 
                currentUser = nullptr; 
                isAdmin = false; 
                return 0; 
            }
            default:
            {
                cout << "Неверный выбор, попробуйте снова." << endl;
                break;
            }
        }
    }

    }
}