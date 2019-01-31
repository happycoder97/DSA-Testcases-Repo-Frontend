#include "imgui.h"
#include <vector>
#include <string>
using namespace std;
void gui_setup(int argc, char** argv) {

}


void signin();
void assignments();
void questions();
void testcases();


void gui_loop() {
    // ImGui::Begin("Test");
    // ImGui::Text("Hello World!");
    // ImGui::End();
    signin();
    assignments();
    questions();
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