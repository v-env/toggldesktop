// Copyright 2019 Toggl Desktop developers.

#include "online_test.h"

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


static Poco::Event *semaphore = new Poco::Event();
static std::map<uint64_t, std::string> countries;
static uint64_t workspace = 0;
test::App *app;



int main(int argc, char **argv) {
    srand(time(nullptr));
    semaphore->reset();

    std::cerr << "Starting\n";
    Poco::File f(TESTDB);
    if (f.exists()) {
        f.remove(false);
    }

    app = new test::App;
    test::Dispatcher::dispatch();

    app->uiStart();
    test::Dispatcher::dispatch();

    std::string name = test::App::randomUser();
    std::string pass = test::App::randomPassword();
    app->signup(name, pass);
    test::Dispatcher::dispatch();

    app->logout();
    test::Dispatcher::dispatch();

    app->login(name, pass);
    test::Dispatcher::dispatch();

    toggl_create_client(app->context_, workspace, "Client Eastwood");
    test::Dispatcher::dispatch();

    auto guid_1 = toggl_start(app->context_, "First", "", 0, 0, nullptr, nullptr, false);
    test::Dispatcher::dispatch();
    toggl_stop(app->context_, false);
    test::Dispatcher::dispatch();
    auto guid_2 = toggl_start(app->context_, "Second", "", 0, 0, nullptr, nullptr, false);
    test::Dispatcher::dispatch();
    toggl_stop(app->context_, false);
    test::Dispatcher::dispatch();
    toggl_set_time_entry_start(app->context_, guid_1, "12:00");
    test::Dispatcher::dispatch();
    toggl_set_time_entry_duration(app->context_, guid_1, "13:00");
    test::Dispatcher::dispatch();
    toggl_set_time_entry_start(app->context_, guid_2, "15:00");
    test::Dispatcher::dispatch();
    toggl_set_time_entry_duration(app->context_, guid_2, "14:00");
    test::Dispatcher::dispatch();
    auto guid_3 = toggl_start(app->context_, "Third", "", 0, 0, nullptr, nullptr, false);
    test::Dispatcher::dispatch();

    while (true) {
        test::Dispatcher::dispatch();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

test::App::App() {
    toggl_set_log_path("test.log");

    context_ = toggl_context_init("tests", "0.1");

    Dispatcher::wireUp(context_);

    poco_assert(toggl_set_db_path(context_, TESTDB));

    Poco::Path path("src/ssl/cacert.pem");
    toggl_set_cacert_path(context_, path.toString().c_str());

    toggl_get_countries(context_);
}

test::App::~App()
{
    //toggl_context_clear(ctx_);
}

std::string test::App::randomString(size_t length) {
    auto randchar = []() -> char {
        const char charset[] = "0123456789"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ static_cast<size_t>(rand()) % max_index ];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

std::string test::App::randomUser() {
    return "toggldesktop-test-" + randomString(8) + "@" + randomString(16) + ".com";
}

std::string test::App::randomPassword() {
    return randomString(16);
}

void test::App::uiStart() {
    toggl_ui_start(context_);
}

void test::App::signup(std::string name, std::string password, Country *country) {
    // if there's no country, pick a random one
    if (!country) {
        if (countries_.size() > 0) {
            auto it = countries_.begin();
            std::advance(it, static_cast<size_t>(rand()) % countries_.size());
            country = *it;
        }
        else {
            // !!!
        }
    }
    std::cerr << "Attempting to create user \"" << name << "\" with password \"" << password << "\"" << std::endl;
    if (!country)
        toggl_signup(context_, name.c_str(), password.c_str(), 1);
    else
        toggl_signup(context_, name.c_str(), password.c_str(), country->id_);
}

void test::App::login(std::string name, std::string password) {
    toggl_login(context_, name.c_str(), password.c_str());
}

void test::App::logout() {
    toggl_logout(context_);
}
