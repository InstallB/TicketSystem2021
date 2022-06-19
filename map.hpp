/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "my_pair.hpp"
#include "exceptions.hpp"

namespace sjtu {
struct my_true_type{};
struct my_false_type{};

template <typename T>
struct my_type_traits{
	typedef typename T::iterator_assignable iterator_assignable;
};

template<class Key,class T,class Compare = std::less<Key> >
class map{
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
	struct node{
		node* ch[2]; // ch[0] is left son, ch[1] is right son.
		node* fa; // father
		value_type* val; // T has default construction func
		bool col; // color, 0 is black, 1 is red.
		node(){ ch[0] = nullptr; ch[1] = nullptr; fa = nullptr; val = nullptr; col = 0; }
	};
	node *root,*begin_,*end_;
	size_t size_;
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	friend class const_iterator;
	friend class map;
	private:
		map* cur;
		node* pos;
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
		using value_type = pair <const Key,T>;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
		typedef my_true_type iterator_assignable;
		// If you are interested in type_traits, toy_traits_test provides a place to 
		// practice. But the method used in that test is old and rarely used, so you
		// may explore on your own.
		// Notice: you may add some code in here and class const_iterator and namespace sjtu to implement toy_traits_test,
		// this part is only for bonus.
		
		iterator(){
			cur = nullptr;
			pos = nullptr;
		}
		iterator(const iterator &other){
			cur = other.cur;
			pos = other.pos;
		}
		iterator(const const_iterator &other){
			cur = other.cur;
			pos = other.pos;
		}
		iterator operator ++ (int){
			if(pos == nullptr) throw invalid_iterator();
			iterator tmp = *this;
			if(pos->ch[1]){
				pos = pos->ch[1];
				while(pos->ch[0]) pos = pos->ch[0];
			}
			else{
				while(pos->fa && pos == pos->fa->ch[1]) pos = pos->fa;
				if(pos->fa == nullptr) pos = nullptr; // end()
				else pos = pos->fa;
			}
			return tmp;
		}
		iterator & operator ++ (){
			if(pos == nullptr) throw invalid_iterator();
			if(pos->ch[1]){
				pos = pos->ch[1];
				while(pos->ch[0]) pos = pos->ch[0];
			}
			else{
				while(pos->fa && pos == pos->fa->ch[1]) pos = pos->fa;
				if(pos->fa == nullptr) pos = nullptr; // end()
				else pos = pos->fa;
			}
			return *this;
		}
		iterator operator -- (int){
			if(pos == cur->begin_) throw invalid_iterator();
			iterator tmp = *this;
			if(pos == nullptr){
				pos = cur->end_;
				return tmp;
			} // end()
			if(pos->ch[0]){
				pos = pos->ch[0];
				while(pos->ch[1]) pos = pos->ch[1];
			}
			else{
				while(pos->fa && pos == pos->fa->ch[0]) pos = pos->fa;
				pos = pos->fa;
			}
			return tmp;
		}
		iterator & operator -- (){
			if(pos == cur->begin_) throw invalid_iterator();
			if(pos == nullptr){
				pos = cur->end_;
				return *this;
			} // end()
			if(pos->ch[0]){
				pos = pos->ch[0];
				while(pos->ch[1]) pos = pos->ch[1];
			}
			else{
				while(pos->fa && pos == pos->fa->ch[0]) pos = pos->fa;
				pos = pos->fa;
			}
			return *this;
		}
		value_type & operator * ()const{
			if(pos == nullptr) throw invalid_iterator();
			return *(pos->val);
		}
		bool operator == (const iterator &rhs)const{
			return cur == rhs.cur && pos == rhs.pos;
		}
		bool operator == (const const_iterator &rhs)const{
			return cur == rhs.cur && pos == rhs.pos;
		}
		bool operator != (const iterator &rhs)const{
			return !(cur == rhs.cur && pos == rhs.pos);
		}
		bool operator != (const const_iterator &rhs)const{
			return !(cur == rhs.cur && pos == rhs.pos);
		}
		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator -> ()const noexcept{
			if(pos == nullptr) throw invalid_iterator();
			return pos->val;
		}
	};
	class const_iterator {
	// it should has similar member method as iterator.
	// and it should be able to construct from an iterator.
	friend class iterator;
	friend class map;
	private:
		const map* cur;
		const node* pos;
	public:
		typedef my_false_type iterator_assignable;
		
