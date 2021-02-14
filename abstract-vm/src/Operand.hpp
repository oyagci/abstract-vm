#include "IOperand.hpp"
#include "OperandFactory.hpp"
#include <fmt/format.h>
#include <stdexcept>

namespace avm {

	class DivisionByZero : public std::runtime_error
	{
	public:
		DivisionByZero() : std::runtime_error("Division by zero")
		{
		}

		const char *what() const noexcept override
		{
			return "Division by zero";
		}
	};

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
		Operand<T>(T p_value, OperandType p_type)
			: m_value(p_value), m_type(p_type)
		{
			m_valueStr = fmt::format("{}", p_value);
		}

		OperandType GetType() const override { return m_type; }
		int GetPrecision() const override { return (int)m_type; }
		T GetValue() const { return m_value; }

		IOperand const *operator+(IOperand const &rhs) const override
		{
			ThrowIfOverflowUnderflowAdd(rhs);

			if (rhs.GetPrecision() > GetPrecision())
				return rhs + *this;

			return new Operand<T>(std::stod(ToString()) + std::stod(rhs.ToString()), m_type);
		}

		IOperand const *operator-(IOperand const &rhs) const override
		{
			ThrowIfOverflowUnderflowSub(rhs);

			if (rhs.GetPrecision() > GetPrecision())
			{
				OperandBase const &l_rhsBase = dynamic_cast<OperandBase const &>(rhs);
				UniquePtr<IOperand const> l_lhs(l_rhsBase.From(ToString()));
				return *l_lhs - rhs;
			}

			return new Operand<T>(std::stod(ToString()) - std::stod(rhs.ToString()), m_type);
		}

		IOperand const *operator*(IOperand const &rhs) const override
		{
			ThrowIfOverflowUnderflowMul(rhs);

			if (rhs.GetPrecision() > GetPrecision())
				return rhs * *this;

			return new Operand<T>(std::stod(ToString()) * std::stod(rhs.ToString()), m_type);
		}

		IOperand const *operator/(IOperand const &rhs) const override
		{
			if (rhs.GetPrecision() > GetPrecision())
			{
				OperandBase const &l_rhsBase = dynamic_cast<OperandBase const &>(rhs);
				UniquePtr<IOperand const> l_lhs(l_rhsBase.From(ToString()));
				return *l_lhs / rhs;
			}

			Operand<T> const &l_rhs = dynamic_cast<Operand<T> const &>(rhs);
			if (l_rhs.GetValue() == 0)
				throw DivisionByZero();

			ThrowIfOverflowUnderflowDiv(rhs);

			return new Operand<T>(std::stod(ToString()) / std::stod(rhs.ToString()), m_type);
		}

		IOperand const *operator%(IOperand const &rhs) const override
		{
			ThrowIfOverflowUnderflowMod(rhs);

			if (rhs.GetPrecision() > GetPrecision())
			{
				OperandBase const &l_rhsBase = dynamic_cast<OperandBase const &>(rhs);
				UniquePtr<IOperand const> l_lhs(l_rhsBase.From(ToString()));
				return *l_lhs % rhs;
			}

			return new Operand<T>(fmod(std::stod(ToString()), std::stod(rhs.ToString())), m_type);
		}

		bool operator!=(IOperand const &rhs) const override
		{
			return GetType() != rhs.GetType() || ToString() != rhs.ToString();
		}

		String const &ToString() const override
		{
			return m_valueStr;
		}

		IOperand const *From(String p_value) const override
		{
			return new Operand<T>(std::stod(p_value), m_type);
		}

		constexpr T MinLimit() const { return std::numeric_limits<T>::min(); }
		constexpr T MaxLimit() const { return std::numeric_limits<T>::max(); }

		void ThrowIfOverflowUnderflowAdd(IOperand const &rhs) const
		{
			if ((std::stod(ToString()) + std::stod(rhs.ToString())) < MinLimit())
			{
				throw std::underflow_error(fmt::format("({} + {}) < {}", ToString(), rhs.ToString(), MinLimit()));
			}
			else if ((std::stod(ToString()) + std::stod(rhs.ToString())) > MaxLimit())
			{
				throw std::overflow_error(fmt::format("({} + {}) > {}", ToString(), rhs.ToString(), MaxLimit()));
			}
		}

		void ThrowIfOverflowUnderflowSub(IOperand const &rhs) const
		{
			if ((std::stod(ToString()) - std::stod(rhs.ToString())) < MinLimit())
			{
				throw std::underflow_error(fmt::format("({} - {}) < {}", ToString(), rhs.ToString(), MinLimit()));
			}
			else if ((std::stod(ToString()) - std::stod(rhs.ToString())) > MaxLimit())
			{
				throw std::overflow_error(fmt::format("({} - {}) > {}", ToString(), rhs.ToString(), MaxLimit()));
			}
		}

		void ThrowIfOverflowUnderflowMul(IOperand const &rhs) const
		{
			if ((std::stod(ToString()) * std::stod(rhs.ToString())) < MinLimit())
			{
				throw std::underflow_error(fmt::format("({} * {}) < {}", ToString(), rhs.ToString(), MinLimit()));
			}
			else if ((std::stod(ToString()) * std::stod(rhs.ToString())) > MaxLimit())
			{
				throw std::overflow_error(fmt::format("({} * {}) > {}", ToString(), rhs.ToString(), MaxLimit()));
			}
		}

		void ThrowIfOverflowUnderflowDiv(IOperand const &rhs) const
		{
			if ((std::stod(ToString()) / std::stod(rhs.ToString())) < MinLimit())
			{
				throw std::underflow_error(fmt::format("({} / {}) < {}", ToString(), rhs.ToString(), MinLimit()));
			}
			else if ((std::stod(ToString()) / std::stod(rhs.ToString())) > MaxLimit())
			{
				throw std::overflow_error(fmt::format("({} / {}) > {}", ToString(), rhs.ToString(), MaxLimit()));
			}
		}

		void ThrowIfOverflowUnderflowMod(IOperand const &rhs) const
		{
			if (fmod(std::stod(ToString()), std::stod(rhs.ToString())) < MinLimit())
			{
				throw std::underflow_error(fmt::format("({} % {}) < {}", ToString(), rhs.ToString(), MinLimit()));
			}
			else if (fmod(std::stod(ToString()), std::stod(rhs.ToString())) > MaxLimit())
			{
				throw std::overflow_error(fmt::format("({} % {}) > {}", ToString(), rhs.ToString(), MaxLimit()));
			}
		}

	private:
		T m_value;
		OperandType m_type;
		String m_valueStr;
	};

	template <>
	Operand<float>::Operand(float p_value, OperandType p_type) : m_value(p_value), m_type(p_type)
	{
		m_valueStr = fmt::format("{:.2f}", p_value);
	}

	template <>
	Operand<double>::Operand(double p_value, OperandType p_type) : m_value(p_value), m_type(p_type)
	{
		m_valueStr = fmt::format("{:.2}", p_value);
	}

	template <>
	Operand<int8_t>::Operand(int8_t p_value, OperandType p_type) : m_value(p_value), m_type(p_type)
	{
		m_valueStr = fmt::format("{}", (int)p_value);
	}
}

