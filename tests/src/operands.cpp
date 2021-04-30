#include "gtest/gtest.h"
#include "avm.hpp"
#include "src/IOperand.hpp"
#include "src/OperandFactory.hpp"
#include "src/Operand.hpp"

using namespace avm;

class OperandsTest : public ::testing::Test
{
};

template <typename InternalType>
void TestOperand(IOperand const *p_operand, InternalType p_value)
{
	ASSERT_NE(p_operand, nullptr);

	Operand<InternalType> const *l_op = dynamic_cast<Operand<InternalType> const *>(p_operand);
	ASSERT_NE(l_op, nullptr);

	ASSERT_EQ(l_op->GetValue(), p_value);
}

template <typename InternalType>
void TestOperandFloating(IOperand const *p_operand, InternalType p_value)
{
	ASSERT_NE(p_operand, nullptr);

	Operand<InternalType> const *l_op = dynamic_cast<Operand<InternalType> const *>(p_operand);
	ASSERT_NE(l_op, nullptr);

	ASSERT_FLOAT_EQ(l_op->GetValue(), p_value);
}

TEST_F(OperandsTest, Int8)
{
	TestOperand<int8_t>(OperandFactory::Get().CreateOperand(eOperandType::INT8, "42"), 42);
}

TEST_F(OperandsTest, Int8_Neg)
{
	TestOperand<int8_t>(OperandFactory::Get().CreateOperand(eOperandType::INT8, "-42"), -42);
}

TEST_F(OperandsTest, Int16)
{
	TestOperand<int16_t>(OperandFactory::Get().CreateOperand(eOperandType::INT16, "42"), 42);
}

TEST_F(OperandsTest, Int16_Neg)
{
	TestOperand<int16_t>(OperandFactory::Get().CreateOperand(eOperandType::INT16, "-42"), -42);
}

TEST_F(OperandsTest, Int32)
{
	TestOperand<int32_t>(OperandFactory::Get().CreateOperand(eOperandType::INT32, "42"), 42);
}

TEST_F(OperandsTest, Int32_Neg)
{
	TestOperand<int32_t>(OperandFactory::Get().CreateOperand(eOperandType::INT32, "-42"), -42);
}

TEST_F(OperandsTest, Float)
{
	TestOperandFloating<float>(OperandFactory::Get().CreateOperand(eOperandType::FLOAT, "42.42"), 42.42);
}

TEST_F(OperandsTest, Float_Neg)
{
	TestOperandFloating<float>(OperandFactory::Get().CreateOperand(eOperandType::FLOAT, "-42.42"), -42.42);
}

TEST_F(OperandsTest, Double)
{
	TestOperandFloating<double>(OperandFactory::Get().CreateOperand(eOperandType::DOUBLE, "42.42"), 42.42);
}

TEST_F(OperandsTest, Double_Neg)
{
	TestOperandFloating<double>(OperandFactory::Get().CreateOperand(eOperandType::DOUBLE, "-42.42"), -42.42);
}

TEST_F(OperandsTest, Overflow_Float)
{
	ASSERT_THROW(OperandFactory::Get().CreateOperand(eOperandType::FLOAT, "99999340282346638528859811704183484516925441"), std::runtime_error);
}

TEST_F(OperandsTest, Overflow_Double)
{
	ASSERT_THROW(OperandFactory::Get().CreateOperand(eOperandType::FLOAT, "99999340282346638528859811704183484516925441"), std::runtime_error);
}

TEST_F(OperandsTest, Overflow_Int8)
{
	ASSERT_THROW({ OperandFactory::Get().CreateOperand(eOperandType::INT8, "512"); }, std::overflow_error);
}

TEST_F(OperandsTest, Underflow_Int8)
{
	ASSERT_THROW({ OperandFactory::Get().CreateOperand(eOperandType::INT8, "-512"); }, std::underflow_error);
}

TEST_F(OperandsTest, Overflow_Int16)
{
	ASSERT_THROW(OperandFactory::Get().CreateOperand(eOperandType::INT16, "32768"), std::runtime_error);
}

