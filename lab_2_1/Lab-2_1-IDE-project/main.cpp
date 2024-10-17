#include <iostream>
#include <list>
#include <string>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <limits>

// Базовий клас Task, який представляє загальне завдання
class Task {
protected:
    std::string taskName;   // Назва завдання
    std::string description;  // Опис завдання
    std::tm deadline;    // Термін виконання завдання

public:
    // Конструктор за замовчуванням
    Task();
    
    // Параметризований конструктор, який приймає назву, опис і дедлайн
    Task(std::string _taskName, std::string _description, std::tm _deadline);
    
    // Віртуальний деструктор (може бути перевизначений у похідних класах)
    virtual ~Task() {}

    // Віртуальний метод для виведення інформації про завдання
    virtual void printTask() const;
    
    // Чисто віртуальний метод, який буде перевизначено в похідних класах (важливість завдання)
    virtual std::string getImportance() const = 0;
    
    // Повертає назву завдання
    std::string& getName() { return taskName; }
    
    // Повертає опис завдання
    std::string& getDescription() { return description; }
    
    // Повертає термін виконання у вигляді рядка
    std::string getDeadlineString() const;
    
    // Повертає термін виконання як std::tm об'єкт
    std::tm& getDeadline() { return deadline; }

    // Дружній оператор для виведення завдання у потік
    friend std::ostream& operator<<(std::ostream& os, const Task& obj);
};

// Реалізація Task
Task::Task() : taskName("Unknown"), description("Unknown") {}

// Параметризований конструктор Task
Task::Task(std::string _taskName, std::string _description, std::tm _deadline)
    : taskName(_taskName), description(_description), deadline(_deadline) {}

// Метод для отримання дедлайну як рядка
std::string Task::getDeadlineString() const {
    if (mktime(const_cast<std::tm*>(&deadline)) != -1) {
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d", &deadline);  // Форматування дати в рядок
        return std::string(buffer);
    } else {
        return "Deadline: Not Set";  // Якщо дедлайн не встановлено
    }
}

// Метод для виведення інформації про завдання
void Task::printTask() const {
    std::cout << *this << "\n";  // Використовуємо перевантажений оператор <<
}

// Перевантажений оператор виведення для Task
std::ostream& operator<<(std::ostream& os, const Task& obj) {
    os << std::left;
    os << std::setw(30) << obj.taskName    // Назва завдання
       << std::setw(50) << obj.description  // Опис завдання
       << obj.getDeadlineString() << "\n";  // Термін виконання
    return os;
}

// Похідний клас NormalTask (звичайне завдання)
class NormalTask : public Task {
public:
    // Конструктор NormalTask, що викликає базовий конструктор Task
    NormalTask(std::string _taskName, std::string _description, std::tm _deadline)
        : Task(_taskName, _description, _deadline) {}

    // Перевизначення методу для виведення інформації про звичайне завдання
    void printTask() const override {
        std::cout << "[Normal] ";  // Позначаємо звичайне завдання
        Task::printTask();  // Викликаємо базовий метод для виведення інформації
    }

    // Перевизначення методу для отримання важливості (звичайні завдання не є важливими)
    std::string getImportance() const override {
        return "Not Important";  // Важливість для звичайного завдання
    }
};

// Похідний клас ImportantTask (важливе завдання)
class ImportantTask : public Task {
    int priority;  // Пріоритет важливого завдання (1-10)

public:
    // Конструктор ImportantTask, що викликає базовий конструктор Task та ініціалізує пріоритет
    ImportantTask(std::string _taskName, std::string _description, std::tm _deadline, int _priority)
        : Task(_taskName, _description, _deadline), priority(_priority) {}

    // Перевизначення методу для виведення інформації про важливе завдання
    void printTask() const override {
        std::cout << "[Important] " << *this << "\n";  // Позначаємо важливе завдання
        std::cout << "Priority: " << priority << "\n";  // Виводимо пріоритет
    }

    // Перевизначення методу для отримання важливості (як пріоритет)
    std::string getImportance() const override {
        return std::to_string(priority);  // Важливість повертається як пріоритет
    }
};


// Клас для управління завданнями
class TaskMananger {
    std::list<Task*> tasks;  // Список вказівників на об'єкти завдань

public:
    ~TaskMananger();  // Деструктор для очищення пам'яті

    // Додає нове завдання
    void addTask(Task* task);
    
    // Видаляє завдання за його назвою
    bool deleteTask(const std::string& taskName);
    
    // Виводить усі завдання
    void printTasks() const;
    
    // Фільтрує завдання за дедлайном
    void filterTasksByDeadline(const std::tm& deadline) const;
    
    // Зберігає завдання у файл
    void saveToFile(const std::string& fileName) const;
    
    // Завантажує завдання з файлу
    void loadFromFile(const std::string& fileName);
    
