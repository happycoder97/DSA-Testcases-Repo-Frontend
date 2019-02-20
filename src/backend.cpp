#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "backend.hpp"

#define URL_LOGIN_CHECK "/login_check"
#define URL_SUBJECT "/subject"
#define URL_TESTCASE "/testcase"
#define URL_SUBMISSION "/submission"

std::optional<Backend> BackendLoggedOut::login(char *server_url, char *username,
                                               char *password) {
    auto r = cpr::Get(cpr::Url{std::string(server_url) + URL_LOGIN_CHECK},
                      cpr::Authentication(username, password));
    try {
        auto j = json::parse(r.text);
        if (j["success"]) {
            Backend backend;
            backend.server_url = server_url;
            backend.username = username;
            backend.password = password;

            if (j["is_admin"]) {
                backend.is_admin = true;
            }

            this->last_result = BackendResult::Success;
            return backend;
        } else {
            this->last_result = BackendResult::Forbidden;
            return {};
        }
    } catch (json::parse_error e) {
        this->last_result = BackendResult::NetworkError;
        return {};
    }
}

BackendResult BackendLoggedOut::get_last_result() {
    return this->last_result;
}

BackendResult Backend::get_last_result() {
    return this->last_result;
}

bool Backend::get_is_admin() { return this->is_admin; }


/******************************************************************************
 *
 * Subjects
 *
 ******************************************************************************/

std::vector<Subject> Backend::get_subjects() {

    auto r = cpr::Get(cpr::Url{std::string(server_url) + URL_SUBJECT},
                      cpr::Authentication(this->username, this->password));
    try {
        std::vector<Subject> retval;
        auto j = json::parse(r.text);
        auto arr = j[0];
        for (auto elem : arr) {
            retval.push_back(Subject{elem["id"], elem["title"]});
        }
        this->last_result = BackendResult::Success;
        return retval;
    } catch (json::parse_error e) {
        this->last_result = BackendResult::NetworkError;
        return {};
    }
}

void Backend::post_subject(const Subject &subject) {
    if (!this->get_is_admin()) {
        this->last_result = BackendResult::Forbidden;
        return;
    }

    json j;
    j["title"] = subject.title;
    auto r = cpr::Post(cpr::Url{std::string(server_url) + URL_SUBJECT},
                       cpr::Authentication{this->username, this->password},
                       cpr::Body{j.dump()});

    if (r.status_code == 200) {
        this->last_result = BackendResult::Success;
    } else if (r.status_code == 403) {
        this->last_result = BackendResult::Forbidden;
    } else {
        this->last_result = BackendResult::NetworkError;
    }
}

void Backend::put_subject(const Subject &subject) {
    if (!this->get_is_admin()) {
        this->last_result = BackendResult::Forbidden;
        return;
    }

    json j;
    j["title"] = subject.title;
    j["id"] = subject.id;
    auto r = cpr::Post(cpr::Url{std::string(server_url) + URL_SUBJECT},
                       cpr::Authentication{this->username, this->password},
                       cpr::Body{j.dump()});

    if (r.status_code == 200) {
        this->last_result = BackendResult::Success;
    } else if (r.status_code == 403) {
        this->last_result = BackendResult::Forbidden;
    } else {
        this->last_result = BackendResult::NetworkError;
    }
}

void Backend::delete_subject(int id) {
    if (!this->get_is_admin()) {
        this->last_result = BackendResult::Forbidden;
        return;
    }

    json j;
    j["id"] = id;
    auto r = cpr::Post(cpr::Url{std::string(server_url) + URL_SUBJECT},
                       cpr::Authentication{this->username, this->password},
                       cpr::Body{j.dump()});

    if (r.status_code == 200) {
        this->last_result = BackendResult::Success;
    } else if (r.status_code == 403) {
        this->last_result = BackendResult::Forbidden;
    } else {
        this->last_result = BackendResult::NetworkError;
    }
}


/******************************************************************************
 *
 * Testcases
 *
 ******************************************************************************/

std::vector<Testcase> Backend::get_testcases(int subject_id,
                                             std::string assignment_char,
                                             int question_num) {
    auto r = cpr::Get(cpr::Url{std::string(server_url) + URL_TESTCASE},
                      cpr::Parameters{{"subject_id", std::to_string(subject_id)},
                                      {"assignment_char", assignment_char},
                                      {"question_num", std::to_string(question_num)}});

    if (r.status_code == 200) {
        this->last_result = BackendResult::Success;

        try {
            std::vector<Testcase> retval;
            auto j = json::parse(r.text);
            auto arr = j[0];
            for (auto elem : arr) {
                retval.push_back(Testcase{j["id"], j["subject_id"],
                                          j["assignment_char"],
                                          j["question_num"], j["content"]

                });
            }
            return retval;
        } catch (json::parse_error e) {
            this->last_result = BackendResult::NetworkError;
        }
    } else if (r.status_code == 403) {
        this->last_result = BackendResult::Forbidden;
    } else {
        this->last_result = BackendResult::NetworkError;
    }
    return {};
}

