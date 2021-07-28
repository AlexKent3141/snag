#ifndef __PASSAGE_H_INCLUDED__
#define __PASSAGE_H_INCLUDED__

#include <functional>
#include <vector>

namespace passage
{

enum class TestResult
{
  SUCCESS = 0,
  FAIL
};

struct Test
{
  int start_line_number;
  std::function<void()> action;
  TestResult result;
};

class Session
{
public:
  static Session& GetInstance();

  void AddTest(const Test);

  TestResult Run() const;

  void Fail(int line_number);

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

void Session::AddTest(const Test test)
{
  tests_.push_back(test);
}

TestResult Session::Run() const
{
  auto res = TestResult::SUCCESS;;
  for (const Test& test : tests_)
  {
    test.action();

    if (test.result == TestResult::FAIL)
    {
      res = TestResult::FAIL;
    }
  }

  return res;
}

void Session::Fail(int line_number)
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
}

class TestAdder
{
public:
  TestAdder(const Test test)
  {
    Session::GetInstance().AddTest(test);
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
  static passage::TestAdder adder_##name( \
    { __LINE__, &name, passage::TestResult::SUCCESS }); \
  static void name()

// If this assertion fails then update the state of the current test
// and stop the test here.
#define REQUIRE(expression) \
  if (!(expression)) \
  { \
    passage::Session::GetInstance().Fail(__LINE__); \
    return; \
  }

#endif // __PASSAGE_H_INCLUDED__
