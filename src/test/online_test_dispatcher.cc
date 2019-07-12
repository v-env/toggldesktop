#include "online_test_dispatcher.h"

#include <iostream>
#include <thread>

#include "online_test_model.h"
#include "online_test.h"

/*
static std::variant<
    TogglDisplayApp,
    TogglDisplaySyncState,
    TogglDisplayUnsyncedItems,
    TogglDisplayError,
    TogglDisplayOverlay,
    TogglDisplayOnlineState,
    TogglDisplayURL,
    TogglDisplayLogin,
    TogglDisplayReminder,
    TogglDisplayPomodoro,
    TogglDisplayPomodoroBreak,
    TogglDisplayAutotrackerNotification,
    TogglDisplayPromotion,
    TogglDisplayObmExperiment,
    TogglDisplayTimeEntryList,
    TogglDisplayAutocomplete,
    TogglDisplayHelpArticles,
    TogglDisplayViewItems,
    TogglDisplayTimeEntryEditor,
    TogglDisplaySettings,
    TogglDisplayTimerState,
    TogglDisplayIdleNotification,
    TogglDisplayUpdate,
    TogglDisplayUpdateDownloadState,
    TogglDisplayAutotrackerRules,
    TogglDisplayProjectColors,
    TogglDisplayCountries
> foo;
    */


namespace test {

std::deque<std::pair<std::function<void(std::vector<std::any>)>, std::vector<std::any>> > Dispatcher::tasks;
std::mutex Dispatcher::tasks_lock;
App *Dispatcher::app = nullptr;

void Dispatcher::Main::on_app(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_pomodoro(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_pomodoro_break(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_sync_state(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_update(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_unsynced_items(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_error(const std::vector<std::any> &args) {
    WHEREAMI;
    std::cerr << "ERROR: " << std::any_cast<std::string>(args.front()) << std::endl;
}

void Dispatcher::Main::on_online_state(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_url(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_login(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_reminder(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_help_articles(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_time_entry_list(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_time_entry_autocomplete(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_mini_timer_autocomplete(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_project_autocomplete(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_client_select(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_workspace_select(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_tags(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_time_entry_editor(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_display_settings(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_project_colors(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_obm_experiment(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_display_timer_state(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_display_idle_notification(const std::vector<std::any> &args) {
    WHEREAMI;
}

void Dispatcher::Main::on_countries(const std::vector<std::any> &args) {
    WHEREAMI;
    app->on_countries(std::any_cast<std::list<Country>>(args[0]));
}

void Dispatcher::Worker::on_app(const bool_t open) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::make_pair( Main::on_app, std::vector<std::any>{ open } ));
}

void Dispatcher::Worker::on_pomodoro(const char_t *title, const char_t *informative_text) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_pomodoro, std::vector<std::any>{ std::string(title), std::string(informative_text) } ));
}

void Dispatcher::Worker::on_pomodoro_break(const char_t *title, const char_t *informative_text) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_pomodoro_break, std::vector<std::any>{ std::string(title), std::string(informative_text) } ));
}

void Dispatcher::Worker::on_sync_state(const int64_t sync_state) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_sync_state, std::vector<std::any>{ sync_state } ));
}

void Dispatcher::Worker::on_unsynced_items(const int64_t count) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_unsynced_items, std::vector<std::any>{ count } ));
}

void Dispatcher::Worker::on_online_state(const int64_t state) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_online_state, std::vector<std::any>{ state } ));
}

void Dispatcher::Worker::on_url(const char *url) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_url, std::vector<std::any>{ std::string(url) } ));
}

void Dispatcher::Worker::on_reminder(const char *title, const char *informative_text) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_reminder, std::vector<std::any>{ std::string(title), std::string(informative_text) } ));
}

void Dispatcher::Worker::on_help_articles(TogglHelpArticleView *first) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_help_articles, std::vector<std::any>{ listFromView<test::HelpArticle>(first) } ));
}

void Dispatcher::Worker::on_time_entry_autocomplete(TogglAutocompleteView *first) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_time_entry_autocomplete, std::vector<std::any>{ listFromView<test::Autocomplete>(first) } ));
}

void Dispatcher::Worker::on_mini_timer_autocomplete(TogglAutocompleteView *first) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_mini_timer_autocomplete, std::vector<std::any>{ listFromView<test::Autocomplete>(first) } ));
}

void Dispatcher::Worker::on_project_autocomplete(TogglAutocompleteView *first) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_project_autocomplete, std::vector<std::any>{ listFromView<test::Autocomplete>(first) } ));
}

