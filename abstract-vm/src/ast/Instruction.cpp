#include "Instruction.hpp"

namespace avm {
namespace ast {

	// Instruction
	// ===========

	Instruction::Instruction(Type p_type) : m_type(p_type)
	{
	}

	Instruction::Type Instruction::GetType() const { return m_type; }


	void Instruction::Print() const
	{
		fmt::print("{}\n", m_type);
	}

	void Instruction::Accept(InstructionVisitor &p_visitor) const
	{
		p_visitor.VisitInstruction(*this);
	}

	// InstructionWithValue
	// ====================

	InstructionWithValue::InstructionWithValue(Instruction::Type p_type, UniquePtr<Value const> p_value)
		: Instruction(p_type), m_value(std::move(p_value))
	{
	}

	Value const *InstructionWithValue::GetValue() const { return m_value.get(); }

	void InstructionWithValue::Print() const
	{
		fmt::print("{} ", m_type);
		m_value->Print();
	}

	void InstructionWithValue::Accept(InstructionVisitor &p_visitor) const
	{
		p_visitor.VisitInstructionWithValue(*this);
	}


	// Program
	// =======

	void Program::AddInstruction(UniquePtr<Instruction const> p_instruction)
	{
		m_instructions.push_back(std::move(p_instruction));
	}

	UniquePtr<Instruction const> Program::GetNextInstruction()
	{
		if (m_instructions.size() > 0)
		{
			auto l_ret = std::move(m_instructions.front());

			m_instructions.pop_front();

			return l_ret;
		}
		else
		{
			return nullptr;
		}
	}

	void Program::Print() const
	{
		for (auto const &l_i : m_instructions)
		{
			fmt::print("Instruction\n\t");
			l_i->Print();
		}
	}
}
}
