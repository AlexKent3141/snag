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

class Session
{
public:
  using Test = std::function<TestResult()>;

  static Session& GetInstance();

  void AddTest(const Test&);

  TestResult Run() const;

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

void Session::AddTest(const Test& test)
{
  tests_.push_back(test);
}

TestResult Session::Run() const
{
  auto res = TestResult::SUCCESS;;
  for (const Test& test : tests_)
  {
    if (test() == TestResult::FAIL)
    {
      res = TestResult::FAIL;
    }
  }

  return res;
}

class TestAdder
{
public:
  TestAdder(const Session::Test& test)
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

#define UNIT_TEST(name) \
  static passage::TestResult name(); \
  static passage::TestAdder adder_##name(&name); \
  static passage::TestResult name()

#define REQUIRE(expression) \
  if (!(expression)) \
    return passage::TestResult::FAIL;

#endif // __PASSAGE_H_INCLUDED__
