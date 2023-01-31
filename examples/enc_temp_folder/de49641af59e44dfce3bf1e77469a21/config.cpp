#include "config.h"
#ifdef _WIN32
#define IM_NEWLINE  "\r\n"
#else
#define IM_NEWLINE  "\n"
#endif



void Config::ColorGui(ImGuiStyle* ref) {
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


    // Save/Revert button
    if (ImGui::Button("Save Ref"))
        *ref = ref_saved_style = style;
    ImGui::SameLine();
    if (ImGui::Button("Revert Ref"))
        style = *ref;
    ImGui::SameLine();

    ImGui::Separator();
    static ImGuiTextFilter filter;
    filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

    ImGui::BeginChild("##colors", ImVec2(0, 150), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
    ImGui::PushItemWidth(-160);
    int selected_fish = 0;
    const char* name = ImGui::GetStyleColorName(0);
    if (ImGui::Button(name, ImVec2(100, 30))) {
        ImGui::OpenPopup("My Popup");
    }
    filter.Draw("Filter colors", ImGui::GetFontSize() * 16);

    if (ImGui::BeginPopup("My Popup")) {
        static ImGuiTextFilter filter;
        ImGui::Separator();
        for (int i = 0; i < ImGuiCol_COUNT; i++) {
            if (ImGui::Selectable(ImGui::GetStyleColorName(i))) {
                selected_fish = i;
            }
        }
        ImGui::EndPopup();
    }

    for (int i = 0; i < ImGuiCol_COUNT; i++)
    {



        if (!filter.PassFilter(name))
            continue;
        ImGui::PushID(i);
        ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar);
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


    ImGui::PopItemWidth();
}

int selected_fish = 0;
ImVec4 shade_color = ImVec4(0, 0, 0, 1);
const char* list_color_option[] = { "Text"
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
void Config::controller(ImGuiStyle& style) {
    static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

    static bool saved_palette_init = true;
    static ImVec4 saved_palette[32] = {};
    if (saved_palette_init)
    {
        for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
        {
            ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
                saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
            saved_palette[n].w = 1.0f; // Alpha
        }
        saved_palette_init = false;
    }
    static ImVec4 backup_color;
    bool open_popup = ImGui::ColorButton("MyColor##3b", color);
    ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
    open_popup |= ImGui::Button("Palette");
    if (open_popup)
    {
        ImGui::OpenPopup("mypicker");
        backup_color = color;
    }
    if (ImGui::BeginPopup("mypicker"))
    {
        ImGui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
        ImGui::Separator();
        ImGui::ColorPicker4("##picker", (float*)&color,  ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
        ImGui::SameLine();

        ImGui::BeginGroup(); // Lock X position
        ImGui::Text("Current");
        ImGui::ColorButton("##current", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
        ImGui::Text("Previous");
        if (ImGui::ColorButton("##previous", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
            color = backup_color;
        ImGui::Separator();
        ImGui::Text("Palette");
        for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
        {
            ImGui::PushID(n);
            if ((n % 8) != 0)
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

            ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
            if (ImGui::ColorButton("##palette", saved_palette[n], palette_button_flags, ImVec2(20, 20)))
                color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!

            // Allow user to drop colors into each palette entry. Note that ColorButton() is already a
            // drag source by default, unless specifying the ImGuiColorEditFlags_NoDragDrop flag.
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
                    memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
                    memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
                ImGui::EndDragDropTarget();
            }

            ImGui::PopID();
        }
        style.Colors[ImGuiCol_Text] = color;
        ImGui::EndGroup();
        ImGui::EndPopup();
    }
}
ImVec4 getColor() {
    return shade_color;
}

void Config::ColorGui()
{
    ColorGui(nullptr);
}


