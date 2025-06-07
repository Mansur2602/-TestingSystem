#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

struct Answer
{
   string answer;
   bool isCorrect;

};

class Question
{
private:
    string questionText; // Текст вопроса
    vector<Answer> answers; // Варианты ответов

public:

    
    Question(const string& text) : questionText(text) {}

    string getQuestionText() const {return questionText;}
    vector<Answer> getAnswers() const {return answers; }
    void setQuestionText(const string& text) {questionText = text; }
    void setAnswers(const vector<Answer>& ans) {answers = ans; }

    void addAnswer(const string& answer, bool isСorrect)
    {
        answers.push_back({answer, isСorrect});
    }
    void ShowQuestion() const
    {
        cout << questionText << endl;
        for (int i = 0; i < answers.size(); i++)
        {
            cout << i + 1 << ". " << answers[i].answer << endl;
        }
    }
    bool CheckAnswer(int answerIndex) const
    {
        if (answerIndex < 1 || answerIndex > answers.size())
        {
            cout << "Неверный ответ!" << endl;
            return false;
        }
        return answers[answerIndex - 1].isCorrect; // Проверяем, является ли ответ правильным
    }
    
};