		const_iterator(){
			cur = nullptr;
			pos = nullptr;
		}
		const_iterator(const iterator &other){
			cur = other.cur;
			pos = other.pos;
		}
		const_iterator(const const_iterator &other){
			cur = other.cur;
			pos = other.pos;
		}
		const_iterator operator ++ (int){
			if(pos == nullptr) throw invalid_iterator();
			const_iterator tmp = *this;
			if(pos->ch[1]){
				pos = pos->ch[1];
				while(pos->ch[0]) pos = pos->ch[0];
			}
			else{
				while(pos->fa && pos == pos->fa->ch[1]) pos = pos->fa;
				if(pos->fa == nullptr) pos = nullptr; // end()
				else pos = pos->fa;
			}
			return tmp;
		}
		const_iterator & operator ++ (){
			if(pos == nullptr) throw invalid_iterator();
			if(pos->ch[1]){
				pos = pos->ch[1];
				while(pos->ch[0]) pos = pos->ch[0];
			}
			else{
				while(pos->fa && pos == pos->fa->ch[1]) pos = pos->fa;
				if(pos->fa == nullptr) pos = nullptr; // end()
				else pos = pos->fa;
			}
			return *this;
		}
		const_iterator operator -- (int){
			if(pos == cur->begin_) throw invalid_iterator();
			const_iterator tmp = *this;
			if(pos == nullptr){
				pos = cur->end_;
				return tmp;
			} // end()
			if(pos->ch[0]){
				pos = pos->ch[0];
				while(pos->ch[1]) pos = pos->ch[1];
			}
			else{
				while(pos->fa && pos == pos->fa->ch[0]) pos = pos->fa;
				pos = pos->fa;
			}
			return tmp;
		}
		const_iterator & operator -- (){
			if(pos == cur->begin_) throw invalid_iterator();
			if(pos == nullptr){
				pos = cur->end_;
				return *this;
			} // end()
			if(pos->ch[0]){
				pos = pos->ch[0];
				while(pos->ch[1]) pos = pos->ch[1];
			}
			else{
				while(pos->fa && pos == pos->fa->ch[0]) pos = pos->fa;
				pos = pos->fa;
			}
			return *this;
		}
		value_type & operator * ()const{
			if(pos == nullptr) throw invalid_iterator();
			return *pos->val;
		}
		bool operator == (const iterator &rhs)const{
			return cur == rhs.cur && pos == rhs.pos;
		}
		bool operator == (const const_iterator &rhs)const{
			return cur == rhs.cur && pos == rhs.pos;
		}
		bool operator != (const iterator &rhs)const{
			return !(cur == rhs.cur && pos == rhs.pos);
		}
		bool operator != (const const_iterator &rhs)const{
			return !(cur == rhs.cur && pos == rhs.pos);
		}
		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator -> ()const noexcept{
			if(pos == nullptr) throw invalid_iterator();
			return pos->val;
		}
	};
	friend class iterator;
	friend class const_iterator;
	
	bool get_color(node* now){
		if(now == nullptr) return 0;
		return now->col;
	}
	// if now is null pointer, return 0 (black node)
	bool which(node* now){
		return now == now->fa->ch[1];
	}
	// whether x is x's parent node's left or right son
	// now should not be root
	
	void rotate(node* u){
		node *p,*gp,*w; bool ku,kp;
		p = u->fa; gp = p->fa; ku = which(u); w = u->ch[!ku]; 
		if(gp != nullptr){ kp = which(p); gp->ch[kp] = u; } u->fa = gp;
		p->ch[ku] = u->ch[!ku]; u->ch[!ku] = p; p->fa = u;
		if(w != nullptr) w->fa = p;
		if(gp == nullptr) root = u;
	}
	// single rotate
	
	void insert_adjust(node* now){
		if((!which(now) && !which(now->fa)) || (which(now) && which(now->fa))){
			now->fa->col = 0; now->fa->fa->col = 1;
			rotate(now->fa);
		} // LLb, RRb
		else{
			now->col = 0; now->fa->fa->col = 1;
			rotate(now); rotate(now);
		} // LRb, RLb
	}
	// now->fa->fa exists.
	
