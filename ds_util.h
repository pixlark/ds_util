#ifndef DS__INCLUDE_DS_UTIL_H
#define DS__INCLUDE_DS_UTIL_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64)
#define DS_PLATFORM_WINDOWS 1
#else
#define DS_PLATFORM_WINDOWS 0
#endif

#if DS_PLATFORM_WINDOWS

#include <Windows.h>
#define DS_PATH_SEPERATOR '\\'

#else

#include <unistd.h>
#define DS_PATH_SEPERATOR '/'

#endif

struct Vector2f;
struct Vector2i;
struct RGBA;

struct Vector2f {
	float x;
	float y;
	Vector2f()
		: x(0), y(0) {};
	Vector2f(float x, float y)
		: x(x), y(y) {};
};

bool     vector2f_compare(Vector2f v0, Vector2f v1);
Vector2f vector2f_add(Vector2f v0, Vector2f v1);
Vector2f vector2f_scalar_mult(Vector2f v0, float scalar);
float    vector2f_dot_product(Vector2f v0, Vector2f v1);
// TODO(pixlark): Cross product

struct Vector2i {
	int x;
	int y;
	Vector2i()
		: x(0), y(0) {};
	Vector2i(int x, int y)
		: x(x), y(y) {};
};

bool     vector2i_compare(Vector2i v0, Vector2i v1);
Vector2i vector2i_add(Vector2i v0, Vector2i v1);
Vector2i vector2i_scalar_mult(Vector2i v0, int scalar);
int      vector2i_dot_product(Vector2i v0, Vector2i v1);

struct RGBA {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
	RGBA()
		: r(0), g(0), b(0), a(0) {};
	RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: r(r), g(g), b(b), a(a) {};
};

template <typename T>
struct List {
	T * arr;
	int len;
	int len_max;

	void    alloc();
	void    dealloc();
	List<T> copy() const;
	
	void    push(T to_push);
	void    insert(T to_insert, int position);
	void    push_many(T * to_push, int size);
	T       pop();

	int     remove(int indice);
	bool    swap(int i, int j);
	void    reverse();
	
	int     find(T search_term, bool(equality)(T, T)) const;
	T&      operator[](int indice)          const;
	
	void _grow();
	void _shrink();
};

template <typename T>
void List<T>::alloc()
{
	len = 0;
	len_max = 4;
	arr = (T*) malloc(sizeof(T) * len_max);
}

template <typename T>
void List<T>::dealloc()
{
	free(arr);
}

template <typename T>
List<T> List<T>::copy()
const
{
	List<T> ret;
	ret.arr = (T*) malloc(sizeof(T) * len_max);
	ret.len = len;
	ret.len_max = len_max;
	for (int i = 0; i < len; i++) {
		ret.arr[i] = arr[i];
	}
	return ret;
}

template <typename T>
void List<T>::push(T to_push)
{
	if (len >= len_max) {
		_grow();
	}
	arr[len++] = to_push;
}

template <typename T>
void List<T>::insert(T to_insert, int position)
{
	assert(position >= 0 && position <= len);
	if (len >= len_max) {
		_grow();
	}
	memmove(arr + position + 1, arr + position, sizeof(T) * (len - position));
	arr[position] = to_insert;
	len++;
}

template <typename T>
void List<T>::push_many(T * to_push, int size)
{
	while (len + size >= len_max) {
		_grow();
	}
	for (int i = 0; i < size; i++) {
		arr[len++] = to_push[i];
	}
}

template <typename T>
T List<T>::pop()
{
	assert(len > 0);
	T ret = arr[--len];
	if (len <= len_max / 4 && len_max > 4) {
		_shrink();
	}
	return ret;
}

template <typename T>
int List<T>::find(T search_term, bool(equality)(T, T))
const
{
	for (int i = 0; i < len; i++) {
		if (equality(arr[i], search_term)) {
			return i;
		}
	}
	return -1;
}

template <typename T>
bool List<T>::swap(int i, int j)
{
	if (i < 0 || i >= len || j < 0 || j >= len) {
		return false;
	}
	T temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
	return true;
}

template <typename T>
void List<T>::reverse()
{
	int i = 0;
	int j = len - 1;
	while (i < j) {
		swap(i++, j--);
	}
}

template <typename T>
T& List<T>::operator[](int indice)
const
{
	return arr[indice];
}

template <typename T>
int List<T>::remove(int indice)
{
	assert(indice < len);
	assert(len > 0);	
	for (int i = indice; i < len - 1; i++) {
		arr[i] = arr[i + 1];
	}
	len--;
	if (len <= len_max / 4 && len_max > 4) {
		_shrink();
	}
	return 0;
}

