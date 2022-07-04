#include <iostream>
#include <thread>
#include <chrono> // для управления временем
#include <vector>
#include <string_view> // Добавилась в 17ом стандарте. Создать просмотрщик любой строки
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
		//mt.lock(); // Блокирует поток
		lock_guard lg(mt);
		std::cout << name; // Отомарные операции (вещи, которые выполняются в разных потоках)
		std::cout << "Value "; // Отомарные операции (вещи, которые выполняются в разных потоках)
		std::cout << *start;  // Отомарные операции (вещи, которые выполняются в разных потоках)
		std::cout << endl; // Отомарные операции (вещи, которые выполняются в разных потоках)

		//mt.unlock(); // Разблокировка потока
		++start;
	}
}

// многопоточность в приложениях

// управляющий поток
// атомарные операции
// гонки данных

int main() {

	std::this_thread::get_id(); // Возвращает ID процесса в диспетчере задач.
	std::this_thread::sleep_for(milliseconds(10)); // На какой временной промежуток потоку поспать.
	// Резервация памяти (выделение памяти под массивы)
	vector<double> usdCource;
	usdCource.reserve(540);
	vector<double> euroCource;
	euroCource.reserve(540);
	vector<double> phoundCource;
	phoundCource.reserve(540);
	//std::this_thread::sleep_until(); // Ждёт системного события
	
	thread tr1(Fill, &usdCource); // Отправляется работа в другой поток. Сейчас у нас 2 потока выполнения
	thread tr2(Fill, &euroCource);
	thread tr3(Fill, &phoundCource);
	// Заполняем массивы
	//Fill(&usdCource);
	//Fill(&euroCource);
	//Fill(&phoundCource);

	//showArrays (& usdCource);
	//showArrays (& euroCource);
	//showArrays (& phoundCource);

	

	tr1.join(); // дожидается пока основной поток завершится.
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