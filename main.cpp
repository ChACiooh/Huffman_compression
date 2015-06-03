#include <iostream>
#include <string>
#include "DS.h"

using namespace std;

string input;
bool flag = false;

void InputInHash(Hash& myHash);
void ConstructMinHeap(min_heap& myMinHeap, const Hash& myHash);
void ConstructHuffmanTree(min_heap& myMinHeap);
void InorderTraversalHuffman(Huffman* hf);
void GetCode(string& output, Huffman *root, const char key);

void Encode(Huffman *root)
{
	FILE *fp = fopen("output.txt", "w");
	unsigned char buf = 0x0, temp = 0x0;
	for (int i = 0; input[i] != '\0'; i++)
	{
		string output;
		GetCode(output, root, input[i]);
		flag = false;

	}
}

int main()
{
    min_heap myMinHeap;
    Huffman* temp_hf = NULL;
    Hash myHash;
    Data *data_ptr;

    InputInHash(myHash);
    ConstructMinHeap(myMinHeap, myHash);
    ConstructHuffmanTree(myMinHeap);
	temp_hf = myMinHeap.pop();

	system("PAUSE");
	
    return 0;
}


void InputInHash(Hash& myHash)
{
	FILE *fp = fopen("input.txt", "rb");
	unsigned char buf = 0;
	while(!feof(fp) && (fscanf(fp, "%c", &buf)) != EOF && buf != 0xFF) {
		myHash.PushLetter(buf);
		input.push_back(buf);
	}
	cout << input << " is read." << endl;
	fclose(fp);
}

void ConstructMinHeap(min_heap& myMinHeap, const Hash& myHash)
{
	for(int i=0;i<HASH_SIZE;i++)
	{
		if(myHash.get_num(i) == 0)	continue;
		Data *temp = myHash.GetData(i);
		Huffman* hf = new Huffman(*temp); // 값을 복사하므로 안전하게 전달될 듯
		myMinHeap.Insert(hf);
		hf = NULL;
		delete(temp);
	}
}

void ConstructHuffmanTree(min_heap& myMinHeap)
{
	Huffman *hf1 = NULL, *hf2 = NULL, *root = NULL;
	do
	{
		hf1 = myMinHeap.pop();
		hf2 = myMinHeap.pop();
		root = new Huffman();
		root->stemL = hf1;
		root->stemR = hf2;
		root->num = hf1->num + hf2->num;
		myMinHeap.Insert(root);
		hf1 = hf2 = root = NULL;
	} while(myMinHeap.size() > 1);
}

void InorderTraversalHuffman(Huffman* hf)
{
	if(hf == NULL)	return;
	InorderTraversalHuffman(hf->stemL);
	if(hf->stemL == NULL && hf->stemR == NULL) // this is leaf node.
	{
		printf("%c %d\n", hf->datum.letter, hf->datum.freq);
	}
	else
	{
		printf("num : %d\n", hf->num);
	}
	InorderTraversalHuffman(hf->stemR);
}

void GetCode(string& output, Huffman *root, const char key)
{
	// DFS 방식으로 탐색하면서 output에 저장해나간다
	if (root->stemL == NULL && root->stemR == NULL)
	{
		// leaf node렷다.
		if (key == root->datum.letter) {
			flag = true;
		}
		return;
	}
	if (root->stemL)
	{
		output.push_back('0');
		GetCode(output, root->stemL, key);
		if (!flag) {
			output.pop_back();
		}
	}
	if (flag)	return;
	if (root->stemR)
	{
		output.push_back('1');
		GetCode(output, root->stemR, key);
		if (!flag){
			output.pop_back();
		}
	}
}