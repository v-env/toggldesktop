#include "online_test_app.h"

#include "online_test_dispatcher.h"

#include <Poco/Path.h>

#include <iostream>

namespace test {

test::App::App() {
    toggl_set_log_path("test.log");

    context_ = toggl_context_init("tests", "0.1");

    Dispatcher::wireUp(context_, this);

    poco_assert(toggl_set_db_path(context_, TESTDB));

    Poco::Path path("src/ssl/cacert.pem");
    toggl_set_cacert_path(context_, path.toString().c_str());

    Dispatcher::dispatch();
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

void App::getCountries() {
    toggl_get_countries(context_);
    while (countries_.size() <= 0) {
        test::Dispatcher::waitForEvents();
    }
}

void test::App::uiStart() {
    toggl_ui_start(context_);
    test::Dispatcher::dispatch();
}

bool App::isStarted() const {
    return isStarted_;
}

bool test::App::signup(std::string name, std::string password, const Country &country) {
    std::cerr << "Attempting to create user \"" << name << "\" with password \"" << password << "\"" << std::endl;
    toggl_signup(context_, name.c_str(), password.c_str(), country.id_);
}

bool App::signup(std::string name, std::string password) {
    if (countries_.empty()) {
        std::cerr << "Cannot register when countries are empty" << std::endl;
        return false;
    }
    // choose a random one
    auto it = countries_.begin();
    std::advance(it, static_cast<size_t>(rand()) % countries_.size());
    auto ret = signup(name, password, *it);
    test::Dispatcher::dispatch();
    return ret;
}

bool test::App::login(std::string name, std::string password) {
    auto ret = toggl_login(context_, name.c_str(), password.c_str());
    test::Dispatcher::dispatch();
    return ret;
}

bool test::App::logout() {
    auto ret = toggl_logout(context_);
    test::Dispatcher::dispatch();
    return ret;
}

bool App::isLoggedIn() const {
    return loggedInId_ > 0 && !loggedInEmail_.empty();
}

const std::set<Country> &App::countries() {
    return countries_;
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
    std::cerr << "ERROR: " << errmsg << std::endl;
}

void test::App::on_online_state(const int64_t state) {
    WHEREAMI;
}

void test::App::on_url(const std::string &url) {
    WHEREAMI;
}

void test::App::on_login(bool open, const uint64_t user_id) {
    WHEREAMI;
    loggedInId_ = user_id;
    loggedInEmail_ = toggl_get_user_email(context_);
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
    for (auto i : list) {
        countries_.insert(i);
    }
}

} // namespace test
