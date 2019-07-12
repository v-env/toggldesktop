#include "online_test_model.h"

namespace test {

test::Country *test::Country::fromView(TogglCountryView *view) {
    if (!view || !view->Name)
        return nullptr;
    return new test::Country(view->Name, view->ID);
}

test::Country::Country(const TogglCountryView *view)
    : Model(view ? view->Name : "INVALID", view ? view->ID : 0)
{

}

test::Country::Country(const std::string &name, uint64_t id)
    : Model(name, id)
{

}

test::Settings::Settings(const TogglSettingsView *view)
    : Model("")
{

}

test::HelpArticle::HelpArticle(const TogglHelpArticleView *view)
    : Model(view ? view->Name : "")
{

}

test::Autocomplete::Autocomplete(const TogglAutocompleteView *view)
    : Model(view ? view->Text : "")
{

}

test::Tag::Tag(const TogglGenericView *view)
    : Model(view ? view->Name : "", view ? view->ID : 0)
{

}

test::Client::Client(const TogglGenericView *view)
    : Model(view ? view->Name : "", view ? view->ID : 0)
{

}

test::Client *test::Client::create(void *context, const std::string &name, test::Workspace *workspace) {
    if (!context || !workspace)
        return nullptr;
    auto c = new test::Client(name);
    c->guid_ = toggl_create_client(context, workspace->id_, name.c_str());
    return c;
}

test::TimeEntry::TimeEntry(const TogglTimeEntryView *view)
    : Model(view ? view->Description : "", view ? view->ID : 0)
{
    if (view) {
        guid_ = view->GUID;
    }
}

test::Workspace::Workspace(const TogglGenericView *view)
    : Model(view ? view->Name : "", view ? view->ID : 0)
{

}

test::Workspace *test::Workspace::fromView(TogglGenericView *view) {
    if (!view || !view->WorkspaceName)
        return nullptr;
    return new test::Workspace(view->WorkspaceName, view->WID);
}


} // namespace test
