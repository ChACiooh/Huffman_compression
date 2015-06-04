#ifndef DS_H_INCLUDED
#define DS_H_INCLUDED

/**< Data Structures > **/

#include <cstdio>
#include <cstdlib>

#define SPEC_LETTERS	4 // ',', '.', ' ', '#'
#define HASH_SIZE	(26*2+SPEC_LETTERS)

/// 연결리스트
struct List
{
	List(char new_letter) : letter(new_letter)
	{
		next = NULL;
	}
	char letter;
	List *next;
};

struct LinkedList
{
	LinkedList() : size(0)
	{
		root = NULL;
	}
	~LinkedList();

	void push_back(char);
	List* pop_back();
	List* pop_front();
	char operator [](int idx);
	int size;
	List *root;
};

/// 기본적인 노드
struct Data
{
	Data() { letter = freq = 0; }
	Data(char a, int b)
	{
		letter = a;
		freq = b;
	}

	// data area
	char letter;
	int freq; // frequency
};


/// 허프만 노드
struct Huffman
{
	Huffman() { stemL = stemR = NULL; num = 0; }
	Huffman(Data new_datum) { stemL = stemR = NULL; datum = new_datum; num = new_datum.freq; }
	~Huffman();
	//void free_tree();

	// data area
	Data datum;
	Huffman *stemL, *stemR;
	int num;
};

/// priority_queue
class min_heap
{
public:
	min_heap() { data_ = NULL; alloc_ = size_ = 0; }
	min_heap(int _size_);
	~min_heap();

	const int& size() const { return size_; }
	void resize(int _size_);
	void Insert(Huffman*);
	bool empty() const;
	Huffman* pop();
	Huffman* operator[](int);
private:
	int alloc_; // the number of storages
	int size_; // the number of items
	Huffman** data_; // array which has Huffman structures.
};

/// 해시
class Hash
{
public:
	const static int gap = 'z' - 'a' + 1;
	Hash()
	{
		for(int i=0;i<HASH_SIZE;i++)	data_[i] = 0;
	}

	Data* GetData(int idx) const;
	void PushLetter(char letter);
	const int& get_num(int idx) const
	{
		return data_[idx];
	}
private:
	int data_[HASH_SIZE];
};


#endif // DS_H_INCLUDED
