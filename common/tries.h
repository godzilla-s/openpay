/* 字典树 */
#ifndef _TRIES_H
#define _TRIES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_N  	10
#define MAX_C	26

typedef struct trie_node_s TrieNode;
typedef struct trie_dict_s TrieDict;

struct trie_node_s
{
	int	   endtag;
	int	   index;
	struct trie_node_s *next[MAX_N];
};

struct trie_dict_s
{
	TrieNode  *root;
};

#define  NODE_SIZE  100000
TrieNode  NodePool[NODE_SIZE];

#endif
