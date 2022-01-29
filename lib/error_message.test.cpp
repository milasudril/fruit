//@	{"target":{"name":"error_message.test"}}

#include "./error_message.hpp"

#include "testfwk/testfwk.hpp"

TESTCASE(MakeErrorMessage)
{
	std::string testval{"foobar"};
	auto msg = fruit::make_error_message("Foo", 10, "Test", "Testing {} placeholder", testval);
	auto const str = std::string{msg.get().data()};
	EXPECT_EQ(str, "Foo:10: Test: Testing foobar placeholder");
}