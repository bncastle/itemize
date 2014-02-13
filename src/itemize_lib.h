//
// This header enables Itemizer functionality in a program
//
// Copyright 2014 Pixelbytestudios LLC
//
#ifndef __ITEM_LIB_INC
#define __ITEM_LIB_INC

//#pragma once

#define ITEMIZED_EXT				".items"
#define HEADER_STAMP 				"ITMIZD_"  //Must be 1 char shorter than length below to account for NULL term
#define ITMIZR_CURRENT_VERSION		1

typedef struct
{
	unsigned int version;			// Itemizer file version
	char res_hdr[8];				// Itemizer header
	unsigned int num_entries; 		// # of items in the blob
}ItemizerHeader;

typedef struct
{ 
	unsigned int size; // data size (in bytes)
	char *name;        // name of the item
    char *data;        // pointer to the item's data
}Item;

typedef struct 
{
	unsigned int count; // # of items in the list
	Item *items;        // The items in the list
}ItemList;

extern const ItemizerHeader CURRENT_HDR;

// Callable helper funcitons
int get_itemized_header(ItemizerHeader **pheader, const char *data);
ItemList get_item_list(ItemizerHeader hdr, const char *data);
Item * get_item(const char *name, ItemList list);
void delete_list(ItemList list);

#endif