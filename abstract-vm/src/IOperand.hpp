#pragma once
#include "abstractvm.hpp"
#include <variant>
#include <sstream>
#include <string>
#include <iomanip>
#include <cmath>

#define IMPL_OPERAND(precision, type) \
	String const ToString() const override \
	{ \
		return String(#type) + "(" + std::to_string(m_value) + ")"; \
	} \
	int GetPrecision() const override { return precision; }


namespace avm {

	/*
	 * IMPORTANT: Keep enum values ordered by precision and incremental
	 */
	enum class OperandType : size_t
	{
		INT8   = 0,
		INT16  = 1,
		INT32  = 2,
		FLOAT  = 3,
		DOUBLE = 4,
	};

	class IOperand
	{
	public:
		IOperand() = default;

		virtual ~IOperand() {}

		virtual OperandType GetType() const = 0;
		virtual int GetPrecision() const = 0;

		virtual IOperand const *operator+(IOperand const &rhs) const = 0;
		virtual IOperand const *operator-(IOperand const &rhs) const = 0;
		virtual IOperand const *operator*(IOperand const &rhs) const = 0;
		virtual IOperand const *operator/(IOperand const &rhs) const = 0;
		virtual IOperand const *operator%(IOperand const &rhs) const = 0;
		virtual bool           operator!=(IOperand const &rhs) const = 0;

		virtual String const &ToString() const = 0;
	};
}
