#ifndef DS_H_INCLUDED
#define DS_H_INCLUDED

/**< Data Structures > **/

#include <cstdio>
#include <cstdlib>

#define SPEC_LETTERS	3 // ',', '.', ' '
#define HASH_SIZE	(26*2+SPEC_LETTERS)

struct Data
{
	Data() {}
	Data(char a, int b)
	{
		letter = a;
		freq = b;
	}

	// data area
	char letter;
	int freq; // frequency
	Data *left, *right;
};

struct Huffman
{
	Huffman() { stemL = stemR = NULL; num = 0; }
	Huffman(const Huffman&);
	~Huffman();
	void free_tree();
	void link(Data*);

	// data area
	Data left, right;
	Huffman *stemL, *stemR;
	int num;
};

class min_heap
{
public:
	min_heap() { data_ = NULL; }
	min_heap(int _size_);
	~min_heap();

	const int& size() const { return size_; }
	void resize(int _size_);
	void Insert(Huffman);
	Huffman pop();
	Huffman operator[](int) const; // get only, cannot edit
private:
	int alloc_; // the number of storages
	int size_; // the number of items
	Huffman* data_; // array which has Huffman structures.
};

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
