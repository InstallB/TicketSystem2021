#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include <iostream>
#include "my_pair.hpp"
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
private:
	struct node{
		T val;
		node* lson,*rson;
		node(const T &_val) : lson(NULL),rson(NULL),val(_val) {}
	};
	node* root;
	size_t size_;
	node* construct(node* other){
		if(other == NULL) return NULL;
		node* ret = static_cast <node*> (malloc(sizeof(node)));
		new(ret) node(other->val);
		ret->lson = construct(other->lson);
		ret->rson = construct(other->rson);
		return ret;
	}
	void destruct(node* now){
		if(now == NULL) return;
		destruct(now->lson);
		destruct(now->rson);
		now->val.~T();
		free(now);
	}
	node* my_merge(node* x,node* y){
		if(x == NULL) return y;
		if(y == NULL) return x;
		if(Compare()(x->val,y->val)) std::swap(x,y);
		x->rson = my_merge(x->rson,y);
		std::swap(x->lson,x->rson);
		return x;
	}
public:
	/**
	 * TODO constructors
	 */
	priority_queue(){
		root = NULL;
		size_ = 0;
	}
	priority_queue(const priority_queue &other){
		root = construct(other.root);
		size_ = other.size_;
	}
	/**
	 * TODO deconstructor
	 */
	~priority_queue(){
		destruct(root);
		root = NULL;
	}
	/**
	 * TODO Assignment operator
	 */
	priority_queue & operator = (const priority_queue &other){
		if(this == &other) return *this;
        node* tmp = root;
		root = construct(other.root);
		size_ = other.size_;
        destruct(tmp);
		return *this;
	}
	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const {
		if(!size_) throw container_is_empty();
		return root->val;
	}
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	void push(const T &e) {
		node* now = static_cast <node*> (malloc(sizeof(node)));
		new(now) node(e);
		root = my_merge(root,now);
		size_ ++;
	}
	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	void pop() {
		if(size_ == 0) throw container_is_empty();
		node* lson = root->lson;
		node* rson = root->rson;
		root->val.~T();
		free(root);
		root = my_merge(lson,rson);
		size_ --;
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const {
		return size_;
	}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const {
		return size_ == 0;
	}
	/**
	 * merge two priority_queues with at least O(logn) complexity.
	 * clear the other priority_queue.
	 */
	void merge(priority_queue &other){
		root = my_merge(root,other.root);
		size_ += other.size_; other.size_ = 0; other.root = NULL;
	}
};

}

#endif
