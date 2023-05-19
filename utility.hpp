#ifndef SJTU_UTILITY_HPP
#define SJTU_UTILITY_HPP

#include <utility>

namespace sjtu {

template<class T1, class T2>
class pair {
public:
	T1 first;
	T2 second;
	constexpr pair() : first(), second() {}
	pair(const pair &other) = default;
	pair(pair &&other) = default;
	pair(const T1 &x, const T2 &y) : first(x), second(y) {}
	template<class U1, class U2>
	pair(U1 &&x, U2 &&y) : first(x), second(y) {}
	template<class U1, class U2>
	pair(const pair<U1, U2> &other) : first(other.first), second(other.second) {}
	template<class U1, class U2>
	pair(pair<U1, U2> &&other) : first(other.first), second(other.second) {}
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
