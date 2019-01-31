#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "cpr/cpr.h"
#include "imgui.h"
#include "imgui_stdlib.h"

using namespace std;

void signin();
void assignments();
void questions();
void testcases();


void gui_setup(int argc, char** argv) {}


void test();
void gui_loop() {
    // ImGui::Begin("Test");
    // ImGui::Text("Hello World!");
    // ImGui::End();
    //signin();
    //assignments();
    //questions();
    test();
}


//========== Sign In Window============//
char signin_userid[1000];
char signin_password[1000];
void signin() {
    
    ImGui::Begin("Sign_in");
    ImGui::Text("User ID :");
    ImGui::PushItemWidth(-1);
    ImGui::InputText("##userid",signin_userid,1000);
    
    //ImGui::Begin("password");
    ImGui::Text("Password :");
    ImGui::InputText("##password",signin_password,1000,ImGuiInputTextFlags_Password,NULL,NULL);

    ImGui::Button("Sign In");
    ImGui::End();
}


//=========assignments------------//
vector<string> assignments_list;
int assignments_i = -1;
void assignments() {
    ImGui::Begin("Assignments");
    ImGui::PushItemWidth(-1);
    bool result = ImGui::Button("+");
    if(result) {
        assignments_list.push_back(std::string("Assignment ") + std::to_string(assignments_list.size()+1));
    }
    int i=0;
    for(string s: assignments_list) {
        
        if(ImGui::Selectable(s.c_str(),assignments_i==i)) {
            assignments_i = i;
        }
        i++;

    }
    ImGui::End();

}
//=================================//


//========questions list===========//
vector<string> questions_list;
int questions_i = -1;
void questions() {
    ImGui::Begin("Questions");
    bool result = ImGui::Button("+");
    if(result) {
        questions_list.push_back(std::string("Questions ") + std::to_string(questions_list.size()+1));
    }
    int i=0;
    for(string s: questions_list) {
        if(ImGui::Selectable(s.c_str(),questions_i == i)) {
            questions_i = i;
        }
        i++;
    }
    ImGui::End();
}
//==================================//


string test_url;
cpr::Response test_resp;
bool test_has_resp = false;
bool test_resp_pending = false;

void test_get() {
    test_resp_pending = true;
    test_resp = cpr::Get(cpr::Url{test_url});
    test_resp_pending = false;
    test_has_resp = true;
}
void test() {
    ImGui::Begin("CPR Test");
    ImGui::Text("Test CPR workings");
    ImGui::InputText("URL", &test_url);
    if(ImGui::Button("Submit")) {
        if(!test_resp_pending) {
            std::thread(test_get).detach();
        }
    }
    if(test_resp_pending) {
        ImGui::Text("Response pending..");
    } else if(!test_has_resp) {
        ImGui::Text("No requests made");
    } else {
        ImGui::Text("URL:");
        ImGui::Text(test_resp.url.c_str());
        ImGui::Text("Response:");
        ImGui::Text(test_resp.text.c_str());
    }
    ImGui::End();
}
