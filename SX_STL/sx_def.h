/**************************************************
 * @brief   : 进行一些必要的宏和常量，函数等的定义
 * @file    : sx_def.h
 * @author  : 宋旭
 * @date    : 2021‎年‎1‎月‎4‎日，‏‎13:58:18
 **************************************************/

#ifndef _SX_DEF_H_
#define _SX_DEF_H_
#include <cstddef>

#define SX_NAMESPACE_BEGIN namespace sx {
#define SX_NAMESPACE_END }
#define SX sx::



#define for_range(var, begin, end) for(auto var = begin; var != end; ++var)

#define TYPE(type) decltype(type)



/**
 * 函数相关
 */

// 终止程序并清理
#define EXIT(n) exit(n)


/**
 * 函数属性相关
 */

// C++ 17
// 表示被修饰的函数的返回值十分重要，或者枚举类型，对象类型十分重要不能被废弃
#define SX_NODISCARD [[nodiscard]]

// C++ 11
// 表示函数不返回值,并只能应用在函数上面,
// 如果函数真的有返回值，那么该属性无效, 并且会给出警告
#define SX_NORETURN	 [[noreturn]]
// 示例
/*
[[noreturn]] void func(int index)
{
	if (index < 0) throw "index out of range!";
}
*/

#endif	// end define _SX_DEF_H_