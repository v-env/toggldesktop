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

int main(int argc, char **argv) {
    srand(time(nullptr));

    std::cerr << "Starting\n";
    Poco::File f(TESTDB);
    if (f.exists()) {
        f.remove(false);
    }

    test::App *app = new test::App;

    app->getCountries();

    app->uiStart();

    std::string name = test::App::randomUser();
    std::string pass = test::App::randomPassword();
    app->signup(name, pass);
    std::cerr << "Is user logged in: " << app->isLoggedIn() << std::endl;

    app->logout();
    std::cerr << "Is user logged in: " << app->isLoggedIn() << std::endl;

    app->login(name, pass);
    std::cerr << "Is user logged in: " << app->isLoggedIn() << std::endl;

    //toggl_create_client(app->context_, workspace, "Client Eastwood");
    //test::Dispatcher::dispatch();
/*
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
*/
    while (true) {
        test::Dispatcher::dispatch();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