    // Сортує завдання за важливістю
    void sortByImportance();
};

// Деструктор очищує пам'ять від завдань
TaskMananger::~TaskMananger() {
    for (Task* task : tasks) {
        delete task;  // Очищаємо пам'ять для кожного завдання
    }
}

// Додає нове завдання до списку
void TaskMananger::addTask(Task* task) {
    tasks.push_back(task);  // Додаємо завдання до кінця списку
}

// Видаляє завдання за його назвою
bool TaskMananger::deleteTask(const std::string& taskName) {
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if ((*it)->getName() == taskName) {  // Якщо знайшли завдання з відповідною назвою
            delete *it;  // Видаляємо об'єкт
            tasks.erase(it);  // Видаляємо вказівник із списку
            return true;
        }
    }
    return false;  // Повертаємо false, якщо завдання не знайдено
}

// Виводить усі завдання на екран
void TaskMananger::printTasks() const {
    if (tasks.empty()) {
        std::cout << "There are no tasks.\n";  // Якщо немає завдань
    } else {
        std::cout << std::left;
        // Додаємо заголовки стовпців
        std::cout << std::setw(30) << "Task Name" 
                  << std::setw(50) << "Description" 
                  << std::setw(20) << "Deadline" 
                  << "Importance" << "\n";
        std::cout << std::string(110, '-') << "\n";  // Роздільна лінія

        // Виводимо інформацію про кожне завдання
        for (Task* task : tasks) {
            std::cout << std::setw(30) << task->getName()
                      << std::setw(50) << task->getDescription()
                      << std::setw(20) << task->getDeadlineString()
                      << task->getImportance() << "\n";  // Виводимо важливість завдання
        }
    }
}

// Фільтрує завдання за дедлайном (ті, що мають дедлайн після заданої дати)
void TaskMananger::filterTasksByDeadline(const std::tm& deadline) const {
    if (tasks.empty()) {
        std::cout << "There are no tasks.\n";  // Якщо немає завдань
    } else {
        bool found = false;  // Прапорець, що вказує, чи були знайдені завдання

        std::cout << std::left;
        // Виводимо заголовок фільтрованих завдань
        std::cout << "Filtered Tasks (on or after " << std::put_time(&deadline, "%Y-%m-%d") << "):\n";
        std::cout << std::setw(30) << "Task Name" << std::setw(50) << "Description" << "Deadline" << "\n";
        std::cout << std::string(100, '-') << "\n";  // Роздільна лінія

        for (Task* task : tasks) {
            if (mktime(&task->getDeadline()) >= mktime(const_cast<std::tm*>(&deadline))) {
                task->printTask();  // Якщо дедлайн завдання підходить, виводимо його
                found = true;  // Знайшли завдання
            }
        }

        if (!found) {
            std::cout << "No tasks found with a deadline on or after the specified date.\n";  // Якщо не знайдено жодного
        }
    }
}

// Зберігає завдання до файлу
void TaskMananger::saveToFile(const std::string& fileName) const {
    try {
        std::ofstream file(fileName, std::ios::out);  // Відкриваємо файл для запису

        if (!file.is_open()) {
            throw std::runtime_error("Error: can't open file for writing");
        }

        // Записуємо кожне завдання до файлу
        for (Task* task : tasks) {
            char buffer[80];
            strftime(buffer, 80, "%Y-%m-%d", &task->getDeadline());  // Форматуємо дату

            ImportantTask* importantTask = dynamic_cast<ImportantTask*>(task);  // Перевіряємо чи це важливе завдання
            if (importantTask) {
                // Записуємо важливе завдання до файлу
                file << "Important|"
                     << task->getName() << "|" 
                     << task->getDescription() << "|" 
                     << buffer << "|" 
                     << importantTask->getImportance() << "\n";
            } else {
                // Записуємо звичайне завдання до файлу
                file << "Normal|"
                     << task->getName() << "|" 
                     << task->getDescription() << "|" 
                     << buffer << "\n";
            }
        }

        std::cout << "Successfully saved to file\n";
        file.close();  // Закриваємо файл
    } catch (const std::exception& e) {
        std::cerr << "Error occurred during file saving: " << e.what() << "\n";  // Якщо сталася помилка
    }
}

