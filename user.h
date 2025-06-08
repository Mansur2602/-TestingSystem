#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <iomanip>
#include <sstream>
using namespace std;

class User //Класс User от которого будут наследоваться гость и админ
{
protected:
   string FIO;
   string Login; 
   string Password;
public:
    User() : FIO(), Login(), Password() {}
    User(string fio, string login, string password) : FIO(fio), Login(login), Password(password) {}
    
    string getFio() const {return FIO;}
    string getLogin() const {return Login;}
    string getPassword() const {return Password;}


static void CreateUser(const string& fio, const string& login, const string& password, string& file)
    {
        // Создание нового пользователя и запись в файл
        string salt = generateSalt(12);
        string hash = Hash(password, salt);
        ofstream outfile(file, ios::app);
        if (!outfile.is_open())
        {
            cout << "Не удалось открыть файл для записи" << endl;
            
        }
       outfile << fio << ";" << login << ";" << hash << ";" << salt << endl; // Запись ФИО, логина и пароля в файл
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

        string inputHash = Hash(password, fileSalt);
        // substr принимает первым аргументом позицию, с которой начинаем извлечение, а вторым аргументом количество символов, которые нужно извлечь
        if (fileLogin == login && fileHash == inputHash) 
        {
            foundUser = User(fio, fileLogin, fileHash);
            return &foundUser;
        }
    }
    return nullptr; // Возвращаем nullptr, если пользователь не найден
}

    static bool isPasswordValid(const string& password)
    {
        if (password.length() < 6)
        {
            return false; // Пароль слишком короткий
        }
        else {
            return true; // Пароль корректен
        }
    }


    static bool isLoginTaken(const string& login, const string& file)
    {
        // Проверка занятости логина в файле
        ifstream infile(file);
        string line;
        while (getline(infile, line))
        {
             size_t pos1 = line.find(';');
            size_t pos2 = line.find(';', pos1 + 1);
            string fileLogin = line.substr(pos1 + 1, pos2 - pos1 - 1);
        if (fileLogin == login)
        {
            return true; // Логин занят
        }
    
        }
        return false; // Логин свободен
    }
    
    // Метод для проверки корректности пароля
    static bool isPasswordValid(const string& password, string& file)
    {
        // Проверка корректности пароля в файле
        ifstream infile(file);
        string line;
        while (getline(infile, line))
        {
            size_t pos1 = line.find(';');
            size_t pos2 = line.find(';', pos1 + 1);
            string filePassword = line.substr(pos2 + 1);
            if (filePassword == password)
            {
                return true; // Пароль найден
            }
        
        return false; // Пароль некорректен
    }
}


   void ChangeData(const string& file )
{
    int choose;
    cout << "Что вы хотите изменить?" << endl;
    cout << "1. ФИО" << endl;
    cout << "2. Логин" << endl;
    cout << "3. Пароль" << endl;
    cout << "4. Вернуться в меню" << endl;
    cout << "Выберите действие: ";
    cin >> choose;
    cin.ignore();
    switch(choose)
    {
        case 1: // Изменение ФИО
        {
            string newFio;
            cout << "Введите новое ФИО: ";
            getline(cin, newFio);
            FIO = newFio;
            ifstream infile(file);
            vector<string> lines;
            string line;
            while (getline(infile, line)) 
            {
                size_t pos1 = line.find(';');
                size_t pos2 = line.find(';', pos1 + 1);
                string fileLogin = line.substr(pos1 + 1, pos2 - pos1 - 1);
                    if (fileLogin == Login) {
                        line = newFio + ";" + Login + ";" + Password; // Обновляем строку с новым ФИО
                    }
                lines.push_back(line);
            }
            infile.close();
            ofstream outfile(file);
            if (outfile.is_open())
            {
                for (const auto& l : lines)
                {
                    outfile << l << endl; // Записываем обновлённые данные в файл
                }
                outfile.close();
            }
            else
            {
                throw runtime_error("Не удалось открыть файл для записи");
            }
            cout << "ФИО успешно изменено!" << endl;
            break;
        }
        case 2: //Изменение логина
        {
            string newLogin;
            cout << "Введите новый логин: ";
            getline(cin, newLogin);
            if (isLoginTaken(newLogin, file))
            {
                cout << "Этот логин уже занят" << endl;
            }
            else
            {
                Login = newLogin;
                ifstream infile(file);
                vector<string> lines;
                string line;
                while (getline(infile, line)) 
                {
                    size_t pos1 = line.find(';');
                    size_t pos2 = line.find(';', pos1 + 1);
                    string fileLogin = line.substr(pos1 + 1, pos2 - pos1 - 1);
                        if (fileLogin == Login) {
                            line = FIO + ";" + newLogin + ";" + Password; // Обновляем строку с новым логином
                        }
                    
                    lines.push_back(line);
                }
                infile.close();
                ofstream outfile(file);
                if (outfile.is_open())
                {
                    for (const auto& l : lines)
                    {
                        outfile << l << endl; // Записываем обновлённые данные в файл
                    }
                    outfile.close();
                }
                else
                {
                    throw runtime_error("Не удалось открыть файл для записи");
                }
                cout << "Логин успешно изменён!" << endl;
            }
            break;
        }
        case 3: // Изменение пароля
        {
            string newPassword;
            cout << "Введите новый пароль: ";
            getline(cin, newPassword);
            if (isPasswordValid(newPassword))
            {
                Password = newPassword;
                ifstream infile(file);
                vector<string> lines;
                string line;
                while (getline(infile, line)) 
                {
                    size_t pos1 = line.find(';'); // find принимает первым аргументом символ, который ищем, а вторым аргументом позицию, с которой начинаем поиск
                    size_t pos2 = line.find(';', pos1 + 1); 
                    string fileLogin = line.substr(pos1 + 1, pos2 - pos1 - 1); // Извлекаем логин между первым и вторым ';'
                        if (fileLogin == Login)
                         {
                            line = FIO + ";" + Login + ";" + newPassword; // Обновляем строку с новым паролем
                        }
                    
                    lines.push_back(line); 
                }
                infile.close();  
                ofstream outfile(file); // открывает новый файл для записи потому что необходимо перезаписать старый файл
                if (outfile.is_open())
                {
                    for (const auto& l : lines)
                    {
                        outfile << l << endl; // Записываем обновлённые данные в файл
                    }
                    outfile.close();
                }
                else
                {
                    throw runtime_error("Не удалось открыть файл для записи");
                } 
                cout << "Пароль успешно изменён!" << endl;
            }
            else
            {
                cout << "Пароль некорректен. Попробуйте ещё раз." << endl;
            }
            break;
        }
        case 4: // Вернуться в меню
        {
            cout << "Возвращение в меню..." << endl;
            break;
        }
        default:
        {
            cout << "Неверный выбор, попробуйте снова." << endl;
            break;
        }
        
    }
}


    void  DeleteAccount(const string& file)
    {
        int choose;
        cout << "Вы уверены, что хотите удалить аккаунт?" << endl;
        cout << "1. Да" << endl;
        cout << "2. Нет" << endl;
        cout << "Выберите действие: ";
        cin >> choose;
        cin.ignore();
        if (choose == 1)
        {
            ifstream infile(file);
            vector<string> lines;
            string line;
            while(getline(infile, line))
            {
                size_t pos1 = line.find(';');
                size_t pos2 = line.find(';', pos1 + 1);
                string fileLogin = line.substr(pos1 + 1, pos2 - pos1 - 1);
                if (fileLogin != Login) // делаем проверку на то, что это не наш логин 
                {
                    lines.push_back(line); // пихаем все что нужно кроме нашего логина который нужно удалить
                }
                infile.close();

                ofstream outfile(file);
                if (!outfile.is_open())
                {
                    cout <<"Не удалось открыть файл для записи" << endl;
                }
                for (const auto& l : lines)
                    {
                        outfile << l << endl; // Записываем оставшиеся данные в файл
                    }
                
                cout << "Аккаунт успешно удалён!" << endl;
                break; 

                
            }
        }
    }


   static string generateSalt(size_t size)
     {
     const char chars[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, sizeof(chars) - 2);

    string salt;
    for (size_t i = 0; i < size; ++i)
        salt += chars[dist(gen)];
    return salt;
    }

  static string Hash(const string& value, string salt)
    {
    unsigned int hash = 5381;
    for (char c : value + salt) 
    {
        hash = hash * 33 + c; 
    }
    stringstream ss;
    ss << hex << hash;
    return ss.str();
    }

    virtual ~User() {} 

    
};

