#include "online_test_model.h"

#include <iterator>
#include <iostream>
#include <sstream>

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
        if (view->GUID)
            guid_ = view->GUID;
        started_ = view->Started;
        if (view->StartTimeString)
            startedString_ = view->StartTimeString;
        ended_ = view->Ended;
        if (view->EndTimeString)
            endedString_ = view->EndTimeString;

        if (view->Tags) {
            std::string intags { view->Tags };
            std::stringstream ss(intags);
            std::istream_iterator<std::string> begin(ss);
            std::istream_iterator<std::string> end;
            tags_ = std::list<std::string>(begin, end);
        }
    }
}

test::Workspace::Workspace(const TogglGenericView *view)
    : Model(view ? view->Name : "", view ? view->ID : 0)
{

}

} // namespace test
