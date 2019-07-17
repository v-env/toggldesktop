// Copyright 2019 Toggl Desktop developers.

#include "online_test.h"

#include "online_test_app.h"

#include <iostream>
#include <map>
#include <deque>
#include <functional>
#include <list>
#include <any>
#include <thread>
#include <mutex>
#include <chrono>
#include <variant>

#include "Poco/DateTime.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Path.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"


static struct {
    test::App *app;

    std::string name { test::App::randomUser() };
    std::string pass { test::App::randomPassword() };

    std::map<std::string, std::string> timeEntries;
} testData;

TEST(Base, Initialize) {
    std::cout << "Will test with the following credentials:" << std::endl;
    std::cout << "Username: " << testData.name << std::endl;
    std::cout << "Password: " << testData.pass << std::endl;
    testData.app = new test::App;
    ASSERT_TRUE(testData.app);
    testData.app->uiStart();
    ASSERT_TRUE(testData.app->isStarted());
}

TEST(Base, GetCountries) {
    testData.app->getCountries();
    ASSERT_FALSE(testData.app->countries().empty());
}

TEST(Base, Register) {
    ASSERT_TRUE(testData.app->signup(testData.name, testData.pass));
    ASSERT_TRUE(testData.app->isLoggedIn());
}

TEST(Base, LogOut) {
    ASSERT_TRUE(testData.app->isLoggedIn());
    ASSERT_TRUE(testData.app->logout());
    ASSERT_FALSE(testData.app->isLoggedIn());
}

TEST(Base, LogIn) {
    ASSERT_FALSE(testData.app->isLoggedIn());
    ASSERT_TRUE(testData.app->login(testData.name, testData.pass));
    ASSERT_TRUE(testData.app->isLoggedIn());
}

TEST(TimeEntry, Start) {
    auto guid = testData.app->start("Time Entry");
    ASSERT_FALSE(guid.empty());
    testData.timeEntries[guid] = "Time Entry";
    ASSERT_EQ(testData.app->runningTimeEntry().name_, "Time Entry");
    ASSERT_EQ(testData.app->runningTimeEntry().guid_, guid);
}

TEST(TimeEntry, Stop) {
    ASSERT_EQ(testData.timeEntries.size(), 1);
    ASSERT_EQ(testData.app->runningTimeEntry().guid_, testData.timeEntries.begin()->first);
    auto oldGuid = testData.app->runningTimeEntry().guid_;
    ASSERT_EQ(testData.app->runningTimeEntry().name_, testData.timeEntries.begin()->second);

    ASSERT_TRUE(testData.app->stop());
    ASSERT_TRUE(testData.app->runningTimeEntry().guid_.empty());

    ASSERT_EQ(testData.app->timeEntries().begin()->guid_, oldGuid);
}

TEST(TimeEntry, SetStart) {
    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
    auto guid = testData.timeEntries.begin()->first;

    ASSERT_TRUE(testData.app->timeEntry_setStart(guid, "12:00 PM"));
    ASSERT_EQ(testData.app->timeEntries().begin()->startedString_, "12:00 PM");

    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
}

TEST(TimeEntry, SetStartInvalidTime) {
    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
    auto guid = testData.timeEntries.begin()->first;

    ASSERT_FALSE(testData.app->timeEntry_setStart(guid, "123:00 PM"));
    // this succeeds, it probably should not
    //ASSERT_FALSE(testData.app->timeEntry_setStart(guid, "12:1 PPM"));
    ASSERT_FALSE(testData.app->timeEntry_setStart(guid, "12:123 PM"));
    ASSERT_FALSE(testData.app->timeEntry_setStart(guid, "PM:PM PM"));
    ASSERT_FALSE(testData.app->timeEntry_setStart(guid, "PM:    AM PM"));
    ASSERT_FALSE(testData.app->timeEntry_setStart(guid, "6354651"));
    ASSERT_FALSE(testData.app->timeEntry_setStart(guid, "AAAAAAAAAAAA"));
    ASSERT_FALSE(testData.app->timeEntry_setStart(guid, "|Bbdsf,§.v§ů.!ě§ů,+č§ůž§+ů,.§řůžý+,§ůlm, ůlm §ůvíé;uáé+hn"));
    ASSERT_FALSE(testData.app->timeEntry_setStart(guid, "šš:čč"));

    ASSERT_EQ(testData.app->timeEntries().begin()->startedString_, "12:00 PM");

    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
}

