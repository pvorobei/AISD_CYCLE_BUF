#include <iostream>
#include <vector>
#include <string>
#include <windows.h>

class CycleBuf
{
public:
	CycleBuf(size_t size) : buffer(size), maxSize(size), head(0), tail(0), full(false) {}

	void add(const std::vector<std::string> &items)
	{
		for (const auto &item : items)
		{
			if (full)
			{
				std::cout << "Буфер забит! " << item << "\n";
				continue; // Пропускаем добавление, если буфер забит
			}
			buffer[head] = item;
			head = (head + 1) % maxSize; // Перемещаем голову
			full = head == tail;		 // Проверяем, забит ли буфер
		}
	}

	std::vector<std::string> get(int count)
	{
		std::vector<std::string> items;
		for (int i = 0; i < count; ++i)
		{
			if (isEmpty())
			{
				items.push_back("Буфер пуст!");
				break;
			}
			items.push_back(buffer[tail]);
			tail = (tail + 1) % maxSize; // Перемещаем хвост
			full = false;
		}
		return items;
	}

	bool isEmpty() const
	{
		return (!full && (head == tail));
	}

	bool isFull() const
	{
		return full;
	}

	void printBuffer() const
	{
		std::cout << "Содержимое буфера: ";
		for (size_t i = 0; i < maxSize; ++i)
		{
			if (!isEmpty() && ((tail <= head && i >= tail && i < head) || (tail > head && (i >= tail || i < head))))
			{
				std::cout << "[" << buffer[i] << "] "; // Отображаем элемент на позиции i
			}
			else
			{
				std::cout << "[  ] "; // Два пробела вместо "Пусто"
			}
		}
		std::cout << "\n";
	}

private:
	std::vector<std::string> buffer;
	size_t maxSize;
	size_t head;
	size_t tail;
	bool full;
};

void setConsoleUTF8()
{
	// Устанавливаем кодировку консоли на UTF-8
	SetConsoleOutputCP(CP_UTF8);
}

int main()
{
	setConsoleUTF8(); // Устанавливаем кодировку UTF-8 для консоли

	CycleBuf cb(5); // Создаем кольцевой буфер размером 5

	// По приколу используем смайлы)
	std::vector<std::string> emojis = {"🧐", "😎", "😄", "😁", "😆", "😅", "🥴", "🤣", "😊", "🤓"};

	// Добавляем элементы в буфер
	for (int i = 0; i < 7; ++i)
	{
		std::vector<std::string> itemsToAdd;

		// Добавляем два смайла из списка
		itemsToAdd.push_back(emojis[i % emojis.size()]);
		itemsToAdd.push_back(emojis[(i + 1) % emojis.size()]);

		cb.add(itemsToAdd); // Добавляем несколько элементов
		for (const auto &item : itemsToAdd)
		{
			std::cout << "Добавлено: " << item << "\n";
		}

		cb.printBuffer(); // Печатаем содержимое буфера после добавления

		// Извлекаем 1 или 2 элемента из буфера и показываем их
		int countToGet = (i % 2 == 0) ? 2 : 1; // Извлекаем 1 элемент на нечетных итерациях и 2 на четных
		auto extractedItems = cb.get(countToGet);

		for (const auto &extractedItem : extractedItems)
		{
			std::cout << "Извлечено: " << extractedItem << "\n";
		}

		cb.printBuffer(); // Печатаем содержимое буфера после извлечения
		std::cout << "\n";
	}

	// Извлекаем оставшиеся элементы из буфера
	std::cout << "Финальное извлечение из буфера:\n";
	while (!cb.isEmpty())
	{
		auto finalItems = cb.get(1);
		for (const auto &finalItem : finalItems)
		{
			std::cout << "Получено: " << finalItem << "\n";
		}
	}

	return 0;
}
