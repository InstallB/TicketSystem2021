#ifndef MY_PAIR_HPP
#define MY_PAIR_HPP

namespace sjtu{

template <class T1,class T2>
class pair{
public:
	T1 first; T2 second;
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
	bool operator < (const pair <T1,T2> &other)const{
		if(first == other.first) return second < other.second;
		return first < other.first;
	}
	bool operator == (const pair <T1,T2> &other)const{
		return first == other.first && second == other.second;
	}
	bool operator != (const pair <T1,T2> &other)const{
		return !(*this == other);
	}
	bool operator > (const pair <T1,T2> &other)const{
		if(first == other.first) return second > other.second;
		return first > other.first;
	}
	pair <T1,T2> & operator = (const pair <T1,T2> &other){
		if(this == &other) return *this;
		first = other.first; second = other.second;
		return *this;
	}
};

}

#endif