TEST(TimeEntry, SetStartInvalidGuid) {
    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
    auto guid = "abcd";

    // don't check, this doesn't actually return false (even though it should, in my opinion)
    testData.app->timeEntry_setStart(guid, "11:00 AM");
    // just check we didn't mess up the existing entry
    ASSERT_EQ(testData.app->timeEntries().begin()->startedString_, "12:00 PM");

    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
}

TEST(TimeEntry, SetEnd) {
    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
    auto guid = testData.timeEntries.begin()->first;

    ASSERT_TRUE(testData.app->timeEntry_setEnd(guid, "01:00 PM"));
    ASSERT_EQ(testData.app->timeEntries().begin()->endedString_, "01:00 PM");
    auto duration = testData.app->timeEntries().begin()->ended_ - testData.app->timeEntries().begin()->started_;
    ASSERT_EQ(duration, 3600);

    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
}

TEST(TimeEntry, SetEndInvalidTime) {
    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
    auto guid = testData.timeEntries.begin()->first;

    ASSERT_FALSE(testData.app->timeEntry_setEnd(guid, "123:00 PM"));
    // this succeeds, it probably should not
    //ASSERT_FALSE(testData.app->timeEntry_setStart(guid, "12:1 PPM"));
    ASSERT_FALSE(testData.app->timeEntry_setEnd(guid, "12:123 PM"));
    ASSERT_FALSE(testData.app->timeEntry_setEnd(guid, "PM:PM PM"));
    ASSERT_FALSE(testData.app->timeEntry_setEnd(guid, "PM:    AM PM"));
    ASSERT_FALSE(testData.app->timeEntry_setEnd(guid, "6354651"));
    ASSERT_FALSE(testData.app->timeEntry_setEnd(guid, "AAAAAAAAAAAA"));
    ASSERT_FALSE(testData.app->timeEntry_setEnd(guid, "|Bbdsf,§.v§ů.!ě§ů,+č§ůž§+ů,.§řůžý+,§ůlm, ůlm §ůvíé;uáé+hn"));
    ASSERT_FALSE(testData.app->timeEntry_setEnd(guid, "šš:čč"));

    ASSERT_EQ(testData.app->timeEntries().begin()->startedString_, "12:00 PM");

    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
}

TEST(TimeEntry, SetEndInvalidGuid) {
    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
    auto guid = "abcd";

    // don't check, this doesn't actually return false (even though it should, in my opinion)
    testData.app->timeEntry_setEnd(guid, "02:00 PM");
    // just check we didn't mess up the existing entry
    ASSERT_EQ(testData.app->timeEntries().begin()->endedString_, "01:00 PM");

    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
}

TEST(TimeEntry, SetDuration) {
    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
    auto guid = testData.timeEntries.begin()->first;

    ASSERT_TRUE(testData.app->timeEntry_setDuration(guid, "2:00"));
    ASSERT_EQ(testData.app->timeEntries().begin()->startedString_, "12:00 PM");
    auto duration = testData.app->timeEntries().begin()->ended_ - testData.app->timeEntries().begin()->started_;
    ASSERT_EQ(duration, 7200);

    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
}

TEST(TimeEntry, SetDurationInvalidGuid) {
    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
    auto guid = "abcd";

    // don't check, this doesn't actually return false (even though it should, in my opinion)
    testData.app->timeEntry_setStart(guid, "3:00");
    // just check we didn't mess up the existing entry
    auto duration = testData.app->timeEntries().begin()->ended_ - testData.app->timeEntries().begin()->started_;
    ASSERT_EQ(duration, 7200);

    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
}

TEST(TimeEntry, SetTags) {
    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
    auto guid = testData.timeEntries.begin()->first;

    ASSERT_TRUE(testData.app->timeEntry_setTags(guid, {"tag1", "tag2"}));
    ASSERT_EQ(testData.app->timeEntries().begin()->tags_, std::list<std::string>({"tag1", "tag2"}));

    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
}

TEST(TimeEntry, SetTagsInvalidGuid) {
    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
    auto guid = "abcd";

    // don't check, this doesn't actually return false (even though it should, in my opinion)
    testData.app->timeEntry_setTags(guid, {"tag3", "tag4"});
    // just check we didn't mess up the existing entry
    ASSERT_EQ(testData.app->timeEntries().begin()->tags_, std::list<std::string>({"tag1", "tag2"}));

    ASSERT_TRUE(testData.app->timeEntries().size() == 1);
}