// Завантажує завдання з файлу
void TaskMananger::loadFromFile(const std::string& fileName) {
    try {
        std::ifstream file(fileName, std::ios::in);  // Відкриваємо файл для читання

        if (!file.is_open()) {
            throw std::runtime_error("Error: can't open file for reading");
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string taskType, name, description, deadlineStr, importanceStr;

            // Читаємо тип завдання (Normal або Important)
            if (std::getline(iss, taskType, '|') && 
                std::getline(iss, name, '|') && 
                std::getline(iss, description, '|') && 
                std::getline(iss, deadlineStr, '|')) {

                std::tm deadline = {};
                if (strptime(deadlineStr.c_str(), "%Y-%m-%d", &deadline)) {
                    if (taskType == "Important") {
                        // Якщо це важливе завдання, читаємо його пріоритет
                        if (std::getline(iss, importanceStr, '|')) {
                            int priority = std::stoi(importanceStr);  // Конвертуємо пріоритет у число
                            addTask(new ImportantTask(name, description, deadline, priority));  // Додаємо важливе завдання
                        }
                    } else {
                        // Якщо це NormalTask
                        addTask(new NormalTask(name, description, deadline));  // Додаємо звичайне завдання
                    }
                } else {
                    std::cerr << "Error parsing date: " << deadlineStr << "\n";  // Якщо помилка при розборі дати
                }
            }
        }

        std::cout << "Successfully loaded from file\n";
        file.close();  // Закриваємо файл
    } catch (const std::exception& e) {
        std::cerr << "Error occurred during file loading: " << e.what() << "\n";  // Якщо сталася помилка
    }
}

// Сортує завдання за важливістю
void TaskMananger::sortByImportance() {
    tasks.sort([](Task* a, Task* b) {
        // Перевіряємо, чи завдання є важливими
        ImportantTask* importantA = dynamic_cast<ImportantTask*>(a);
        ImportantTask* importantB = dynamic_cast<ImportantTask*>(b);

        if (importantA && importantB) {
            // Порівнюємо пріоритети важливих завдань
            int priorityA = std::stoi(importantA->getImportance());
            int priorityB = std::stoi(importantB->getImportance());
            return priorityA > priorityB;  // Більший пріоритет "вище"
        } else if (importantA) {
            // Якщо лише A важливе
            return true;
        } else if (importantB) {
            // Якщо лише B важливе
            return false;
        } else {
            // Якщо обидва завдання NormalTask
            return false;
        }
    });

    std::cout << "Tasks sorted by importance.\n";  // Повідомляємо, що сортування завершено
}

// Клас для управління меню
class Menu {
    TaskMananger taskManager;  // Об'єкт менеджера завдань

public:
    Menu() {}

    void displayMenu();  // Відображає меню
    void handleInput();  // Обробляє введення користувача

    void addTask();      // Додає звичайне завдання
    void deleteTask();   // Видаляє завдання
    void printTasks();   // Виводить усі завдання
    void filterTasks();  // Фільтрує завдання за дедлайном
    void saveToFile();   // Зберігає завдання у файл
    void loadFromFile(); // Завантажує завдання з файлу
};

// Відображає меню користувачеві
void Menu::displayMenu() {
    std::cout << "\nTask Manager Menu:\n";
    std::cout << "1. Add Normal Task\n";       // Додати звичайне завдання
    std::cout << "2. Add Important Task\n";    // Додати важливе завдання
    std::cout << "3. Delete Task\n";           // Видалити завдання
    std::cout << "4. View All Tasks\n";        // Переглянути всі завдання
    std::cout << "5. Filter Tasks by Deadline\n";  // Фільтрувати завдання за дедлайном
    std::cout << "6. Sort Tasks by Importance\n";  // Сортувати завдання за важливістю
    std::cout << "7. Save Tasks to File\n";    // Зберегти завдання у файл
    std::cout << "8. Load Tasks from File\n";  // Завантажити завдання з файлу
    std::cout << "9. Exit\n";                  // Вийти з програми
    std::cout << "Enter your choice: ";        // Запитати вибір користувача
}

