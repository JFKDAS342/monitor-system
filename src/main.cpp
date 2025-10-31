#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <dirent.h> //предоставляет функции для чтения

#include "InfoCPUMonitoring.h"
using namespace std;
//%idle: Процент времени, когда процессор был в простое, ничего не делая.
//%user: Процент времени, потраченный на выполнение задач в пользовательском режиме.
//%system: Процент времени, потраченный на выполнение задач в системном режиме (ядра ОС).
//%iowait: Процент времени, когда процессор ждал завершения операций ввода-вывода (например, с диска).

struct Underscores
{
    int lowLine = 31;
};

void date()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[80];

    strftime(buffer, 80, "%Y-%m-%d", ltm);
    cout << "Date parsing data: " << buffer << endl;
}

int allMemory()
{
    Underscores lowL;

    try
    {
        string line;
        // name fields in meminfo
        string fields[] = {"MemTotal", "MemFree", "Buffers", "Cached", "MemAvailable"};
        const int num_fields = 5;
        string values[num_fields] = {""}; // save fileds in array

        ifstream meminfo_file("/proc/meminfo"); // the way to dir meminfo

        if (!meminfo_file.is_open())
        {
            cerr << "Failed open /proc/meminfo" << endl;
            return 1;
        }
        // read the file line
        while (getline(meminfo_file, line))
        {
            for (int i = 0; i < num_fields; i++)
            {
                if (line.find(fields[i]) != string::npos)
                {
                    values[i] = line; // if the found is save line
                    break;            // needed if there may be repetitions
                }
            }
        }
        meminfo_file.close(); // be sure to close file

        cout << "Key memory statistics: " << endl;
        for (int i = 0; i < num_fields; i++)
        {
            if (!values[i].empty())
            {                              // we check for null
                cout << values[i] << endl; // enter line meminfo
            }
        }
        for (int i = 0; i < lowL.lowLine; i++)
        {
            cout << "_";
        }
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
    }
    return 0;
}
int fileProc()
{

    DIR *dir;
    struct dirent *entry;
    const char *proc_dir = "/proc";

    if ((dir = opendir(proc_dir)) == NULL)
    {
        cerr << "Error opened dir /proc" << endl;
        return 1;
    }

    cout << "Output of processes: " << endl;

    while ((entry = readdir(dir)) != NULL)
    {
        string name = entry->d_name;
        // Whether the subdirectory is a number (PID)
        bool is_pid = true;
        for (char c : name)
        {
            if (!isdigit(c))
            { // isdigit - является ли символ не цифрой
                is_pid = false;
                break;
            }
        }
        // if the name consists only is nymber and not is null it is der (PID)
        if (is_pid && !name.empty())
        {
            // Формируем путь к файлу cmdline, который содержит команду и аргументы процесса
            string cmdline_path = string(proc_dir) + "/" + name + "/cmdline";
            ifstream cmdline_file(cmdline_path); // open file for read
            string cmdline_str;
            // проверка на успешно ли открылся файл
            if (cmdline_file.is_open())
            {
                // читаем сообщения до первого нулевого символа '/0'
                getline(cmdline_file, cmdline_str, '\0');
                // заменяем все оставшиеся нулевые сиволы на пробелы для читаемости
                for (char &c : cmdline_str)
                {
                    if (c == '\0')
                    {
                        c = ' ';
                    }
                }
                cout << "PID: " << name << ", Команда: " << cmdline_str << endl;
                cmdline_file.close();
            }
            else
            {
                cout << "PID: " << name << "Failed to get thee command!Try again" << endl;
            }
        }
    }
    closedir(dir);
    return 0;
}

void showMenu()
{
    cout << "\n=== System Information Tool ===\n";
    cout << "1. CPU Information\n";
    cout << "2. CPU Processis\n";
    cout << "3. CPU Monitoring\n";
    cout << "4. Exit\n";

    cout << "Enter commands: ";
}
int main()
{
    setlocale(LC_ALL, "ru");
    try
    {
        Underscores lowL;

        string userChoice;
        getline(cin, userChoice);
        for (char &c : userChoice)
        {
            c = tolower(c);
        }

        bool running = true;

        cout << "Hello, this is a console app where you can learn about your OS\n";

        while (running)
        {
            showMenu();
            cin >> userChoice;

            if (userChoice == "1")
            {
                date();
                for (int i = 0; i < lowL.lowLine; i++)
                {
                    cout << "=";
                }
                cout << endl;
                cout << "=== CPU Information: " << endl;

                allMemory();
            }
            else if (userChoice == "2" || userChoice == "cpuProcesses")
            {
                date();
                for (int i = 0; i < lowL.lowLine; i++)
                {
                    cout << "=";
                }
                cout << endl;
                cout << "=== Active processes: ";
                fileProc();
            }
            else if (userChoice == "3" || userChoice == "CPUMonitoring")
            {
                date();
                CPUMonitoring cpuMonitoring;
                int enterCPUMonitoring = cpuMonitoring.mainCPU();
                cout << enterCPUMonitoring << endl;
            }
            else if (userChoice == "4" || userChoice == "q" || userChoice == "Q")
            {
                running = false;
                cout << "𝔾𝕠𝕠𝕕𝕓𝕒𝕪!" << endl;
            }
            else
            {
                cerr << "Invalid option! Please try again.";
            }
        }
    }
    catch (exception &e)
    {
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
}