TEST_F(OperandsTest, Overflow_Int32)
{
	ASSERT_THROW(OperandFactory::Get().CreateOperand(eOperandType::INT32, "4294967296"), std::runtime_error);
}

TEST_F(OperandsTest, ToString)
{
	auto const *l_op = new Operand<int8_t>(-126, eOperandType::INT8);

	ASSERT_EQ(l_op->toString(), "-126");
}

TEST_F(OperandsTest, Add)
{
	auto const *l_a = dynamic_cast<Operand<int32_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT32, "1"));
	auto const *l_b = dynamic_cast<Operand<int32_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT32, "1"));
	auto const *l_c = dynamic_cast<Operand<int32_t> const *>(*l_a + *l_b);

	ASSERT_EQ(l_c->GetValue(), 2);
}

TEST_F(OperandsTest, Sub)
{
	auto const *l_a = dynamic_cast<Operand<int32_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT32, "43"));
	auto const *l_b = dynamic_cast<Operand<int32_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT32, "1"));
	auto const *l_c = dynamic_cast<Operand<int32_t> const *>(*l_a - *l_b);

	ASSERT_EQ(l_c->GetValue(), 42);
}

TEST_F(OperandsTest, Mul)
{
	auto const *l_a = dynamic_cast<Operand<int32_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT32, "3"));
	auto const *l_b = dynamic_cast<Operand<int32_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT32, "6"));
	auto const *l_c = dynamic_cast<Operand<int32_t> const *>(*l_a * *l_b);

	ASSERT_EQ(l_c->GetValue(), 3 * 6);
}

TEST_F(OperandsTest, Div)
{
	auto const *l_a = dynamic_cast<Operand<int32_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT32, "10"));
	auto const *l_b = dynamic_cast<Operand<int32_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT32, "2"));
	auto const *l_c = dynamic_cast<Operand<int32_t> const *>(*l_a / *l_b);

	ASSERT_EQ(l_c->GetValue(), 10 / 2);
}

TEST_F(OperandsTest, Mod)
{
	auto const *l_a = dynamic_cast<Operand<int32_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT32, "1234"));
	auto const *l_b = dynamic_cast<Operand<int32_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT32, "30"));
	auto const *l_c = dynamic_cast<Operand<int32_t> const *>(*l_a % *l_b);

	ASSERT_EQ(l_c->GetValue(), 1234 % 30);
}

TEST_F(OperandsTest, Overflow_Add_Int8)
{
	auto const *l_a = dynamic_cast<Operand<int8_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT8, "127"));
	auto const *l_b = dynamic_cast<Operand<int8_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT8, "127"));

	ASSERT_NE(l_a, nullptr);
	ASSERT_NE(l_b, nullptr);
	ASSERT_THROW(*l_a + *l_b, std::runtime_error);
}

TEST_F(OperandsTest, Underflow_Sub_Int8)
{
	auto const *l_a = dynamic_cast<Operand<int8_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT8, "-125"));
	auto const *l_b = dynamic_cast<Operand<int8_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT8, "127"));

	ASSERT_NE(l_a, nullptr);
	ASSERT_NE(l_b, nullptr);
	ASSERT_THROW(*l_a - *l_b, std::runtime_error);
}

TEST_F(OperandsTest, Underflow_Mul_Int8)
{
	auto const *l_a = dynamic_cast<Operand<int8_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT8, "127"));
	auto const *l_b = dynamic_cast<Operand<int8_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT8, "-3"));

	ASSERT_NE(l_a, nullptr);
	ASSERT_NE(l_b, nullptr);
	ASSERT_THROW(*l_a * *l_b, std::runtime_error);
}

TEST_F(OperandsTest, Overflow_Mul_Int8)
{
	auto const *l_a = dynamic_cast<Operand<int8_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT8, "127"));
	auto const *l_b = dynamic_cast<Operand<int8_t> const *>(OperandFactory::Get().CreateOperand(eOperandType::INT8, "3"));

	ASSERT_NE(l_a, nullptr);
	ASSERT_NE(l_b, nullptr);
	ASSERT_THROW(*l_a * *l_b, std::runtime_error);
}
