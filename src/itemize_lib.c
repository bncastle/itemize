//
// This contains functions that can be used to access an Itemized file
//
// Copyright 2014 Pixelbytestudios LLC
//
//--------------------------------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include "itemize_lib.h"

// Current Itemizer header version and text signature
// This is what we look for at the beginning of the binary blob
const ItemizerHeader CURRENT_HDR = {ITMIZR_CURRENT_VERSION, HEADER_STAMP, 0};

//
// Given a pointer to the itemizer data blob, this will take a
// pointer pointer to an ItemizerHeader and point it to the actual header
//
// returns: 1 if the data has a valid Itemizer header, 0 otherwise
//
int get_itemized_header(ItemizerHeader **pheader, const char *data)
{
	//If this is a valid Itemizer file, the header should be at the beginning of the data
	*pheader = (ItemizerHeader *) data;

	//Look for a valid itemizer stamp
	 if(strcmp((*pheader)->res_hdr, CURRENT_HDR.res_hdr))
	 	return 0;
	return 1;
}

//
// Given a valid Itemizer header and a pointer to the data blob,
// this will fill and return an ItemList struct
// 
// returns: An ItemList struct 
//
ItemList get_item_list(ItemizerHeader hdr, const char *data)
{
	int i = 0;
	ItemList list;
	char *pointer = (char *) data + sizeof(ItemizerHeader);

	list.count = hdr.num_entries;
	list.items = (Item *) malloc(list.count * sizeof(Item));

	//Zero out the whole list
	memset(list.items, 0, list.count * sizeof(Item));

	//Populate the item structs
	for(i = 0; i < list.count; i++)
	{
		//printf("orig: %p pt: %p\n", data, pointer);
		list.items[i].size = *((unsigned int *)pointer);
		pointer += 4;
		list.items[i].name = pointer;
		pointer += strlen(pointer) + 1; //+1 for the NULL
		list.items[i].data = pointer; 
		pointer += list.items[i].size;
		//printf("[%d] %s\n", list.items[i].size,list.items[i].name);
	}
	return list;
}

//
// Finds an Item with the specific name in the given list
//
// returns: Pointer to the item struct if found, NULL otherwise
//
Item * get_item(const char *name, ItemList list)
{
	int i;
	for(i = 0; i < list.count; i++)
	{
		if(!strcmp(name, list.items[i].name))
			return &list.items[i];
	}
	//No items found
	return NULL;
}

//
// Cleans up an ItemList
//
void delete_list(ItemList list)
{
	if(list.items != NULL)
		free(list.items);
	list.items = NULL;
}