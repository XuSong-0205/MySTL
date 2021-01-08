/**************************************************
 * @brief   : 关于 type_traits 方面的内容
 * @file    : sx_type_traits.h
 * @author  : 宋旭
 * @date    : 2021‎年‎1‎月‎4‎日，‏‎15:27:19
 **************************************************/

#ifndef _SX_TYPE_TRAITS_H_
#define _SX_TYPE_TRAITS_H_
#include "sx_def.h"

SX_NAMESPACE_BEGIN

// 基类类型
template<class T, T v>
struct sx_integral_constant
{
	static constexpr T value = v;
	using value_type = T;
	using type = sx_integral_constant;

	constexpr operator value_type()noexcept { return value; }
	constexpr value_type operator()()noexcept { return value; }
};

// bool 类型
template<bool b>
using sx_bool_constant = sx_integral_constant<bool, b>;

using sx_true_type = sx_bool_constant<true>;
using sx_false_type = sx_bool_constant<false>;

// bool 类型别名
template<bool b>
using sx_bool_constant_t = typename sx_bool_constant<b>::type;

// bool 类型的值
template<bool b>
constexpr bool sx_bool_constant_v = sx_bool_constant<b>::value;


/**
 * is_const
 * is_const_v
 * 
 * is_volatile
 * is_volatile_v
 * 
 * is_reference
 * is_reference_v
 */

// is_const and is_const_v
template<class T>
struct is_const : sx_false_type {};

template<class T>
struct is_const<T const> : sx_true_type {};

template<class T>
constexpr bool is_const_v = is_const<T>::value;

// is_volatile and is_volatile_v
template<class T>
struct is_volatile : sx_false_type {};

template<class T>
struct is_volatile<T volatile> : sx_true_type {};

template<class T>
constexpr bool is_volatile_v = is_volatile<T>::value;

// is_reference and is_reference_v
template<class T>
struct is_reference : sx_false_type {};

template<class T>
struct is_reference<T&> : sx_true_type {};

template<class T>
struct is_reference<T&&> : sx_true_type {};

template<class T>
constexpr bool is_reference_v = is_reference<T>::value;


/**
 * 一个类型系统的基类
 */
template<class T>
struct type_identity { using type = T; };

/**
 * remove_const
 * remove_const_t
 *
 * remove_volatile
 * remove_volatile_t
 *
 * remove_cv
 * remove_cv_t
 *
 * remove_reference
 * remove_reference_t
 */

 // remove_const and remove_const_t
template<class T>
struct remove_const : type_identity<T> {};

template<class T>
struct remove_const<const T> : type_identity<T> {};

template<class T>
using remove_const_t = typename remove_const<T>::type;

// remove_volatile and remove_volatile_t
template<class T>
struct remove_volatile : type_identity<T> {};

template<class T>
struct remove_volatile<volatile T> : type_identity<T> {};

template<class T>
using remove_volatile_t = typename remove_volatile<T>::type;


/**
 * 通过修改下列宏的值，可使用不同的实现
 * 每一个值对应一种实现
 * 当前仅提供两种实现，所以该宏的值仅可为 0, 1
 * 若将此宏设置为不存在的值，那么下列实现将全部失效
 */

#ifndef	__REMOVE_CV
#define __REMOVE_CV 1
#if		__REMOVE_CV == 0

 // 下列是一种可以的实现
 // remove_cv and remove_cv_t
 remove_cv and remove_cv_t
 template<class T>
 struct remove_cv : type_identity<T> {};

 template<class T>
 struct remove_cv<const T> : type_idnetity<T> {};

 template<class T>
 struct remove_cv<volatile T> : type_identity<T> {};

 template<class T>
 struct remove_cv<const volatile T> : type_identity<T> {};

#elif	__REMOVE_CV == 1

// 下列是另一种可以的实现
// remove_cv and remove_cv_t
template<class T>
struct remove_cv : type_identity<remove_const_t<remove_volatile_t<T>>> {};

