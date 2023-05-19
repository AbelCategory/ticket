/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include <iostream>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<typename T>
void swap(T &a,T &b){
	T t = a;
	a = b;
	b = t;
}

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
private:
	inline bool is_equal(Key x, Key y) const{return !Compare()(x, y) && !Compare()(y, x);}
	struct Node{
		Node *ch[2], *p; bool col;
		value_type val;
		Node(){ch[0] = ch[1] = p = nullptr; col = 0;}
		Node(const value_type& v, Node *f = nullptr, bool c = 0, Node *ls = nullptr, Node *rs = nullptr):val(v), p(f), col(c){
			ch[0] = ls;
			ch[1] = rs;
		}
	};
	Node *rt;
	int siz;
	
/*public:
	bool dfs(int &dep, Node *cur,int len){
		if(cur == nullptr){
			if(dep == 0) return dep = len, 1;
			else return dep == len;
		}
		bool ok = 1;
		ok &= dfs(dep, cur -> ch[0], len + (cur -> col));
		ok &= dfs(dep, cur -> ch[1], len + (cur -> col));
		return ok;
	}

	void check(){
		int dep = 0;
		if(dfs(dep, rt, 0)) puts("OK");
		else puts("Bad");
	}
*/
private:
	void dfs_copy(Node* &cur, Node *pre, Node *p){
		if(pre == nullptr) return;
		cur = new Node(pre -> val, p, pre -> col);
		dfs_copy(cur -> ch[0], pre -> ch[0], cur);
		dfs_copy(cur -> ch[1], pre -> ch[1], cur);
	}

	void dfs_erase(Node* &cur){
		if(cur == nullptr) return;
		dfs_erase(cur -> ch[0]);
		dfs_erase(cur -> ch[1]);
		delete cur; cur = nullptr;
	}

	inline int get(Node *cur) const{return cur -> p -> ch[1] == cur;}
	inline Node* oth(Node *cur) const{return cur -> p -> ch[!get(cur)];}
	inline bool check(Node *cur) const{return cur == nullptr || cur -> col;}

	void rotate(Node *x, int k){
		if(x -> p == nullptr){
			rt = x;
			return;
		}
		Node *y = x -> p, *z = y -> p;
		x -> p = z;
		if(z != nullptr) z -> ch[get(y)] = x;
		else rt = x;
		if(x -> ch[!k] != nullptr) x -> ch[!k] -> p = y;
		y -> ch[k] = x -> ch[!k];
		y -> p = x; x -> ch[!k] = y;
	}

	Node* _find(Key x) const{
		Node *cur = rt, *lst = cur;
		while(cur != nullptr){
			if(is_equal((cur -> val).first, x)) return cur;
			lst = cur;
			if(Compare()(x, (cur -> val).first)) cur = cur -> ch[0];
			else cur = cur -> ch[1];
		}
		return lst;
	}

	Node* get_left(Node *cur){
		if(cur -> ch[0] == nullptr){
			while(1){
				if(cur -> p == nullptr || cur == cur -> p -> ch[1]) return cur -> p;
				cur = cur -> p;
			}
		}
		cur = cur -> ch[0];
		while(cur -> ch[1] != nullptr) cur = cur -> ch[1];
		return cur;
	}

	const Node* cget_left(const Node *cur) const{
		if(cur -> ch[0] == nullptr){
			while(1){
				if(cur -> p == nullptr || cur == cur -> p -> ch[1]) return cur -> p;
				cur = cur -> p;
			}
		}
		cur = cur -> ch[0];
		while(cur -> ch[1] != nullptr) cur = cur -> ch[1];
		return cur;
	}

	Node* get_right(Node *cur){
		if(cur -> ch[1] == nullptr){
			while(1){
				if(cur -> p == nullptr || cur == cur -> p -> ch[0]) return cur -> p;
				cur = cur -> p;
			}
		}
		cur = cur -> ch[1];
		while(cur -> ch[0] != nullptr) cur = cur -> ch[0];
		return cur;
	}

	const Node* cget_right(const Node *cur) const{
		if(cur -> ch[1] == nullptr){
			while(1){
				if(cur -> p == nullptr || cur == cur -> p -> ch[0]) return cur -> p;
				cur = cur -> p;
			}
		}
		cur = cur -> ch[1];
		while(cur -> ch[0] != nullptr) cur = cur -> ch[0];
		return cur;
	}

	Node* get_fir() const{
		Node *cur = rt;
		if(cur == nullptr) return cur;
		while(cur -> ch[0] != nullptr) cur = cur -> ch[0];
		return cur;
	}

	Node* get_lst() const{
		Node *cur = rt;
		if(cur == nullptr) return cur;
		while(cur -> ch[1] != nullptr) cur = cur -> ch[1];
		return cur;
	}

	void insert(Node *p, Node* cur){
		//Node *p = _find((cur -> val).first);
		//if((cur -> val).first == (p - > val).first) return 0;
		if(p != nullptr){
			cur -> p = p;
			if(Compare()((cur -> val).first, (p -> val).first)) p -> ch[0] = cur;
			else p -> ch[1] = cur;
		}
		else rt = cur;
		while(1){
			p = cur -> p;
			if(p == nullptr){cur -> col = 1; break;}
			if(p -> col) break;
			Node *y = oth(p), *z = p -> p;
			if(!(p -> col) && !check(y)){
				p -> col = y -> col = 1;
				z -> col = 0;
				cur = z; continue;
			}int k = get(cur);
			if(get(p) != k){
				rotate(cur, k);
				swap(cur, p); k = !k;
			}
			p -> col = 1; z -> col = 0;
			rotate(p, k); break;
		}
	}

	void erase_fix(Node *cur, Node *pre = nullptr, int kk = 0){
		while(1){
			Node *p, *y; int k;
			if(cur != nullptr){
				p = cur -> p;
				if(p == nullptr) break;
				y = oth(cur); k = get(cur);
				//std :: cout << cur -> val.second << std :: endl;
			}
			else{
				p = pre -> p;
				if(p == nullptr) break;
				k = kk; y = p -> ch[!k];
			}
			if(!check(y)){
				p -> col = 0;  y -> col = 1;
				rotate(y, !k);
				y = p -> ch[!k];
			}
			if((p -> col) && (y -> col) && check(y -> ch[0]) && check(y -> ch[1])){
				y -> col = 0; cur = p;
				continue;
			}
			if(!(p -> col) && check(y -> ch[0]) && check(y -> ch[1])){
				p -> col = 1; y -> col = 0;
				break;
			}
			if(!check(y -> ch[k]) && check(y -> ch[!k])){
				y -> ch[k] -> col = 1; y -> col = 0;
				rotate(y -> ch[k], k);
			}
			if(!check(y -> ch[!k])){
				swap(p -> col, y -> col); y -> ch[!k] -> col = 1;
				rotate(y, !k);
				break;
			}
		}
	}

	void erase(Node *cur){
		if(cur -> ch[0] != nullptr && cur -> ch[1] != nullptr){
			Node *x = get_left(cur);
			//swap(cur)
			//(cur -> val).~value_type();
			//new(&(cur -> val)) value_type(x -> val);
			//cur = x;
			if(cur -> p != nullptr) cur -> p -> ch[get(cur)] = x;
			else rt = x;
			if(x -> p != nullptr) x -> p -> ch[get(x)] = cur;
			else rt = cur;
			swap(cur -> p, x -> p);
			if(cur -> ch[0] != nullptr) cur -> ch[0] -> p = x;
			if(cur -> ch[1] != nullptr) cur -> ch[1] -> p = x;
			if(x -> ch[0] != nullptr) x -> ch[0] -> p = cur;
			if(x -> ch[1] != nullptr) x -> ch[1] -> p = cur;
			swap(cur -> ch[0], x -> ch[0]);
			swap(cur -> ch[1], x -> ch[1]);
			swap(cur -> col, x -> col);
			
		}
		Node *ch = cur -> ch[0] != nullptr ? cur -> ch[0] : cur -> ch[1];
		//std::cerr << cur -> p << std::endl;
		if(cur -> p == nullptr){
			delete cur; rt = ch; 
			if(rt != nullptr) rt -> p = nullptr, rt -> col = 1;
			return;
		}
		//std::cerr << cur -> p << std::endl;
		if(ch == nullptr){
			int k = get(cur); cur -> p -> ch[k] = ch;
			if(cur -> col) erase_fix(ch, cur, k);
			/*if(cur -> col){
				Node *p = cur -> p, *y = p -> ch[!k];
				
			}*/
		}
		else{
			ch -> p = cur -> p; cur -> p -> ch[get(cur)] = ch;
			if(cur -> col){
				if(ch -> col) erase_fix(ch);
				else ch -> col = 1;
			}
		}
		delete cur;
	}
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
public:
	class const_iterator;
	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		Node *it;
		map *cur;
	public:
		// The following code is written for the C++ type_traits library.
		// Type traits is a C++ feature for describing certain properties of a type.
		// For instance, for an iterator, iterator::value_type is the type that the 
		// iterator points to. 
		// STL algorithms and containers may use these type_traits (e.g. the following 
		// typedef) to work properly. 
		// See these websites for more information:
		// https://en.cppreference.com/w/cpp/header/type_traits
		// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
		// About iterator_category: https://en.cppreference.com/w/cpp/iterator
		using difference_type = std::ptrdiff_t;
		using value_type = map::value_type;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator_category = std::output_iterator_tag;
		// If you are interested in type_traits, toy_traits_test provides a place to 
		// practice. But the method used in that test is old and rarely used, so you
		// may explore on your own.
		// Notice: you may add some code in here and class const_iterator and namespace sjtu to implement toy_traits_test,
		// this part is only for bonus.
		friend class const_iterator;
		friend class map;

		iterator(Node *_it = nullptr, map *x = nullptr):it(_it), cur(x){
			// TODO
		}
		iterator(const iterator &other) {
			it = other.it;
			cur = other.cur;
			// TODO
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator now(*this);
			if(it == nullptr && cur != nullptr) throw invalid_iterator();
			it = cur -> get_right(it);
			return now;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() {
			if(it == nullptr && cur != nullptr) throw invalid_iterator();
			it = cur -> get_right(it);
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator now(*this);
			if(it == nullptr && cur != nullptr) it = cur -> get_lst();
			else it = cur -> get_left(it);
			if(it == nullptr) throw invalid_iterator();
			return now;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() {
			if(it == nullptr && cur != nullptr) it = cur -> get_lst();
			else it = cur -> get_left(it);	
			if(it == nullptr) throw invalid_iterator();
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {return it -> val;}
		bool operator==(const iterator &rhs) const {return it == rhs.it && cur == rhs.cur;}
		bool operator==(const const_iterator &rhs) const {return it == rhs.it && cur == rhs.cur;}
		bool valid(map *x){return x == cur;}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return it != rhs.it || cur != rhs.cur;}
		bool operator!=(const const_iterator &rhs) const {return it != rhs.it || cur != rhs.cur;}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
			return &(it->val);
		}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			// data members.
			const Node *it;
			const map *cur;
		public:
			friend class iterator;
			friend class map;
			const_iterator(const Node *_it = nullptr, const map *x = nullptr):it(_it), cur(x){
				// TODO
			}
			const_iterator(const const_iterator &other){
				it = other.it;
				cur = other.cur;
				// TODO
			}
			const_iterator(const iterator &other){
				it = other.it;
				cur = other.cur;
				// TODO
			}
			// And other methods in iterator.
			// And other methods in iterator.
			// And other methods in iterator.
			/**
		 * TODO iter++
		 */
		const_iterator operator++(int) {
			const_iterator now(*this);
			if(it == nullptr && cur != nullptr) throw invalid_iterator();
			it = cur -> cget_right(it);
			return now;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator & operator++() {
			if(it == nullptr && cur != nullptr) throw invalid_iterator();
			it = cur -> cget_right(it);
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
			const_iterator now(*this);
			if(it == nullptr && cur != nullptr) it = cur -> get_lst();
			else it = cur -> cget_left(it);
			if(it == nullptr) throw invalid_iterator();
			return now;
		}
		/**
		 * TODO --iter
		 */
		const_iterator & operator--() {
			if(it == nullptr && cur != nullptr) it = cur -> get_lst();
			else it = cur -> cget_left(it);
			if(it == nullptr) throw invalid_iterator();
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		const value_type & operator*() const {return it->val;}
		bool operator==(const iterator &rhs) const {return it == rhs.it && cur == rhs.cur;}
		bool operator==(const const_iterator &rhs) const {return it == rhs.it && cur == rhs.cur;}
		bool valid(const map *x){return cur == x;}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {return it != rhs.it || cur != rhs.cur;}
		bool operator!=(const const_iterator &rhs) const {return it != rhs.it || cur != rhs.cur;}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		const value_type* operator->() const noexcept {
			return &(it->val);
		}
	};
	/**
	 * TODO two constructors
	 */
	map() {rt = nullptr; siz = 0;}
	map(const map &other) {
		siz = other.siz; rt = nullptr;
		dfs_copy(rt, other.rt, nullptr);
	}
	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) {
		if(this == &other) return *this;
		siz = other.siz;
		dfs_erase(rt); rt = nullptr;
		dfs_copy(rt, other.rt, nullptr);
		return *this;
	}
	/**
	 * TODO Destructors
	 */
	~map() {
		dfs_erase(rt);
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
		Node *cur = _find(key);
		if(cur != nullptr && is_equal((cur -> val).first, key)) return (cur -> val).second;
		else throw index_out_of_bound();
	}
	const T & at(const Key &key) const {
		Node *cur = _find(key);
		if(cur != nullptr && is_equal((cur -> val).first, key)) return (cur -> val).second;
		else throw index_out_of_bound();
	}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
		Node *cur = _find(key);
		if(cur != nullptr && is_equal((cur -> val).first, key)) return (cur -> val).second;
		else{
			value_type val(key, T());
			Node *nw = new Node(val);
			insert(cur, nw); ++ siz;
			return (nw -> val).second;
		}
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
		Node *cur = _find(key);
		if(cur != nullptr && is_equal((cur -> val).first, key)) return (cur -> val).second;
		else throw index_out_of_bound();
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {return iterator(get_fir(), this);}
	const_iterator cbegin() const {return const_iterator(get_fir(), this);}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {return iterator(nullptr, this);}
	const_iterator cend() const {return const_iterator(nullptr, this);}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {return siz == 0;}
	/**
	 * returns the number of elements.
	 */
	size_t size() const {return siz;}
	/**
	 * clears the contents
	 */
	void clear() {
		dfs_erase(rt);
		siz = 0;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
		Node *p = _find(value.first);
		if(p != nullptr && is_equal((p -> val).first, value.first)) return pair(iterator(p, this), false);
		else{
			Node *cur = new Node(value);
			insert(p, cur); ++siz;
			return pair(iterator(cur, this), true);
		}
	}
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	friend class iterator;
	friend class const_iterator;
	void erase(iterator pos) {
		if(pos == end()) throw invalid_iterator();
		if(!pos.valid(this)) throw invalid_iterator();
		-- siz;
		Node *cur = pos.it;
		erase(cur);
	}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {
		Node *cur = _find(key);
		if(cur != nullptr && is_equal((cur -> val).first, key)) return 1;
		else return 0;
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
		Node *cur = _find(key);
		if(cur != nullptr && is_equal((cur -> val).first, key)) return iterator(cur, this);
		else return end();
	}
	const_iterator find(const Key &key) const {
		Node *cur = _find(key);
		if(is_equal((cur -> val).first, key)) return const_iterator(cur, this);
		else return cend();
	}
};

}

#endif
