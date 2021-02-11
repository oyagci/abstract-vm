#pragma once
#include "../abstractvm.hpp"
#include "../Lexer.hpp"

namespace avm {
namespace ast {

	class Value
	{
	public:
		Value(Token p_type, Token p_number) : m_type(p_type), m_number(p_number)
		{
		}

		virtual ~Value() {}

		Token const &GetType() const { return m_type; }
		Token const &GetToken() const { return m_number; }

		void Print() const
		{
			fmt::print("VALUE {} {}\n", m_type.m_lexeme, *m_number.m_literal);
		}

	private:
		Token m_type;
		Token m_number;
	};

	class Instruction;
	class InstructionWithValue;

	class InstructionVisitor
	{
	public:
		virtual ~InstructionVisitor() {}

		virtual void VisitInstruction(Instruction const &) {}
		virtual void VisitInstructionWithValue(InstructionWithValue const &) {}
	};

	class InstructionVisitee
	{
	public:
		virtual ~InstructionVisitee() {}

		virtual void Accept(InstructionVisitor &) const = 0;
	};

	class Instruction : public InstructionVisitee
	{
	public:
		enum class Type
		{
			PUSH,
			POP,
			DUMP,
			ASSERT,
			ADD,
			SUB,
			MUL,
			DIV,
			MOD,
			PRINT,
			EXIT,
		};

	public:
		virtual ~Instruction() {}

		Instruction() = delete;
		Instruction(Type p_type) : m_type(p_type)
		{
		}

		Type GetType() const { return m_type; }

		virtual void Print() const
		{
			fmt::print("{}\n", m_type);
		}

		void Accept(InstructionVisitor &p_visitor) const override
		{
			p_visitor.VisitInstruction(*this);
		}

	protected:
		Type const m_type;
	};

	class InstructionWithValue : public Instruction
	{
	public:
		InstructionWithValue() = delete;
		InstructionWithValue(Instruction::Type p_type, UniquePtr<Value const> p_value)
			: Instruction(p_type), m_value(std::move(p_value))
		{
		}

		virtual ~InstructionWithValue() {}

		virtual Value const *GetValue() const { return m_value.get(); }

		void Print() const override
		{
			fmt::print("{} ", m_type);
			m_value->Print();
		}

		void Accept(InstructionVisitor &p_visitor) const override
		{
			p_visitor.VisitInstructionWithValue(*this);
		}

	protected:
		UniquePtr<Value const> m_value;
	};

	class Program
	{
	public:
		virtual ~Program() {}

		void AddInstruction(UniquePtr<Instruction const> p_instruction)
		{
			m_instructions.push_back(std::move(p_instruction));
		}

		UniquePtr<Instruction const> GetNextInstruction()
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

		void Print() const
		{
			for (auto const &l_i : m_instructions)
			{
				fmt::print("Instruction\n\t");
				l_i->Print();
			}
		}

	private:
		List<UniquePtr<Instruction const>> m_instructions;
	};

}
}