#endif	// endif __REMOVE_CV == 0
#undef	__REMOVE_CV	
#endif	// endif __REMOVE_CV

template<class T>
using remove_cv_t = typename remove_cv<T>::type;


// remove_reference and remove_reference_t
template<class T>
struct remove_reference : type_identity<T> {};

template<class T>
struct remove_reference<T&> : type_identity<T> {};

template<class T>
struct remove_reference<T&&> : type_identity<T> {};

template<class T>
using remove_refernece_t = typename remove_reference<T>::type;


/**
 * add_const
 * add_const_t
 *
 * add_volatile
 * add_volatile_t
 *
 * add_cv
 * add_cv_t
 */

// add_const and add_const_t
template<class T>
struct add_const : type_identity<const T> {};

template<class T>
using add_const_t = typename add_const<T>::type;

// add_volatile and add_volatile_t
template<class T>
struct add_volatile : type_identity<volatile T> {};

template<class T>
using add_volatile_t = typename add_volatile<T>::type;

// add_cv
// template<class T>
// struct add_cv : type_identity<add_const_t<add_volatile_t<T>>> {};

// add_cv and add_cv_t
template<class T>
struct add_cv : type_identity<const volatile T> {};

template<class T>
using add_cv_t = typename add_cv<T>::type;


/**
 * add_lvalue_reference
 * add_lvalue_reference_t
 * 
 * add_rvalue_reference
 * add_rvalue_reference_t
 * 
 * 引用折叠规则 : 
 * T		add_lvalue_reference	--> T&
 * T&		add_lvalue_reference	--> T&
 * T&&		add_lvalue_reference	--> T&
 * T		add_rvalue_reference	--> T&&
 * T&		add_rvalue_reference	--> T&		// 此项注意！
 * T&&		add_rvalue_reference	--> T&&
 */


/**
 * 通过修改下列宏的值，可使用不同的实现
 * 每一个值对应一种实现
 * 当前仅提供两种实现，所以该宏的值仅可为 0, 1
 * 若将此宏设置为不存在的值，那么下列实现将全部失效
 */

#ifndef __ADD_REFERENCE
#define __ADD_REFERENCE 0
#if		__ADD_REFERENCE == 0

// 下列是一种实现
template<class T>
auto __try_add_lvalue_reference(int)->type_identity<T&>;

template<class T>
auto __try_add_lvalue_reference(...)->type_identity<T>;

template<class T>
auto __try_add_rvalue_reference(int)->type_identity<T&&>;

template<class T>
auto __try_add_rvalue_reference(...)->type_identity<T>;


template<class T>
struct add_lvalue_reference : decltype(__try_add_lvalue_reference<T>(0)) {};

template<class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

template<class T>
struct add_rvalue_reference : decltype(__try_add_rvalue_reference<T>(0)) {};

template<class T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

#elif	__ADD_REFERENCE == 1

// 下列是另一种可以的实现
// 方法是对 void 进行特化
// add_lvalue_reference and add_lvalue_reference_t
template<class T>
struct add_lvalue_reference : type_identity<T&> {};

// 对 void 进行特化
template<>
struct add_lvalue_reference<void> : type_identity<void> {};

template<class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

// add_rvalue_reference and add_rvalue_reference_t
template<class T>
struct add_rvalue_reference : type_identity<T&&> {};

// 对 void 进行特化
template<>
struct add_rvalue_reference<void> : type_identity<void> {};

template<class T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

#endif	// endif __ADD_REFERENCE == 0
#undef	__ADD_REFERENCE
#endif	// endif __ADD_REFERENCE


/**
 * 此函数假装返回一个该类型的右值对象
 * 实际上不会进行任何返回，甚至此函数都没有实现
 * 使用此函数可以假装返回一个纯虚函数的对象并且调用其内部方法
 * 实际不会真的实例化，也不会有任何的函数调用
 * 仅仅是在编译期使用的一些小手段
 */
template<class T>
auto declval(...)noexcept->add_rvalue_reference_t<T>;


