#include <iostream>
#include "DS.h"

using namespace std;

bool flag = false;

/* declarations */
void InputInHash(Hash& myHash, LinkedList& ll);
void ConstructMinHeap(min_heap& myMinHeap, const Hash& myHash);
void ConstructHuffmanTree(min_heap& myMinHeap);
void InorderTraversalHuffman(Huffman* hf); // ������ Ʈ�� Ž���ϴ� �Լ�. ������.
void GetCode(const Huffman*, unsigned char&, const char, int& cnt);
void Encode(LinkedList& ll, const Huffman* hf);
void Decode(LinkedList& ll, const Huffman* hf);

int main()
{
	/// ������ ������!
    min_heap myMinHeap;
    Huffman* complete_hf = NULL;
    Hash myHash;
	LinkedList ll;

	/// ������ ��!
    InputInHash(myHash, ll); // �ؽ� ���̺��� �����, �ϼ��� ������ ll�� ����.
    ConstructMinHeap(myMinHeap, myHash); // �ؽ� ���̺��� ���� �켱���� ť�� ����.
    ConstructHuffmanTree(myMinHeap); // ����� ť�� ���� ������ Ʈ���� ����.
	complete_hf = myMinHeap.pop(); // �ϼ��� ������ Ʈ���� �����°���.
	//InorderTraversalHuffman(complete_hf);
	Encode(ll, complete_hf);
	ll.~LinkedList();
	Decode(ll, complete_hf);
	
	/// ������!
	system("PAUSE");
    return 0;
}

/* definitions */
void InputInHash(Hash& myHash, LinkedList& ll)
{
	FILE *fp = fopen("input.txt", "rb");
	char buf = 0;
	while(!feof(fp) && (fscanf(fp, "%c", &buf)) != EOF && buf != 0xFF) {
		myHash.PushLetter(buf);
		ll.push_back(buf);
	}
	fclose(fp);
}

void ConstructMinHeap(min_heap& myMinHeap, const Hash& myHash)
{
	for(int i=0;i<HASH_SIZE;i++)
	{
		if(myHash.get_num(i) == 0)	continue;
		Data *temp = myHash.GetData(i);
		Huffman* hf = new Huffman(*temp); // ���� �����ϹǷ� �����ϰ� ���޵� ��
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

void GetCode(const Huffman* hf, unsigned char& base, const char check, int& cnt)
{
	unsigned char temp = 0x0;
	if (hf == NULL)	return;
	if (hf->stemL == NULL && hf->stemR == NULL && check == hf->datum.letter)
	{
		// leaf node�̰� üũ�ϰ��� �ϴ� ���ڿ� ����.
		flag = true;
		return;
	}
	else
	{
		if (hf->stemL)
		{
			base = base << 1;
			base = base | temp;
			GetCode(hf->stemL, base, check, ++cnt);
			if (!flag) { // ������ ���� �ʾ����� ������� �ǵ�����
				base = base >> 1;
				cnt--;
			}
		}
		if (flag)	return;
		if (hf->stemR)
		{
			base = base << 1;
			temp = 1;
			base = base | temp;
			GetCode(hf->stemR, base, check, ++cnt);
			if (!flag){ // ������ ���� �ʾ����� ������� �ǵ�����
				base = base >> 1;
				cnt--;
			}
		}
	}
}

void Encode(LinkedList& ll, const Huffman* hf)
{
	unsigned char buf = 0x0;
	int remained = sizeof(char) * 8;
	FILE *fp = fopen("output.txt", "wb");
	fprintf(fp, "%d\n", ll.size);
	for (int i = 0; i < ll.size; i++)
	{
		char letter = ll[i];
		unsigned char temp = 0x0;
		int cnt = 0;
		flag = false;
		GetCode(hf, temp, letter, cnt); // hf�� ������� temp�� �����ϸ� letter�� ���ϰ� cnt�� ���̸� �ǹ��Ѵ�.
		for (int j = 0; j < sizeof(char)*8 - cnt; j++)	temp = temp << j;
		remained -= cnt;
		if (remained == 0)
		{
			fprintf(fp, "%c", buf);
			buf = 0x0;
			remained = sizeof(char) * 8 - cnt;
		}
		else if (remained < 0) // shifting ó���� ������ �� �־�� �Ѵ�.
		{
			buf = buf | (temp >> -remained);
			fprintf(fp, "%c", buf);
			buf = 0x0;
			remained = sizeof(char) * 8 + remained;
		}
		buf = buf | temp;
		buf = buf << remained;
	}
	if (buf)
	{
		// remained�� ���� ����ŭ �׿� 0�� �ִ�.
		fprintf(fp, "%c", buf);
	}
	fclose(fp);
}

void Decode(LinkedList& ll, const Huffman* hf)
{
	// ll�� ����� �����Ѵ�ϴ� ���~
	unsigned char base = 0x0;

}