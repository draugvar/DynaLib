//
// Created by Salvatore Rivieccio on 20/03/22.
//

#include "util.h"
#include <stdio.h>

int open()
{
	printf("open\n");
	return 0;
}

int util_print()
{
	printf("print\n");
	return 0;
}

int util_print_int(int a)
{
	printf("print_int %d\n", a);
	return 0;
}

int close()
{
	printf("close\n");
	return 0;
}