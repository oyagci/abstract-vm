#pragma once
#include "IOperand.hpp"

namespace avm {

	class OperandFactory
	{
	private:
		OperandFactory() = default;

	public:
		static OperandFactory &Get()
		{
			static OperandFactory l_instance;
			return l_instance;
		}

		IOperand const *CreateOperand(OperandType p_type, String const &p_value) const;

	private:
		IOperand const *CreateInt8(String const &p_value) const;
		IOperand const *CreateInt16(String const &p_value) const;
		IOperand const *CreateInt32(String const &p_value) const;
		IOperand const *CreateFloat(String const &p_value) const;
		IOperand const *CreateDouble(String const &p_value) const;

		typedef IOperand const *(OperandFactory::*OperandFn)(String const &) const;
	};
}
