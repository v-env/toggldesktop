// Copyright 2019 Toggl Desktop developers.

#ifndef SRC_ONLINE_TEST_H_
#define SRC_ONLINE_TEST_H_

//#include "gtest/gtest.h"
#include "../toggl_api.h"

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

struct Model {
    Model(const std::string &name, uint64_t id = 0) : name_(name), id_(id) {}
    std::string name_;
    uint64_t id_ { 0 };
    std::string guid_ {""};
};

struct Country : public Model {
    static Country *fromView(TogglCountryView *view);
private:
    Country(const std::string &name, uint64_t id = 0);
};

struct Workspace : public Model {
    static Workspace *fromView(TogglGenericView *view);
private:
    Workspace(const std::string &name, uint64_t id = 0) : Model(name, id) {}
};

struct Client : public Model {
    static Client *create(void *context, const std::string &name, Workspace *workspace);
private:
    Client(const std::string &name, uint64_t id = 0) : Model(name, id) {}
};

struct Project : public Model {
private:
    Project(const std::string &name, uint64_t id = 0) : Model(name, id) {}
};

struct TimeEntry : public Model {
    static TimeEntry *start(void *context, const std::string &name);
private:
    TimeEntry(const std::string &name, uint64_t id = 0) : Model(name, id) {}
};


} // namespace test

#endif  // SRC_ONLINE_TEST_H_
