#include <thread>

#include "gtest/gtest.h"
#include "Utils/EventSystem/EventComponent.hpp"
#include "Utils/Logging.hpp"
#include "Utils/Timer.hpp"

class StringEvent : public Event
{
public:
  StringEvent() : Event() {}
  StringEvent(const StringEvent& other) : Event() { _message = other._message; }
  std::string getMessage() const
  {
    return _message;
  }
  std::string _message;
};

class SingleThreadEventComponent : public EventComponent
{
public:
  SingleThreadEventComponent() : EventComponent() {}
  virtual ~SingleThreadEventComponent(){}
  void init()
  {
    subscribe(0, std::bind(&SingleThreadEventComponent::receivedStringEvent, 
            this, std::placeholders::_1));
  }

  void receivedStringEvent(const Event& event)
  {
    StringEvent stringEvent = event.as<StringEvent>();
    _message = stringEvent.getMessage();
    Log::info(_message);
  }
  
  void run()
  {
    fireEvents();
  }

  std::string _message;
};

class EventComponentTest : public testing::Test
{
protected:
  void SetUp() override
  {
    singleThread_a.init();
    singleThread_b.init();
    stringEvent._message = "single thread string event message";
  };

  void TearDown() override
  {
  };
  SingleThreadEventComponent singleThread_a;
  SingleThreadEventComponent singleThread_b;
  StringEvent stringEvent;
};

TEST_F(EventComponentTest, SingleThreadReceivedEvent)
{
  EXPECT_EQ(singleThread_a._message, "");
  EventComponent::publish(0, stringEvent);
  singleThread_a.run();
  EXPECT_EQ(singleThread_a._message, stringEvent._message);
}