	node* insert_(const value_type &value){
		node *now = root,*tmp; bool k;
		if(root == nullptr){
			root = new node;
			root->val = new value_type(value); root->col = 0; tmp = now = root;
		}
		else{
			while(1){
				k = Compare()(now->val->first,value.first);
				if(now->ch[k] == nullptr){
					now->ch[k] = new node;
					now->ch[k]->fa = now; now->ch[k]->col = 1; now->ch[k]->val = new value_type(value);
					now = now->ch[k]; tmp = now;
					break;
				}
				else now = now->ch[k];
			}
		}
		// insert
		while(now->fa && now->fa->fa){
			if(get_color(now) && get_color(now->fa)){
				if(get_color(now->fa->fa->ch[!which(now->fa)])){
					now->fa->fa->col = 1;
					now->fa->fa->ch[0]->col = 0;
					now->fa->fa->ch[1]->col = 0;
					now = now->fa->fa;
				}
				else{
					insert_adjust(now);
					break;
				}
			}
			else now = now->fa;
		}
		root->col = 0;
		// adjust
		begin_ = root; if(begin_ != nullptr) while(begin_->ch[0] != nullptr) begin_ = begin_->ch[0];
		end_ = root; if(end_ != nullptr) while(end_->ch[1] != nullptr) end_ = end_->ch[1];
		size_ ++;
		// update values
		return tmp;
	}
	void erase_(const iterator &it){
		node *now = it.pos,*tmp = it.pos,*del = it.pos;
		if(size_ == 1){
			delete root->val; delete root;
			begin_ = end_ = root = nullptr; size_ = 0;
			return;
		}
		tmp = now;
		if(now->ch[0] != nullptr && now->ch[1] != nullptr){
			tmp = now->ch[1];
			while(tmp->ch[0] != nullptr) tmp = tmp->ch[0];
			node* t; bool t1; value_type* t2;
			if(now == tmp->fa){
				bool k = which(tmp);
				if(now->ch[!k] != nullptr) now->ch[!k]->fa = tmp;
				if(tmp->ch[!k] != nullptr) tmp->ch[!k]->fa = now;
				if(tmp->ch[k] != nullptr) tmp->ch[k]->fa = now;
				if(now->fa != nullptr){ t1 = which(now); now->fa->ch[t1] = tmp; }
				t = now->ch[!k]; now->ch[!k] = tmp->ch[!k]; tmp->ch[!k] = t;
				now->ch[k] = tmp->ch[k]; tmp->ch[k] = now;
				tmp->fa = now->fa; now->fa = tmp;
			}
			else{
				if(now->ch[0] != nullptr) now->ch[0]->fa = tmp;
				if(now->ch[1] != nullptr) now->ch[1]->fa = tmp;
				if(tmp->ch[0] != nullptr) tmp->ch[0]->fa = now;
				if(tmp->ch[1] != nullptr) tmp->ch[1]->fa = now;
				if(now->fa != nullptr){ bool k = which(now); now->fa->ch[k] = tmp; }
				if(tmp->fa != nullptr){ bool k = which(tmp); tmp->fa->ch[k] = now; }
				t = tmp->ch[0]; tmp->ch[0] = now->ch[0]; now->ch[0] = t;
				t = tmp->ch[1]; tmp->ch[1] = now->ch[1]; now->ch[1] = t;
				t = tmp->fa; tmp->fa = now->fa; now->fa = t;
			}
			t1 = tmp->col; tmp->col = now->col; now->col = t1;
			if(root == now) root = tmp;
			t = now; now = tmp; tmp = t;
		}
		del = tmp;
		// find substitute (del,tmp)
		if(tmp->ch[0] != nullptr || tmp->ch[1] != nullptr){
			// black node, son is red leaf
			if(tmp == root){
				// extra judge : tmp is root
				bool k = (tmp->ch[1] != nullptr);
				root = tmp->ch[k]; root->fa = nullptr; root->col = 0;
				delete tmp->val; delete tmp;
			}
			else{
				bool k = (tmp->ch[1] != nullptr),kt = which(tmp);
				tmp->ch[k]->col = 0; tmp->ch[k]->fa = tmp->fa; tmp->fa->ch[kt] = tmp->ch[k];
				delete tmp->val; delete tmp;
			}
		}
		else if(tmp->ch[0] == nullptr && tmp->ch[1] == nullptr && get_color(tmp)){
			// red leaf
			bool k = which(tmp);
			tmp->fa->ch[k] = nullptr;
			delete tmp->val; delete tmp;
		}
		else{
			// black leaf
			now = tmp;
			while(now->fa){
				if(get_color(now)){ now->col = 0; break; }
				tmp = now->fa->ch[!which(now)];
				if(get_color(tmp)){
					// brother node is red
					tmp->col = 0; now->fa->col = 1;
					rotate(tmp);
				}
				else{
					// brother node is black
					if(!get_color(tmp->ch[0]) && !get_color(tmp->ch[1])){
						tmp->col = 1;
						if(get_color(now->fa)){
							now->fa->col = 0;
							break;
						}
						else now = now->fa;
					} // L0,R0
					else if(get_color(tmp->ch[!which(now)])){
						tmp->ch[!which(now)]->col = 0;
						tmp->col = now->fa->col; now->fa->col = 0;
						rotate(tmp);
						break;
					} // L1L,R1R
					else{
						node* ts = tmp->ch[which(now)];
						ts->col = now->fa->col; now->fa->col = 0;
						rotate(ts); rotate(ts);
						break;
					} // L1R,R1L,L2,R2
				}
			}
			tmp = del;
			bool k = which(tmp);
			tmp->fa->ch[k] = nullptr;
			delete tmp->val; delete tmp;
		}
		root->col = 0;
		// remove & adjust
		begin_ = root; if(begin_ != nullptr) while(begin_->ch[0] != nullptr) begin_ = begin_->ch[0];
		end_ = root; if(end_ != nullptr) while(end_->ch[1] != nullptr) end_ = end_->ch[1];
		size_ --;
		// update values
	}
	node* find_(const Key &key)const{
		node* now = root;
		while(now){
			if(Compare()(key,now->val->first)) now = now->ch[0]; // key < value
			else if(Compare()(now->val->first,key)) now = now->ch[1]; // key > value
			else return now; // equal
		}
		return nullptr;
	}
	void construct(node* x,node* y){
		if(y == nullptr){ x = nullptr; return; }
		x->val = new value_type(*y->val);
		x->col = y->col;
		if(y->ch[0] != nullptr){
			x->ch[0] = new node;
			x->ch[0]->fa = x;
			construct(x->ch[0],y->ch[0]);
		}
		if(y->ch[1] != nullptr){
			x->ch[1] = new node;
			x->ch[1]->fa = x;
			construct(x->ch[1],y->ch[1]);
		}
	}
	void destruct(node* x){
		if(x == nullptr) return;
		destruct(x->ch[0]);
		destruct(x->ch[1]);
		delete x->val;
		delete x;
	}
	
