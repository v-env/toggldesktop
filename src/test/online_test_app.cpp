#include "online_test_app.h"

#include "online_test_dispatcher.h"

#include <Poco/Path.h>

#include <iostream>

namespace test {

test::App::App() {
    context_ = toggl_context_init("tests", "0.1");

    Dispatcher::wireUp(context_, this);

    toggl_set_log_path("test.log");

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
    srand(time(nullptr));
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
    isStarted_ = true;
    test::Dispatcher::dispatch();
}

bool App::isStarted() const {
    return isStarted_;
}

bool test::App::signup(std::string name, std::string password, const Country &country) {
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

std::string App::start(const std::string &description) {
    auto ret = toggl_start(context_, description.c_str(), "", 0, 0, nullptr, nullptr, 0);
    test::Dispatcher::dispatch();
    return ret;
}

const TimeEntry &App::runningTimeEntry() const {
    return runningTimeEntry_;
}

bool App::stop() {
    auto ret = toggl_stop(context_, 0);
    test::Dispatcher::dispatch();
    return ret;
}

bool App::timeEntry_setDescription(const std::string &guid, const std::string &value) {
    auto ret = toggl_set_time_entry_description(context_, guid.c_str(), value.c_str());
    test::Dispatcher::dispatch();
    return ret;
}

bool App::timeEntry_setStart(const std::string &guid, const std::string &value) {
    auto ret = toggl_set_time_entry_start(context_, guid.c_str(), value.c_str());
    test::Dispatcher::dispatch();
    return ret;
}

bool App::timeEntry_setEnd(const std::string &guid, const std::string &value) {
    auto ret = toggl_set_time_entry_end(context_, guid.c_str(), value.c_str());
    test::Dispatcher::dispatch();
    return ret;
}

bool App::timeEntry_setDuration(const std::string &guid, const std::string &value) {
    auto ret = toggl_set_time_entry_duration(context_, guid.c_str(), value.c_str());
    test::Dispatcher::dispatch();
    return ret;
}

bool App::timeEntry_setBillable(const std::string &guid, bool value) {
    auto ret = toggl_set_time_entry_billable(context_, guid.c_str(), value);
    test::Dispatcher::dispatch();
    return ret;
}

bool App::timeEntry_setTags(const std::string &guid, const std::list<std::string> &value) {
    std::string merged;
    for (auto it = value.begin(); it != value.end(); it++) {
        merged += *it;
        if (std::next(it) != value.end())
            merged += "\t";
    }
    auto ret = toggl_set_time_entry_tags(context_, guid.c_str(), merged.c_str());
    test::Dispatcher::dispatch();
    return ret;
}

bool App::timeEntry_setDate(const std::string &guid, int64_t value) {
    auto ret = toggl_set_time_entry_date(context_, guid.c_str(), value);
    test::Dispatcher::dispatch();
    return ret;
}

const std::set<Country> &App::countries() const {
    return countries_;
}

const std::list<TimeEntry> &App::timeEntries() const {
    return timeEntries_;
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
    timeEntries_ = std::list<test::TimeEntry>(list);
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
    runningTimeEntry_ = te;
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

void App::on_display_overlay(const int64_t type) {
    WHEREAMI;
}

void App::on_display_promotion(const int64_t promotion_type) {
    WHEREAMI;
}

void App::on_display_update_download_state(const std::string &version, const int64_t download_state) {
    WHEREAMI;
}

} // namespace test
