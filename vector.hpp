#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>

namespace sjtu{
	/*
	 * a data container like std::vector
	 * store data in a successive memory and support random access.
	 */
	template <typename T>
	class vector{
	private:
		void double_space(){
			T* tmp;
			max_size = max_size << 1;
			tmp = static_cast <T*> (malloc (max_size * sizeof(T)));
			for(int i = 0;i < size_;i ++) new(tmp + i) T(data[i]);
			for(int i = 0;i < size_;i ++) data[i].~T();
			free(data);
			data = tmp; 
		}
	public:
		size_t size_,max_size;
		T* data;
		/*
		 * a type for actions of the elements of a vector, and you should write
		 *   a class named const_iterator with same interfaces.
		 */
		/*
		 * you can see RandomAccessIterator at CppReference for help.
		 */
		class const_iterator;
		class iterator{
		friend class vector;
		friend class const_iterator;
		// The following code is written for the C++ type_traits library.
		// Type traits is a C++ feature for describing certain properties of a type.
		// For instance, for an iterator, iterator::value_type is the type that the 
		// iterator points to. 
		// STL algorithms and containers may use these type_traits (e.g. the following 
		// typedef) to work properly. In particular, without the following code, 
		// @code{std::sort(iter, iter1);} would not compile.
		// See these websites for more information:
		// https://en.cppreference.com/w/cpp/header/type_traits
		// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
		// About iterator_category: https://en.cppreference.com/w/cpp/iterator
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::output_iterator_tag;
		private:
			vector <T>* p;
			int x;
		public:
			/**
			 * return a new iterator which pointer n-next elements
			 * as well as operator-
			 */
			iterator operator + (const int &n)const{
				iterator tmp = *this; tmp.x += n;
				return tmp;
			}
			iterator operator - (const int &n)const{
				iterator tmp = *this; tmp.x -= n;
				return tmp;
				return *this;
			}
			// return the distance between two iterators,
			// if these two iterators point to different vectors, throw invalid_iterator.
			int operator - (const iterator &rhs)const{
				if(p != rhs.p) throw invalid_iterator();
				return x - rhs.x;
			}
			iterator & operator += (const int &n){
				x += n;
				return *this;
			}
			iterator & operator -= (const int &n) {
				x -= n;
				return *this;
			}
			iterator operator ++ (int){
				iterator tmp = *this; x ++;
				return tmp;
			}
			iterator & operator ++ (){
				x ++;
				return *this;
			}
			iterator operator -- (int){
				iterator tmp = *this; x --;
				return tmp;
			}
			iterator & operator -- (){
				x --;
				return *this;
			}
			T & operator * ()const{
				return p->data[x];
			}
			bool operator == (const iterator &rhs)const{
				return (p == rhs.p && x == rhs.x);
			}
			bool operator == (const const_iterator &rhs)const{
				return (p == rhs.p && x == rhs.x);
			}
			bool operator != (const iterator &rhs)const{
				return !(p == rhs.p && x == rhs.x);
			}
			bool operator != (const const_iterator &rhs)const{
				return !(p == rhs.p && x == rhs.x);
			}
		};
		/**
		 * has same function as iterator, just for a const object.
		 */
		class const_iterator{
		friend class vector;
		friend class iterator;
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::output_iterator_tag;
		private:
			const vector <T>* p;
			int x;
		public:
			const_iterator operator + (const int &n)const{
				const_iterator tmp = *this; tmp.x += n;
				return tmp;
			}
			const_iterator operator - (const int &n)const{
				const_iterator tmp = *this; tmp.x -= n;
				return tmp;
				return *this;
			}
			int operator - (const const_iterator &rhs)const{
				if(p != rhs.p) throw invalid_iterator();
				return x - rhs.x;
			}
			const_iterator & operator += (const int &n){
				x += n;
				return *this;
			}
			const_iterator & operator -= (const int &n) {
				x -= n;
				return *this;
			}
			const_iterator operator ++ (int){
				const_iterator tmp = *this; x ++;
				return tmp;
			}
			const_iterator & operator ++ (){
				x ++;
				return *this;
			}
			const_iterator operator -- (int){
				const_iterator tmp = *this; x --;
				return tmp;
			}
			const_iterator & operator -- (){
				x --;
				return *this;
			}
			const T & operator * ()const{
				return p->data[x];
			}
			bool operator == (const iterator &rhs)const{
				return (p == rhs.p && x == rhs.x);
			}
			bool operator == (const const_iterator &rhs)const{
				return (p == rhs.p && x == rhs.x);
			}
			bool operator != (const iterator &rhs)const{
				return !(p == rhs.p && x == rhs.x);
			}
			bool operator != (const const_iterator &rhs)const{
				return !(p == rhs.p && x == rhs.x);
			}
		};
		/*
		 * Constructs
		 * Atleast two: default constructor, copy constructor
		 */
		vector(size_t _size = 0){
			size_ = _size; max_size = size_ + 8;
			data = static_cast <T*> (malloc (max_size * sizeof(T)));
		}
		vector(const vector &other){
			size_ = other.size_; max_size = other.max_size;
			data = static_cast <T*> (malloc (max_size * sizeof(T)));
			for(int i = 0;i < size_;i ++) new(data + i) T(other.data[i]);
		}
		/*
		 * Destructor
		 */
		~vector(){
			for(int i = 0;i < size_;i ++) data[i].~T();
			free (data);
		}
		/*
		 * Assignment operator
		 */
		vector & operator = (const vector &other){
			if(this == &other) return *this;
			size_ = other.size_; max_size = other.max_size;
            for(int i = 0;i < size_;i ++) data[i].~T(); free(data);
			data = static_cast <T*> (malloc (max_size * sizeof(T)));
			for(int i = 0;i < size_;i ++) new(data + i) T(other.data[i]);
			return *this;
		}
		/*
		 * assigns specified element with bounds checking
		 * throw index_out_of_bound if pos is not in [0, size)
		 */
		T & at(const size_t &pos){
			if(pos < 0 || pos >= size_) throw index_out_of_bound();
			return data[pos];
		}
		const T & at(const size_t &pos)const{
			if(pos < 0 || pos >= size_) throw index_out_of_bound();
			return data[pos];
		}
		/*
		 * assigns specified element with bounds checking
		 * throw index_out_of_bound if pos is not in [0, size)
		 * !!! Pay attentions
		 *   In STL this operator does not check the boundary but I want you to do.
		 */
		T & operator[](const size_t &pos){
			if(pos < 0 || pos >= size_) throw index_out_of_bound();
			return data[pos];
		}
		const T & operator[](const size_t &pos)const{
			if(pos < 0 || pos >= size_) throw index_out_of_bound();
			return data[pos];
		}
		/*
		 * access the first element.
		 * throw container_is_empty if size == 0
		 */
		const T & front()const{
			if(size_ == 0) throw container_is_empty();
			return data[0];
		}
		/*
		 * access the last element.
		 * throw container_is_empty if size == 0
		 */
		const T & back()const{
			if(size_ == 0) throw container_is_empty();
			return data[size_ - 1];
		}
		/*
		 * returns an iterator to the beginning.
		 */
		iterator begin(){
			iterator ret;
			ret.p = this; ret.x = 0;
			return ret;
		}
		const_iterator cbegin()const{
			const_iterator ret;
			ret.p = this; ret.x = 0;
			return ret;
		}
		/*
		 * returns an iterator to the end.
		 */
		iterator end(){
			iterator ret;
			ret.p = this; ret.x = size_;
			return ret;
		}
		const_iterator cend()const{
			const_iterator ret;
			ret.p = this; ret.x = size_;
			return ret;
		}
		/*
		 * checks whether the container is empty
		 */
		bool empty()const{
			return size_ == 0;
		}
		/*
		 * returns the number of elements
		 */
		size_t size()const{
			return size_;
		}
		/*
		 * clears the contents
		 */
		void clear(){
			size_ = 0;
		}
		/*
		 * inserts value before pos
		 * returns an iterator pointing to the inserted value.
		 */
		iterator insert(iterator pos, const T &value){
			if(size_ == max_size) double_space();
			size_ ++;
			new (data + size_ - 1) T(value);
			for(int i = size_ - 1;i > pos.x;i --) data[i] = data[i - 1];
			data[pos.x] = value;
			return pos;
		}
		/*
		 * inserts value at index ind.
		 * after inserting, this->at(ind) == value
		 * returns an iterator pointing to the inserted value.
		 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
		 */
		iterator insert(const size_t &ind, const T &value){
			if(ind > size_) throw index_out_of_bound();
			iterator ret;
			if(size_ == max_size) double_space();
			size_ ++;
			new (data + size_ - 1) T(value);
			for(int i = size_ - 1;i > ind;i --) data[i] = data[i - 1];
			data[ind] = value;
			ret.p = this; ret.x = ind;
			return ret;
		}
		/*
		 * removes the element at pos.
		 * return an iterator pointing to the following element.
		 * If the iterator pos refers the last element, the end() iterator is returned.
		 */
		iterator erase(iterator pos){
			for(int i = pos.x;i < size_ - 1;i ++) data[i] = data[i + 1];
			data[size_ - 1].~T(); size_ --;
			return pos;
		}
		/*
		 * removes the element with index ind.
		 * return an iterator pointing to the following element.
		 * throw index_out_of_bound if ind >= size
		 */
		iterator erase(const size_t &ind){
			if(ind >= size_) throw index_out_of_bound();
			iterator ret;
			for(int i = ind;i < size_ - 1;i ++) data[i] = data[i + 1];
			data[size_ - 1].~T(); size_ --;
			ret.p = this; ret.x = ind;
			return ret;
		}
		/*
		 * adds an element to the end.
		 */
		void push_back(const T &value){
			if(size_ == max_size) double_space();
			new(data + size_) T(value); size_ ++; // placement new
		}
		/*
		 * remove the last element from the end.
		 * throw container_is_empty if size() == 0
		 */
		void pop_back(){
			if(size_ == 0) throw container_is_empty();
			data[size_ - 1].~T(); size_ --;
		}
	};
}

#endif
