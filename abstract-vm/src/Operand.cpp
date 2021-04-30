#include "Operand.hpp"

namespace avm {

	template <typename T>
	Operand<T>::Operand(T p_value, eOperandType p_type)
		: m_value(p_value), m_type(p_type)
	{
		m_valueStr = fmt::format("{}", p_value);
	}

	template <>
	Operand<int>::Operand(int p_value, eOperandType p_type) : m_value(p_value), m_type(p_type)
	{
		m_valueStr = fmt::format("{}", p_value);
	}

	template <>
	Operand<int16_t>::Operand(short p_value, eOperandType p_type) : m_value(p_value), m_type(p_type)
	{
		m_valueStr = fmt::format("{}", p_value);
	}

	template <>
	Operand<float>::Operand(float p_value, eOperandType p_type) : m_value(p_value), m_type(p_type)
	{
		m_valueStr = fmt::format("{:.2f}", p_value);
	}

	template <>
	Operand<double>::Operand(double p_value, eOperandType p_type) : m_value(p_value), m_type(p_type)
	{
		m_valueStr = fmt::format("{:.2}", p_value);
	}

	template <>
	Operand<int8_t>::Operand(int8_t p_value, eOperandType p_type) : m_value(p_value), m_type(p_type)
	{
		m_valueStr = fmt::format("{}", (int)p_value);
	}

	template <typename T>
	Operand<T> &Operand<T>::operator=(Operand<T> const &other)
	{
		m_value = other.m_value;
		m_type = other.m_type;
		m_valueStr = other.m_valueStr;

		return *this;
	}

	template <typename T>
	eOperandType Operand<T>::getType() const
	{
		return m_type;
	}

	template <typename T>
	int Operand<T>::getPrecision() const
	{
		return (int)m_type;
	}

	template <typename T>
	T Operand<T>::GetValue() const
	{
		return m_value;
	}

	template <typename T>
	IOperand const *Operand<T>::operator+(IOperand const &rhs) const
	{
		ThrowIfOverflowUnderflowAdd(rhs);

		if (rhs.getPrecision() > getPrecision())
			return rhs + *this;

		return new Operand<T>(std::stod(toString()) + std::stod(rhs.toString()), m_type);
	}

	template <typename T>
	IOperand const *Operand<T>::operator-(IOperand const &rhs) const
	{
		ThrowIfOverflowUnderflowSub(rhs);

		if (rhs.getPrecision() > getPrecision())
		{
			OperandBase const &l_rhsBase = dynamic_cast<OperandBase const &>(rhs);
			UniquePtr<IOperand const> l_lhs(l_rhsBase.From(toString()));
			return *l_lhs - rhs;
		}

		return new Operand<T>(std::stod(toString()) - std::stod(rhs.toString()), m_type);
	}

	template <typename T>
	IOperand const *Operand<T>::operator*(IOperand const &rhs) const
	{
		ThrowIfOverflowUnderflowMul(rhs);

		if (rhs.getPrecision() > getPrecision())
			return rhs * *this;

		return new Operand<T>(std::stod(toString()) * std::stod(rhs.toString()), m_type);
	}

	template <typename T>
	IOperand const *Operand<T>::operator/(IOperand const &rhs) const
	{
		if (rhs.getPrecision() > getPrecision())
		{
			OperandBase const &l_rhsBase = dynamic_cast<OperandBase const &>(rhs);
			UniquePtr<IOperand const> l_lhs(l_rhsBase.From(toString()));
			return *l_lhs / rhs;
		}

		Operand<T> const &l_rhs = dynamic_cast<Operand<T> const &>(rhs);
		if (l_rhs.GetValue() == 0)
			throw DivisionByZero();

		ThrowIfOverflowUnderflowDiv(rhs);

		return new Operand<T>(std::stod(toString()) / std::stod(rhs.toString()), m_type);
	}

