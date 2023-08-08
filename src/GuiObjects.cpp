#pragma once

void windowInfo() {
    static bool on = true;
    ImGui::Begin("Window Info", &on);

    std::string cl = "ClockRate: " + std::to_string(physics::deltaTime);
    std::string wh = "Window Height:" + std::to_string(window::height);
    std::string ww = "Window Width: " + std::to_string(window::width);
    std::string cp = "Camera position: (" + toString(render::activeCamera->pos.x, 2) + ", " + toString(render::activeCamera->pos.y, 2) + ")";

    ImGui::Text(cl.c_str());
    ImGui::Text(ww.c_str());
    ImGui::Text(wh.c_str());
    ImGui::Text(cp.c_str());

    ImGui::SliderFloat("Scale", &render::activeCamera->scale, 0.1, 10000);

    ImGui::End();
}
