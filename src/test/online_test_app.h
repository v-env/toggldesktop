#ifndef ONLINE_TEST_APP_H
#define ONLINE_TEST_APP_H

#include "online_test_model.h"

#include <set>

#define TESTDB "test.db"

namespace test {

class Dispatcher;

class App {
public:
    App();
    ~App();

    static std::string randomString(size_t length);
    static std::string randomUser();
    static std::string randomPassword();

    void uiStart();
    bool isStarted() const;

    bool signup(std::string name, std::string password, const Country &country);
    bool signup(std::string name, std::string password);
    bool login(std::string name, std::string password);
    bool logout();
    bool isLoggedIn() const;

    const std::set<Country> &countries();

private:
    void *context_;

    bool isStarted_;

    std::string loggedInEmail_;
    uint64_t loggedInId_;

    std::set<Country> countries_;
    std::set<Workspace> workspaces_;
    std::set<Project> projects_;
    std::set<Client> clients_;
    std::set<TimeEntry> timeEntries_;
    std::set<Tag> tags_;
    std::set<Autocomplete> timeEntryAutocomplete_;
    std::set<Autocomplete> miniTimerAutocomplete_;
    std::set<Autocomplete> projectAutocomplete_;

    // callbacks
    friend class Dispatcher;
    void on_app(bool open);
    void on_pomodoro(const std::string &title, const std::string &informative_text);
    void on_pomodoro_break(const std::string &title, const std::string &informative_text);
    void on_sync_state(const int64_t sync_state);
    void on_update(const std::string &url);
    void on_unsynced_items(const int64_t count);
    void on_error(const std::string &errmsg, bool user_error);
    void on_online_state(const int64_t state);
    void on_url(const std::string &url);
    void on_login(bool open, const uint64_t user_id);
    void on_reminder(const std::string &title, const std::string &informative_text);
    void on_help_articles(const std::list<HelpArticle> &list);
    void on_time_entry_list(bool open, const std::list<TimeEntry> &list, bool show_load_more);
    void on_time_entry_autocomplete(const std::list<Autocomplete> &list);
    void on_mini_timer_autocomplete(const std::list<Autocomplete> &list);
    void on_project_autocomplete(const std::list<Autocomplete> &list);
    void on_client_select(const std::list<Client> &list);
    void on_workspace_select(const std::list<Workspace> &list);
    void on_tags(const std::list<Tag> &list);
    void on_time_entry_editor(bool open, const TimeEntry &te, const std::string &focused_field_name);
    void on_display_settings(bool open, const Settings &settings);
    void on_project_colors(const std::list<std::string> &color_list, const uint64_t color_count);
    void on_obm_experiment(const uint64_t nr, bool included, bool seen);
    void on_display_timer_state(const TimeEntry &te);
    void on_display_idle_notification(const std::string &guid, const std::string &since, const std::string &duration, const uint64_t started, const std::string &description);
    void on_countries(const std::list<Country> &list);
};

} // namespace test;

#endif // ONLINE_TEST_APP_H
