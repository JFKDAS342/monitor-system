#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h> //for sleep
#include <stdlib.h> // atol()- функция из загаловочного файла (stdlib) в языке программирования с/с+, которая преобразует строку в целое число типа long int

using namespace std;
class CPUMonitoring
{
public:
    // хранит счетчики времени ЦПУ из /proc/stat
    struct CPUData
    {
        string userChoice;
        unsigned long user;
        unsigned long nice;
        unsigned long system;
        unsigned long idle;
        unsigned long iowait;
        unsigned long irq;
        unsigned long softirq;
        unsigned long steal;
    };
    bool readCPUData(CPUData &data)
    {
        ifstream file("/proc/stat");
        string line;

        if (getline(file, line))
        {
            istringstream iss(line);
            string cpuLabel;

            iss >> cpuLabel >> data.user >> data.nice >> data.system >> data.idle >> data.iowait >> data.irq >> data.irq >> data.softirq >> data.steal;

            return true; // return true if the data was read successfully
            file.close();
        }
        return false;
    }
    // unsigned является модификатором типа данных, который указывает, что переменная может хранить только //неотрицательные (положительные целые числа и ноль) значения
    // функция для вычесления времени CPU
    unsigned long calculateTotalTime(const CPUData &data)
    {
        return data.user + data.nice + data.system + data.idle + data.iowait + data.irq + data.softirq + data.steal;
    }

    int mainCPU()
    {
        int maxCycle = 10;
        int currentCycle = 0;
        CPUData previousData, currentData;

        if (!readCPUData(previousData))
        {
            cerr << "Error: failed read /proc/stat" << endl;
            return 1;
        }
        while (currentCycle < maxCycle)
        {
            sleep(1);

            if (!readCPUData(currentData))
            {
                cerr << "Error: failed read /proc/stat" << endl;
                continue;
            }
            // вычисляем общее время для предыдущего и текущего размера
            unsigned long previousTotal = calculateTotalTime(previousData);
            unsigned long currentTotal = calculateTotalTime(currentData);

            // вычисляем  разницы:cite[4]:cite[6]
            unsigned long totalDelta = currentTotal - previousTotal;
            unsigned long idleDelta = currentData.idle - previousData.idle;

            double cpuUsage = 100.0;
            if (totalDelta > 0)
            {
                cpuUsage = 100.0 * (totalDelta - idleDelta) / totalDelta;
            }
            for (int i = 0; i < 10; i++)
            {
                cout << "Using CPU: " << cpuUsage << "%" << endl;
                break;
            }
            previousData = currentData;
            currentCycle++;
        }

        return 0;
    }
};
