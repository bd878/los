#include <iostream>

using namespace std;

// without ld,
// it is public : nm |grep T
extern void util_function();

void function1()
{
	util_function();
	cout << "hi\n";
}

__attribute__((visibility ("default"))) void entry_point()
{
	function1();
}
