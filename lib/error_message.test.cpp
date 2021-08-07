//@	{"target":{"name":"error_message.test"}}

#include "./error_message.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(MakeErrorMessage)
{
	std::string testval{"foobar"};
	auto msg = fruit::make_error_message(__FILE__, __LINE__, "Test", "Testing {} placeholder", testval);
	auto const str = std::string{msg.get().data()};
	EXPECT_EQ(str, "lib/error_message.test.cpp:10: Test: Testing foobar placeholder");
}