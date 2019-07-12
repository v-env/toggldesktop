#include "online_test_model.h"

namespace test {

test::Country::Country(const TogglCountryView *view)
    : Model(view ? view->Name : "INVALID", view ? view->ID : 0)
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

} // namespace test
