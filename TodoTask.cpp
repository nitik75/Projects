#include <iostream>
#include <string>
#include <list>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;

#ifdef _WIN32
#include <Windows.h>
#endif

class TodoTask
{
private:
    static int nextId;
    int id;
    string description;
    string notes; // New member to store additional notes
    bool completed;
    time_t dueDateTime;
    int priority;

public:
    TodoTask(string new_description) : id(nextId++), description(new_description), completed(false), dueDateTime(0), priority(0) {}

    int getId() const { return id; }
    string getDescription() const { return description; }
    string getNotes() const { return notes; } // New getter function for notes
    bool isCompleted() const { return completed; }
    time_t getDueDateTime() const { return dueDateTime; }
    int getPriority() const { return priority; }

    void setCompleted(bool val) { completed = val; }
    void setDueDateTime(const string &dateTimeStr)
    {
        struct tm timeinfo = {0};
        istringstream ss(dateTimeStr);
        ss >> get_time(&timeinfo, "%d-%m-%Y %H:%M:%S");
        dueDateTime = mktime(&timeinfo);
    }
    void setPriority(int newPriority) { priority = newPriority; }
    void setNotes(const string &newNotes) { notes = newNotes; } // New setter function for notes
};

int TodoTask::nextId = 1;

class TodoList
{
private:
    list<TodoTask> todoItems;

public:
    void addTodoItem(const string &description)
    {
        TodoTask newItem(description);

        string dueDateTimeStr;
        cout << "Enter due date and time (DD-MM-YYYY HH:MM:SS) or leave blank: ";
        getline(cin, dueDateTimeStr);

        if (!dueDateTimeStr.empty())
        {
            newItem.setDueDateTime(dueDateTimeStr);
        }

        int priority;
        cout << "Enter priority level (1-5): ";
        cin >> priority;
        newItem.setPriority(priority);

        cin.ignore(); // Ignore the newline character after inputting the priority

        string notes;
        cout << "Add additional notes (press Enter to leave blank): ";
        getline(cin, notes);
        newItem.setNotes(notes);

        todoItems.push_back(newItem);
    }

    void markCompleted(int id)
    {
        for (auto &item : todoItems)
        {
            if (item.getId() == id)
            {
                item.setCompleted(true);
                break;
            }
        }
    }

    void sortTodoList(int sortOption)
    {
        switch (sortOption)
        {
        case 1: // Sort by due date
            todoItems.sort([](const TodoTask &a, const TodoTask &b)
                           { return a.getDueDateTime() < b.getDueDateTime(); });
            break;
        case 2: // Sort by priority
            todoItems.sort([](const TodoTask &a, const TodoTask &b)
                           { return a.getPriority() < b.getPriority(); });
            break;
        case 3: // Sort by completion status (completed tasks first)
            todoItems.sort([](const TodoTask &a, const TodoTask &b)
                           { return a.isCompleted() > b.isCompleted(); });
            break;
        default:
            cout << "Invalid sort option! " << endl;
            cout << "Please try again." << endl;
#ifdef _WIN32
            Sleep(2000); 
#endif
            break;
        }
    }

    void displayTodoList() const
    {
        system("cls");
        cout << "Todo List Maker" << endl;
        cout << endl
             << endl;

        if (todoItems.empty())
        {
            cout << "Let the journey of productivity begin!\nKickstart your ToDo List by adding your first task!" << endl;
        }
        else
        {
            for (const auto &item : todoItems)
            {
                string completed = item.isCompleted() ? "DONE" : "NOT DONE";
                string priorityStr = to_string(item.getPriority());

                time_t dueDateTime = item.getDueDateTime();
                string dueDateTimeStr = "-";
                if (dueDateTime > 0)
                {
                    struct tm *timeinfo = localtime(&dueDateTime);
                    char buffer[80];
                    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
                    dueDateTimeStr = buffer;
                }

                string priorityColor = ""; // Store the escape sequence for the color
                if (item.getPriority() >= 1 && item.getPriority() <= 5)
                {
                    // You can set custom colors based on priority levels
                    switch (item.getPriority())
                    {
                    case 1:
                        priorityColor = "\x1B[31m"; // Red
                        break;
                    case 2:
                        priorityColor = "\x1B[33m"; // Yellow
                        break;
                    case 3:
                        priorityColor = "\x1B[32m"; // Green
                        break;
                    case 4:
                        priorityColor = "\x1B[34m"; // Blue
                        break;
                    case 5:
                        priorityColor = "\x1B[35m"; // Purple
                        break;
                    }
                }

                cout << priorityColor << item.getId() << " | " << item.getDescription() << " | " << completed
                     << " | Due: " << dueDateTimeStr << " | Priority: " << priorityStr << "\x1B[0m" << endl;

                if (!item.getNotes().empty())
                {
                    cout << "Notes: " << item.getNotes() << endl;
                }
                cout << endl;
            }
        }

        cout << endl
             << endl;
    }
};

int main()
{
    char input_option;
    int input_id;
    string input_description;
    string version = "v0.4.0";
    TodoList todoList;
    int sortOption;

    while (true)
    {
        todoList.displayTodoList();

        cout << "Choose any of the alternatives to begin:" << endl;
        cout << "(a)-To add a new Todo" << endl;
        cout << "(c)-To complete a Todo" << endl;
        cout << "(s)-To sort Todo List" << endl;
        cout << "(q)-To quit" << endl;
        cout << endl;
        cout << "What's your choice? ";
        cin >> input_option;

        switch (input_option)
        {
        case 'q':
            cout << "Wishing you an amazing day ahead!" << endl;
            return 0;
        case 'a':
            cout << "Add description to your task: ";
            cin.ignore();
            getline(cin, input_description);
            todoList.addTodoItem(input_description);
            break;
        case 'c':
            cout << "Enter id to mark completed: ";
            cin >> input_id;
            todoList.markCompleted(input_id);
            break;
        case 's':
            cout << "Sort Todo List by:" << endl;
            cout << "(1) Due Date" << endl;
            cout << "(2) Priority" << endl;
            cout << "(3) Completion Status (completed tasks first)" << endl;
            cout << "What's your choice? ";
            cin >> sortOption;
            todoList.sortTodoList(sortOption);
            break;

        default:
            cout << "Invalid choice!" << endl;
            cout << "Please try again." << endl;
#ifdef _WIN32
            Sleep(2000); // Sleep for 2 seconds (2000 milliseconds)
#endif
            cout << endl;
            break;
        }
    }
}