/**
 * is_same
 * is_same_v
 * 
 * is_pair
 * is_pair_v
 */

// is_same and is_same_v
template<class T1, class T2>
struct is_same : sx_false_type {};

template<class T>
struct is_same<T, T> : sx_true_type {};

template<class T1, class T2>
constexpr bool is_same_v = is_same<T1, T2>::value;

// pair 的一个简单定义
template<class T1, class T2>
struct pair
{
	using type = pair;
	T1 first;
	T2 second;
	
	pair(const T1& t1 = T1(), const T2& t2 = T2())
		:first(t1), second(t2) {}
};

// is_pair and is_pair_v
template<class>
struct is_pair : sx_false_type {};

template<class T1, class T2>
struct is_pair<sx::pair<T1, T2>> : sx_true_type {};

// 对 std::pair 的偏特化
template<class T1, class T2>
struct is_pair<std::pair<T1, T2>> : sx_true_type {};

template<class T>
constexpr bool is_pair_v = is_pair<T>::value;


/**
 * *	-- 语言级支持，已实现
 * +	-- 需编译器支持，已实现
 * -	-- 暂未实现
 * 
 * is_void						*
 * is_null_pointer				*
 * is_integral					*
 * is_floating_point			*
 * is_array						*
 * is_enum						+
 * is_union						+
 * is_class						+
 * is_function					*
 * is_pointer					*
 * is_lvalue_reference			*
 * is_rvalue_reference			*
 * is_member_object_pointer		*
 * is_member_function_pointer	*
 */


 // is_void and is_void_t
template<class T>
struct is_void : sx_false_type {};

template<>
struct is_void<void> : sx_true_type {};


// is_null_pointer and is_null_pointer_v
template<class T>
struct is_null_pointer : sx_bool_constant<is_same_v<remove_cv_t<T>, std::nullptr_t>> {};

template<class T>
constexpr bool is_null_pointer_v = is_null_pointer<T>::value;


/**
 * is_true_in_pack
 * is_false_in_pack
 * is_type_in_pack
 */

// 检查不定长非类型参数列表中是否包含 true, 存在 true 即返回 true
// is_true_in_packand is_true_in_pakc_v
template<bool... args>
struct __is_true_in_pack : sx_false_type {};

template<bool... args>
struct __is_true_in_pack<false, args...> : __is_true_in_pack<args...> {};

template<bool... args>
struct __is_true_in_pack<true, args...> : sx_true_type {};

template<bool... args>
constexpr bool __is_true_in_pack_v = __is_true_in_pack<args...>::value;

// 检查不定长非类型参数列表中是否包含 false, 存在 false 即返回 true
// is_false_in_packand is_false_in_pack_v
template<bool... args>
struct __is_false_in_pack : sx_false_type {};

template<bool... args>
struct __is_false_in_pack<false, args...> : sx_true_type {};

template<bool... args>
struct __is_false_in_pack<true, args...> : __is_false_in_pack<args...> {};

template<bool... args>
constexpr bool __is_false_in_pack_v = __is_false_in_pack<args...>;

// 检查不定长类型参数列表中是否包含类型 T, 若存在类型 T 即返回 true
// __is_type_in_pack __is_type_in_pack_v
template<class T, class... Types>
struct __is_type_in_pack : sx_bool_constant_t<__is_true_in_pack_v<sx::is_same_v<remove_cv_t<T>, Types>...>> {};

template<class T, class... Types>
constexpr bool __is_type_in_pack_v = __is_type_in_pack<T, Types...>::value;


// 辅助 is_integral 的实现，std 的实现方法
template<bool _first_value, class First, class... Rest>
struct __Disjunction : sx::type_identity<First> {};

template<class False, class Next, class... Rest>
struct __Disjunction<false, False, Next, Rest...> : __Disjunction<Next::value, Next, Rest...>::type {};

template<class... Traits>
struct __disjunction : sx::sx_false_type {};

