#include "DS.h"

/// Hash
void Hash::PushLetter(char letter)
{
	int idx = 0;
	if('A' <= letter && letter <= 'Z')	idx = (letter - 'A');
	else if('a' <= letter && letter <= 'z')	idx = gap + (letter - 'a');
	else
	{
		if(letter == ' ')	idx = gap*2;
		else if(letter == ',')	idx = gap*2 + 1;
		else if(letter == '.')	idx = gap*2 + 2;
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
	case 52:
		letter = ' ';
		break;
	case 53:
		letter = ',';
		break;
	case 54:
		letter = '.';
		break;
	}

	return new Data(letter, data_[idx]);
}

/// Huffman
Huffman::Huffman(const Huffman& new_hf)
{
	stemL = new_hf.stemL;
	stemR = new_hf.stemR;
	num = new_hf.num;
}
Huffman::~Huffman()
{
	free_tree();
	num = 0;
}

void Huffman::free_tree()
{
	if(stemL) {
		stemL->free_tree();
	}
	free(stemL);
	if(stemR) {
		stemR->free_tree();
	}
	free(stemR);
}

/// min_heap
min_heap::min_heap(int _size_) : size_(_size_)
{
	data_ = new Huffman[_size_+1]; // numbering from 1
	for(int i=0;i<=_size_;i++){
		data_[i] = Huffman();
	}
}

min_heap::~min_heap()
{
	free(data_);
}

void min_heap::resize(int _size_)
{
	Huffman *new_data = NULL;
	if(_size_ == alloc_)	return;
	else
	{
		new_data = new Huffman[_size_+1];
		// numbering from 1
		if(_size_ < size_)
		{
			for(int i=1;i<=_size_;i++)	new_data[i] = data_[i];
			size_ = _size_; // 줄어들었으므로.
		}
		else if(_size_ > alloc_)
		{
			for(int i=1;i<=size_;i++)	new_data[i] = data_[i];
			alloc_ = _size_;
		}
		free(data_);
		data_ = new_data;
	}
}

Huffman min_heap::operator[](int idx) const
{
	return data_[idx];
}

void min_heap::Insert(Huffman hf)
{
	if(size_ == alloc_)	resize(size_+1);
	data_[++size_] = hf;
	int parent, child;
	Huffman temp;

	child = size_;
	parent = child/2;
	// shift up
	while(child > 1)
	{
		if(data_[parent].num > data_[child].num)
		{
			temp = data_[child];
			data_[child] = data_[parent];
			data_[parent] = temp;
			child = parent;
			parent = child/2;
		}
		else	break;
	}
}

Huffman min_heap::pop()
{
	int parent = 1, child = 2;
	Huffman temp = data_[parent];
	data_[parent] = data_[size_];
	data_[size_--] = temp;
	// returning hf is in size_+1

	// shift down
	while(child <= size_)
	{
		if(data_[child].num > data_[child+1].num)	++child;
		if(data_[parent].num > data_[child].num)
		{
			temp = data_[child];
			data_[child] = data_[parent];
			data_[parent] = temp;
			parent = child;
			child = parent * 2;
		}
		else	break;
	}
	return data_[size_+1];
}
