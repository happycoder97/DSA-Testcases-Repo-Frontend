#include "imgui.h"

void gui_setup(int argc, char** argv) {

}

void gui_loop() {
    ImGui::Begin("Test");
    ImGui::Text("Hello World!");
    ImGui::End();
}