	map(){
		root = begin_ = end_ = nullptr;
		size_ = 0;
	}
	map(const map &other){
		root = new node;
		size_ = other.size_;
		construct(root,other.root);
		begin_ = end_ = root;
		if(begin_ != nullptr) while(begin_->ch[0] != nullptr) begin_ = begin_->ch[0];
		if(end_ != nullptr) while(end_->ch[1] != nullptr) end_ = end_->ch[1];
	}
	map & operator = (const map &other){
		if(this == &other) return *this;
		destruct(root);
		root = new node;
		size_ = other.size_;
		construct(root,other.root);
		begin_ = end_ = root;
		if(begin_ != nullptr) while(begin_->ch[0] != nullptr) begin_ = begin_->ch[0];
		if(end_ != nullptr) while(end_->ch[1] != nullptr) end_ = end_->ch[1];
		return *this;
	}
	~map(){
		destruct(root);
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key){
		node* now = find_(key);
		if(now == nullptr) throw index_out_of_bound();
		return now->val->second;
	}
	const T & at(const Key &key)const{
		node* now = find_(key);
		if(now == nullptr) throw index_out_of_bound();
		return now->val->second;
	}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key){
		node* now = find_(key);
		if(now == nullptr) now = insert_(value_type(key,T()));
		return now->val->second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key)const{
		node* now = find_(key);
		if(now == nullptr) throw index_out_of_bound();
		return now->val->second;
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin(){
		iterator now;
		now.cur = this; now.pos = begin_;
		return now;
	}
	const_iterator cbegin()const{
		const_iterator now;
		now.cur = this; now.pos = begin_;
		return now;
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end(){
		iterator now;
		now.cur = this; now.pos = nullptr;
		return now;
	}
	const_iterator cend()const{
		const_iterator now;
		now.cur = this; now.pos = nullptr;
		return now;
	}
	bool empty()const{
		return size_ == 0;
	}
	size_t size()const{
		return size_;
	}
	/**
	 * clears the contents
	 */
	void clear(){
		destruct(root);
		size_ = 0; root = begin_ = end_ = nullptr;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value){
		iterator it;
		it.cur = this; it.pos = find_(value.first);
		if(it.pos != nullptr) return pair<iterator, bool>(it,false);
		it.pos = insert_(value);
		return pair<iterator, bool>(it,true);
	}
	/**
	 * erase the element at pos.
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos){
		if(pos.cur != this || pos == end()) throw invalid_iterator();
		if(find_((pos.pos)->val->first) == nullptr) throw invalid_iterator();
		erase_(pos);
	}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key)const{
		node* now = find_(key);
		return (now != nullptr);
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 * If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key){
		iterator now;
		now.cur = this; now.pos = find_(key);
		return now;
	}
	const_iterator find(const Key &key)const{
		const_iterator now;
		now.cur = this; now.pos = find_(key);
		return now;
	}
};

}

#endif
