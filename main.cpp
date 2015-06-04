#include <iostream>
#include "DS.h"

using namespace std;

bool flag = false;
int original_byte, after_byte;

/* declarations */
void InputInHash(Hash& myHash, LinkedList& ll);
void ConstructMinHeap(min_heap& myMinHeap, const Hash& myHash);
void ConstructHuffmanTree(min_heap& myMinHeap);
void InorderTraversalHuffman(Huffman* hf); // 허프만 트리 탐색하는 함수. 디버깅용.
void GetCode(LinkedList& outLL, const Huffman* hf, const char& key);
void Encode(LinkedList& ll, const Huffman* hf);
void Decode(const Huffman* hf);

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
	printf("\n압축률 : %g\n", (double)after_byte / (double)original_byte * 100.0);
	
	/// 마무리!
	Decode(complete_hf);
	system("PAUSE");
    return 0;
}

/* definitions */
void InputInHash(Hash& myHash, LinkedList& ll)
{
	// ll은 완성된 문장을 담는다.
	FILE *fp = fopen("input.txt", "rb");
	char buf = 0;
	bool check = false;
	while(!feof(fp) && fscanf(fp, "%c", &buf) != EOF)
	{
		if (buf == 0xFF)
		{
			// 파일의 끝부분
			// 간혹 EOF를 읽는 경우를 위해 예외 처리를 해 줌.
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
		// 파일의 끝부분
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
		Huffman* hf = new Huffman(*temp); // 값을 복사하므로 안전하게 전달될 듯
		myMinHeap.Insert(hf);
		hf = NULL;
		delete(temp);
	}
}

void ConstructHuffmanTree(min_heap& myMinHeap)
{
	// Huffman Tree를 만드는 과정
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
	// 아무리 봐도 재귀가 너무 좋다.
	// outLL에는 허프만 비트 코드를 남기도록 한다.
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
		if (hf->stemL) // 왼쪽은 0
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
		if (hf->stemR) // 오른쪽은 1
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
		LinkedList outLL; // 글자 하나마다 코드를 입력받기 위해 반복문 처음에 반복적으로 생성
		flag = false;
		GetCode(outLL, hf, ll_ptr->letter); // outLL에 코드가 저장됨
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
	FILE *ofp = fopen("recoveredinput.txt", "w"); // 원본으로 복구하기 위해 wb 대신 w를 사용.
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
	// 순회 한 번 해보자
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
				cout << "종결 코드 읽음." << endl;
				puts("Decoding end!");
				cout << "프로그램 종료." << endl;
				break;
			}
			fprintf(ofp, "%c", (*pptr)->datum.letter);
			pptr = &hf;
			continue; // 지금 코드는 다시 맨 위로 올라가서 확인해야 하므로 continue를 해야 한다.
		}
		else if (list_ptr->letter)
		{
			// 오른쪽
			pptr = &((*pptr)->stemR);
		}
		else
		{
			// 왼쪽 
			pptr = &((*pptr)->stemL);
		}
		list_ptr = list_ptr->next;
	}
	fclose(ofp);
}