#pragma once
#include "question.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

class Test
{
private:
    string name;
    string section; // Раздел теста
    vector<Question> questions; // Вектор для хранения вопросов теста

public:
    Test(string& testName, string& sectionName ) : name(testName), section(sectionName) {};

    string getName() const {return name;}
    string getSection() const {return section; }
    vector<Question> getQuestions() const {return questions; }
    void setName(const string& testName) {name = testName; }
    void setSection(const string& sectionName) {section = sectionName; }

    void addQuestion(const Question& question) 
    {
        questions.push_back(question); // Добавление вопроса в тест
    }

   static void ShowTests(const string& file) 
    {
        ifstream infile(file);
        if (!infile.is_open())
        {
            cout << "Ошибка открытия файла для чтения!" << endl;
            return;
        }
        string line;
        cout << "<---------------------------------------->" << endl;
        cout << "Список тестов:" << endl;
        while (getline(infile, line))
        {
        if (line.empty()) continue;
        if (line.find("TEST:") == 0)
        {
            string testName = line.substr(5); 
            cout << "Тест: " << testName << endl;
        }
    }}


    void SaveTest(const string& file) const
    {
        ofstream outfile(file, ios::app);
        if (!outfile.is_open())
        {
            cout << "Ошибка открытия файла для записи!" << endl;
            return;
        }
        outfile << "Тест: " << name << endl;
        for (const auto& question : questions)
        {
            outfile << "Вопрос: " << question.getQuestionText() << endl;
            for (const auto& answer : question.getAnswers())
            {
                outfile << "Ответ: " << answer.answer << endl;
                outfile << "Правильный: " << (answer.isCorrect ? "Да" : "Нет") << endl;
            }
        }
    }
    ~Test();
};