template<class First, class... Rest>
struct __disjunction<First, Rest...> : __Disjunction<First::value, First, Rest...>::type {};

template<class... Traits>
constexpr bool __disjunction_v = __disjunction<Traits...>::value;

// 使用下列宏选取不同的实现
template<class T, class... Types>
constexpr bool __is_any_of_v =
#ifndef __USE_MY_ACHIEVE
#define __USE_MY_ACHIEVE 1
#if		__USE_MY_ACHIEVE == 0
	__disjunction_v<sx::is_same<T, Types>...>;	// std 实现
#elif	__USE_MY_ACHIEVE == 1
	__is_type_in_pack_v<T, Types...>;			// 我的实现
#endif // __USE_MY_ACHIEVE == 0
#endif // !__USE_MY_ACHIEVE 0
#undef	__USE_MY_ACHIEVE


// is_integral and in_integral_v
template<class T>
struct is_integral : sx_bool_constant <__is_any_of_v<remove_cv_t<T>,
	bool, char, signed char, unsigned char, wchar_t,
#ifdef __cpp_char8_t
	char8_t,
#endif // __cpp_char8_t
	char16_t, char32_t, short, unsigned short, int, unsigned int, long, unsigned long, long long, unsigned long long>> {};

template<class T>
constexpr bool is_integral_v = is_integral<T>::value;


// is_floating_point and is_floating_point
template<class T>
struct is_floating_point : sx_bool_constant_t<__is_any_of_v<remove_cv_t<T>, float, double, long double>> {};

template<class T>
constexpr bool is_floating_point_v = is_floating_point<T>::value;


// is_array and is_array_v
template<class T>
struct is_array : sx_false_type {};

template<class T, size_t N>
struct is_array<T[N]> : sx_true_type {};

template<class T>
struct is_array<T[]> : sx_true_type {};

template<class T>
constexpr bool is_array_v = is_array<T>::value;


// is_enum and is_enum_v
template<class T>
struct is_enum : sx_bool_constant_t<__is_enum(T)> {};	// __is_enum() 由编译器支持

template<class T>
constexpr bool is_enum_v = is_enum<T>::value;


// is_union and is_union_v
template<class T>
struct is_union : sx_bool_constant_t<__is_union(T)> {};	// __is_union() 由编译器支持

template<class T>
constexpr bool is_union_v = is_union<T>::value;


// is_class and is_class_v
template<class T>
struct is_class : sx_bool_constant_t<__is_class(T)> {};	// __is_class() 由编译器支持

template<class T>
constexpr bool is_class_v = is_class<T>::value;


#ifndef __IS_FUNCTION_ACHIEVE
#define	__IS_FUNCTION_ACHIEVE 1

#if __IS_FUNCTION_ACHIEVE == 0

#ifndef __IS_FUNCTION_HELPER
#define __IS_FUNTCION_HELPER

// 一些辅助宏定义
#define __IS_FUNCTION_BASE_TYPE()						\
template<class>											\
struct __is_function_helper :  sx_false_type {}

#define __IS_FUNCTION_ARG(CV_REF_NOEXCEPT)												\
template<class Ret, class... Args>														\
struct __is_function_helper<Ret(Args...) CV_REF_NOEXCEPT> :  sx_true_type {}

#define __IS_FUNCTION_ARGS(CV_REF_NOEXCEPT)												\
template<class Ret, class... Args>														\
struct __is_function_helper<Ret(Args..., ...) CV_REF_NOEXCEPT> :  sx_true_type {}


#define __IS_FUNCTION(CV_REF_NOEXCEPT)			\
__IS_FUNCTION_ARG(CV_REF_NOEXCEPT);				\
__IS_FUNCTION_ARGS(CV_REF_NOEXCEPT)

#define	__NULL_CV_REF_NOEXCEPT 

// 对常规函数的特化
// 对变参函数模板，列入 std::printf 的特化
// 对拥有 cv 限定符的函数类型的特化
// 对拥有 noexcept 限定符的函数类型的特化

