#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct Question {
    string questionText;
    string options[4];
    int correctOption;
};

void saveQuestion(const Question& q) {
    ofstream outFile("questions.txt", ios::binary | ios::app);
    if (!outFile) {
        cerr << "Error opening file for writing." << endl;
        return;
    }
    outFile.write(reinterpret_cast<const char*>(&q), sizeof(Question));
    outFile.close();
}

void inputQuestion() {
    Question q;
    cin.ignore(); // Ignore any leftover newline characters in the input buffer
    cout << "Enter the question: ";
    getline(cin, q.questionText);

    for (int i = 0; i < 4; ++i) {
        cout << "Enter option " << i + 1 << ": ";
        getline(cin, q.options[i]);
    }

    cout << "Enter the correct option number (1-4): ";
    cin >> q.correctOption;

    saveQuestion(q);
}

bool checkPassword(const string& inputPassword, const string& actualPassword) {
    return inputPassword == actualPassword;
}

void updatePassword(string& password) {
    cout << "Enter new password: ";
    cin >> password;
    ofstream passwordFile("password.txt");
    if (!passwordFile) {
        cerr << "Error opening password file for writing." << endl;
        return;
    }
    passwordFile << password;
    passwordFile.close();
    cout << "Password updated successfully." << endl;
}

vector<Question> loadQuestions() {
    ifstream inFile("questions.txt", ios::binary);
    vector<Question> questions;
    if (!inFile) {
        cerr << "Error opening file for reading." << endl;
        return questions;
    }
    Question q;
    while (inFile.read(reinterpret_cast<char*>(&q), sizeof(Question))) {
        questions.push_back(q);
    }
    inFile.close();
    return questions;
}

void saveAllQuestions(const vector<Question>& questions) {
    ofstream outFile("questions.txt", ios::binary | ios::trunc);
    if (!outFile) {
        cerr << "Error opening file for writing." << endl;
        return;
    }
    for (const auto& q : questions) {
        outFile.write(reinterpret_cast<const char*>(&q), sizeof(Question));
    }
    outFile.close();
}

void deleteQuestion() {
    vector<Question> questions = loadQuestions();
    if (questions.empty()) {
        cout << "No questions available to delete." << endl;
        return;
    }

    for (size_t i = 0; i < questions.size(); ++i) {
        cout << i + 1 << ". " << questions[i].questionText << endl;
    }

    int questionNumber;
    cout << "Enter the number of the question to delete: ";
    cin >> questionNumber;

    if (questionNumber < 1 || questionNumber > questions.size()) {
        cout << "Invalid question number." << endl;
        return;
    }

    questions.erase(questions.begin() + (questionNumber - 1));
    saveAllQuestions(questions);
    cout << "Question deleted successfully." << endl;
}

int main() {
    string password;
    ifstream passwordFile("password.txt");
    if (!passwordFile) {
        ofstream outFile("password.txt");
        password = "admin";  // Default password
        outFile << password;
        outFile.close();
    } else {
        getline(passwordFile, password);
        passwordFile.close();
    }

    string inputPassword;
    cout << "Enter password to access admin panel: ";
    cin >> inputPassword;

    if (!checkPassword(inputPassword, password)) {
        cout << "Incorrect password. Access denied." << endl;
        return 1;
    }

    int choice;
    do { 
        cout << "\nWelcome Back, Chief!\n";
        cout << "1. Add a new question\n";
        cout << "2. Delete a question\n";
        cout << "3. Update password\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                inputQuestion();
                break;
            case 2:
                deleteQuestion();
                break;
            case 3:
                updatePassword(password);
                break;
            case 4:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (choice != 4);

    return 0;
}
