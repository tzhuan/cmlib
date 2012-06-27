/*
Copyright NTU CSIE CMLAB 2005 - 2012
Distributed under the Boost Software License, Version 1.0.
(See accompanying file ../../LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef CMLIB_IMAGE_FUNCTIONAL
#define CMLIB_IMAGE_FUNCTIONAL

#include <functional>

namespace cmlib {
namespace image {

	template<typename ObjectType, typename ConstantType, typename Op>
	struct OpAssignsConstant {
		const ConstantType& constant;	
		const Op& op;

		OpAssignsConstant(const ConstantType& c, const Op& o): 
			constant(c), op(o)
		{
		}

		ObjectType& operator()(ObjectType& obj) const
		{
			return op(obj, constant);
		}
	};

	template<typename Arg1, typename Arg2, typename Result>
	struct Plus: public std::binary_function<Arg1, Arg2, Result> {
		Result operator()(const Arg1& arg1, const Arg2& arg2) const
		{ 
			return Result(arg1 + arg2); 
		}
	};

	template<typename Arg1, typename Arg2, typename Result>
	struct Minus: public std::binary_function<Arg1, Arg2, Result> {
		Result operator()(const Arg1& arg1, const Arg2& arg2) const
		{ 
			return Result(arg1 - arg2); 
		}
	};

	template<typename Arg1, typename Arg2, typename Result>
	struct Multiplies: public std::binary_function<Arg1, Arg2, Result> {
		Result operator()(const Arg1& arg1, const Arg2& arg2) const
		{ 
			return Result(arg1  * arg2); 
		}
	};

	template<typename Arg1, typename Arg2, typename Result>
	struct Divides: public std::binary_function<Arg1, Arg2, Result> {
		Result operator()(const Arg1& arg1, const Arg2& arg2) const
		{ 
			return Result(arg1 / arg2); 
		}
	};

	template<typename Arg1, typename Arg2, typename Result>
	struct Modulus: public std::binary_function<Arg1, Arg2, Result> {
		Result operator()(const Arg1& arg1, const Arg2& arg2) const
		{ 
			return Result(arg1 % arg2); 
		}
	};

	template<typename Arg1, typename Arg2, typename Result>
	struct LogicalAnd: public std::binary_function<Arg1, Arg2, Result> {
		Result operator()(const Arg1& arg1, const Arg2& arg2) const
		{ 
			return Result(arg1 & arg2); 
		}
	};

	template<typename Arg1, typename Arg2, typename Result>
	struct LogicalOr: public std::binary_function<Arg1, Arg2, Result> {
		Result operator()(const Arg1& arg1, const Arg2& arg2) const
		{ 
			return Result(arg1 | arg2); 
		}
	};

	template<typename Arg1, typename Arg2, typename Result>
	struct LogicalXor: public std::binary_function<Arg1, Arg2, Result> {
		Result operator()(const Arg1& arg1, const Arg2& arg2) const
		{ 
			return Result(arg1 ^ arg2); 
		}
	};

	template<typename Arg1, typename Arg2>
	struct PlusAssigns: public std::binary_function<Arg1, Arg2, Arg1> {
		Arg1& operator()(Arg1& arg1, const Arg2& arg2) const
		{ 
			return (arg1 += arg2); 
		}
	};

	template<typename Arg1, typename Arg2>
	struct MinusAssigns: public std::binary_function<Arg1, Arg2, Arg1> {
		Arg1& operator()(Arg1& arg1, const Arg2& arg2) const
		{ 
			return (arg1 -= arg2); 
		}
	};

	template<typename Arg1, typename Arg2>
	struct MultiplyAssigns: public std::binary_function<Arg1, Arg2, Arg1> {
		Arg1& operator()(Arg1& arg1, const Arg2& arg2) const
		{ 
			return (arg1 *= arg2); 
		}
	};

	template<typename Arg1, typename Arg2>
	struct DivideAssigns: public std::binary_function<Arg1, Arg2, Arg1> {
		Arg1& operator()(Arg1& arg1, const Arg2& arg2) const
		{ 
			return (arg1 /= arg2); 
		}
	};

	template<typename Arg1, typename Arg2>
	struct ModuluAssigns: public std::binary_function<Arg1, Arg2, Arg1> {
		Arg1& operator()(Arg1& arg1, const Arg2& arg2) const
		{ 
			return (arg1 %= arg2); 
		}
	};

	template<typename Arg1, typename Arg2>
	struct LogicalAndAssigns: public std::binary_function<Arg1, Arg2, Arg1> {
		Arg1& operator()(Arg1& arg1, const Arg2& arg2) const
		{ 
			return (arg1 &= arg2); 
		}
	};

	template<typename Arg1, typename Arg2>
	struct LogicalOrAssigns: public std::binary_function<Arg1, Arg2, Arg1> {
		Arg1& operator()(Arg1& arg1, const Arg2& arg2) const
		{ 
			return (arg1 |= arg2); 
		}
	};

	template<typename Arg1, typename Arg2>
	struct LogicalXorAssigns: public std::binary_function<Arg1, Arg2, Arg1> {
		Arg1& operator()(Arg1& arg1, const Arg2& arg2) const
		{ 
			return (arg1 ^= arg2); 
		}
	};

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_FUNCTIONAL
