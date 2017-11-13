#include "tries.h"

int main()
{
	TrieDict	*trie = TrieDict_New();

	FILE  *fp = fopen("cardbin.txt", "r");
	if(!fp)
		return -1;

	char *pstr;
	char  temp[20] = {'\0'};
	int	  i = 1;
	while( (pstr = fgets(temp, 20, fp)) != NULL )
	{
		temp[strlen(pstr)-1] = '\0';
		TrieAdd(trie, i, temp);
		memset(temp, 0, sizeof(temp));
		i++;
	}
	
	printf("Get CardBin at: %d\n", TrieGet(trie, "62268898732897284"));
	printf("Get CardBin at: %d\n", TrieGet(trie, "652314507348572"));
	printf("Get CardBin at: %d\n", TrieGet(trie, "6200485043226974"));
	printf("Get CardBin at: %d\n", TrieGet(trie, "62284800311747213"));
	printf("alloc size: %d * %d = %d\n", nCount, sizeof(TrieNode), nCount * sizeof(TrieNode));

	return 0;
}
