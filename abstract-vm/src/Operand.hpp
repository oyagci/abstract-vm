#pragma once
#include "IOperand.hpp"
#include "OperandFactory.hpp"
#include <fmt/format.h>
#include <stdexcept>

namespace avm {

	class OperandBase : public IOperand
	{
	public:
		virtual ~OperandBase() = default;
		virtual IOperand const *From(String p_value) const = 0;
	};

	template <typename T>
	class Operand : public OperandBase
	{
	public:
		Operand<T>() = delete;
		Operand<T>(T p_value, eOperandType p_type);
		Operand<T>(const Operand<T> &) = delete;
		virtual ~Operand() = default;
		Operand<T> &operator=(Operand<T> const &other);

		eOperandType getType() const override;
		int getPrecision() const override;
		T GetValue() const;

		IOperand const *operator+(IOperand const &rhs) const override;
		IOperand const *operator-(IOperand const &rhs) const override;
		IOperand const *operator*(IOperand const &rhs) const override;
		IOperand const *operator/(IOperand const &rhs) const override;
		IOperand const *operator%(IOperand const &rhs) const override;
		bool operator!=(IOperand const &rhs) const override;

		std::string const &toString() const override;
		IOperand const *From(String p_value) const override;

		constexpr T MinLimit() const;
		constexpr T MaxLimit() const;

		void ThrowIfOverflowUnderflowAdd(IOperand const &rhs) const;
		void ThrowIfOverflowUnderflowSub(IOperand const &rhs) const;
		void ThrowIfOverflowUnderflowMul(IOperand const &rhs) const;
		void ThrowIfOverflowUnderflowDiv(IOperand const &rhs) const;
		void ThrowIfOverflowUnderflowMod(IOperand const &rhs) const;

	private:
		T m_value;
		eOperandType m_type;
		String m_valueStr;
	};

	class DivisionByZero : public std::runtime_error
	{
	public:
		DivisionByZero();

		const char *what() const noexcept override;
	};
}

