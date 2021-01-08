﻿/**************************************************
 * @brief   : iterator 相关内容
 * @file    : sx_iterator.h
 * @author  : 宋旭
 * @date    : 2021‎年‎1‎月‎4‎日，‏15:49:27
 **************************************************/

#ifndef _SX_ITERATOR_H_
#define _SX_ITERATOR_H_
// #include <type_traits>
#include "sx_type_traits.h"

SX_NAMESPACE_BEGIN

// 五种迭代器类型
struct input_iterator_tag {};
struct output_iterator_tag : public input_iterator_tag {};
struct forward_iterator_tag : public output_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};


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

// 萃取迭代器的某种类型
template<class Iterator>
struct iterator_traits
{
	using iterator_category = typename Iterator::iterator_category;
	using value_type		= typename Iterator::value_type;
	using difference_type	= typename Iterator::difference_type;
	using pointer			= typename Iterator::pointer;
	using reference			= typename Iterator::reference;
	using const_pointer		= const pointer;
	using const_reference	= const reference;
};


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
 * 以下类模板在编译期确定该类型是否拥有 iterator_category 类型
 * 具体做法是通过静态模板函数的返回值大小确定是否与预期符合
 * 必须使用模板函数进行类型匹配测试，否则若没有相应类型，函数将在编译期报错
 * 若该类型拥有此种类型，静态模板函数将匹配它参数为该类型指针类型的函数
 * 否则根据函数重载将匹配另一个（不符合）
 * 设置这两个函数的返回值大小不同
 * 通过 sizeof 运算符在编译期进行判断是否与预期相符来判断是否拥有此类型
 */
template<class T>
struct has_iterator_category
{
private:
	template<class U> static int test(...);
	template<class U> static char test(typename U::iterator_category* = nullptr);
public:
	static constexpr bool value = sizeof(test<T>(nullptr)) == sizeof(char);
};


/**
 * 若有类型 iterator_category 判断其是否为其中某一迭代器类型
 * 以下用来判断是否是某一迭代器
 * 最后给出一个判断是否符合标准的迭代器
 */
template<class T, class U, bool b = has_iterator_category<has_iterator_category<T>>::value>
struct has_iterator_category_of : public sx_bool_constant<std::is_convertible<
	typename iterator_traits<T>::iterator_category, U>::value> {};


template<class T, class U>
struct has_iterator_category_of<T, U, false> : public sx_false_type {};


template<class Iterator>
struct is_input_iterator : public has_iterator_category_of<Iterator, input_iterator_tag> {};

template<class Iterator>
struct is_output_iterator : public has_iterator_category_of<Iterator, output_iterator_tag> {};

template<class Iterator>
struct is_forward_iterator : public has_iterator_category_of<Iterator, forward_iterator_tag> {};

template<class Iterator>
struct is_bidirectional_iterator : public has_iterator_category_of<Iterator, bidirectional_iterator_tag> {};

template<class Iterator>
struct is_random_access_iterator : public has_iterator_category_of<Iterator, random_access_iterator_tag> {};

template<class Iterator>
struct is_iterator : public sx_bool_constant<
	is_input_iterator<Iterator>::value || is_output_iterator<Iterator>::value> {};



/**
 * 类模板 revered_iterator 
 */
template<class Iterator>
class reverse_iterator
{

};


SX_NAMESPACE_END
#endif	// end define _SX_ITERATOR_H_