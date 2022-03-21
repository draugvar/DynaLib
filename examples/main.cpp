//
// Created by Salvatore Rivieccio on 20/03/22.
//

#include <dynalib.h>

int main()
{
	dl::DynaLib<int> dl;

	dl.open("libutil", "open", "close");

	dl.init();
	dl.callFunction("util_print");
	dl.callFunction("util_print_int", 42);
	dl.callFunction("util_print_int", 55);
	dl.callFunction("util_print_params", 138, true, "hello");
}