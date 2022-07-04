#include <iostream>
#include <thread>
#include <chrono> // ��� ���������� ��������
#include <vector>
#include <string_view> // ���������� � 17�� ���������. ������� ����������� ����� ������
#include <mutex>
#include <atomic>

using namespace std;
using namespace std::chrono;

void Fill(vector<double>* cources) {
	for (size_t i = 0; i < cources->capacity(); i++)
	{
		this_thread::sleep_for(milliseconds(15));
		cources->emplace_back(15 + ::rand() / (1 + ::rand()));
	}
}

void showArrays(vector<double>* cources) {
	for (size_t i = 0; i < cources->capacity(); i++)
	{
		std::cout << &cources[i] << endl;
	}
}

mutex mt;

void print(std::vector<double>::iterator start, std::vector<double>::iterator stop, string_view name) {
	
	while (start <= stop)
	{
		//mt.lock(); // ��������� �����
		lock_guard lg(mt);
		std::cout << name; // ��������� �������� (����, ������� ����������� � ������ �������)
		std::cout << "Value "; // ��������� �������� (����, ������� ����������� � ������ �������)
		std::cout << *start;  // ��������� �������� (����, ������� ����������� � ������ �������)
		std::cout << endl; // ��������� �������� (����, ������� ����������� � ������ �������)

		//mt.unlock(); // ������������� ������
		++start;
	}
}

// ��������������� � �����������

// ����������� �����
// ��������� ��������
// ����� ������

int main() {

	std::this_thread::get_id(); // ���������� ID �������� � ���������� �����.
	std::this_thread::sleep_for(milliseconds(10)); // �� ����� ��������� ���������� ������ �������.
	// ���������� ������ (��������� ������ ��� �������)
	vector<double> usdCource;
	usdCource.reserve(540);
	vector<double> euroCource;
	euroCource.reserve(540);
	vector<double> phoundCource;
	phoundCource.reserve(540);
	//std::this_thread::sleep_until(); // ��� ���������� �������
	
	thread tr1(Fill, &usdCource); // ������������ ������ � ������ �����. ������ � ��� 2 ������ ����������
	thread tr2(Fill, &euroCource);
	thread tr3(Fill, &phoundCource);
	// ��������� �������
	//Fill(&usdCource);
	//Fill(&euroCource);
	//Fill(&phoundCource);

	//showArrays (& usdCource);
	//showArrays (& euroCource);
	//showArrays (& phoundCource);

	

	tr1.join(); // ���������� ���� �������� ����� ����������.
	thread tr4(print, usdCource.begin(), usdCource.begin() + 100, "USD ");
	thread tr5(
		[&tr2, &euroCource]() {
			tr2.join();
			print(euroCource.begin(), euroCource.begin() + 100, "Euro ");
		}
	);

	tr3.join();
	print(
		phoundCource.begin(), phoundCource.begin() + 100, "Phound ");
		
	tr5.join();
	tr4.join();

	return 0;
}