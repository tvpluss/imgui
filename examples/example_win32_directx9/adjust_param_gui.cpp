#include "adjust_param_gui.h"
#ifdef _WIN32
#define IM_NEWLINE  "\r\n"
#else
#define IM_NEWLINE  "\n"
#endif

namespace ParamAdjust {

    void ColorGui(ImGuiStyle* ref) {
        // You can pass in a reference ImGuiStyle structure to compare to, revert to and save to
        // (without a reference style pointer, we will use one compared locally as a reference)
        static ImGuiStyle ref_saved_style;
        static ImGuiStyle& style = ImGui::GetStyle();
        // Default to using internal storage as reference
        static bool init = true;
        if (init && ref == NULL)
            ref_saved_style = style;
        init = false;
        if (ref == NULL)
            ref = &ref_saved_style;
        const ImGuiViewport* vp = ImGui::GetMainViewport();

        ImGui::PushItemWidth(-1);

        if (ImGui::ShowStyleSelector("Colors##Selector"))
            ref_saved_style = style;
        ImGui::ShowFontSelector("Fonts##Selector");

        // Simplified Settings (expose floating-pointer border sizes as boolean representing 0.0f or 1.0f)
        if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
            style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
        { bool border = (style.WindowBorderSize > 0.0f); if (ImGui::Checkbox("WindowBorder", &border)) { style.WindowBorderSize = border ? 1.0f : 0.0f; } }
        ImGui::SameLine();
        { bool border = (style.FrameBorderSize > 0.0f);  if (ImGui::Checkbox("FrameBorder", &border)) { style.FrameBorderSize = border ? 2.0f : 0.0f; } }
        ImGui::SameLine();
        { bool border = (style.PopupBorderSize > 0.0f);  if (ImGui::Checkbox("PopupBorder", &border)) { style.PopupBorderSize = border ? 1.0f : 0.0f; } }

        // Save/Revert button
        if (ImGui::Button("Save Ref"))
            *ref = ref_saved_style = style;
        ImGui::SameLine();
        if (ImGui::Button("Revert Ref"))
            style = *ref;
        ImGui::SameLine();
  
        ImGui::Separator();

        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
            

            if (ImGui::BeginTabItem("Colors"))
            {
                static int output_dest = 0;
                static bool output_only_modified = true;
                if (ImGui::Button("Export"))
                {
                    if (output_dest == 0)
                        ImGui::LogToClipboard();
                    else
                        ImGui::LogToTTY();
                    for (int i = 0; i < ImGuiCol_COUNT; i++)
                    {
                        const ImVec4& col = style.Colors[i];
                        const char* name = ImGui::GetStyleColorName(i);
                        if (!output_only_modified || memcmp(&col, &ref->Colors[i], sizeof(ImVec4)) != 0)
                            ImGui::LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);" IM_NEWLINE,
                                name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
                    }
                    ImGui::LogFinish();
                }
                ImGui::SameLine(); ImGui::SetNextItemWidth(120); ImGui::Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0");
                ImGui::SameLine(); ImGui::Checkbox("Only Modified Colors", &output_only_modified);

                static ImGuiTextFilter filter;
                filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

                static ImGuiColorEditFlags alpha_flags = 0;
                if (ImGui::RadioButton("Opaque", alpha_flags == ImGuiColorEditFlags_None)) { alpha_flags = ImGuiColorEditFlags_None; } ImGui::SameLine();
                if (ImGui::RadioButton("Alpha", alpha_flags == ImGuiColorEditFlags_AlphaPreview)) { alpha_flags = ImGuiColorEditFlags_AlphaPreview; } ImGui::SameLine();
                if (ImGui::RadioButton("Both", alpha_flags == ImGuiColorEditFlags_AlphaPreviewHalf)) { alpha_flags = ImGuiColorEditFlags_AlphaPreviewHalf; } 
                
                ImGui::BeginChild("##colors", ImVec2(0, 150), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
                ImGui::PushItemWidth(-160);
                for (int i = 0; i < ImGuiCol_COUNT; i++)
                {
                    const char* name = ImGui::GetStyleColorName(i);
                    if (!filter.PassFilter(name))
                        continue;
                    ImGui::PushID(i);
                    ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
                    if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0)
                    {
                        // Tips: in a real user application, you may want to merge and use an icon font into the main font,
                        // so instead of "Save"/"Revert" you'd use icons!
                        // Read the FAQ and docs/FONTS.md about using icon fonts. It's really easy and super convenient!
                        ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Save")) { ref->Colors[i] = style.Colors[i]; }
                        ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Revert")) { style.Colors[i] = ref->Colors[i]; }
                    }
                    ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
                    ImGui::TextUnformatted(name);
                    ImGui::PopID();
                }
                ImGui::PopItemWidth();
                ImGui::EndChild();

                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::PopItemWidth();
    }

    static int selected_fish = 0;
    ImVec4 shade_color = ImVec4(0, 0, 0, 1);
    char* list_color_option[] = { "Text"
                                ,"Text Disabled"
                                ,"WindowBg"
                                ,"ChillBg"
                                ,"PopUpBG"
                                ,"Border"
                                ,"Border Shadow"
                                ,"Frame Bg"
                                ,"Frame Bg Hovered"
                                ,"Frame Bg Active"
                                ,"Ti"
                                ,"Border Shadow"
                                ,"Border Shadow"
                                ,"Border Shadow"
                                };
    //OIL IN PLACE
    void controller() {
        if (ImGui::Button(list_color_option[selected_fish], ImVec2(100, 30))) {
            ImGui::OpenPopup("My Popup");
        }
        if (ImGui::BeginPopup("My Popup")) {
            ImGui::Text("Pick your color option");
            ImGui::Separator();
            for (int i = 0; i < IM_ARRAYSIZE(list_color_option); i++) {
                if (ImGui::Selectable(list_color_option[i])) {
                    selected_fish = i;
                }
            }
            ImGui::EndPopup();
        }
        ImGui::ColorEdit3("Color Edit", (float*)&shade_color);
    }
    ImVec4 getColor() {
        return shade_color;
    }

    void ColorGui()
    {
        ColorGui(nullptr);
    }

}
