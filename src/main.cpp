#include <iostream>
#include <cstdlib>
#include <string>

#include "List.h"

using std::cout;
using std::endl;
using std::string;

using rcd::List;

/*
 * Test harness for rcd::List
 */
int main() {

	List<string> str_list;

	cout << "Size: " << str_list.size() << endl;

	str_list.insert(0, "Hello");
	str_list.push_back("There");
	str_list.push_back("World");

	/*
	 * This is to test our copy constructor
	 */
	List<string> list_cpy = str_list;
	cout << str_list << endl;

	list_cpy.erase("There");
	list_cpy.push_front("Wow!");
	list_cpy.insert(1, "Super...");
	cout << list_cpy << endl;
	cout << str_list << endl;

	cout << str_list[0] << endl;

	bool found = list_cpy.find("Hello");
	cout << "Found Hello: " << found << endl;

	return EXIT_SUCCESS;
}