void Backend::post_testcase(const Testcase &testcase) {
    json j;
    j["subject_id"] = testcase.subject_id;
    j["assignment_char"] = testcase.assignment_char;
    j["question_num"] = testcase.question_num;
    j["content"] = testcase.content;


    auto r = cpr::Post(cpr::Url{std::string(server_url) + URL_TESTCASE},
                       cpr::Authentication{this->username, this->password},
                       cpr::Body{j.dump()});

    if (r.status_code == 200) {
        this->last_result = BackendResult::Success;
    } else if (r.status_code == 403) {
        this->last_result = BackendResult::Forbidden;
    } else {
        this->last_result = BackendResult::NetworkError;
    }
}

void Backend::put_testcase(const Testcase &testcase) {
    json j;
    j["subject_id"] = testcase.subject_id;
    j["assignment_char"] = testcase.assignment_char;
    j["question_num"] = testcase.question_num;
    j["content"] = testcase.content;


    auto r =
        cpr::Put(cpr::Url{std::string(server_url) + URL_TESTCASE},
                 cpr::Authentication{this->username, this->password},
                 cpr::Parameters{{"id", std::to_string(testcase.id)}}, cpr::Body{j.dump()});

    if (r.status_code == 200) {
        this->last_result = BackendResult::Success;
    } else if (r.status_code == 403) {
        this->last_result = BackendResult::Forbidden;
    } else {
        this->last_result = BackendResult::NetworkError;
    }
}

std::vector<Testcase> Backend::get_my_testcases() {
    auto r = cpr::Get(cpr::Url{std::string(server_url) + URL_TESTCASE},
                      cpr::Parameters{{"username", this->username}});

    if (r.status_code == 200) {
        this->last_result = BackendResult::Success;

        try {
            std::vector<Testcase> retval;
            auto j = json::parse(r.text);
            auto arr = j[0];
            for (auto elem : arr) {
                retval.push_back(Testcase{j["id"], j["subject_id"],
                                          j["assignment_char"],
                                          j["question_num"], j["content"]

                });
            }
            return retval;
        } catch (json::parse_error e) {
            this->last_result = BackendResult::NetworkError;
        }
    } else if (r.status_code == 403) {
        this->last_result = BackendResult::Forbidden;
    } else {
        this->last_result = BackendResult::NetworkError;
    }
    return {};
}

void Backend::delete_my_testcase(int testcase_id) {
    auto r = cpr::Put(cpr::Url{std::string(server_url) + URL_TESTCASE},
                      cpr::Authentication{this->username, this->password},
                      cpr::Parameters{{"id", std::to_string(testcase_id)}});

    if (r.status_code == 200) {
        this->last_result = BackendResult::Success;
    } else if (r.status_code == 403) {
        this->last_result = BackendResult::Forbidden;
    } else {
        this->last_result = BackendResult::NetworkError;
    }
}

/******************************************************************************
 *
 * Submissions
 *
 ******************************************************************************/

std::vector<SubmissionAggregate> Backend::get_submissions(int testcase_id) {
    auto r = cpr::Get(cpr::Url{std::string(server_url) + URL_SUBMISSION},
                      cpr::Parameters{{"testcase_id", std::to_string(testcase_id)}});

    if (r.status_code == 200) {
        this->last_result = BackendResult::Success;

        try {
            std::vector<SubmissionAggregate> retval;
            auto j = json::parse(r.text);
            auto arr = j[0];
            for (auto elem : arr) {
                retval.push_back(
                    SubmissionAggregate{j["content"], j["hash1"], j["count"]});
            }
            return retval;
        } catch (json::parse_error e) {
            this->last_result = BackendResult::NetworkError;
        }
    } else if (r.status_code == 403) {
        this->last_result = BackendResult::Forbidden;
    } else {
        this->last_result = BackendResult::NetworkError;
    }
    return {};
}

void Backend::post_submission(const NewSubmission &submission) {
    json j;
    j["testcase_id"] = submission.testcase_id;
    j["content"] = submission.content;

    auto r = cpr::Put(cpr::Url{std::string(server_url) + URL_SUBMISSION},
                      cpr::Authentication{this->username, this->password});

    if (r.status_code == 200) {
        this->last_result = BackendResult::Success;
    } else if (r.status_code == 403) {
        this->last_result = BackendResult::Forbidden;
    } else {
        this->last_result = BackendResult::NetworkError;
    }
}