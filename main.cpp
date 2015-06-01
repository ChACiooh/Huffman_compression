#include <iostream>
#include "DS.h"

using namespace std;

void InputInHash(Hash& myHash);

int main()
{
    min_heap myMinHeap;
    Huffman temp_hf;
    Hash myHash;
    Data *data_ptr;

    InputInHash(myHash);

	for(int i=0;i<HASH_SIZE;i++)
	{
		if(myHash.get_num(i))
		{
			data_ptr = myHash.GetData(i);
			printf("%c %d\n", data_ptr->letter, data_ptr->freq);
			free(data_ptr);
		}
	}
    return 0;
}


void InputInHash(Hash& myHash)
{
	FILE *fp = fopen("input.txt", "rb");
	unsigned char buf;
	while(!feof(fp) && (fscanf(fp, "%c", &buf)) != EOF)
	{
		myHash.PushLetter(buf);
	}
	fclose(fp);
}