template <typename T>
void List<T>::_grow()
{
	T * new_arr = (T*) malloc(sizeof(T) * len_max * 2);
	for (int i = 0; i < len_max; i++) {
		new_arr[i] = arr[i];
	}
	free(arr);
	len_max *= 2;
	arr = new_arr;
}

template <typename T>
void List<T>::_shrink()
{
	T * new_arr = (T*) malloc(sizeof(T) * (len_max / 2));
	for (int i = 0; i < (len_max / 2); i++) {
		new_arr[i] = arr[i];
	}
	free(arr);
	len_max /= 2;
	arr = new_arr;
}

// TODO(pixlark): Optimize methods here, probably have to use custom
// grow/shrink and stuff like that.
struct String_Builder {
	List<char> list;
	
	void   alloc();
	void   dealloc();
	
	void   prepend(char * str, int len);
	void   append(char * str, int len);
	char * str();
};

void auto_convert_path_seperators(char * path, int len);
char * get_executable_path();

#endif

#ifdef DS_UTIL_IMPLEMENTATION

// Vector2f

bool vector2f_compare(Vector2f v0, Vector2f v1)
{
	return (v0.x == v1.x) && (v0.y == v1.y);
}

Vector2f vector2f_add(Vector2f v0, Vector2f v1)
{
	return Vector2f(v0.x + v1.x, v0.y + v1.y);
}

Vector2f vector2f_scalar_mult(Vector2f v0, float scalar)
{
	return Vector2f(v0.x * scalar, v0.y * scalar);
}

float vector2f_dot_product(Vector2f v0, Vector2f v1)
{
	return v0.x * v1.x + v0.y * v1.y;
}

// Vector2i

bool vector2i_compare(Vector2i v0, Vector2i v1)
{
	return (v0.x == v1.x) && (v0.y == v1.y);
}

Vector2i vector2i_add(Vector2i v0, Vector2i v1)
{
	return Vector2i(v0.x + v1.x, v0.y + v1.y);
}

Vector2i vector2i_scalar_mult(Vector2i v0, int scalar)
{
	return Vector2i(v0.x * scalar, v0.y * scalar);
}

int vector2i_dot_product(Vector2i v0, Vector2i v1)
{
	return v0.x * v1.x + v0.y * v1.y;
}

void String_Builder::alloc()
{
	list.alloc();
}

void String_Builder::dealloc()
{
	list.dealloc();
}

void String_Builder::prepend(char * str, int len)
{
	while (list.len + len >= list.len_max) {
		list._grow();
	}
	for (int i = list.len - 1; i >= 0; i--) {
		list[i + len] = list[i];
	}
	for (int i = 0; i < len; i++) {
		list[i] = str[i];
	}
	list.len += len;
}

void String_Builder::append(char * str, int len)
{
	if (len == -1) {
		len = strlen(str);
	}
	while (list.len + len >= list.len_max) {
		list._grow();
	}
	for (int i = 0; i < len; i++) {
		list[list.len + i] = str[i];
	}
	list.len += len;
}

char * String_Builder::str()
{
	/* There should always be unused space past the end of len,
	 * because of the way _grow() works. This means we can just place
	 * a null char there to make sure it works as a string. If _grow()
	 * or _shrink() changes, we need to make sure this still works.
	 */
	list[list.len] = '\0';
	return list.arr;
}

void auto_convert_path_seperators(char * path, int len)
{
	for (int i = 0; i < len; i++) {
		if (path[i] == '/' || path[i] == '\\') {
			path[i] = DS_PATH_SEPERATOR;
		}
	}
}

char * get_executable_path()
{
	char buffer[512];
	int string_len; // string_len is length NOT including null terminator
#if DS_PLATFORM_WINDOWS
	// GetModuleFileName copies in null terminator for us
	string_len = GetModuleFileName(NULL, buffer, 512);
#else
	// readlink does not copy in null terminator for us, so we leave a
	// space and do it ourselves.
	string_len = readlink("/proc/self/exe", buffer, 511);
	buffer[string_len] = '\0';
#endif
	char * last_seperator = strrchr(buffer, DS_PATH_SEPERATOR);
	*(last_seperator + 1) = '\0'; // Truncate string before executable name
	char * dyn_string = (char*) malloc(strlen(buffer) + 1);
	strcpy(dyn_string, buffer);
	return dyn_string;
}

#endif
