#include <iostream>
#include <fstream> //files  
#include <list>
#include <sstream>

template<typename T = std::string>
class List
{
	//variables
private:
	int m_size = 0;

	struct node
	{
		T my_Data;
		int number;

		node* previous = nullptr;
		node* next = nullptr;

		node& operator = (const node& other)
		{
			this->my_Data = other.my_Data;
			this->number = other.number;

			return *this;
		}
	};

	node* front_element = nullptr;
	node* back_element = nullptr;

	//functions
private:

	node* createNewNode(const T& element, int number = 0)
	{
		node* tmp = new node();;
		tmp->my_Data = element;
		tmp->number = number;

		return tmp;
	}

	void swap(node* a, node* b) { node tmp = *a; *a = *b; *b = tmp; }

	node* partition(node* last, node* head, bool ascending = true)
	{
		int x = head->number;

		node* i = last->previous;

		for (node* j = last; j != head; j = j->next)
		{
			if (j->number <= x && ascending)
			{
				i = (i == NULL) ? last : i->next;

				swap(i, j);
			}
			else if (j->number >= x && !ascending)
			{
				i = (i == NULL) ? last : i->next;

				swap(i, j);
			}
		}

		i = (i == NULL) ? last : i->next;
		swap(i, head);

		return i;
	}

	void quickSort(node* last, node* head, bool ascending = true)
	{
		if (head != NULL && last != head && last != head->next)
		{
			node* p = partition(last, head, ascending);
			quickSort(last, p->previous, ascending);
			quickSort(p->next, head, ascending);
		}
	}

public:

	List<T> operator = (std::ifstream& myFile)
	{
		if (m_size != 0) { clear(); }

		std::string line;
		std::string subStr;
		int value;

		if (myFile.is_open())
		{
			while (std::getline(myFile, line))
			{
				std::reverse(line.begin(), line.end());

				subStr = line.substr(0, line.find(' '));

				line.erase(0, subStr.length());

				std::reverse(line.begin(), line.end());
				std::reverse(subStr.begin(), subStr.end());

				value = std::stoi(subStr);

				push_back(line, value);
			}
		}
		return *this;
	}

	List<T> operator=(const List<T>& other)
	{
		if (this != &other)
		{
			if (m_size != 0)
			{
				clear();
			}
		}

		node* tmp = other.front_element;
		while (tmp != nullptr)
		{
			push_back(tmp->my_Data, tmp->number);
			tmp = tmp->next;
		}

		return *this;
	}

	List(const List<T>& other)
	{
		if (m_size != 0)
		{
			clear();
		}

		node* tmp = other.front_element;
		while (tmp != nullptr)
		{
			push_back(tmp->my_Data, tmp->number);
			tmp = tmp->next;
		}
	}

	List() { m_size = 0; }

	int Size() const { return m_size; }

	void sortAscending() { quickSort(front_element, back_element); }

	void sortDescending() { quickSort(front_element, back_element, false); }

	T atData(int index) const
	{
		node* tmp = front_element;

		for (int i = 0; i < index; ++i)
		{
			tmp = tmp->next;
		}

		return tmp->my_Data;
	}

	int atNumber(int index) const
	{
		node* tmp = front_element;

		for (int i = 0; i < index; ++i)
		{
			tmp = tmp->next;
		}

		return tmp->number;
	}

	bool empty() { return m_size == 0 ? true : false; }

	void writeToFile(std::string fileName) {
		std::ofstream file(fileName);


		if (file.is_open()) {
			for (node* tmp = front_element; tmp != nullptr; tmp = tmp->next) {
				file << tmp->my_Data << " " << tmp->number << std::endl;
			}

			file.close();
		}
		else {
			throw std::runtime_error("Failed to open file: " + fileName);
		}
	};

	T back() { return back_element->my_Data; }

	T front() { return front_element->my_Data; }

	void push_back(const T& element, int number = 0)
	{
		node* toAdd = createNewNode(element, number);
		if (m_size == 0)
		{
			back_element = toAdd;
			front_element = toAdd;
			++m_size;
			return;
		}
		back_element->next = toAdd;
		toAdd->previous = back_element;
		back_element = toAdd;
		++m_size;
	}

	void push_front(const T& element, int number = 0)
	{
		node* current = front_element;
		front_element = createNewNode(element, number);
		front_element->next = current;
		if (current != nullptr)
		{
			current->previous = front_element;
		}
		if (m_size == 0)
		{
			back_element = front_element;
		}

		++m_size;
	}

	void  pop_front()
	{
		if (m_size == 0) { return; }

		--m_size;
		if (front_element->next == nullptr)
		{
			delete(front_element);
			return;
		}
		node* current = front_element->next;

		delete(front_element);
		front_element = current;
	}

	void pop_back()
	{
		if (m_size == 0) { return; }

		--m_size;
		if (front_element->previous == nullptr)
		{
			delete(back_element);
			return;
		}
		node* current = back_element->previous;

		delete(back_element);
		current->next = nullptr;
		back_element = current;
	}

	void merge(const List<T>& other)
	{
		node* tmp = other.front_element;
		while (tmp != nullptr)
		{
			push_back(tmp->my_Data, tmp->number);
			tmp = tmp->next;
		}
	}

	void clear()
	{
		node* current = back_element;
		node* previous = back_element->previous;

		while (current->previous != nullptr)
		{
			delete(current);
			current = previous;
			previous = current->previous;
		}
		delete(previous);
		delete(front_element);
		front_element = nullptr;
		back_element = nullptr;
		m_size = 0;
	}

	~List()
	{
		clear();
	}
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const List<T>& other)
{
	for (int i = 0; i < other.Size(); ++i)
	{
		stream << other.atData(i) << " " << other.atNumber(i) << std::endl;
	}

	return stream;
}

template<typename T = std::string>
std::ofstream& operator<<(std::ofstream& stream, const List<T>& other)
{
	for (int i = 0; i < other.Size(); ++i)
	{
		stream << other.atData(i) << " " << other.atNumber(i) << std::endl;
	}

	return stream;
}