#include "DS.h"
#include <iostream>

/// 연결리스트
void LinkedList::push_back(char letter)
{
	List **temp = &root; // 포인터들이 모두 연관되어 있으므로, 더블 포인터를 사용하여야 함.
	if (!(*temp))
	{
		*temp = new List(letter);
		this->size++;
		return;
	}
	while ((*temp)->next)	temp = &((*temp)->next);
	(*temp)->next = new List(letter);
	this->size++;
}

List* LinkedList::pop_back()
{
	List **temp = &root; // 포인터들이 모두 연관되어 있으므로, 더블 포인터를 사용하여야 함.
	if (!(*temp))	return NULL;
	while ((*temp)->next)	temp = &(*temp)->next;
	List *return_val = new List((*temp)->letter);
	delete(*temp);
	*temp = NULL;
	temp = NULL;
	this->size--;
	return return_val;
}

List* LinkedList::pop_front()
{
	if (!root)	return NULL;
	List* return_val = root;
	root = root->next;
	this->size--;
	return return_val;
}

LinkedList::~LinkedList()
{
	List *temp = root;
	while (temp)
	{
		List *removed = temp;
		temp = temp->next;
		delete(removed);
	}
}

char LinkedList::operator [](int idx)
{
	List *temp = root;
	for (int i = 0; i < idx && temp->next; i++) {
		temp = temp->next;
	}
	return temp->letter;
}

/// Hash
void Hash::PushLetter(char letter)
{
	int idx = 0;
	if('A' <= letter && letter <= 'Z')	idx = (letter - 'A');
	else if('a' <= letter && letter <= 'z')	idx = gap + (letter - 'a');
	else
	{
		if (letter == ' ')	idx = gap * 2;
		else if (letter == ',')	idx = gap * 2 + 1;
		else if (letter == '.')	idx = gap * 2 + 2;
		else if (letter == '#')	idx = gap * 2 + 3; // 종결 코드
		else{
			fprintf(stderr, "unexpected charactor.\n");
			return;
		}
	}
	data_[idx]++;
}

Data* Hash::GetData(int idx) const
{
	char letter;
	if(idx < 0 || idx >= HASH_SIZE)	return NULL;
	if(0 <= idx && idx < 26)
	{
		letter = 'A' + idx;
	}
	else if(26 <= idx && idx < 52)
	{
		letter = 'a' + idx - 26;
	}
	else switch(idx)
	{
	case gap*2:
		letter = ' ';
		break;
	case gap*2+1:
		letter = ',';
		break;
	case gap*2+2:
		letter = '.';
		break;
	case gap*2+3:
		letter = '#';
	}

	return new Data(letter, data_[idx]);
}

/// Huffman
Huffman::~Huffman()
{
	num = 0;
}
/*
void Huffman::free_tree() // 알아서 call destructor recursively
{
	if(stemL) {
		try { delete(stemL); }
		catch (std::exception e) {}
	}
	if(stemR) {
		try { delete(stemR); }
		catch (std::exception e) {}
	}
}*/

/// min_heap
min_heap::min_heap(int _size_) : size_(_size_)
{
	data_ = (Huffman**)calloc(sizeof(Huffman*), _size_+1);
	alloc_ = size_;
}

min_heap::~min_heap()
{
	delete[](data_);
}

void min_heap::resize(int _size_)
{
	Huffman **new_data = NULL;
	if(_size_ == alloc_)	return;
	else
	{
		new_data = new Huffman*[_size_+1];
		for (int i = 0; i <= _size_; i++)
		{
			new_data[i] = new Huffman();
		}
		// numbering from 1
		if(_size_ < size_)
		{
			for(int i=1;i<=_size_;i++)	new_data[i] = data_[i];
			for (int i = _size_; i <= size_; i++)	delete(data_[i]);
			size_ = _size_; // 줄어들었으므로.
		}
		else if(_size_ > alloc_)
		{
			for(int i=1;i<=size_;i++)	new_data[i] = data_[i];
			alloc_ = _size_;
		}
		data_ = new_data;
	}
}

void min_heap::Insert(Huffman* hf)
{
	if (size_ == alloc_){
		resize(size_ + 1);
	}
	data_[++size_] = hf;
	int child = size_;
	int parent = child / 2;
	while (child > 1)
	{
		if (data_[parent]->num > data_[child]->num)
		{
			Huffman* temp = data_[parent];
			data_[parent] = data_[child];
			data_[child] = temp;
		}
		else   break;
	}
}

Huffman* min_heap::pop()
{
	if (empty())	return NULL;
	Huffman *return_val = data_[1];
	Huffman *temp = data_[size_];
	data_[size_] = data_[1];
	data_[1] = temp;
	data_[size_--] = NULL;
	int parent = 1, child = 2;

	// shift down
	while (child <= size_)
	{
		if (child < size_ && data_[child]->num > data_[child + 1]->num)	++child;
		if (data_[parent]->num > data_[child]->num)
		{
			temp = data_[parent];
			data_[parent] = data_[child];
			data_[child] = temp;
			parent = child;
			child = parent * 2;
		}
		else    break;
	}

	return return_val;
}

Huffman* min_heap::operator[](int idx)
{
	return data_[idx];
}

bool min_heap::empty() const
{
	return size_ == 0;
}
