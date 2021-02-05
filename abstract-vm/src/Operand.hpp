#include "IOperand.hpp"
#include "OperandFactory.hpp"
#include <fmt/format.h>

namespace avm {

	template <typename T>
	class Operand : public IOperand
	{
	public:
		Operand<T>(T p_value, OperandType p_type)
			: m_value(p_value), m_type(p_type)
		{
			std::stringstream l_stream;
			l_stream << std::fixed << std::setprecision(2) << (int)p_value;
			m_valueStr = l_stream.str();
		}

		OperandType GetType() const override { return m_type; }
		int GetPrecision() const override { return (int)m_type; }
		T GetValue() const { return m_value; }

		IOperand const *operator+(IOperand const &rhs) const override
		{
			ThrowIfOverflowUnderflowAdd(rhs);

			return new Operand<T>(std::stod(ToString()) + std::stod(rhs.ToString()), m_type);
		}

		IOperand const *operator-(IOperand const &rhs) const override
		{
			ThrowIfOverflowUnderflowSub(rhs);

			return new Operand<T>(std::stod(ToString()) - std::stod(rhs.ToString()), m_type);
		}

		IOperand const *operator*(IOperand const &rhs) const override
		{
			ThrowIfOverflowUnderflowMul(rhs);

			return new Operand<T>(std::stod(ToString()) * std::stod(rhs.ToString()), m_type);
		}

		IOperand const *operator/(IOperand const &rhs) const override
		{
			ThrowIfOverflowUnderflowDiv(rhs);

			return new Operand<T>(std::stod(ToString()) / std::stod(rhs.ToString()), m_type);
		}

		IOperand const *operator%(IOperand const &rhs) const override
		{
			ThrowIfOverflowUnderflowMod(rhs);

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

		T MinLimit() const { return std::numeric_limits<T>::min(); }
		T MaxLimit() const { return std::numeric_limits<T>::max(); }

		void ThrowIfOverflowUnderflowAdd(IOperand const &rhs) const
		{
			if ((std::stod(ToString()) + std::stod(rhs.ToString())) < MinLimit())
			{
				throw std::underflow_error(fmt::format("({} - {}) < {}", ToString(), rhs.ToString(), MinLimit()));
			}
			else if ((std::stod(ToString()) + std::stod(rhs.ToString())) > MaxLimit())
			{
				throw std::overflow_error(fmt::format("({} - {}) > {}", ToString(), rhs.ToString(), MaxLimit()));
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
				throw std::underflow_error(fmt::format("({} - {}) < {}", ToString(), rhs.ToString(), MinLimit()));
			}
			else if ((std::stod(ToString()) * std::stod(rhs.ToString())) > MaxLimit())
			{
				throw std::overflow_error(fmt::format("({} - {}) > {}", ToString(), rhs.ToString(), MaxLimit()));
			}
		}

		void ThrowIfOverflowUnderflowDiv(IOperand const &rhs) const
		{
			if ((std::stod(ToString()) / std::stod(rhs.ToString())) < MinLimit())
			{
				throw std::underflow_error(fmt::format("({} - {}) < {}", ToString(), rhs.ToString(), MinLimit()));
			}
			else if ((std::stod(ToString()) / std::stod(rhs.ToString())) > MaxLimit())
			{
				throw std::overflow_error(fmt::format("({} - {}) > {}", ToString(), rhs.ToString(), MaxLimit()));
			}
		}

		void ThrowIfOverflowUnderflowMod(IOperand const &rhs) const
		{
			if (fmod(std::stod(ToString()), std::stod(rhs.ToString())) < MinLimit())
			{
				throw std::underflow_error(fmt::format("({} - {}) < {}", ToString(), rhs.ToString(), MinLimit()));
			}
			else if (fmod(std::stod(ToString()), std::stod(rhs.ToString())) > MaxLimit())
			{
				throw std::overflow_error(fmt::format("({} - {}) > {}", ToString(), rhs.ToString(), MaxLimit()));
			}
		}

	private:
		T m_value;
		OperandType m_type;
		String m_valueStr;
	};
}

