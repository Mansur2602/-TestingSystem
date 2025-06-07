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
        cin.ignore(); 
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

        User* user = User::LoginToAccount(login, password, "admins.txt");
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
                else { break; }
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
        cin.ignore(); 
        cout << "Ошибка ввода! Введите число!" << endl;
        continue; 
        }    
        switch(choose)
        {
            case 1: // Пройти тестирование
                {
                    Test::ShowTests(testsFile);
                    guest.StartTest(testsFile);
                    break;
                }
            case 2: // просмотреть результаты
            {
                cout << "Не реализовано" << endl;
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
        cin.ignore(); 
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
                cout << "Пока не реализовано. Хочу позже вывести этого пользователя и все его резуальтаты тестирования" << endl;
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
            case 7: // Изменить тест
            {
                cout << "Пока не реализовано" << endl;
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