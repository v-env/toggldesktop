// Copyright 2019 Toggl Desktop developers.

#ifndef SRC_ONLINE_TEST_H_
#define SRC_ONLINE_TEST_H_

//#include "gtest/gtest.h"
#include "../toggl_api.h"

#include "online_test_model.h"
#include "online_test_dispatcher.h"

#include <string>
#include <set>
#include <algorithm>

#define TESTDB "test.db"

namespace test {

struct Country;

struct App {
    App();
    ~App();

    static std::string randomString(size_t length);
    static std::string randomUser();
    static std::string randomPassword();

    void uiStart();

    void signup(std::string name, std::string password, Country *country = nullptr);

    void login(std::string name, std::string password);
    void logout();

    void *context_;

    std::set<Country*> countries_;
};

} // namespace test

#endif  // SRC_ONLINE_TEST_H_