// Обробляє введення користувача та викликає відповідні функції
void Menu::handleInput() {
    int choice = 0;

    while (true) {
        displayMenu();  // Показати меню
        std::cin >> choice;  // Отримати вибір користувача

        if (std::cin.fail()) {  // Якщо введено некоректні дані
            std::cin.clear();  // Очищуємо стан помилки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Пропускаємо некоректний ввід
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        // Виконати дію на основі вибору користувача
        switch (choice) {
            case 1:
                addTask();  // Додати звичайне завдання
                break;
            case 2: {
                std::string name, description, deadlineStr;
                int priority;
                std::tm deadline = {};

                std::cin.ignore();  // Ігноруємо символ нового рядка
                std::cout << "Enter task name: ";  // Запитуємо назву завдання
                std::getline(std::cin, name);
                std::cout << "Enter task description: ";  // Запитуємо опис завдання
                std::getline(std::cin, description);
                std::cout << "Enter task deadline (YYYY-MM-DD): ";  // Запитуємо дедлайн
                std::getline(std::cin, deadlineStr);

                // Перевіряємо коректність формату дати
                if (strptime(deadlineStr.c_str(), "%Y-%m-%d", &deadline) == nullptr) {
                    std::cout << "Invalid date format. Please use YYYY-MM-DD.\n";
                    break;
                }

                // Запитуємо пріоритет завдання, доки не буде введено коректне значення
                do {
                    std::cout << "Enter priority (1-10): ";
                    std::cin >> priority;

                    if (std::cin.fail()) {
                        std::cin.clear();  // Очищуємо стан помилки
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Пропускаємо некоректний ввід
                        std::cout << "Invalid input. Please enter a number.\n";
                    } else if (priority < 1 || priority > 10) {
                        std::cout << "Priority must be between 1 and 10.\n";  // Перевірка діапазону
                    }
                } while (std::cin.fail() || priority < 1 || priority > 10);

                // Додаємо важливе завдання
                taskManager.addTask(new ImportantTask(name, description, deadline, priority));
                std::cout << "Important task added successfully.\n";
                break;
            }
            case 3:
                deleteTask();  // Видалити завдання
                break;
            case 4:
                printTasks();  // Вивести усі завдання
                break;
            case 5:
                filterTasks();  // Фільтрувати завдання за дедлайном
                break;
            case 6:
                taskManager.sortByImportance();  // Сортувати завдання за важливістю
                break;
            case 7:
                saveToFile();  // Зберегти завдання у файл
                break;
            case 8:
                loadFromFile();  // Завантажити завдання з файлу
                break;
            case 9:
                std::cout << "Exiting...\n";  // Вихід з програми
                return;
            default:
                std::cout << "Invalid choice. Please select again.\n";  // Невірний вибір
        }
    }
}

// Додає звичайне завдання
void Menu::addTask() {
    std::string name, description, deadlineStr;
    std::tm deadline = {};

    std::cin.ignore();  // Ігноруємо символ нового рядка
    std::cout << "Enter task name: ";  // Запитуємо назву завдання
    std::getline(std::cin, name);
    std::cout << "Enter task description: ";  // Запитуємо опис завдання
    std::getline(std::cin, description);
    std::cout << "Enter task deadline (YYYY-MM-DD): ";  // Запитуємо дедлайн
    std::getline(std::cin, deadlineStr);

    // Перевіряємо коректність формату дати
    if (strptime(deadlineStr.c_str(), "%Y-%m-%d", &deadline) == nullptr) {
        std::cout << "Invalid date format. Please use YYYY-MM-DD.\n";
        return;
    }

    // Додаємо звичайне завдання
    taskManager.addTask(new NormalTask(name, description, deadline));
    std::cout << "Normal task added successfully.\n";
}

// Видаляє завдання за назвою
void Menu::deleteTask() {
    std::string name;

    std::cin.ignore();  // Ігноруємо символ нового рядка
    std::cout << "Enter task name to delete: ";  // Запитуємо назву завдання для видалення
    std::getline(std::cin, name);

    // Якщо завдання знайдено і видалено
    if (taskManager.deleteTask(name)) {
        std::cout << "Task deleted successfully.\n";
    } else {
        std::cout << "Task not found.\n";  // Якщо завдання не знайдено
    }
}

// Виводить усі завдання
void Menu::printTasks() {
    taskManager.printTasks();  // Викликаємо функцію виведення завдань менеджера завдань
}

// Фільтрує завдання за дедлайном
void Menu::filterTasks() {
    std::string deadlineStr;
    std::tm deadline = {};

    std::cin.ignore();  // Ігноруємо символ нового рядка
    std::cout << "Enter the deadline (YYYY-MM-DD) to filter tasks: ";  // Запитуємо дедлайн для фільтрації
    std::getline(std::cin, deadlineStr);

    // Перевіряємо коректність формату дати
    if (strptime(deadlineStr.c_str(), "%Y-%m-%d", &deadline) == nullptr) {
        std::cout << "Invalid date format. Please use YYYY-MM-DD.\n";
        return;
    }

    // Фільтруємо завдання за дедлайном
    taskManager.filterTasksByDeadline(deadline);
}

// Зберігає завдання у файл
void Menu::saveToFile() {
    std::string fileName;

    std::cin.ignore();  // Ігноруємо символ нового рядка
    std::cout << "Enter file name to save tasks: ";  // Запитуємо назву файлу для збереження
    std::getline(std::cin, fileName);

    // Зберігаємо завдання у файл
    taskManager.saveToFile(fileName);
}

// Завантажує завдання з файлу
void Menu::loadFromFile() {
    std::string fileName;

    std::cin.ignore(); // Ігноруємо символ нового рядка
    std::cout << "Enter file name to load tasks: "; // Запитуємо назву файлу для загрузки
    std::getline(std::cin, fileName);

     // Загружаєм завдання з файлу
    taskManager.loadFromFile(fileName);
}

int main() {
    Menu menu;
    menu.handleInput();
    return 0;
}
