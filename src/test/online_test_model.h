#ifndef ONLINE_TEST_MODEL_H
#define ONLINE_TEST_MODEL_H

#include "../toggl_api.h"

#include <string>
#include <list>

// a helper macro that will spit out the file, line and method to stderr when reached
#define WHEREAMI // do {std::cerr << __FILE__ << ":" << __LINE__ << "\t- " << __PRETTY_FUNCTION__ << std::endl; std::cerr.flush(); } while(0)

namespace test {

// template method that will create a list of Model objects from the Views
// it relies on the existence of a Toggl<something>View* constructor in the corresponding Model class
template <typename TestType, typename LibType> inline std::list<TestType> listFromView(LibType view) {
    std::list<TestType> ret;
    while (view) {
        ret.emplace_back(TestType { view });
        view = static_cast<LibType>(view->Next);
    }
    return ret;
}
// a specialization for a stringlist (while not technically a Model class, it simplifies the code in the dispatcher)
template <> inline std::list<std::string> listFromView<std::string>(char **list) {
    std::list<std::string> ret;
    while (list && *list) {
        ret.push_back(std::string(*list));
    }
    return ret;
}
// same as above, but doesn't create a list
template <typename TestType, typename LibType>
inline TestType oneFromView(LibType view) {
    if (view) {
        return TestType { view };
    }
    return TestType {};
}

struct Model {
    Model(const std::string &name, uint64_t id = 0) : name_(name), id_(id) {}
    std::string name_;
    uint64_t id_ { 0 };
    std::string guid_ {""};
};

struct Country : public Model {
    static Country *fromView(TogglCountryView *view);
    Country(const TogglCountryView *view);
private:
    Country(const std::string &name, uint64_t id = 0);
};

struct Workspace : public Model {
    Workspace(const TogglGenericView *view);
    static Workspace *fromView(TogglGenericView *view);
private:
    Workspace(const std::string &name, uint64_t id = 0) : Model(name, id) {}
};

struct Client : public Model {
    Client(const TogglGenericView *view);
    static Client *create(void *context, const std::string &name, Workspace *workspace);
private:
    Client(const std::string &name, uint64_t id = 0) : Model(name, id) {}
};

struct Project : public Model {
private:
    Project(const std::string &name, uint64_t id = 0) : Model(name, id) {}
};

struct TimeEntry : public Model {
    TimeEntry(const TogglTimeEntryView *view = nullptr);
private:
    TimeEntry(const std::string &name, uint64_t id = 0) : Model(name, id) {}
};

struct Tag : public Model {
    Tag(const TogglGenericView *view);
private:
};

struct Autocomplete : public Model {
    Autocomplete(const TogglAutocompleteView *view);
private:
};

struct HelpArticle : public Model {
    HelpArticle(const TogglHelpArticleView *view);
private:
};

struct Settings : public Model {
    Settings(const TogglSettingsView *view = nullptr);
private:
};

inline bool operator<(const test::Country &l, const test::Country &r) {
    return l.name_ < r.name_;
}

} // namespace test

#endif // ONLINE_TEST_MODEL_H
