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
	struct LogicAndAssigns: public std::binary_function<Arg1, Arg2, Arg1> {
		Arg1& operator()(Arg1& arg1, const Arg2& arg2) const
		{ 
			return (arg1 &= arg2); 
		}
	};

	template<typename Arg1, typename Arg2>
	struct LogicOrAssigns: public std::binary_function<Arg1, Arg2, Arg1> {
		Arg1& operator()(Arg1& arg1, const Arg2& arg2) const
		{ 
			return (arg1 |= arg2); 
		}
	};

	template<typename Arg1, typename Arg2>
	struct LogicXorAssigns: public std::binary_function<Arg1, Arg2, Arg1> {
		Arg1& operator()(Arg1& arg1, const Arg2& arg2) const
		{ 
			return (arg1 &= arg2); 
		}
	};

} // namespace image
} // namespace cmlib

#endif // CMLIB_IMAGE_FUNCTIONAL