void Dispatcher::Worker::on_client_select(TogglGenericView *first) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_client_select, std::vector<std::any>{ listFromView<test::Client>(first) } ));
}

void Dispatcher::Worker::on_workspace_select(TogglGenericView *first) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_workspace_select, std::vector<std::any>{ listFromView<test::Workspace>(first) } ));
}

void Dispatcher::Worker::on_tags(TogglGenericView *first) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_tags, std::vector<std::any>{ listFromView<test::Tag>(first) } ));
}

void Dispatcher::Worker::on_project_colors(string_list_t color_list, const uint64_t color_count) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_project_colors, std::vector<std::any>{ listFromView<std::string, char**>(color_list), color_count } ));
}

void Dispatcher::Worker::on_display_timer_state(TogglTimeEntryView *te) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_display_timer_state, std::vector<std::any>{ oneFromView<test::TimeEntry>(te) } ));
}

void Dispatcher::Worker::on_display_idle_notification(const char *guid, const char *since, const char *duration, const uint64_t started, const char *description) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_display_idle_notification, std::vector<std::any>{ std::string(guid), std::string(since), std::string(duration), started, std::string(description) } ));
}

void Dispatcher::Worker::on_countries(TogglCountryView *first) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_countries, std::vector<std::any>{ listFromView<test::Country>(first) } ));
}

void Dispatcher::Worker::on_obm_experiment(const uint64_t nr, const bool_t included, const bool_t seen) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_obm_experiment, std::vector<std::any>{ nr, included, seen } ));
}

void Dispatcher::Worker::on_display_settings(const bool_t open, TogglSettingsView *settings) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_display_settings, std::vector<std::any>{ open, oneFromView<test::Settings>(settings) } ));
}

void Dispatcher::Worker::on_time_entry_editor(const bool_t open, TogglTimeEntryView *te, const char *focused_field_name) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_time_entry_editor, std::vector<std::any>{ open, oneFromView<test::TimeEntry>(te), std::string(focused_field_name) } ));
}

void Dispatcher::Worker::on_time_entry_list(const bool_t open, TogglTimeEntryView *first, const bool_t show_load_more) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_time_entry_list, std::vector<std::any>{ open, listFromView<test::TimeEntry>(first), show_load_more } ));
}

void Dispatcher::Worker::on_login(const bool_t open, const uint64_t user_id) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_login, std::vector<std::any>{ open, user_id } ));
}

void Dispatcher::Worker::on_error(const char *errmsg, const bool_t user_error) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_error, std::vector<std::any>{ std::string(errmsg), user_error } ));
}

void Dispatcher::Worker::on_update(const char_t *url) {

    std::scoped_lock l(tasks_lock);
    tasks.emplace_back(std::pair( Main::on_update, std::vector<std::any>{ std::string(url) } ));
}

void Dispatcher::dispatch() {
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

void Dispatcher::wireUp(void *context, App *app) {
    Dispatcher::app = app;

    toggl_on_error(context, Worker::on_error);
    toggl_on_show_app(context, Worker::on_app);
    toggl_on_sync_state(context, Worker::on_sync_state);
    toggl_on_unsynced_items(context, Worker::on_unsynced_items);
    toggl_on_update(context, Worker::on_update);
    toggl_on_online_state(context, Worker::on_online_state);
    toggl_on_login(context, Worker::on_login);
    toggl_on_url(context, Worker::on_url);
    toggl_on_reminder(context, Worker::on_reminder);
    toggl_on_time_entry_list(context, Worker::on_time_entry_list);
    toggl_on_time_entry_autocomplete(context, Worker::on_time_entry_autocomplete);
    toggl_on_mini_timer_autocomplete(context, Worker::on_mini_timer_autocomplete);
    toggl_on_project_autocomplete(context, Worker::on_project_autocomplete);
    toggl_on_workspace_select(context, Worker::on_workspace_select);
    toggl_on_client_select(context, Worker::on_client_select);
    toggl_on_tags(context, Worker::on_tags);
    toggl_on_time_entry_editor(context, Worker::on_time_entry_editor);
    toggl_on_settings(context, Worker::on_display_settings);
    toggl_on_timer_state(context, Worker::on_display_timer_state);
    toggl_on_idle_notification(context, Worker::on_display_idle_notification);
    toggl_on_project_colors(context, Worker::on_project_colors);
    toggl_on_help_articles(context, Worker::on_help_articles);
    toggl_on_obm_experiment(context, Worker::on_obm_experiment);
    toggl_on_pomodoro(context, Worker::on_pomodoro);
    toggl_on_pomodoro_break(context, Worker::on_pomodoro_break);
    toggl_on_countries(context, Worker::on_countries);
}

} // namespace test
