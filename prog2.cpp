#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
using std::vector;
using std::thread;
using std::cout;
using std::cin;

void ReadAndCheck();

int M = 3; //Количество потоков

std::mutex rmutex;
std::mutex wmutex;
std::mutex cmutex;
std::ifstream ifile;
std::ofstream ofile;
int magiccount = 0;
int main()
{
	std::string ifileName;
	std::string ofileName;
	cout << "Threads count(M) = ";
	cin >> M;
	cout << "\nINPUT file name = ";
	cin.get();
	std::getline(cin, ifileName);
	cout << "\nOUTPUT file name = ";
	std::getline(cin, ofileName);
	
	try{
		if (ifileName == "")ifileName = "out.txt";
		if (ofileName == "")ofileName = "DevilsSquares.txt";
		ifile.open(ifileName);
		ofile.open(ofileName);
		if (M < 1)
		{
			std::cout << "\nКоличество потоков должно быть больше 0";
			return 0;
		}
		if (!ifile.is_open())
		{
			std::cout << "\nНевозмжно прочитать файл!";
			return 0;
		}

		vector<thread> threads(M - 1);
		for (int i = 0; i < threads.size(); ++i)
			threads[i] = thread(ReadAndCheck);
		std::thread::id id = std::this_thread::get_id();
		ReadAndCheck();

		for (int i = 0; i < threads.size(); ++i)
			if (threads[i].joinable())
				threads[i].join();
		ifile.close();
		ofile.close();
		cout << "\nCount: " << magiccount << std::endl;
		system("pause");
	}
	catch (...)
	{
		if(ifile.is_open()) ifile.close();
		if(ofile.is_open()) ofile.close();
	}
	return 0;


}

void ReadAndCheck()
{
	vector<vector<int>> matrix;
	std::thread::id id = std::this_thread::get_id();
	//cout << std::this_thread::get_id();
	
	while (true)
	{
		matrix.clear();
		
		// Блокировка чтения файла
		{
			std::lock_guard<std::mutex> read(rmutex);
			if (ifile.eof()){ 
				break; 
			}
			std::string str;
			int i = 0;
			while (std::getline(ifile, str))
			{
				if (str == ""){ break; }


				matrix.push_back(vector<int>());
				std::istringstream iss(str);
				int x;
				while (iss >> x)
				{
					matrix[i].push_back(x);
				}

				++i;
			}
		}
		if (matrix.size() == 0) continue;
		vector<int> sums(4 * matrix.size() , 0);
		//Суммирование по диагоналям, строкам, столбцам
		for (int i = 0; i < matrix.size(); ++i)
		{
			for (int j = 0; j < matrix.size(); ++j)
			{
				if (i == j){ sums[0] += matrix[i][i]; sums[1] += matrix[i][matrix.size() - 1 - i]; }
				sums[i + 2] += matrix[i][j];
				sums[j + 2 + matrix.size()] += matrix[i][j];
			}
		}
		//Ломаные диагонали над главной 
		for (int k = 0; k < ((int)matrix.size()) - 1; ++k)
			for (int i = 0; i < matrix.size(); ++i)
			{
				int j = i + 1 + k < matrix.size()? i + 1 + k: (i + 1 + k) % matrix.size();
				sums[k + 2 + 2 * matrix.size()] += matrix[i][j];
			}
		//Ломаные диагонали над побочной 
		for (int k = 0; k < ((int)matrix.size()) - 1; ++k)
			for (int i = 0; i < matrix.size(); ++i)
			{
				int j = (int)matrix.size() - 2 - i - k;
				if (j < 0)
					j = 2 * matrix.size() - i - k - 2;
				
				sums[k + 1 + 3 * matrix.size()] += matrix[i][j];
			}
		bool isMagic = true;
		//Проверка сумм
		for (int i = 1; i < sums.size(); ++i)
			if (sums[i] != sums[0]) { isMagic = false; break; }

		if (isMagic)
		{
			++magiccount;
			{
				std::lock_guard<std::mutex> write(wmutex);
				for (int i = 0; i < matrix.size(); ++i)
				{
					for (int j = 0; j < matrix.size(); ++j)
					{
						ofile << matrix[i][j] << " ";
					}
					ofile << std::endl;
				}
				ofile << std::endl;
			}
			//Вывод в консоль значительно ухудшает производительность, если необходимо - закомментировать блок {...} ниже 
			{
				std::lock_guard<std::mutex> write(cmutex);
				cout << "The Matrix is a devil\'s magic square: \n";
				for (int i = 0; i < matrix.size(); ++i)
				{
					for (int j = 0; j < matrix.size(); ++j)
					{
						cout << matrix[i][j] << " ";
					}
					cout << std::endl;
				}
				cout << std::endl;
			}
		}
		else
		{
			//Вывод в консоль значительно ухудшает производительность, если необходимо - закомментировать блок {...} ниже 
			{
				std::lock_guard<std::mutex> write(cmutex);
				cout << "The Matrix is NOT a devil\'s magic square: \n";
				for (int i = 0; i < matrix.size(); ++i)
				{
					for (int j = 0; j < matrix.size(); ++j)
					{
						cout << matrix[i][j] << " ";
					}
					cout << std::endl;
				}
				cout << std::endl;
			}

		}

		
	}
		

}