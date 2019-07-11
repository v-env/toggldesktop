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

#include "Poco/DateTime.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Path.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"

#define WHEREAMI do {std::cerr << __FILE__ << ":" << __LINE__ << "\t- " << __PRETTY_FUNCTION__ << std::endl; std::cerr.flush(); } while(0)


static Poco::Event *semaphore = new Poco::Event();
static std::map<uint64_t, std::string> countries;
static uint64_t workspace = 0;
test::App *app;

class MainWorker {
public:
    static void on_app(const std::list<std::any> &args/*const bool_t open*/) {
    }

    static void on_pomodoro(const std::list<std::any> &args/*const char_t *title, const char_t *informative_text*/) {
        WHEREAMI;
    }

    static void on_pomodoro_break(const std::list<std::any> &args/*const char_t *title, const char_t *informative_text*/) {
        WHEREAMI;
    }

    static void on_sync_state(const std::list<std::any> &args/*const int64_t sync_state*/) {
        WHEREAMI;
    }

    static void on_update(const std::list<std::any> &args/*const char_t *url*/) {
        WHEREAMI;
    }

    static void on_unsynced_items(const std::list<std::any> &args/*const int64_t count*/) {
        WHEREAMI;
    }

    static void on_error(const std::list<std::any> &args/*const char *errmsg, const bool_t user_error*/) {
        WHEREAMI;
        std::cerr << "ERROR: " << std::any_cast<std::string>(args.front()) << std::endl;
    }

    static void on_online_state(const std::list<std::any> &args /*const int64_t state*/) {
        WHEREAMI;
    }

    static void on_url(const std::list<std::any> &args /*const char *url*/) {
        WHEREAMI;
    }

    static void on_login(const std::list<std::any> &args /*const bool_t open, const uint64_t user_id*/) {
        WHEREAMI;
    }

    static void on_reminder(const std::list<std::any> &args /*const char *title, const char *informative_text*/) {
        WHEREAMI;
    }

    static void on_help_articles(const std::list<std::any> &args /*TogglHelpArticleView *first*/) {
        WHEREAMI;
    }

    static void on_time_entry_list(const std::list<std::any> &args /*const bool_t open, TogglTimeEntryView *first, const bool_t show_load_more*/) {
        WHEREAMI;
    }

    static void on_time_entry_autocomplete(const std::list<std::any> &args /*TogglAutocompleteView *first*/) {
        WHEREAMI;
    }

    static void on_mini_timer_autocomplete(const std::list<std::any> &args /*TogglAutocompleteView *first*/) {
        WHEREAMI;
    }

    static void on_project_autocomplete(const std::list<std::any> &args /*TogglAutocompleteView *first*/) {
        WHEREAMI;
    }

    static void on_client_select(const std::list<std::any> &args /*TogglGenericView *first*/) {
        WHEREAMI;
    }

    static void on_workspace_select(const std::list<std::any> &args /*TogglGenericView *first*/) {
        WHEREAMI;
    }

    static void on_tags(const std::list<std::any> &args /*TogglGenericView *first*/) {
        WHEREAMI;
    }

    static void on_time_entry_editor(const std::list<std::any> &args /*const bool_t open, TogglTimeEntryView *te, const char *focused_field_name*/) {
        WHEREAMI;
    }

    static void on_display_settings(const std::list<std::any> &args /*const bool_t open, TogglSettingsView *settings*/) {
        WHEREAMI;
    }

    static void on_project_colors(const std::list<std::any> &args /*string_list_t color_list, const uint64_t color_count*/) {
        WHEREAMI;
    }

    static void on_obm_experiment(const std::list<std::any> &args /*const uint64_t nr, const bool_t included, const bool_t seen*/) {
        WHEREAMI;
    }

    static void on_display_timer_state(const std::list<std::any> &args /*TogglTimeEntryView *te*/) {
        WHEREAMI;
    }

    static void on_display_idle_notification(const std::list<std::any> &args /*const char *guid, const char *since, const char *duration, const uint64_t started, const char *description*/) {
        WHEREAMI;
    }

    static void on_countries(const std::list<std::any> &args /*TogglCountryView *first*/) {
        WHEREAMI;
    }
};

class ThreadWorker {
public:
    static std::deque<std::pair<
                        std::function<void(std::list<std::any>)>,
                        std::list<std::any>>
                     > tasks;
    static std::mutex tasks_lock;

    static void dispatch() {
        while (true) {
            std::unique_lock l(tasks_lock);
            if (tasks.empty())
                return;
            auto task = tasks.front();
            tasks.pop_front();
            l.unlock();
            task.first(task.second);
        }
    }


