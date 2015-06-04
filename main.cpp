#include <iostream>
#include "DS.h"

using namespace std;

bool flag = false;
int original_byte, after_byte;

/* declarations */
void InputInHash(Hash& myHash, LinkedList& ll);
void ConstructMinHeap(min_heap& myMinHeap, const Hash& myHash);
void ConstructHuffmanTree(min_heap& myMinHeap);
void InorderTraversalHuffman(Huffman* hf); // ������ Ʈ�� Ž���ϴ� �Լ�. ������.
void GetCode(LinkedList& outLL, const Huffman* hf, const char& key);
void Encode(LinkedList& ll, const Huffman* hf);
void Decode(const Huffman* hf);

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
	printf("\n����� : %g\n", (double)after_byte / (double)original_byte * 100.0);
	
	/// ������!
	Decode(complete_hf);
	system("PAUSE");
    return 0;
}

/* definitions */
void InputInHash(Hash& myHash, LinkedList& ll)
{
	// ll�� �ϼ��� ������ ��´�.
	FILE *fp = fopen("input.txt", "rb");
	char buf = 0;
	bool check = false;
	while(!feof(fp) && fscanf(fp, "%c", &buf) != EOF)
	{
		if (buf == 0xFF)
		{
			// ������ ���κ�
			// ��Ȥ EOF�� �д� ��츦 ���� ���� ó���� �� ��.
			myHash.PushLetter('#');
			ll.push_back('#');
			check = true;
			break;
		}
		myHash.PushLetter(buf);
		ll.push_back(buf);
		original_byte++;
	}
	if (!check)
	{
		// ������ ���κ�
		myHash.PushLetter('#');
		ll.push_back('#');
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
	// Huffman Tree�� ����� ����
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

void GetCode(LinkedList& outLL, const Huffman* hf, const char& key)
{
	// �ƹ��� ���� ��Ͱ� �ʹ� ����.
	// outLL���� ������ ��Ʈ �ڵ带 ���⵵�� �Ѵ�.
	if (hf == NULL)	return;
	if (hf->stemL == NULL && hf->stemR == NULL)
	{
		if (hf->datum.letter == key)
		{
			flag = true;
			return;
		}
	}
	else
	{
		if (hf->stemL) // ������ 0
		{
			outLL.push_back('0');
			GetCode(outLL, hf->stemL, key);
			if (!flag)
			{
				List* temp = outLL.pop_back();
				if (temp)	delete(temp);
			}
		}
		if (flag)	return;
		if (hf->stemR) // �������� 1
		{
			outLL.push_back('1');
			GetCode(outLL, hf->stemR, key);
			if (!flag)
			{
				List* temp = outLL.pop_back();
				if (temp)	delete(temp);
			}
		}
	}
}

void Encode(LinkedList& ll, const Huffman* hf)
{
	List* ll_ptr = ll.root;
	FILE *fp = fopen("output.txt", "wb");
	int bit_shift_remained = 8;
	unsigned char buf = 0x0;

	puts("Encoding...");
	while (ll_ptr)
	{
		LinkedList outLL; // ���� �ϳ����� �ڵ带 �Է¹ޱ� ���� �ݺ��� ó���� �ݺ������� ����
		flag = false;
		GetCode(outLL, hf, ll_ptr->letter); // outLL�� �ڵ尡 �����
		int now_size = outLL.size;
		for (int i = 0; i < now_size; i++)
		{
			List* temp_ptr = outLL.pop_front();
			unsigned char base = temp_ptr->letter - '0';
			//printf("%d", base);
			delete(temp_ptr);
			buf = buf << 1;
			buf = buf | base;
			bit_shift_remained--;
			if (bit_shift_remained == 0)
			{
				bit_shift_remained = 8;
				fprintf(fp, "%c", buf);
				//printf("\n");
				after_byte++;
				buf = 0x0;
			}
		}
		ll_ptr = ll_ptr->next;
	}
	if (bit_shift_remained != 8)
	{
		buf = buf << bit_shift_remained;
		fprintf(fp, "%c", buf);
		after_byte++;
	}
	puts(" Encoding end!");
	fclose(fp);
}

void Decode(const Huffman* hf)
{
	FILE *fp = fopen("output.txt", "rb");
	FILE *ofp = fopen("recoveredinput.txt", "w"); // �������� �����ϱ� ���� wb ��� w�� ���.
	char recoverd = 0x0;
	unsigned char buf = 0x0;
	int bit_shifting = 7;
	LinkedList tempLL;
	while (!feof(fp) && fscanf(fp, "%c", &buf) != EOF)
	{
		for (int i = 0; i <= bit_shifting; i++)
		{
			tempLL.push_back((buf & 0x80) >> bit_shifting);
			buf = buf << 1;
		}
	}
	fclose(fp);

	List *list_ptr = tempLL.root;
	const Huffman *const*pptr = &hf;
	// ��ȸ �� �� �غ���
	/*int cnt = 8;
	puts("Decoding...");
	while (list_ptr)
	{
		printf("%d", list_ptr->letter);
		cnt--;
		if (cnt == 0)
		{
			cnt = 8;
			puts("");
		}
		list_ptr = list_ptr->next;
	}*/
	
	list_ptr = tempLL.root;

	while (list_ptr)
	{
		if ((*pptr)->stemL == NULL && (*pptr)->stemR == NULL)
		{
			if ((*pptr)->datum.letter == '#')
			{
				cout << "���� �ڵ� ����." << endl;
				puts("Decoding end!");
				cout << "���α׷� ����." << endl;
				break;
			}
			fprintf(ofp, "%c", (*pptr)->datum.letter);
			pptr = &hf;
			continue; // ���� �ڵ�� �ٽ� �� ���� �ö󰡼� Ȯ���ؾ� �ϹǷ� continue�� �ؾ� �Ѵ�.
		}
		else if (list_ptr->letter)
		{
			// ������
			pptr = &((*pptr)->stemR);
		}
		else
		{
			// ���� 
			pptr = &((*pptr)->stemL);
		}
		list_ptr = list_ptr->next;
	}
	fclose(ofp);
}