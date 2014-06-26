//
// Testing the Itemizer functionality
//
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item_lib.h"

long file_size(FILE *fp)
{
	long current = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);
	fseek(fp,current, SEEK_SET);
	return size;
}

int main(int argc, char **argv)
{
	ItemizerHeader *phdr;
	FILE *fin = fopen("items.bin", "rb");
	int sz = file_size(fin);
	char *data = (char *) malloc(sz);
	int status = 0;
	ItemList list;

	//Grab the data
	//printf("Data Size: %d\n", sz);
	fread(data, 1, sz, fin);
	fclose(fin);

	status = get_itemizer_header(&phdr, data);
	if(!status) printf("ERROR: INVALID file!\n");

	list = get_item_list(*phdr, data);

	Item *pitem = get_item("itemizer_c", list);

	if(pitem == NULL)
		printf("Unable to find item!\n");
	else
		printf("Found: %s\n", pitem->name);

	delete_list(list);
	free(data);
	return EXIT_SUCCESS;
}