
template <class T>
class Node
{
public:
	T value;
	Node* next;
	Node* previous;

	Node();

	~Node();
};

template <class T>
class List
{
public:
	Node<T>* head;
	Node<T>* tail;

	List();
	~List();

	int Length();

	void Add(T value);

	void AddAfter(T value, int index);

	T Get(int index);

	void RemoveAt(int index);

	void PrintAll(bool reverse = false);
};
template<class T>
Node<T>::Node()
{
	next = NULL;
	previous = NULL;
}

template<class T>
Node<T>::~Node()
{

}

template<class T>
List<T>::List()
{
	head = NULL;
	tail = NULL;
}

template<class T>
List<T>::~List()
{

}
template<class T>
int List<T>::Length()
{
	Node<T>* node = head;
	int count = 0;
	while (node)
	{
		count++;
		node = node->next;
	}
	return count;
}

template<class T>
void List<T>::Add(T value)
{
	Node<T>* node = new Node<T>();
	node->value = value;

	if (!tail)
	{
		head = node;
		tail = node;
	}
	else
	{
		tail->next = node;
		node->previous = tail;
		tail = node;
	}
}

template<class T>
void List<T>::AddAfter(T value, int index)
{
	int length = Length();

	if (length <= index)
		return; //error

	if (length == index - 1)
		Add(value);

	Node<T>* node = head;
	int count = 0;
	while (count < index)
	{
		count++;
		node = node->next;
	}
	Node<T>* n = new Node();
	n->value = value;
	node->next->previous = n;
	n->next = node->next;
	node->next = n;
	n->previous = node;

}

template<class T>
T List<T>::Get(int index)
{
	int length = Length();

	if (length <= index)
		return NULL; //error

	Node<T>* node = head;
	int count = 0;
	while (count < index)
	{
		count++;
		node = node->next;
	}
	return node->value;

}

template<class T>
void List<T>::RemoveAt(int index)
{
	int length = Length();
	if (index >= length)
		return;//error

	if (index == 0)
	{
		Node<T>* node = head;
		head = head->next;
		delete node;
		return;
	}

	Node<T>* node = head;
	for (int i = 0; i < index; node = node->next, i++);

	if (index == length - 1)
	{
		node->previous->next;
		delete node;
	}
	else
	{
		node->next->previous = node->previous;
		node->previous->next = node->next;
		delete node;
	}
}

template<class T>
void List<T>::PrintAll(bool reverse)
{
	if (!reverse)
	{
		Node<T>* node = head;
		int i = 0;
		while (node)
		{
			cout << "[" << i++ << "] = " << node->value << endl;
			node = node->next;
		}
	}
	else
	{
		Node<T>* node = tail;
		int i = Length() - 1;
		while (node)
		{
			cout << "[" << i-- << "] = " << node->value << endl;
			node = node->previous;
		}
	}
}