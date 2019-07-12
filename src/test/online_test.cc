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

    Dispatcher::wireUp(context_, this);

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

void test::App::on_app(bool open) {
    WHEREAMI;
}

void test::App::on_pomodoro(const std::string &title, const std::string &informative_text) {
    WHEREAMI;
}

void test::App::on_pomodoro_break(const std::string &title, const std::string &informative_text) {
    WHEREAMI;
}

void test::App::on_sync_state(const int64_t sync_state) {
    WHEREAMI;
}

void test::App::on_update(const std::string &url) {
    WHEREAMI;
}

void test::App::on_unsynced_items(const int64_t count) {
    WHEREAMI;
}

void test::App::on_error(const std::string &errmsg, bool user_error) {
    WHEREAMI;
}

void test::App::on_online_state(const int64_t state) {
    WHEREAMI;
}

void test::App::on_url(const std::string &url) {
    WHEREAMI;
}

void test::App::on_login(bool open, const uint64_t user_id) {
    WHEREAMI;
}

void test::App::on_reminder(const std::string &title, const std::string &informative_text) {
    WHEREAMI;
}

void test::App::on_help_articles(const std::list<test::HelpArticle> &list) {
    WHEREAMI;
}

void test::App::on_time_entry_list(bool open, const std::list<test::TimeEntry> &list, bool show_load_more) {
    WHEREAMI;
}

void test::App::on_time_entry_autocomplete(const std::list<test::Autocomplete> &list) {
    WHEREAMI;
}

void test::App::on_mini_timer_autocomplete(const std::list<test::Autocomplete> &list) {
    WHEREAMI;
}

void test::App::on_project_autocomplete(const std::list<test::Autocomplete> &list) {
    WHEREAMI;
}

void test::App::on_client_select(const std::list<test::Client> &list) {
    WHEREAMI;
}

void test::App::on_workspace_select(const std::list<test::Workspace> &list) {
    WHEREAMI;
}

void test::App::on_tags(const std::list<test::Tag> &list) {
    WHEREAMI;
}

void test::App::on_time_entry_editor(bool open, const test::TimeEntry &te, const std::string &focused_field_name) {
    WHEREAMI;
}

void test::App::on_display_settings(bool open, const test::Settings &settings) {
    WHEREAMI;
}

void test::App::on_project_colors(const std::list<std::string> &color_list, const uint64_t color_count) {
    WHEREAMI;
}

void test::App::on_obm_experiment(const uint64_t nr, bool included, bool seen) {
    WHEREAMI;
}

void test::App::on_display_timer_state(const test::TimeEntry &te) {
    WHEREAMI;
}

void test::App::on_display_idle_notification(const std::string &guid, const std::string &since, const std::string &duration, const uint64_t started, const std::string &description) {
    WHEREAMI;
}

void test::App::on_countries(const std::list<test::Country> &list) {
    WHEREAMI;
    std::cerr << "Received countries!" << std::endl;
    for (auto i : list) {
        std::cerr << "Country ID: " << i.id_ << ": " << i.name_ << std::endl;
    }
}
