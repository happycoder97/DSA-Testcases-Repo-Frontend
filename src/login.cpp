#include "login.hpp"
#include "backend.hpp"
#include "urls.hpp"
#include <async++.h>
#include <cpr/cpr.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>

using json = nlohmann::json;

static char server_url[1000];
static char username[1000];
static char password[1000];

BackendLoggedOut backend;

std::optional<async::task<std::optional<Backend>>> task_login_check;

struct LoginViewData {
    bool auth_failed = false;
    bool network_failed = false;
};

LoginViewData login_view_data;

std::optional<Backend> login() {

    ImGui::Begin("Login");
    ImGui::PushItemWidth(-1);

    ImGui::Text("User ID :");
    ImGui::InputText("##username", username, 1000);

    ImGui::Text("Password :");
    ImGui::InputText("##password", password, 1000, ImGuiInputTextFlags_Password,
                     NULL, NULL);

    ImGui::Text("Server URL:");
    ImGui::InputText("##server_url", server_url, 1000);

    std::optional<Backend> return_value = {};

    if (task_login_check) {
        if (task_login_check->ready()) {
            auto _backend = task_login_check->get();
            task_login_check = {};

            switch (backend.get_last_result()) {
            case BackendResult::Success:
                return_value = _backend;
                break;

            case BackendResult::Forbidden:
                login_view_data.auth_failed = true;
                break;
            case BackendResult::NetworkError:
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
                async::spawn([] { return backend.login(server_url, username, password); });
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