	template <typename T>
	IOperand const *Operand<T>::operator%(IOperand const &rhs) const
	{
		ThrowIfOverflowUnderflowMod(rhs);

		if (rhs.getPrecision() > getPrecision())
		{
			OperandBase const &l_rhsBase = dynamic_cast<OperandBase const &>(rhs);
			UniquePtr<IOperand const> l_lhs(l_rhsBase.From(toString()));
			return *l_lhs % rhs;
		}

		return new Operand<T>(fmod(std::stod(toString()), std::stod(rhs.toString())), m_type);
	}

	template <typename T>
	bool Operand<T>::operator!=(IOperand const &rhs) const
	{
		return getType() != rhs.getType() || toString() != rhs.toString();
	}

	template <typename T>
	String const &Operand<T>::toString() const
	{
		return m_valueStr;
	}

	template <typename T>
	IOperand const *Operand<T>::From(String p_value) const
	{
		return new Operand<T>(std::stod(p_value), m_type);
	}

	template <typename T>
	constexpr T Operand<T>::MinLimit() const
	{
		return std::numeric_limits<T>::min();
	}

	template <typename T>
	constexpr T Operand<T>::MaxLimit() const
	{
		return std::numeric_limits<T>::max();
	}

	template <typename T>
	void Operand<T>::ThrowIfOverflowUnderflowAdd(IOperand const &rhs) const
	{
		if ((std::stod(toString()) + std::stod(rhs.toString())) < MinLimit())
		{
			throw std::underflow_error(fmt::format("({} + {}) < {}", toString(), rhs.toString(), MinLimit()));
		}
		else if ((std::stod(toString()) + std::stod(rhs.toString())) > MaxLimit())
		{
			throw std::overflow_error(fmt::format("({} + {}) > {}", toString(), rhs.toString(), MaxLimit()));
		}
	}

	template <typename T>
	void Operand<T>::ThrowIfOverflowUnderflowSub(IOperand const &rhs) const
	{
		if ((std::stod(toString()) - std::stod(rhs.toString())) < MinLimit())
		{
			throw std::underflow_error(fmt::format("({} - {}) < {}", toString(), rhs.toString(), MinLimit()));
		}
		else if ((std::stod(toString()) - std::stod(rhs.toString())) > MaxLimit())
		{
			throw std::overflow_error(fmt::format("({} - {}) > {}", toString(), rhs.toString(), MaxLimit()));
		}
	}

	template <typename T>
	void Operand<T>::ThrowIfOverflowUnderflowMul(IOperand const &rhs) const
	{
		if ((std::stod(toString()) * std::stod(rhs.toString())) < MinLimit())
		{
			throw std::underflow_error(fmt::format("({} * {}) < {}", toString(), rhs.toString(), MinLimit()));
		}
		else if ((std::stod(toString()) * std::stod(rhs.toString())) > MaxLimit())
		{
			throw std::overflow_error(fmt::format("({} * {}) > {}", toString(), rhs.toString(), MaxLimit()));
		}
	}

	template <typename T>
	void Operand<T>::ThrowIfOverflowUnderflowDiv(IOperand const &rhs) const
	{
		if ((std::stod(toString()) / std::stod(rhs.toString())) < MinLimit())
		{
			throw std::underflow_error(fmt::format("({} / {}) < {}", toString(), rhs.toString(), MinLimit()));
		}
		else if ((std::stod(toString()) / std::stod(rhs.toString())) > MaxLimit())
		{
			throw std::overflow_error(fmt::format("({} / {}) > {}", toString(), rhs.toString(), MaxLimit()));
		}
	}

	template <typename T>
	void Operand<T>::ThrowIfOverflowUnderflowMod(IOperand const &rhs) const
	{
		if (fmod(std::stod(toString()), std::stod(rhs.toString())) < MinLimit())
		{
			throw std::underflow_error(fmt::format("({} % {}) < {}", toString(), rhs.toString(), MinLimit()));
		}
		else if (fmod(std::stod(toString()), std::stod(rhs.toString())) > MaxLimit())
		{
			throw std::overflow_error(fmt::format("({} % {}) > {}", toString(), rhs.toString(), MaxLimit()));
		}
	}

	DivisionByZero::DivisionByZero() : std::runtime_error("Division by zero")
	{
	}

	const char *DivisionByZero::what() const noexcept
	{
		return "Division by zero";
	}
}
