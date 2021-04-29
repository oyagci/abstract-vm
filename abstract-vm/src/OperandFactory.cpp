#include "OperandFactory.hpp"
#include "abstractvm.hpp"
#include <algorithm>
#include <fast_float/fast_float.h>
#include <cmath>
#include "IOperand.hpp"
#include "Operand.hpp"

namespace avm {

	OperandFactory &OperandFactory::Get()
	{
		static OperandFactory l_instance;
		return l_instance;
	}

	IOperand const *OperandFactory::CreateOperand(OperandType p_type, String const &p_value) const
	{
		static OperandFn l_operands[5] = {
			&OperandFactory::CreateInt8,
			&OperandFactory::CreateInt16,
			&OperandFactory::CreateInt32,
			&OperandFactory::CreateFloat,
			&OperandFactory::CreateDouble
		};

		return (this->*(l_operands[static_cast<size_t>(p_type)]))(p_value);

		/*
		 * Safer implementation (with an array of pair) but oh well...
		 */

		/*
		decltype(l_operands)::const_iterator l_op = std::find_if(l_operands.begin(), l_operands.end(),
			[p_type](Pair<OperandType, OperandFn> &l_pair) -> bool {
				return l_pair.first == p_type;
			}
		);

		if (l_op != l_operands.end()) {
			return (this->*(l_op->second))(p_value);
		}

		IOperand const *l_res = (l_op != l_operands.end())
			? (this->*(l_op->second))(p_value)
			: nullptr;

		return l_res;
		*/
	}

	IOperand const *OperandFactory::CreateInt8(String const &p_value) const
	{
		int l_res = 0;

		try
		{
			l_res = std::stoi(p_value);
		}
		catch (std::exception const &e)
		{
			throw e;
		}

		if (l_res > std::numeric_limits<int8_t>::max())
		{
			throw std::overflow_error("Invalid Int8");
		}
		else if (l_res < std::numeric_limits<int8_t>::min())
		{
			throw std::underflow_error("Invalid Int16");
		}

		return new Operand<int8_t>(static_cast<int8_t>(l_res), OperandType::INT8);
	}

	IOperand const *OperandFactory::CreateInt16(String const &p_value) const
	{
		int l_res = 0;

		try
		{
			l_res = std::stoi(p_value);
		}
		catch (std::exception &e)
		{
			throw std::runtime_error("Invalid Int16");
		}

		if (l_res > std::numeric_limits<int16_t>::max())
		{
			throw std::overflow_error("Invalid Int16");
		}
		else if (l_res < std::numeric_limits<int16_t>::min())
		{
			throw std::underflow_error("Invalid Int16");
		}

		return new Operand<int16_t>(static_cast<int16_t>(l_res), OperandType::INT16);
	}

	IOperand const *OperandFactory::CreateInt32(String const &p_value) const
	{
		int l_res = 0;

		try
		{
			l_res = std::stoi(p_value);
		}
		catch (std::exception &e)
		{
			throw std::runtime_error("Invalid Int32");
		}

		if (l_res > std::numeric_limits<int32_t>::max())
		{
			throw std::overflow_error("Invalid Int32");
		}
		else if (l_res < std::numeric_limits<int32_t>::min())
		{
			throw std::underflow_error("Invalid Int32");
		}

		return new Operand<int32_t>(static_cast<int32_t>(l_res), OperandType::INT32);
	}

	IOperand const *OperandFactory::CreateFloat(String const &p_value) const
	{
		float l_value = 0.0f;

		fast_float::from_chars_result l_result =
			fast_float::from_chars(p_value.data(), p_value.data() + p_value.length(), l_value);

		if (l_result.ec != std::errc() || std::isnan(l_value))
		{
			throw std::runtime_error("Failed to parse float");
		}

		if (l_value > std::numeric_limits<float>::max())
		{
			throw std::overflow_error("Overflow Float");
		}
		else if (l_value < std::numeric_limits<float>::lowest())
		{
			fmt::print("{} {}\n", l_value, std::numeric_limits<float>::min());
			throw std::underflow_error("Underflow Float");
		}

		return new Operand<float>(static_cast<float>(l_value), OperandType::FLOAT);
	}

	IOperand const *OperandFactory::CreateDouble(String const &p_value) const
	{
		double l_value = 0.0f;

		fast_float::from_chars_result l_result =
			fast_float::from_chars(p_value.data(), p_value.data() + p_value.length(), l_value);

		if (l_result.ec != std::errc() || std::isnan(l_value))
		{
			throw std::runtime_error("Failed to parse double precision float");
		}

		if (l_value > std::numeric_limits<double>::max())
		{
			throw std::overflow_error("Invalid Double");
		}
		else if (l_value < std::numeric_limits<double>::lowest())
		{
			throw std::underflow_error("Invalid Double");
		}

		return new Operand<double>(static_cast<double>(l_value), OperandType::DOUBLE);
	}
}
