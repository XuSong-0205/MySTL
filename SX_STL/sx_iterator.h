/**************************************************
 * @brief   : iterator 相关内容
 * @file    : sx_iterator.h
 * @author  : 宋旭
 * @date    : 2021‎年‎1‎月‎4‎日，‏15:49:27
 **************************************************/

#ifndef _SX_ITERATOR_H_
#define _SX_ITERATOR_H_
#include <type_traits>		// is_constructible
#include "sx_type_traits.h"

SX_NAMESPACE_BEGIN

// 五种迭代器类型
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : output_iterator_tag {};
struct bidirectional_iterator_tag : forward_iterator_tag {};
struct random_access_iterator_tag : bidirectional_iterator_tag {};


// 迭代器模板
template<class Category, class T, class Distance = ptrdiff_t,
		class Pointer = T*, class Reference = T&>
	struct iterator
{
	using iterator_category = Category;
	using value_type		= T;
	using difference_type	= Distance;
	using pointer			= Pointer;
	using reference			= Reference;
	using const_pointer		= const pointer;
	using const_reference	= const reference;
};


namespace detail {
	template<class T>
	struct __has_iterator_category
	{
		template<class U> static int test(...);
		template<class U> static char test(typename U::iterator_category*);
		static constexpr bool value = sizeof(test<T>(nullptr)) == sizeof(char);
	};

	template<class Iterator, bool>
	struct __iterator_traits_impl {};

	template<class Iterator>
	struct __iterator_traits_impl<Iterator, true>
	{
		using iterator_category = typename Iterator::iterator_category;
		using value_type		= typename Iterator::value_type;
		using difference_type	= typename Iterator::difference_type;
		using pointer			= typename Iterator::pointer;
		using reference			= typename Iterator::reference;
		using const_pointer		= const pointer;
		using const_reference	= const reference;
	};

	template<class Iterator, bool>
	struct __iterator_traits_helper {};

	template<class Iterator>
	struct __iterator_traits_helper<Iterator, true> : __iterator_traits_impl<Iterator,
		std::is_convertible_v<Iterator, input_iterator_tag> ||
		std::is_convertible_v<Iterator, output_iterator_tag>> {};
}


// 萃取迭代器的某种类型
template<class Iterator>
struct iterator_traits : detail::__iterator_traits_helper<Iterator, 
	detail::__has_iterator_category<Iterator>::value> {};


// 对原生指针进行偏特化
template<class T>
struct iterator_traits<T*>
{
	using iterator_category = random_access_iterator_tag;
	using value_type		= T;
	using difference_type	= ptrdiff_t;
	using pointer			= value_type*;
	using reference			= value_type&;
	using const_pointer		= const pointer;
	using const_reference	= const reference;
};

// 对原生指针进行偏特化
template<class T>
struct iterator_traits<const T*>
{
	using iterator_category = random_access_iterator_tag;
	using value_type		= T;
	using difference_type	= ptrdiff_t;
	using pointer			= const value_type*;
	using reference			= const value_type&;
	using const_pointer		= pointer;
	using const_reference	= reference;
};


// 这个函数可以很方便的决定某个迭代器的类型(category)
template<class Iterator>
inline typename iterator_traits<Iterator>::iterator_category 
iterator_category(const Iterator&)
{
	return typename iterator_traits<Iterator>::iterator_category();
}

// 这个函数可以很方便的决定某个迭代器的 distance type
template<class Iterator>
inline typename iterator_traits<Iterator>::difference_type* 
difference_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::difference_type*>(nullptr);
}

// 这个函数可以很方便的决定某个迭代器的 value type
template<class Iterator>
inline typename iterator_traits<Iterator>::value_type* 
value_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::value_type*>(nullptr);
}


/**
 * 以下是整组 distance 函数
 * 返回迭代器 first 到 last 直接的距离
 */
template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type 
__distance(InputIterator first, InputIterator last,
	input_iterator_tag)
{
	typename iterator_traits<InputIterator>::difference_type n = 0;
	while (first != last)
	{
		++first;
		++n;
	}
	return n;
}

template<class RandomIterator>
inline typename iterator_traits<RandomIterator>::difference_type 
__distance(RandomIterator first, RandomIterator last,
	random_access_iterator_tag)
{
	return last - first;
}

template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type 
distance(InputIterator first, InputIterator last)
{
	return __distance(first, last, iterator_category(first));
}


/**
 * 以下是整组 advance 函数
 * 将迭代器 iter 前进 n
 */
template<class InputIterator, class Distance>
void __advance(InputIterator& iter, Distance n,
	input_iterator_tag)
{
	while (n--) ++iter;
}