    static void on_app(const bool_t open) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::make_pair( MainWorker::on_app, std::list<std::any>{ open } ));
    }

    static void on_pomodoro(const char_t *title, const char_t *informative_text) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_pomodoro, std::list<std::any>{ std::string(title), std::string(informative_text) } ));
    }

    static void on_pomodoro_break(const char_t *title, const char_t *informative_text) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_pomodoro_break, std::list<std::any>{ std::string(title), std::string(informative_text) } ));
    }

    static void on_sync_state(const int64_t sync_state) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_sync_state, std::list<std::any>{ sync_state } ));
    }

    static void on_update(const char_t *url) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_update, std::list<std::any>{ std::string(url) } ));
    }

    static void on_unsynced_items(const int64_t count) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_unsynced_items, std::list<std::any>{ count } ));
    }

    static void on_error(const char *errmsg, const bool_t user_error) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_error, std::list<std::any>{ std::string(errmsg), user_error } ));
    }

    static void on_online_state(const int64_t state) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_online_state, std::list<std::any>{ state } ));
    }

    static void on_url(const char *url) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_url, std::list<std::any>{ std::string(url) } ));
    }

    static void on_login(const bool_t open, const uint64_t user_id) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_login, std::list<std::any>{ open, user_id } ));
    }

    static void on_reminder(const char *title, const char *informative_text) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_reminder, std::list<std::any>{ std::string(title), std::string(informative_text) } ));
    }

    static void on_help_articles(TogglHelpArticleView *first) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_help_articles, std::list<std::any>{ first ? *first : TogglHelpArticleView{} } ));
    }

    static void on_time_entry_list(const bool_t open, TogglTimeEntryView *first, const bool_t show_load_more) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_time_entry_list, std::list<std::any>{ open, first ? *first : TogglTimeEntryView{}, show_load_more } ));
    }

    static void on_time_entry_autocomplete(TogglAutocompleteView *first) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_time_entry_autocomplete, std::list<std::any>{ first ? *first : TogglAutocompleteView{} } ));
    }

    static void on_mini_timer_autocomplete(TogglAutocompleteView *first) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_mini_timer_autocomplete, std::list<std::any>{ first ? *first : TogglAutocompleteView{} } ));
    }

    static void on_project_autocomplete(TogglAutocompleteView *first) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_project_autocomplete, std::list<std::any>{ first ? *first : TogglAutocompleteView{} } ));
    }

    static void on_client_select(TogglGenericView *first) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_client_select, std::list<std::any>{ first ? *first : TogglGenericView{} } ));
    }

    static void on_workspace_select(TogglGenericView *first) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_workspace_select, std::list<std::any>{ first ? *first : TogglGenericView{} } ));
    }

    static void on_tags(TogglGenericView *first) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_tags, std::list<std::any>{ first ? *first : TogglGenericView{} } ));
    }

    static void on_time_entry_editor(const bool_t open, TogglTimeEntryView *te, const char *focused_field_name) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_time_entry_editor, std::list<std::any>{ open, te ? *te : TogglTimeEntryView{}, std::string(focused_field_name) } ));
    }

    static void on_display_settings(const bool_t open, TogglSettingsView *settings) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_display_settings, std::list<std::any>{ open, settings ? *settings : TogglSettingsView{} } ));
    }

    static void on_project_colors(string_list_t color_list, const uint64_t color_count) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        std::list<std::string> stringlist;
        while (color_list++)
            stringlist.push_back(std::string(*color_list));
        tasks.emplace_back(std::pair( MainWorker::on_project_colors, std::list<std::any>{ stringlist, color_count } ));
    }

    static void on_obm_experiment(const uint64_t nr, const bool_t included, const bool_t seen) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_obm_experiment, std::list<std::any>{ nr, included, seen } ));
    }

    static void on_display_timer_state(TogglTimeEntryView *te) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_display_timer_state, std::list<std::any>{ te ? *te : TogglTimeEntryView{} } ));
    }

    static void on_display_idle_notification(const char *guid, const char *since, const char *duration, const uint64_t started, const char *description) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_display_idle_notification, std::list<std::any>{ std::string(guid), std::string(since), std::string(duration), started, std::string(description) } ));
    }

    static void on_countries(TogglCountryView *first) {
        WHEREAMI;
        std::scoped_lock l(tasks_lock);
        tasks.emplace_back(std::pair( MainWorker::on_countries, std::list<std::any>{ *first } ));
    }
};

std::deque<std::pair<
                    std::function<void(std::list<std::any>)>,
                    std::list<std::any>>
                 > ThreadWorker::tasks;
std::mutex ThreadWorker::tasks_lock;


