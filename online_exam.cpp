#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Question {
private:
    string questionText;
    string options[4];
    char correctOption;

public:
    void inputQuestion() {
        cout << "\nEnter question: ";
        cin.ignore();
        getline(cin, questionText);
        cout << "Enter 4 options:\n";
        for (int i = 0; i < 4; i++) {
            cout << "Option " << char('A' + i) << ": ";
            getline(cin, options[i]);
        }
        cout << "Enter correct option (A/B/C/D): ";
        cin >> correctOption;
        correctOption = toupper(correctOption);
    }

    void displayQuestion() const {
        cout << "\n" << questionText << "\n";
        for (int i = 0; i < 4; i++) {
            cout << char('A' + i) << ") " << options[i] << "\n";
        }
    }

    char getCorrectOption() const {
        return correctOption;
    }

    void saveToFile(ofstream &file) const {
        file << questionText << "\n";
        for (int i = 0; i < 4; i++)
            file << options[i] << "\n";
        file << correctOption << "\n";
    }

    bool loadFromFile(ifstream &file) {
        getline(file, questionText);
        if (file.eof()) return false;
        for (int i = 0; i < 4; i++)
            getline(file, options[i]);
        file >> correctOption;
        file.ignore();
        return true;
    }
};


class Exam {
protected:
    vector<Question> questions;

public:
    void loadQuestions() {
        ifstream file("questions.txt");
        if (!file) {
            cout << "\nNo questions available. Please ask admin to add some.\n";
            return;
        }
        Question q;
        while (q.loadFromFile(file)) {
            questions.push_back(q);
        }
        file.close();
    }
};

class Admin : public Exam {
public:
    void addQuestions() {
        ofstream file("questions.txt", ios::app);
        int n;
        cout << "How many questions do you want to add? ";
        cin >> n;
        for (int i = 0; i < n; i++) {
            cout << "\n--- Enter details for question " << i + 1 << " ---\n";
            Question q;
            q.inputQuestion();
            q.saveToFile(file);
        }
        file.close();
        cout << "\nQuestions added successfully!\n";
    }
};


class Student : public Exam {
private:
    string name;
    int score = 0;

public:
    void takeExam() {
        cout << "\nEnter your name: ";
        cin.ignore();
        getline(cin, name);
        loadQuestions();

        if (questions.empty()) {
            cout << "\nNo questions found. Exam cannot start.\n";
            return;
        }

        char ans;
        for (size_t i = 0; i < questions.size(); i++) {
            cout << "\nQuestion " << i + 1 << ": ";
            questions[i].displayQuestion();
            cout << "Your answer (A/B/C/D): ";
            cin >> ans;
            ans = toupper(ans);
            if (ans == questions[i].getCorrectOption()) {
                score++;
            }
        }

        cout << "\nExam completed!\n";
        cout << "Your Score: " << score << "/" << questions.size() << "\n";

        saveResult();
    }

    void saveResult() {
        ofstream file("results.txt", ios::app);
        file << name << " " << score << "/" << questions.size() << "\n";
        file.close();
    }
};


int main() {
    int choice;
    Admin admin;
    Student student;

    do {
        cout << "\n===== Online Examination System =====";
        cout << "\n1. Admin - Add Questions";
        cout << "\n2. Student - Take Exam";
        cout << "\n3. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            admin.addQuestions();
            break;
        case 2:
            student.takeExam();
            break;
        case 3:
            cout << "\nThank you for using Online Examination System!\n";
            break;
        default:
            cout << "\nInvalid choice! Try again.\n";
        }
    } while (choice != 3);

    return 0;
}
