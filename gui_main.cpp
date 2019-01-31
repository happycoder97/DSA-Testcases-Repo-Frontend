#include "imgui.h"

void gui_setup(int argc, char** argv) {

}


void signin();
void assignments();
void questions();
void testcases();


void gui_loop() {
    ImGui::Begin("Test");
    ImGui::Text("Hello World!");
    ImGui::End();
}