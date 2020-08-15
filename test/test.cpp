#include <gtest/gtest.h>
#include <string>
#include <DynamicString>

#define ASSERT_STRING_EQ(lhs, rhs) {			\
	ASSERT_EQ(lhs.size(), rhs.size());			\
	ASSERT_EQ(lhs.capacity(), rhs.capacity());	\
	ASSERT_STREQ(lhs.c_str(), rhs.c_str());		\
}

TEST(Construct, empty)
{
	String lhs;
	std::string rhs;
	ASSERT_STRING_EQ(lhs, rhs);
	ASSERT_EQ(lhs.category(), StringCategory::SC_Small);
}

TEST(Construct_small, char_ptr)
{
	String lhs("String");
	std::string rhs("String");
	ASSERT_STRING_EQ(lhs, rhs);
	ASSERT_EQ(lhs.category(), StringCategory::SC_Small);
}

TEST(Construct_large, char_ptr)
{
	String lhs("Some string 123456789");
	std::string rhs("Some string 123456789");
	ASSERT_STRING_EQ(lhs, rhs);
	ASSERT_EQ(lhs.category(), StringCategory::SC_Large);
}

TEST(Construct_small, n_copies)
{
	String lhs(15, 's');
	std::string rhs(15, 's');
	ASSERT_STRING_EQ(lhs, rhs);
	ASSERT_EQ(lhs.category(), StringCategory::SC_Small);
}

TEST(Construct_large, n_copies)
{
	String lhs(42, 's');
	std::string rhs(42, 's');
	ASSERT_STRING_EQ(lhs, rhs);
	ASSERT_EQ(lhs.category(), StringCategory::SC_Large);
}

TEST(Construct_small, copy)
{
	String lhs("string");
	String rhs(lhs);
	ASSERT_STRING_EQ(lhs, rhs);
	ASSERT_EQ(lhs.category(), StringCategory::SC_Small);
	ASSERT_EQ(rhs.category(), StringCategory::SC_Small);
}

TEST(Construct_large, copy)
{
	String lhs(42, 's');
	String rhs(lhs);
	ASSERT_STRING_EQ(lhs, rhs);
	ASSERT_EQ(lhs.category(), StringCategory::SC_Large);
	ASSERT_EQ(rhs.category(), StringCategory::SC_Large);
}

TEST(Construct_small, move)
{
	String lhs("string");
	String rhs(std::move(lhs));
	ASSERT_STRING_EQ(lhs, std::string());
	ASSERT_STRING_EQ(rhs, std::string("string"));
	ASSERT_EQ(lhs.category(), StringCategory::SC_Small);
	ASSERT_EQ(rhs.category(), StringCategory::SC_Small);
}

TEST(Construct_large, move)
{
	String lhs(42, 's');
	String rhs(std::move(lhs));
	ASSERT_STRING_EQ(lhs, std::string());
	ASSERT_STRING_EQ(rhs, std::string(42, 's'));
	ASSERT_EQ(lhs.category(), StringCategory::SC_Small);
	ASSERT_EQ(rhs.category(), StringCategory::SC_Large);
}

TEST(Append, char_ptr)
{
	String lhs;
	std::string rhs;
	const char str[] = "string123";

	lhs.append(str);
	rhs.append(str);
	ASSERT_STRING_EQ(lhs, rhs);
	ASSERT_EQ(lhs.category(), StringCategory::SC_Small);

	lhs.append(str);
	rhs.append(str);
	ASSERT_STRING_EQ(lhs, rhs);
	ASSERT_EQ(lhs.category(), StringCategory::SC_Large);
}

TEST(Append, string_class)
{
	String lhs("string");
	std::string rhs("string");

	lhs.append(lhs);
	rhs.append(rhs);
	ASSERT_STRING_EQ(lhs, rhs);
	ASSERT_EQ(lhs.category(), StringCategory::SC_Small);

	lhs.append(lhs);
	rhs.append(rhs);
	ASSERT_STRING_EQ(lhs, rhs);
	ASSERT_EQ(lhs.category(), StringCategory::SC_Large);
}

TEST(Append, n_copies)
{
	String lhs("string");
	std::string rhs("string");

	lhs.append(6, 'g');
	rhs.append(6, 'g');
	ASSERT_STRING_EQ(lhs, rhs);
	ASSERT_EQ(lhs.category(), StringCategory::SC_Small);

	lhs.append(6, 'r');
	rhs.append(6, 'r');
	ASSERT_STRING_EQ(lhs, rhs);
	ASSERT_EQ(lhs.category(), StringCategory::SC_Large);
}
