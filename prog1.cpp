#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <vector>
#include <string>
using std::vector;
using std::thread;
using std::cout;
using std::cin;

void CWMatrix();

int K = 4; //Количество потоков
int N = 3; //Размер матрицы
int X = 1; //Стартовое число
long long currentIndex = 1;

std::mutex wmutex;
std::mutex indmutex;
std::ofstream file;
int main() {
	std::string fileName;
	cout << "Threads count(K) = ";
	cin >> K;
	cout << "\nMatrix size(N) = ";
	cin >> N;
	cout << "\nX = ";
	cin >> X;
	cout << "\nOutput file name: ";
	cin.get();
	std::getline(cin, fileName);
	
	if (fileName == "")fileName = "out.txt";
	file.open(fileName);
	if (K < 1)
	{
		std::cout << "\nКоличество потоков должно быть больше 0";
		return 0;
	}
	
	if (!file.is_open())
	{
		std::cout << "\nНевозмжно создать файл!";
		return 0;
	}
	
	vector<thread> threads(K - 1);
	for (int i = 0; i < threads.size(); ++i)
		threads[i] = thread(CWMatrix);

	CWMatrix();

	for (auto& t : threads)
		if (t.joinable())
			t.join();
	file.close();
	return 0;
}


long long factorial(int n)
{
	long long res = 1;
	if (n <= 1) return res;
	for (int i = 2; i <= n; ++i)
		res *= i;
	return res;
}

int notUsed(bool * used, int size,int blockNum) {

	int j, pos = 0;
	for (j = 1; j < size; j++) {
		if (!used[j]) pos++;
		if (blockNum == pos)
			break;
	}
	return j;
}
vector<int> permutation_by_num(int n, int num) {
	
	vector<int> res(n);
	bool * used = new bool[n + 1];
	for (int i = 0; i < n + 1; ++i)
		used[i] = false;
	for (int i = 0; i < n; i++) {
		long long fact = factorial(n - i - 1);
		int blockNum = (num - 1) / fact + 1;
		int j = notUsed(used, n + 1,blockNum);
		res[i] = X + j - 1;
		used[j] = true;
		num = (num - 1) % fact + 1;
	}
	return res;
}

void CWMatrix()
{
	while (true)
	{
		int index;
		{
			std::lock_guard<std::mutex> ind(indmutex);
			//system("cls");
			//long long fact = factorial(N*N);
			index = currentIndex++;
			if (index > factorial(N*N))
				break;
			//cout << index << " / " << fact;
		}
		int n = N * N;
		vector<int> matrix = permutation_by_num(N * N, index);

		{
			std::lock_guard<std::mutex> write(wmutex);
			for (int i = 0; i < N * N; ++i)
			{

				file << matrix[i] << " ";
				if ((i + 1) % N == 0 && i != 0)file << std::endl;
			}
			file << std::endl;
		}

	}


	

}


