#include "tries.h"

static int nCount = 0;

TrieNode* TrieNode_New()
{
	int i;
	
	if(nCount == NODE_SIZE)
		return NULL;
	TrieNode *node = (TrieNode *)&NodePool[nCount++];
	
	for(i=0; i<MAX_N; i++)
	{
		node->next[i] = NULL;
	}

	node->endtag = 0;
	node->index = -1;
	return node;
}

TrieDict *TrieDict_New()
{
	TrieDict *trie = (TrieDict *)malloc(sizeof(TrieDict));
	trie->root = TrieNode_New();

	return trie;
}

void check_node(TrieNode *node)
{
	int i;
	for(i = 0; i<MAX_N; i++)
	{
		if(node->next[i] != NULL)
		{
			printf("next[%d] Not Null\n", i);
		}
	}
}

int TrieAdd(TrieDict *trie, int index, char *str)
{
	int  i, key, len;
	TrieNode *p = trie->root;
	
	len = strlen(str);
	
	for(i=0; i<len; i++)
	{
		key = str[i] - '0';
		if(p->next[key] == NULL)
		{
			p->next[key] = TrieNode_New();
		}
		p = p->next[key];
		
	}

	p->endtag = 1;
	p->index = index;

	return 0;
}

int TrieGet(TrieDict *trie, char *str)
{
	int	i, key, len;
	int	count = 0;
	TrieNode *p = trie->root;

	len = strlen(str);

	for(i=0; i<len; i++)
	{
		key = str[i] - '0';
		if(p->next[key] != NULL)
		{
			p = p->next[key];
		}
		else
		{
			break;
		}
		count++;
	}

	if(p->endtag == 1)
		return p->index;
	return -1;
}

/*
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
*/