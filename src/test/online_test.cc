// Copyright 2019 Toggl Desktop developers.

#include "online_test.h"

#include <iostream>
#include <map>

#include "Poco/DateTime.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Path.h"
#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/Event.h"

#define WHEREAMI std::cerr << __FILE__ << ":" << __LINE__ << "\t- " << __FUNCTION__ << std::endl; std::cerr.flush();

std::string random_string( size_t length ) {
    auto randchar = []() -> char {
        const char charset[] = "0123456789"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

static Poco::Event *semaphore = new Poco::Event();
static void *ctx_;
static std::map<uint64_t, std::string> countries;
static uint64_t workspace = 0;

void on_app(const bool_t open) {
    WHEREAMI
    //testresult::on_app_open = open;
}

void on_pomodoro(const char_t *title, const char_t *informative_text) {
    WHEREAMI
}

void on_pomodoro_break(const char_t *title, const char_t *informative_text) {
    WHEREAMI
}

void on_sync_state(const int64_t sync_state) {
    WHEREAMI
    //testresult::sync_state = sync_state;
}

void on_update(const char_t *url) {
    WHEREAMI
    //testresult::update_url = std::string(url);
}

void on_unsynced_items(const int64_t count) {
    WHEREAMI
    //testresult::unsynced_item_count = count;
}

void on_error(const char *errmsg, const bool_t user_error) {
    WHEREAMI
    std::cerr << errmsg << std::endl;
    /*
    if (errmsg) {
        testresult::error = std::string(errmsg);
        return;
    }
    testresult::error = std::string("");
    */
}

void on_online_state(const int64_t state) {
    WHEREAMI
    //testresult::online_state = state;
}

void on_url(const char *url) {
    WHEREAMI
    //testresult::url = std::string(url);
}

void on_login(const bool_t open, const uint64_t user_id) {
    WHEREAMI
    std::cerr << "Logged in as " << toggl_get_user_fullname(ctx_) << " with email " << toggl_get_user_email(ctx_) << " (user ID " << user_id << ")" << std::endl;
    //testresult::user_id = user_id;
}

void on_reminder(const char *title, const char *informative_text) {
    WHEREAMI
    /*
    testresult::reminder_title = std::string(title);
    testresult::reminder_informative_text = std::string(informative_text);
    */
}

void on_help_articles(TogglHelpArticleView *first) {
    WHEREAMI
    /*
    testing::testresult::help_article_names.clear();
    TogglHelpArticleView *it = first;
    while (it) {
        std::string name(it->Name);
        testing::testresult::help_article_names.push_back(name);
        it = reinterpret_cast<TogglHelpArticleView *>(it->Next);
    }
    */
}

void on_time_entry_list(const bool_t open, TogglTimeEntryView *first, const bool_t show_load_more) {
    WHEREAMI
    /*
    testing::testresult::time_entries.clear();
    TogglTimeEntryView *it = first;
    while (it) {
        TimeEntry te;
        te.SetGUID(it->GUID);
        te.SetID(it->ID);
        te.SetDurationInSeconds(it->DurationInSeconds);
        te.SetDescription(it->Description);
        te.SetStart(it->Started);
        te.SetStop(it->Ended);
        testing::testresult::time_entries.push_back(te);
        it = reinterpret_cast<TogglTimeEntryView *>(it->Next);
    }
    */
}

void on_time_entry_autocomplete(TogglAutocompleteView *first) {
    WHEREAMI
}

void on_mini_timer_autocomplete(TogglAutocompleteView *first) {
    WHEREAMI
}

void on_project_autocomplete(TogglAutocompleteView *first) {
    WHEREAMI
    /*
    testing::testresult::projects.clear();
    TogglAutocompleteView *it = first;
    while (it) {
        testing::testresult::projects.push_back(
            std::string(it->ProjectLabel));
        it = reinterpret_cast<TogglAutocompleteView *>(it->Next);
    }
    */
}

void on_client_select(TogglGenericView *first) {
    WHEREAMI
    std::cerr << "Clients now are:" << std::endl;
    while (first) {
        std::cerr << "\t" << first->Name << std::endl;
        if (first && first->Name && std::string(first->Name) == "Client Eastwood") {
        }
        first = reinterpret_cast<TogglGenericView*>(first->Next);
    }
    /*
    testing::testresult::clients.clear();
    TogglGenericView *it = first;
    while (it) {
        testing::testresult::clients.push_back(std::string(it->Name));
        it = reinterpret_cast<TogglGenericView *>(it->Next);
    }
    */
}

void on_workspace_select(TogglGenericView *first) {
    WHEREAMI
    workspace = first->WID;
}

void on_tags(TogglGenericView *first) {
    WHEREAMI
}

void on_time_entry_editor(const bool_t open, TogglTimeEntryView *te, const char *focused_field_name) {
    WHEREAMI
    /*
    testing::testresult::editor_state = TimeEntry();
    testing::testresult::editor_state.SetGUID(te->GUID);
    testing::testresult::editor_open = open;
    testing::testresult::editor_focused_field_name =
        std::string(focused_field_name);
        */
}

void on_display_settings(const bool_t open, TogglSettingsView *settings) {
    WHEREAMI
    /*
    testing::testresult::open_settings = open;

    testing::testresult::settings.use_idle_detection =
        settings->UseIdleDetection;
    testing::testresult::settings.menubar_project = settings->MenubarProject;
    testing::testresult::settings.autodetect_proxy = settings->AutodetectProxy;
    testing::testresult::settings.menubar_timer = settings->MenubarTimer;
    testing::testresult::settings.reminder = settings->Reminder;
    testing::testresult::settings.dock_icon = settings->DockIcon;
    testing::testresult::settings.on_top = settings->OnTop;
    testing::testresult::settings.idle_minutes = settings->IdleMinutes;
    testing::testresult::settings.reminder_minutes = settings->ReminderMinutes;
    testing::testresult::settings.focus_on_shortcut = settings->FocusOnShortcut;
    testing::testresult::settings.manual_mode = settings->ManualMode;
    testing::testresult::settings.autotrack = settings->Autotrack;

    testing::testresult::use_proxy = settings->UseProxy;

    testing::testresult::proxy.SetHost(std::string(settings->ProxyHost));
    testing::testresult::proxy.SetPort(settings->ProxyPort);
    testing::testresult::proxy.SetUsername(
        std::string(settings->ProxyUsername));
    testing::testresult::proxy.SetPassword(
        std::string(settings->ProxyPassword));

    testing::testresult::settings.remind_starts = settings->RemindStarts;
    testing::testresult::settings.remind_ends = settings->RemindEnds;
    testing::testresult::settings.remind_mon = settings->RemindMon;
    testing::testresult::settings.remind_tue = settings->RemindTue;
    testing::testresult::settings.remind_wed = settings->RemindWed;
    testing::testresult::settings.remind_thu = settings->RemindThu;
    testing::testresult::settings.remind_fri = settings->RemindFri;
    testing::testresult::settings.remind_sat = settings->RemindSat;
    testing::testresult::settings.remind_sun = settings->RemindSun;
    */
}

void on_project_colors(string_list_t color_list, const uint64_t color_count) {
    WHEREAMI
    /*
    testresult::project_colors.clear();
    for (uint64_t i = 0; i < color_count; i++) {
        testresult::project_colors.push_back(std::string(color_list[i]));
    }
    */
}

void on_obm_experiment(const uint64_t nr, const bool_t included, const bool_t seen) {
    WHEREAMI
    /*
    ObmExperiment experiment;
    experiment.SetNr(nr);
    experiment.SetIncluded(included);
    experiment.SetHasSeen(seen);
    testresult::obm_experiments.push_back(experiment);
    */
}

void on_display_timer_state(TogglTimeEntryView *te) {
    WHEREAMI
    if (te && te->Description) {
        std::cerr << te->Description << std::endl;
    }
    /*
    testing::testresult::timer_state = TimeEntry();
    if (te) {
        testing::testresult::timer_state.SetID(te->ID);
        testing::testresult::timer_state.SetStart(te->Started);
        testing::testresult::timer_state.SetGUID(te->GUID);
        testing::testresult::timer_state.SetDurationInSeconds(
            te->DurationInSeconds);
        testing::testresult::timer_state.SetDescription(te->Description);
        if (te->Tags) {
            testing::testresult::timer_state.SetTags(te->Tags);
        }
        testing::testresult::timer_state.SetBillable(te->Billable);
        testing::testresult::timer_state.SetPID(te->PID);
        testing::testresult::timer_state.SetTID(te->TID);
    }
    */
}

void on_display_idle_notification(const char *guid, const char *since, const char *duration, const uint64_t started, const char *description) {
    WHEREAMI
    /*
    testing::testresult::idle_since = std::string(since);
    testing::testresult::idle_started = started;
    testing::testresult::idle_duration = std::string(duration);
    testing::testresult::idle_guid = std::string(guid);
    testing::testresult::idle_description = std::string(description);
    */
}

void on_countries(TogglCountryView *first) {
    WHEREAMI
    while (first) {
        countries.emplace(first->ID, first->Name);
        first = reinterpret_cast<TogglCountryView*>(first->Next);
    }
}

int main(int argc, char **argv) {
    srand(time(nullptr));
    semaphore->reset();

    std::cerr << "Starting\n";
    Poco::File f(TESTDB);
    if (f.exists()) {
        f.remove(false);
    }

    toggl_set_log_path("test.log");

    ctx_ = toggl_context_init("tests", "0.1");

    poco_assert(toggl_set_db_path(ctx_, TESTDB));

    Poco::Path path("src/ssl/cacert.pem");
    toggl_set_cacert_path(ctx_, path.toString().c_str());

    toggl_on_show_app(ctx_, on_app);
    toggl_on_sync_state(ctx_, on_sync_state);
    toggl_on_unsynced_items(ctx_, on_unsynced_items);
    toggl_on_update(ctx_, on_update);
    toggl_on_error(ctx_, on_error);
    toggl_on_online_state(ctx_, on_online_state);
    toggl_on_login(ctx_, on_login);
    toggl_on_url(ctx_, on_url);
    toggl_on_reminder(ctx_, on_reminder);
    toggl_on_time_entry_list(ctx_, on_time_entry_list);
    toggl_on_time_entry_autocomplete(ctx_, on_time_entry_autocomplete);
    toggl_on_mini_timer_autocomplete(ctx_, on_mini_timer_autocomplete);
    toggl_on_project_autocomplete(ctx_, on_project_autocomplete);
    toggl_on_workspace_select(ctx_, on_workspace_select);
    toggl_on_client_select(ctx_, on_client_select);
    toggl_on_tags(ctx_, on_tags);
    toggl_on_time_entry_editor(ctx_, on_time_entry_editor);
    toggl_on_settings(ctx_, on_display_settings);
    toggl_on_timer_state(ctx_, on_display_timer_state);
    toggl_on_idle_notification(ctx_, on_display_idle_notification);
    toggl_on_project_colors(ctx_, on_project_colors);
    toggl_on_help_articles(ctx_, on_help_articles);
    toggl_on_obm_experiment(ctx_, on_obm_experiment);
    toggl_on_pomodoro(ctx_, on_pomodoro);
    toggl_on_pomodoro_break(ctx_, on_pomodoro_break);
    toggl_on_countries(ctx_, on_countries);

    toggl_get_countries(ctx_);

    toggl_ui_start(ctx_);

    std::string username = "toggldesktop-test-" + random_string(8) + "@" + random_string(16) + ".com";
    std::string password = random_string(16);
    size_t country_position = rand() % countries.size() - 1;
    uint64_t country_id = 0;
    for (auto i : countries) {
        if (country_position <= 0) {
            country_id = i.first;
            break;
        }
        country_position--;
    }

    std::cerr << "Attempting to create user \"" << username << "\" with password \"" << password << "\"" << std::endl;

    toggl_signup(ctx_, username.c_str(), password.c_str(), country_id);
    toggl_fullsync(ctx_);

    toggl_logout(ctx_);

    toggl_login(ctx_, username.c_str(), password.c_str());
    toggl_fullsync(ctx_);

    toggl_create_client(ctx_, workspace, "Client Eastwood");
    toggl_fullsync(ctx_);

    auto guid_1 = toggl_start(ctx_, "First", "", 0, 0, nullptr, nullptr, false);
    toggl_stop(ctx_, false);
    auto guid_2 = toggl_start(ctx_, "Second", "", 0, 0, nullptr, nullptr, false);
    toggl_stop(ctx_, false);
    toggl_set_time_entry_start(ctx_, guid_1, "12:00");
    toggl_set_time_entry_duration(ctx_, guid_1, "13:00");
    toggl_set_time_entry_start(ctx_, guid_2, "15:00");
    toggl_set_time_entry_duration(ctx_, guid_2, "14:00");
    auto guid_3 = toggl_start(ctx_, "Third", "", 0, 0, nullptr, nullptr, false);

    toggl_fullsync(ctx_);

    //poco_assert(!toggl_ui_start(ctx_));
    std::cerr << "Quitting\n";
    semaphore->wait();
    //toggl_context_clear(ctx_);
}

