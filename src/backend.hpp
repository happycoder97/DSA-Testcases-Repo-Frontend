#ifndef __BACKEND_HPP__
#define __BACKEND_HPP__

#include <optional>
#include <string>
#include <vector>

enum class BackendResult { Success, Forbidden, NetworkError };

class Backend;

class BackendLoggedOut {
  public:
    std::optional<Backend> login(char *server_url, char *username,
                                 char *password);
    BackendResult get_last_result();

  private:
    BackendResult last_result;
};

struct Subject {
    int id;
    std::string title;
};

struct Testcase {
    int id;
    int subject_id;
    std::string assignment_char;
    int question_num;
    std::string content;
};

struct NewSubmission {
    int testcase_id;
    std::string content;
};

struct SubmissionAggregate {
    std::string content;
    std::string hash1;
    int count;
};

class Backend {
  public:
    bool get_is_admin();

    std::vector<Subject> get_subjects();

    // admin only
    void post_subject(const Subject &subject);
    void put_subject(const Subject &subject);
    // admin only
    void delete_subject(int id);

    std::vector<Testcase> get_testcases(int subject_id,
                                        std::string assignment_char,
                                        int question_num);
    void post_testcase(const Testcase &testcaPostse);
    void put_testcase(const Testcase &testcaPostse);
    std::vector<Testcase> get_my_testcases();
    void delete_my_testcase(int testcase_id);

    std::vector<SubmissionAggregate> get_submissions(int testcase_id);
    void post_submission(const NewSubmission &submission);

    BackendResult get_last_result();

  private:
    BackendResult last_result;

    std::string server_url;
    std::string username;
    std::string password;
    bool is_admin;

    friend class BackendLoggedOut;
};

#endif // __BACKEND_HPP__