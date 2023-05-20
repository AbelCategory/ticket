#ifndef SJTU_UTILITY_HPP
#define SJTU_UTILITY_HPP

#include "vector.hpp"
#include <string>

std::string to_str2(int x){
	std::string s;
	for(int i = 0; i < 2; ++i){
		s = char(x % 10 + 48) + s;
		x /= 10;
	}
	return s;
}

namespace sjtu {

template<typename T>
void swap(T &a,T &b){
	T t = a;
	a = b;
	b = t;
}

template<class T1, class T2>
class pair {
public:
	T1 first;
	T2 second;
	constexpr pair() : first(), second() {}
	pair(const pair &other) = default;
	pair(pair &&other) = default;
	pair(const T1 &x, const T2 &y) : first(x), second(y) {}
	pair& operator =(const pair &oth){
		if(this == &oth) return *this;
		first = oth.first;
		second = oth.second;
		return *this;
	}
	template<class U1, class U2>
	pair(U1 &&x, U2 &&y) : first(x), second(y) {}
	template<class U1, class U2>
	pair(const pair<U1, U2> &other) : first(other.first), second(other.second) {}
	template<class U1, class U2>
	pair(pair<U1, U2> &&other) : first(other.first), second(other.second) {}

	inline bool operator <(const pair& B) const{return first < B.first || first == B.first && second < B.second;}
	inline bool operator ==(const pair& B) const{return first == B.first && second == B.second;}
	inline bool operator !=(const pair& B) const{return first != B.first || second != B.second;}
};

template <class T, class Cmp>
void qsort(vector<T> &a, int l, int r, const Cmp &cmp){
	if(l >= r) return;
	int i = l - 1, j = r + 1;
	T x = a[(l + r) >> 1];
	while(1){
		do i++; while(cmp(a[i], x));
		do j--; while(cmp(x, a[j]));
		if(i >= j) break;
		swap(a[i], a[j]);
	}
	qsort(a, l, j, cmp);
	qsort(a, j + 1, r, cmp);
}

template<class T, class Cmp>
void sort(vector<T> &a, int l, int r, const Cmp &cmp){
	qsort(a, l, r, cmp);
}

}

#endif
