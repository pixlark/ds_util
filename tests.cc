#define DS_UTIL_IMPLEMENTATION
#include "ds_util.h"

void print_platform_information()
{
	printf(
		"Platform: %s\n",
		DS_PLATFORM_WINDOWS ? "Windows" : "Not Windows");
	printf(
		"Path Seperator: %c\n",
		DS_PATH_SEPERATOR);
	printf(
		"Path to executable folder: %s\n",
		get_executable_folder_path());
}

void vector_tests()
{
	printf("Testing Vector2f...\n");
	assert( vector2f_compare(Vector2f(4, 5.5), Vector2f(4, 5.5)));
	assert(!vector2f_compare(Vector2f(4, 5.7), Vector2f(3, 5.65)));
	printf("  [x] vector2f_compare\n");

	assert(vector2f_compare(
			vector2f_add(Vector2f(3, 4.5), Vector2f(4, 1.5)),
			Vector2f(7, 6)));
	printf("  [x] vector2f_add\n");

	assert(vector2f_compare(
			vector2f_scalar_mult(Vector2f(5.5, 3.5), 2),
			Vector2f(11, 7)));
	printf("  [x] vector2f_scalar_mult\n");

	assert(vector2f_dot_product(Vector2f(3.5, 4), Vector2f(2, 3)) == 19);
	printf("  [x] vector2f_dot_product\n");
	
	printf("Testing Vector2i...\n");
	assert( vector2i_compare(Vector2i(4, 5), Vector2i(4, 5)));
	assert(!vector2i_compare(Vector2i(4, 5), Vector2i(3, 6)));
	printf("  [x] vector2i_compare\n");

	assert(vector2i_compare(
			vector2i_add(Vector2i(3, 4), Vector2i(4, 1)),
			Vector2i(7, 5)));
	printf("  [x] vector2i_add\n");

	assert(vector2i_compare(
			vector2i_scalar_mult(Vector2i(5, 3), 2),
			Vector2i(10, 6)));
	printf("  [x] vector2i_scalar_mult\n");

	assert(vector2i_dot_product(Vector2i(3, 4), Vector2i(2, 3)) == 18);
	printf("  [x] vector2i_dot_product\n");
}

void rgba_tests()
{
	printf("Testing RGBA...\n");
	// Nothing to test yet
}

bool arrcmp(int * arr0, int * arr1, int size)
{
	for (int i = 0; i < size; i++) {
		if (arr0[i] != arr1[i]) return false;
	}
	return true;
}

bool intcmp(int i0, int i1)
{
	return i0 == i1;
}

void list_tests()
{
	printf("Testing List<T>...\n");
	
	List<int> list;
	list.alloc();
	assert(list.len == 0);
	printf("  [x] alloc\n");

	list.push(5);
	list.push(9);
	list.pop();
	{
		int a[4] = {7, 3, 4, 1};
		list.push_many(a, 4);
	}
	list.push(1);
	{
		int a[6] = {5, 7, 3, 4, 1, 1};
		assert(arrcmp(list.arr, a, 6));
	}
	printf("  [x] push, push_many, pop\n");

	list.remove(5);
	{
		int a[5] = {5, 7, 3, 4, 1};
		assert(arrcmp(list.arr, a, 5));
	}
	list.push(1);
	printf("  [x] remove\n");
	
	list.insert(20, 2);
	{
		int a[7] = {5, 7, 20, 3, 4, 1, 1};
		assert(arrcmp(list.arr, a, 7));
	}
	list.remove(2);
	{
		int ti[3] = {9, 8, 9};
		list.insert_many(ti, 3, 2);
	}
	{
		int a[9] = {5, 7, 9, 8, 9, 3, 4, 1, 1};
		assert(arrcmp(list.arr, a, 9));
		assert(list.len == 9);
	}
	for (int i = 0; i < 3; i++) list.remove(2);
	assert(list.len == 6);
	printf("  [x] insert, insert_many\n");
	
	list.remove(1);
	list.remove(3);
	list.swap(0, 2);
	list.reverse();
	{
		int a[4] = {1, 5, 3, 4};
		assert(arrcmp(list.arr, a, 4));
	}
	printf("  [x] swap, reverse\n");

	assert(list.find(3, intcmp) == 2);
	printf("  [x] find\n");

	List<int> list_copy = list.copy();
	assert(arrcmp(list.arr, list_copy.arr, 4));
	printf("  [x] copy\n");
}

void string_builder_tests()
{
	printf("Testing String_Builder...\n");
	String_Builder builder;
	builder.alloc();
	assert(strcmp(builder.str(), "") == 0);
	printf("  [x] alloc\n");

	builder.append ("asdf", 4);
	builder.prepend("a"   , 1);
	builder.prepend("gg"  , 2);
	builder.append (""    , 0);
	builder.prepend(""    , 0);
	builder.append ("ff"  , 2);
	assert(strcmp(builder.str(), "ggaasdfff") == 0);
	printf("  [x] append, prepend\n");
}

#define TEST_FILE_CONTENTS "1 2 3 4\n1 2 3 4\n1 2 3 4\n"

void file_read_tests()
{
	printf("Testing load_string_from_file...\n");
	char * str = load_string_from_file("test_file");
	assert(strcmp(str, TEST_FILE_CONTENTS) == 0);
}

int int_hash(int k, int size)
{
	return k % size;
}

bool int_comp(int a, int b)
{
	return a == b;
}

void hash_table_tests()
{
	printf("Testing HashTable<K,V>...\n");
	HashTable<int,char*> table;
	table.init(100, int_hash, int_comp);
	{
		table.insert(15, "hello");
		char * ret;
		table.index(15, &ret);
		assert(strcmp(ret, "hello") == 0);
		printf("  [x] insert, index\n");
	}
	{
		table.insert(215, "asdf");
		table.insert(115, "other");
		char * ret;
		table.index(115, &ret);
		assert(strcmp(ret, "other") == 0);
		printf("  [x] collision probing\n");
	}
}

int main()
{
	defer { printf("This should fire at the end of testing.\n"); };
	
	print_platform_information();
	printf("\n");
	vector_tests();
	printf("\n");
	rgba_tests();
	printf("\n");
	list_tests();
	printf("\n");
	string_builder_tests();
	printf("\n");
	file_read_tests();
	printf("\n");
	hash_table_tests();
	printf("\n");

	return 0;
}