template<class BidirectionalIterator, class Distance>
void __advance(BidirectionalIterator& iter, Distance n,
	bidirectional_iterator_tag)
{
	if (n >= 0) 
		while (n--) ++iter;
	else 
		while (n++) --iter;
}


template<class RandomIterator, class Distance>
void __advance(RandomIterator& iter, Distance n,
	random_access_iterator_tag)
{
	iter += n;
}

template<class InputIterator, class Distance>
void advance(InputIterator& iter, Distance n)
{
	__advance(iter, n, iterator_category(iter));
}


/**
 * 若有类型 iterator_category 判断其是否为其中某一迭代器类型
 * 以下用来判断是否是某一迭代器
 * 最后给出一个判断是否符合标准的迭代器
 */
template<class T, class U, bool b = detail::__has_iterator_category<iterator_traits<T>>::value>
struct has_iterator_category_of : sx_bool_constant_t<std::is_convertible_v<iterator_traits<T>, U>> {};

template<class T, class U>
struct has_iterator_category_of<T, U, false> : sx_false_type {};


template<class Iterator>
struct is_input_iterator : has_iterator_category_of<Iterator, input_iterator_tag> {};

template<class Iterator>
struct is_output_iterator : has_iterator_category_of<Iterator, output_iterator_tag> {};

template<class Iterator>
struct is_forward_iterator : has_iterator_category_of<Iterator, forward_iterator_tag> {};

template<class Iterator>
struct is_bidirectional_iterator : has_iterator_category_of<Iterator, bidirectional_iterator_tag> {};

template<class Iterator>
struct is_random_access_iterator : has_iterator_category_of<Iterator, random_access_iterator_tag> {};

template<class Iterator>
struct is_iterator : public sx_bool_constant<
	is_input_iterator<Iterator>::value || is_output_iterator<Iterator>::value> {};



/**
 * 类模板 revered_iterator 
 */
template<class Iterator>
class reverse_iterator
{
private:
	Iterator current;

public:
	using iterator_category = typename iterator_traits<Iterator>::iterator_category;
	using value_type		= typename iterator_traits<Iterator>::value_type;
	using difference_type	= typename iterator_traits<Iterator>::difference_type;
	using pointer			= typename iterator_traits<Iterator>::pointer;
	using reference			= typename iterator_traits<Iterator>::reference;
	using const_pointer		= const pointer;
	using const_reference	= const reference;

	using iterator_type		= Iterator;
	using reverse_iterator<iterator_type> = self;

public:
	reverse_iterator() {}
	explicit reverse_iterator(iterator_type iter) : current (iter) {}
	reverse_iterator(const self& rhs) : current(rhs.current) {}

	// 取出正向迭代器
	iterator_type base()const 
	{ 
		return current; 
	}

	// 重载操作符
	// 实际对应正向迭代器的前一个位置
	reference operator*()const
	{
		auto temp = current;
		return *--temp;
	}

	pointer operator->()const 
	{
		return &(operator*());
	}

	// 前进(++)变为后退(--)
	self& operator++()
	{
		--current;
		reutrn* this;
	}

	self operator++(int)
	{
		auto temp = *this;
		--current;
		return temp;
	}

	// 后退(--)变为前进(++)
	self& operator--()
	{
		++current;
		return *this;
	}

	self operator--(int)
	{
		auto temp = *this;
		++current;
		return temp;
	}

	self& operator+=(difference_type n)
	{
		// 使用此头文件中的全局函数，对不同类型的迭代器选取不同的策略执行
		advance(current, -n);
		return *this;
	}

	self operator+(difference_type n)const
	{
		auto temp = current;
		advance(temp, -n);
		return *temp;
	}

	self& operator-=(difference_type n)
	{
		advance(current, n);
		return *this;
	}

	self operator-(difference_type n)const
	{
		auto temp = current;
		advance(temp, n);
		return *temp;
	}

	self& operator[](difference_type n)
	{
		return *(*this + n);
	}

	// 对operator-() 的重载
	difference_type operator-(const self& rhs)const
	{
		// 使用全局函数 distance() 进行实现
		return distance(rhs.current, current);
	}

	bool operator==(const self& rhs)const
	{
		return base() == rhs.base();
	}

	bool operator!=(const self& rhs)const
	{
		return !(*this == rhs);
	}

	bool operator<(const self& rhs)const
	{
		return rhs.base() < base();
	}

	bool operator<=(const self& rhs)const
	{
		return rhs < *this || rhs == *this;
	}

	bool operator>(const self& rhs)const
	{
		return !(rhs <= *this);
	}

	bool operator>=(const self& rhs)const
	{
		return !(rhs < *this);
	}
};

SX_NAMESPACE_END
#endif	// end define _SX_ITERATOR_H_