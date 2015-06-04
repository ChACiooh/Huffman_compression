#include <iostream>
#include "DS.h"

using namespace std;

bool flag = false;

/* declarations */
void InputInHash(Hash& myHash, LinkedList& ll);
void ConstructMinHeap(min_heap& myMinHeap, const Hash& myHash);
void ConstructHuffmanTree(min_heap& myMinHeap);
void InorderTraversalHuffman(Huffman* hf); // 허프만 트리 탐색하는 함수. 디버깅용.
void GetCode(const Huffman*, unsigned char&, const char, int& cnt);
void Encode(LinkedList& ll, const Huffman* hf);
void Decode(LinkedList& ll, const Huffman* hf);

int main()
{
	/// 만들자 변수들!
    min_heap myMinHeap;
    Huffman* complete_hf = NULL;
    Hash myHash;
	LinkedList ll;

	/// 일하자 일!
    InputInHash(myHash, ll); // 해시 테이블을 만들고, 완성된 문장을 ll에 저장.
    ConstructMinHeap(myMinHeap, myHash); // 해시 테이블을 통해 우선순위 큐를 구축.
    ConstructHuffmanTree(myMinHeap); // 구축된 큐를 통해 허프만 트리를 구축.
	complete_hf = myMinHeap.pop(); // 완성된 허프만 트리를 얻어오는과정.
	//InorderTraversalHuffman(complete_hf);
	Encode(ll, complete_hf);
	ll.~LinkedList();
	Decode(ll, complete_hf);
	
	/// 마무리!
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

void GetCode(const Huffman* hf, unsigned char& base, const char check, int& cnt)
{
	unsigned char temp = 0x0;
	if (hf == NULL)	return;
	if (hf->stemL == NULL && hf->stemR == NULL && check == hf->datum.letter)
	{
		// leaf node이고 체크하고자 하는 글자와 같다.
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
			if (!flag) { // 끝까지 가지 않았으면 원래대로 되돌리기
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
			if (!flag){ // 끝까지 가지 않았으면 원래대로 되돌리기
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
		GetCode(hf, temp, letter, cnt); // hf를 기반으로 temp에 저장하며 letter로 비교하고 cnt는 깊이를 의미한다.
		for (int j = 0; j < sizeof(char)*8 - cnt; j++)	temp = temp << j;
		remained -= cnt;
		if (remained == 0)
		{
			fprintf(fp, "%c", buf);
			buf = 0x0;
			remained = sizeof(char) * 8 - cnt;
		}
		else if (remained < 0) // shifting 처리를 별도로 해 주어야 한다.
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
		// remained가 가진 값만큼 잉여 0이 있다.
		fprintf(fp, "%c", buf);
	}
	fclose(fp);
}

void Decode(LinkedList& ll, const Huffman* hf)
{
	// ll에 결과를 저장한답니다 우왕~
	unsigned char base = 0x0;

}