#ifndef __PASSAGE_H_INCLUDED__
#define __PASSAGE_H_INCLUDED__

#include <functional>
#include <iostream>
#include <vector>
#include <string>

namespace passage
{

enum class TestResult
{
  PASS = 0,
  FAIL
};

struct Test
{
  std::string name;
  int start_line_number;
  std::function<void()> action;
  TestResult result;

  std::string file_name;
  int fail_line_number;
  std::string expression;
};

std::ostream& operator<<(std::ostream& s, const Test& test)
{
  if (test.result == TestResult::PASS)
  {
    s << test.name << " Passed" << std::endl;
  }
  else
  {
    s << test.file_name << ": line "
      << test.fail_line_number << ": "
      << test.expression << std::endl;
    s << test.name << " Failed " << std::endl;
  }

  return s;
}

class Session
{
public:
  static Session& GetInstance();

  void AddTest(Test&&);

  TestResult Run() const;

  void Fail(
    const std::string& file_name,
    int line_number,
    const std::string& expression);

private:
  std::vector<Test> tests_;

  Session() = default;
  Session(const Session&) = delete;
  Session(Session&&) = delete;
};

Session& Session::GetInstance()
{
  static Session main_session_;
  return main_session_;
}

void Session::AddTest(Test&& test)
{
  tests_.push_back(std::move(test));
}

TestResult Session::Run() const
{
  auto res = TestResult::PASS;;
  for (const Test& test : tests_)
  {
    test.action();

    std::cout << test;

    if (test.result == TestResult::FAIL)
    {
      res = TestResult::FAIL;
    }
  }

  return res;
}

// Find the failed test and decorate it with the failure details.
void Session::Fail(
  const std::string& file_name,
  int line_number,
  const std::string& expression)
{
  // Identify which test this is the result for based on line number.
  Test* matching_test = &tests_.back();
  for (std::size_t i = 0; i < tests_.size(); i++)
  {
    const Test& test = tests_[i];
    if (test.start_line_number > line_number && i > 0)
    {
      matching_test = &tests_[i - 1];
      break;
    }
  }

  matching_test->result = TestResult::FAIL;
  matching_test->file_name = file_name;
  matching_test->fail_line_number = line_number;
  matching_test->expression = expression;
}

class TestAdder
{
public:
  TestAdder(Test&& test)
  {
    Session::GetInstance().AddTest(std::move(test));
  }
};

}

// The passage library defines main.
int main()
{
  return static_cast<int>(passage::Session::GetInstance().Run());
}

// Register the test.
#define UNIT_TEST(name) \
  static void name(); \
  namespace \
  { \
    passage::TestAdder adder_##name({ #name, __LINE__, &::name }); \
  } \
  static void name()

// If this assertion fails then update the state of the current test
// and stop the test here.
#define REQUIRE(expression) \
  if (!(expression)) \
  { \
    passage::Session::GetInstance().Fail(__FILE__, __LINE__, #expression); \
    return; \
  }

#endif // __PASSAGE_H_INCLUDED__
