#include "login.hpp"
#include "urls.hpp"
#include <async++.h>
#include <cpr/cpr.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>

using json = nlohmann::json;

static char username[1000];
static char password[1000];


enum class __LoginResult { AuthFailed, NetworkFailed, User, Admin };

__LoginResult login_check(const char *username, const char *password);
std::optional<async::task<__LoginResult>> task_login_check;

struct LoginViewData {
    bool auth_failed = false;
    bool network_failed = false;
};

LoginViewData login_view_data;

std::optional<User> login() {

    ImGui::Begin("Login");
    ImGui::Text("User ID :");
    ImGui::PushItemWidth(-1);
    ImGui::InputText("##username", username, 1000);

    ImGui::Text("Password :");
    ImGui::InputText("##password", password, 1000, ImGuiInputTextFlags_Password,
                     NULL, NULL);

    std::optional<User> return_value = {};

    if (task_login_check) {
        if (task_login_check->ready()) {
            auto __login_result = task_login_check->get();
            task_login_check = {};

            switch (__login_result) {
            case __LoginResult::Admin:
                return_value = User {
                    std::string(username),
                    std::string(password),
                    true
                };
                break;
            case __LoginResult::User:
                return_value = User {
                    std::string(username),
                    std::string(password),
                    true
                };
                break;

            case __LoginResult::AuthFailed:
                login_view_data.auth_failed = true;
                break;
            case __LoginResult::NetworkFailed:
                login_view_data.network_failed = true;
                break;
            }

        } else {
            ImGui::Button("Please wait..");
        }

    } else {
        if (ImGui::Button("Sign In")) {
            login_view_data.auth_failed = false;
            login_view_data.network_failed = false;

            task_login_check =
                async::spawn([] { return login_check(username, password); });
        }
    }

    if (login_view_data.auth_failed) {
        ImGui::Text("Incorrect username/password");
    }
    if (login_view_data.network_failed) {
        ImGui::Text("Error connecting to server");
    }

    ImGui::End();

    return return_value;
}

__LoginResult login_check(const char *username, const char *password) {
    auto r = cpr::Get(cpr::Url{URL_LOGIN_CHECK},
                      cpr::Authentication(username, password));
    try {
        auto j = json::parse(r.text);
        if (j["success"]) {
            if (j["is_admin"]) {
                return __LoginResult::Admin;
            } else {
                return __LoginResult::User;
            }
        } else {
            return __LoginResult::AuthFailed;
        }
    } catch (json::parse_error e) {
        std::cout << e.what() << std::endl;
        std::cout << "-------------" << std::endl;
        std::cout << r.text << std::endl;
        std::cout << "-------------" << std::endl;
        return __LoginResult::NetworkFailed;
    }
}