int main(int argc, char **argv) {
    srand(time(nullptr));
    semaphore->reset();

    std::cerr << "Starting\n";
    Poco::File f(TESTDB);
    if (f.exists()) {
        f.remove(false);
    }

    app = new test::App;
    ThreadWorker::dispatch();

    app->uiStart();
    ThreadWorker::dispatch();

    std::string name = test::App::randomUser();
    std::string pass = test::App::randomPassword();
    app->signup(name, pass);
    ThreadWorker::dispatch();

    app->logout();
    ThreadWorker::dispatch();

    app->login(name, pass);
    ThreadWorker::dispatch();

    toggl_create_client(app->context_, workspace, "Client Eastwood");
    ThreadWorker::dispatch();

    auto guid_1 = toggl_start(app->context_, "First", "", 0, 0, nullptr, nullptr, false);
    ThreadWorker::dispatch();
    toggl_stop(app->context_, false);
    ThreadWorker::dispatch();
    auto guid_2 = toggl_start(app->context_, "Second", "", 0, 0, nullptr, nullptr, false);
    ThreadWorker::dispatch();
    toggl_stop(app->context_, false);
    ThreadWorker::dispatch();
    toggl_set_time_entry_start(app->context_, guid_1, "12:00");
    ThreadWorker::dispatch();
    toggl_set_time_entry_duration(app->context_, guid_1, "13:00");
    ThreadWorker::dispatch();
    toggl_set_time_entry_start(app->context_, guid_2, "15:00");
    ThreadWorker::dispatch();
    toggl_set_time_entry_duration(app->context_, guid_2, "14:00");
    ThreadWorker::dispatch();
    auto guid_3 = toggl_start(app->context_, "Third", "", 0, 0, nullptr, nullptr, false);
    ThreadWorker::dispatch();

    while (true) {
        ThreadWorker::dispatch();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}






test::Client *test::Client::create(void *context, const std::string &name, test::Workspace *workspace) {
    if (!context || !workspace)
        return nullptr;
    auto c = new test::Client(name);
    c->guid_ = toggl_create_client(context, workspace->id_, name.c_str());
    return c;
}

test::TimeEntry *test::TimeEntry::start(void *context, const std::string &name) {
    if (!context)
        return nullptr;
    auto te = new test::TimeEntry(name);
    te->guid_ = toggl_start(context, name.c_str(), "", 0, 0, nullptr, nullptr, false);
    return te;
}

test::Workspace *test::Workspace::fromView(TogglGenericView *view) {
    if (!view || !view->WorkspaceName)
        return nullptr;
    return new test::Workspace(view->WorkspaceName, view->WID);
}

test::App::App() {
    toggl_set_log_path("test.log");

    context_ = toggl_context_init("tests", "0.1");
    toggl_on_error(context_, ThreadWorker::on_error);

    poco_assert(toggl_set_db_path(context_, TESTDB));

    Poco::Path path("src/ssl/cacert.pem");
    toggl_set_cacert_path(context_, path.toString().c_str());

    toggl_on_show_app(context_, ThreadWorker::on_app);
    toggl_on_sync_state(context_, ThreadWorker::on_sync_state);
    toggl_on_unsynced_items(context_, ThreadWorker::on_unsynced_items);
    toggl_on_update(context_, ThreadWorker::on_update);
    toggl_on_online_state(context_, ThreadWorker::on_online_state);
    toggl_on_login(context_, ThreadWorker::on_login);
    toggl_on_url(context_, ThreadWorker::on_url);
    toggl_on_reminder(context_, ThreadWorker::on_reminder);
    toggl_on_time_entry_list(context_, ThreadWorker::on_time_entry_list);
    toggl_on_time_entry_autocomplete(context_, ThreadWorker::on_time_entry_autocomplete);
    toggl_on_mini_timer_autocomplete(context_, ThreadWorker::on_mini_timer_autocomplete);
    toggl_on_project_autocomplete(context_, ThreadWorker::on_project_autocomplete);
    toggl_on_workspace_select(context_, ThreadWorker::on_workspace_select);
    toggl_on_client_select(context_, ThreadWorker::on_client_select);
    toggl_on_tags(context_, ThreadWorker::on_tags);
    toggl_on_time_entry_editor(context_, ThreadWorker::on_time_entry_editor);
    toggl_on_settings(context_, ThreadWorker::on_display_settings);
    toggl_on_timer_state(context_, ThreadWorker::on_display_timer_state);
    toggl_on_idle_notification(context_, ThreadWorker::on_display_idle_notification);
    toggl_on_project_colors(context_, ThreadWorker::on_project_colors);
    toggl_on_help_articles(context_, ThreadWorker::on_help_articles);
    toggl_on_obm_experiment(context_, ThreadWorker::on_obm_experiment);
    toggl_on_pomodoro(context_, ThreadWorker::on_pomodoro);
    toggl_on_pomodoro_break(context_, ThreadWorker::on_pomodoro_break);
    toggl_on_countries(context_, ThreadWorker::on_countries);

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

test::Country *test::Country::fromView(TogglCountryView *view) {
    if (!view || !view->Name)
        return nullptr;
    return new test::Country(view->Name, view->ID);
}

test::Country::Country(const std::string &name, uint64_t id)
    : Model(name, id)
{

}