__IS_FUNCTION_BASE_TYPE();
__IS_FUNCTION(__NULL_CV_REF_NOEXCEPT);
__IS_FUNCTION(const);
__IS_FUNCTION(volatile);
__IS_FUNCTION(const volatile);
__IS_FUNCTION(&);
__IS_FUNCTION(const&);
__IS_FUNCTION(volatile&);
__IS_FUNCTION(const volatile&);
__IS_FUNCTION(&&);
__IS_FUNCTION(const&&);
__IS_FUNCTION(volatile&&);
__IS_FUNCTION(const volatile&&);

#if __cplusplus >= 201103L
__IS_FUNCTION(noexcept);
__IS_FUNCTION(const noexcept);
__IS_FUNCTION(volatile noexcept);
__IS_FUNCTION(const volatile noexcept);
__IS_FUNCTION(& noexcept);
__IS_FUNCTION(const& noexcept);
__IS_FUNCTION(volatile& noexcept);
__IS_FUNCTION(const volatile& noexcept);
__IS_FUNCTION(&& noexcept);
__IS_FUNCTION(const&& noexcept);
__IS_FUNCTION(volatile&& noexcept);
__IS_FUNCTION(const volatile&& noexcept);
#endif	// __cplusplus >= 201711L
#undef	__NULL_CV_REF_NOEXCEPT
#undef	__IS_FUNCTION_HELPER
#endif // !__IS_FUNCTION_HELPER

#else

// 一种简单直接且有效的实现方法
template<class T>
struct __is_function_helper : sx_bool_constant_t<!is_reference_v<T> && !is_const_v<const T>> {};

#endif // __IS_FUNCTION_ACHIEVE == 0
#endif // !__IS_FUNCTION_ACHIEVE


// is_function and is_function_v
template<class T>
struct is_function : __is_function_helper<T> {};

template<class T>
constexpr bool is_function_v = is_function<T>::value;


// is_pointer and is_pointer_v
template<class T>
struct __is_pointer_helper : sx_false_type {};

template<class T>
struct __is_pointer_helper<T*> : sx_true_type {};

template<class T>
struct is_pointer : __is_pointer_helper<remove_cv_t<T>> {};

template<class T>
constexpr bool is_pointer_v = is_pointer<T>::value;


// is_lvalue_reference and is_lvalue_reference_v
template<class T>
struct is_lvalue_reference : sx_false_type {};

template<class T>
struct is_lvalue_reference<T&> : sx_true_type {};

template<class T>
constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;


// is_rvalue_reference and is_rvalue_reference_v
template<class T>
struct is_rvalue_reference : sx_false_type {};

template<class T>
struct is_rvalue_reference<T&&> : sx_true_type {};

template<class T>
constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;


// is_member_pointer and is_member_pointer_v
template<class T>
struct __is_member_pointer_helper : sx_false_type {};

template<class T, class U>
struct __is_member_pointer_helper<T U::*> : sx_true_type {};

template<class T>
struct is_member_pointer : __is_member_pointer_helper<remove_cv_t<T>> {};

template<class T>
constexpr bool is_member_pointer_v = is_member_pointer<T>::value;


// is_member_object_pointer and is_member_object_pointer_v
template<class T>
struct __is_member_object_pointer_helper : sx_false_type {};

template<class T, class U>
struct __is_member_object_pointer_helper<T U::*> : sx_bool_constant_t<!is_function_v<T>> {};

template<class T>
struct is_member_object_pointer : __is_member_object_pointer_helper<T> {};

template<class T>
constexpr bool is_member_object_pointer_v = is_member_object_pointer<T>::value;


// is_member_function_pointer and is_member_function_pointer_v
template<class T>
struct is_member_function_pointer : sx_bool_constant_t<is_member_pointer_v<T> && !is_member_object_pointer_v<T>> {};

template<class T>
constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;



SX_NAMESPACE_END
#endif	// end _SX_TYPE_TRAITS_H_