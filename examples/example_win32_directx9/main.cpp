// Dear ImGui: standalone example application for DirectX 9
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include "implot.h"
#include <imgui_internal.h>
// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
float RandomRange(float min, float max) {
    float scale = rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}
void TextCentered(char* text) {
    auto windowWidth = ImGui::GetColumnWidth();
    auto textWidth = ImGui::CalcTextSize(text).x;

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (windowWidth - textWidth) * 0.5f);
    ImGui::Text(text);
}
void DrawBasicTable(char* label, char* contents[], int row, int col) {
    if (ImGui::BeginTable(label, col)) {

        for (int i = 0; i < row; i++) {
            ImGui::TableNextRow();
            for (int j = 0; j < col; j++) {
                ImGui::TableSetColumnIndex(j);
                ImGui::PushItemWidth(-FLT_MIN);
                //ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - ImGui::CalcTextSize(contents[i * col + j]).x
                //    - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x);
                //ImGui::Text(contents[i * col + j]);
                TextCentered(contents[i * col + j]);
                ImGui::PopItemWidth();
            }
        }
        ImGui::EndTable();
    }
};

void SetUpData(double * x, double* y, double y_increasement,int data_count, double x_min, double x_max) {
    
    for (int j = 0; j < data_count; j++) {
        x[j] = RandomRange(x_min, x_max);
        y[j] = j * y_increasement;
    }
}

void SetUpData(double* x, double* y, double y_increasement, double* x_min, int data_count, double x_max) {

    for (int j = 0; j < data_count; j++) {
        x[j] = RandomRange(x_min[j], x_max);
        y[j] = j * y_increasement;
    }
}
int main(int, char**)
{
    RECT desktop;
    GetWindowRect(GetDesktopWindow(), &desktop);
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_OVERLAPPEDWINDOW, desktop.right / 2, desktop.top, desktop.right / 2, desktop.bottom - 100, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
    ImFont* font_h0 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 30.0f);
    ImFont* font_h1 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 24.0f);

    ImFont* font_h2 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 20.0f);
    ImFont* font_text = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 16.0f);
    IM_ASSERT(font_h0 != NULL);
    IM_ASSERT(font_h1 != NULL);
    IM_ASSERT(font_h2 != NULL);
    IM_ASSERT(font_text != NULL);
    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    bool my_tool_active = false;
    bool show_implot_demo_window = false;
    bool my_form = false;
    bool my_table = true;
    const int rand_data_count = 200;
    ImVec4 clear_color = ImVec4(0.25f, 0.35f, 0.00f, 1.00f);

    static double x[rand_data_count];
    static double y[rand_data_count];
    for (int j = 0; j < rand_data_count; j++) {
        x[j] = RandomRange(60, 150);
        if (j > 0)
            y[j] = y[j - 1] + 1;
        else
            y[j] = 1;
    }
    static double x1[rand_data_count];
    static double y1[rand_data_count];
    for (int j = 0; j < rand_data_count; j++) {
        x1[j] = RandomRange(75, 16 + 75);
        if (j > 0)
            y1[j] = y1[j - 1] + 1;
        else
            y1[j] = 1;

    }
    static double resist_x[rand_data_count];
    static double resist_y[rand_data_count];
    for (int j = 0; j < rand_data_count; j++) {
        resist_x[j] = RandomRange(2, 2000);
        if (j > 0)
            resist_y[j] = resist_y[j - 1] + 1;
        else
            resist_y[j] = 1;

    }

    float root = 0;
    float y_increasement = 1;
    float max_y_axis = rand_data_count * y_increasement;

    static double data_col5_x[6][rand_data_count];
    static double data_col5_y[6][rand_data_count];
    for(int i = 0; i < 6; i++){
        SetUpData(data_col5_x[i], data_col5_y[i], y_increasement, rand_data_count,2400- 400*(i+1),2400- 400*i);
    }
    static double data_col6_x[3][rand_data_count];
    static double data_col6_y[3][rand_data_count];
    SetUpData(data_col6_x[0], data_col6_y[0], y_increasement, rand_data_count, 0, 1000);
    SetUpData(data_col6_x[1], data_col6_y[1], y_increasement, data_col6_x[0], rand_data_count, 2000);
    SetUpData(data_col6_x[2], data_col6_y[2], y_increasement, rand_data_count, 0, 2000);
    static double data_col7_x[2][rand_data_count];
    static double data_col7_y[2][rand_data_count];
    SetUpData(data_col7_x[0], data_col7_y[0], y_increasement, rand_data_count, 0, 2000);
    SetUpData(data_col7_x[1], data_col7_y[1], y_increasement, rand_data_count,0,2000);
    bool show1st = true;
    bool show2nd = true;
    bool show3rd = true;
    bool show4th = true;
    bool show5th = true;
    bool show6th = true;

    static double neutron_y[rand_data_count];
    static double neutron_nphi[rand_data_count];
    static double neutron_rhob[rand_data_count];
    static double neutron_pe[rand_data_count];
    for (int j = 0; j < rand_data_count; j++) {
        neutron_nphi[j] = RandomRange(0.45, -0.15);
        neutron_rhob[j] = RandomRange(1.95, 2.95);
        neutron_pe[j] = RandomRange(0, 10);
        if (j > 0)
            neutron_y[j] = neutron_y[j - 1] + 1;
        else
            neutron_y[j] = 1;

    }
    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        if (show_implot_demo_window) {
            ImPlot::ShowDemoWindow(&show_implot_demo_window);
        }
        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;
            
            ImGui::SetNextWindowPos(ImVec2(100, 100));
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Implot Demo", &show_implot_demo_window);
            ImGui::Checkbox("Another Window", &show_another_window);
            ImGui::Checkbox("My first Window", &my_tool_active);
            ImGui::Checkbox("Demo Table", &my_table);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        const ImGuiViewport* vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowSize(vp->WorkSize);
        ImGui::SetNextWindowPos(vp->WorkPos);
        if (my_table) {
            ImGui::Begin("My Table", &my_table);
            static ImGuiTableFlags flags_petropy = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY;
            static ImGuiTableColumnFlags column_flags = ImGuiTableColumnFlags_NoHeaderWidth | ImGuiTableColumnFlags_WidthFixed;
            if (ImGui::BeginTable("petropy", 8, flags_petropy)) {

                // Header Row
                //ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn("GAMMA RAY", column_flags);
                ImGui::TableSetupColumn("DEPTH", column_flags);
                ImGui::TableSetupColumn("RESISTIVITY", column_flags);
                ImGui::TableSetupColumn("NEUTRON DENSITY", column_flags);
                ImGui::TableSetupColumn("MINERALOGOY", column_flags);
                ImGui::TableSetupColumn("POROSITY SATURATION", column_flags);
                ImGui::TableSetupColumn("OIL IN PLACE ", column_flags);
                ImGui::TableSetupColumn("ELECTROFACIES", column_flags);
                ImGui::TableHeadersRow();
                // Legend Row
                ImGui::TableNextRow();

                // Sub-table GAMMA RAY
                char* contents[6] = { "0", "GR", "150", "0", "CAL", "16" };
                ImGui::TableSetColumnIndex(0);
                DrawBasicTable("gamma-ray", contents, 2, 3);

                // Sub-table Resist
                char* resist_contents[6] = { "2","RESDEEP", "2000", "2","RESMED", "2000" };
                ImGui::TableSetColumnIndex(2);
                DrawBasicTable("resist", resist_contents, 2, 3);

                // Sub-table Neutron
                char* neutron_contents[9] = { "0.45", "NPHI", "-0.15", "1.95", "RHOB", "2.95", "0", "PE", "10" };
                ImGui::TableSetColumnIndex(3);
                DrawBasicTable("neutron", neutron_contents, 3, 3);

                // Sub-table Mineral
                char* mineral_contents[9] = { "VCLAY", "VQTZ", "VDOL", "VCLC", "VPYR", "VOM", "BVH", "BVWF", "BVWI" };
                ImGui::TableSetColumnIndex(4);
                DrawBasicTable("mineral", mineral_contents, 1, 9);
                // Sub-table Porosity
                char* porosity_contents[3] = { "1", "Sw", "0" };
                ImGui::TableSetColumnIndex(5);
                DrawBasicTable("porosity", porosity_contents, 1, 3);

                char* oil_contents[6] = { "0", "OIP", "0.25", "50", "SUM OIP", "0" };
                ImGui::TableSetColumnIndex(6);
                DrawBasicTable("oil", oil_contents, 2, 1);

                char* electro_contents[2] = { "0", "1" };
                ImGui::TableSetColumnIndex(7);
                DrawBasicTable("electro", electro_contents, 1, 2);


                //Gamma Row
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImPlot::PushStyleVar(ImPlotStyleVar_PlotDefaultSize, ImVec2(300, 1000));
                ImPlot::PushStyleVar(ImPlotStyleVar_PlotMinSize, ImVec2(300, 1000));
                if (ImPlot::BeginPlot("##Gamma_Plot", ImVec2(0,0))) {
                    // Set opactity of shade to 25%
                    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                    ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoDecorations| ImPlotAxisFlags_AutoFit);
                    ImVec4 color = { 0.5f, 0.3f, 0.4f, 0.9f };
                    ImPlot::PlotShaded("##Test", x, y, rand_data_count, 0, ImPlotShadedFlags_Vertical);
                    ImPlot::SetNextLineStyle(color);
                    ImPlot::PlotLine("##Test", x, y, rand_data_count);
                    ImPlot::SetNextLineStyle(color);
                    ImPlot::PlotLine("##Test1", x1, y1, rand_data_count);
                    ImPlot::PopStyleVar();

                    ImPlot::EndPlot();
                }
                ImPlot::PopStyleVar(2);

                //Depth Row
                ImGui::TableSetColumnIndex(1);
                int depth_start = 7050;
                char buffer[6];
                for (int i = 0; i < 20; i++) {
                    ImGui::Dummy(ImVec2(0.0f, 20.0f));
                    ImGui::Text(itoa(depth_start, buffer, 10));
                    depth_start += 50;

                }

                //Resist Row
                ImGui::TableSetColumnIndex(2);
                ImPlot::PushStyleVar(ImPlotStyleVar_PlotDefaultSize, ImVec2(200, 1000));
                ImPlot::PushStyleVar(ImPlotStyleVar_PlotMinSize, ImVec2(200, 1000));
                if (ImPlot::BeginPlot("##Resist_Plot", ImVec2(0,0))) {
                    // Set opactity of shade to 25%

                    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                    ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
                    ImPlot::PlotShaded("##Test_Depth", resist_x, resist_y, rand_data_count, 0, ImPlotShadedFlags_Vertical);
                    ImPlot::PlotLine("##Test_Depth", resist_x, resist_y, rand_data_count);
                    ImPlot::PopStyleVar();
                    ImPlot::EndPlot();
                }
                
                ImGui::TableSetColumnIndex(4);
                ImGui::Checkbox("show1st", &show1st);      // Edit bools storing our window open/close state
                ImGui::SameLine();
                ImGui::Checkbox("show2nd", &show2nd);
                ImGui::SameLine();
                ImGui::Checkbox("show3rd", &show3rd);
                ImGui::Checkbox("show4th", &show4th);
                ImGui::SameLine();
                ImGui::Checkbox("show5th", &show5th);
                ImGui::SameLine();

                ImGui::Checkbox("show6th", &show6th);
                if (ImPlot::BeginPlot("##MINERALOGOY", ImVec2(-1, -1))) {
                    // Set opactity of shade to 25%
                   
                    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.85f);
                    ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels|ImPlotAxisFlags_Invert, ImPlotAxisFlags_NoDecorations);
                    ImPlot::SetupAxesLimits(0, 2800, 0 , rand_data_count);
                    if(show1st){
                        ImPlot::SetNextFillStyle(ImVec4(1.000f, 1.000f, 0.584f, 1.000f));
                        ImPlot::PlotShaded("##Test_Depth1", data_col5_x[0], data_col5_y[0], rand_data_count, 0, ImPlotShadedFlags_Vertical);
                    }
                ImPlot::PopStyleVar(2);

                //Neutron Row
                ImGui::TableSetColumnIndex(3);
                ImPlot::PushStyleVar(ImPlotStyleVar_PlotDefaultSize, ImVec2(200, 1000));
                ImPlot::PushStyleVar(ImPlotStyleVar_PlotMinSize, ImVec2(200, 1000));
                if (ImPlot::BeginPlot("##Neutron_Plot", ImVec2(0, 0))) {
                    // Set opactity of shade to 25%
                    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                    ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
                    ImPlot::PlotLine("##Neutron_nphi", neutron_nphi, neutron_y, rand_data_count);
                    ImPlot::PlotLine("##Neutron_rhob", neutron_rhob, neutron_y, rand_data_count);
                    ImPlot::PlotLine("##Neutron_pe", neutron_pe, neutron_y, rand_data_count);
                    ImPlot::PopStyleVar();
                    ImPlot::EndPlot();
                }
                ImPlot::PopStyleVar(2);


                ImGui::EndTable();
            }
            //if (ImPlot::BeginSubplots("My Subplot", 1, 2, ImVec2(800, 400))) {
            //    for (int i = 0; i < 2; ++i) {
            //        char id[5] = "##id";
            //        id[4] = (char)i;
            //        if (ImPlot::BeginPlot(id)) {
            //            
            //            ImPlot::PlotLine(id, x, y, rand_data_count);

                    if (show2nd) {
                        ImPlot::SetNextFillStyle(ImVec4(1.000f, 0.800f, 0.800f, 1.000f));

                        ImPlot::PlotShaded("##Test_Depth3", data_col5_x[1], data_col5_y[1], rand_data_count, 0, ImPlotShadedFlags_Vertical);
                    }
                    if (show3rd) {
                        ImPlot::SetNextFillStyle(ImVec4(0.200f, 0.200f, 1.000f, 1.000f));

                        ImPlot::PlotShaded("##Test_Depth4", data_col5_x[2], data_col5_y[2], rand_data_count, 0, ImPlotShadedFlags_Vertical);
                    }
                    if (show4th) {
                        ImPlot::SetNextFillStyle(ImVec4(1.000f, 0.800f, 0.200f, 1.000f));

                        ImPlot::PlotShaded("##Test_Depth5", data_col5_x[3], data_col5_y[3], rand_data_count, 0, ImPlotShadedFlags_Vertical);
                    }
                    if (show5th) {
                        ImPlot::SetNextFillStyle(ImVec4(0.200f, 0.200f, 0.200f, 1.000f));

                        ImPlot::PlotShaded("##Test_Depth6", data_col5_x[4], data_col5_y[4], rand_data_count, 0, ImPlotShadedFlags_Vertical);
                    }
                    ImPlot::PopStyleVar();
                    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 1.0f);
                    ImPlot::SetNextFillStyle(ImVec4(0.624f, 0.624f, 0.549f, 1.000f));
                    ImPlot::PlotShaded("##VClay", data_col5_x[0], data_col5_y[0], rand_data_count, 2800, ImPlotShadedFlags_Vertical);
                    

                    ImPlot::PopStyleVar();
                    ImPlot::EndPlot();
                }

                ImGui::TableSetColumnIndex(5);
                

                if (ImGui::BeginTable("petropy", 2, flags_petropy)) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    // bảng này bên trái 
                    if (ImPlot::BeginPlot("##POROSITY SATURATION", ImVec2(-1, -1))) {
                        // Set opactity of shade to 25%

                        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                        ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_Invert, ImPlotAxisFlags_NoDecorations);
                        ImPlot::PlotShaded("##plot5_1", data_col6_x[0], data_col6_y[0], rand_data_count, 0, ImPlotShadedFlags_Vertical);
                        ImPlot::PlotLine("##plot5_1", data_col6_x[0], data_col6_y[0], rand_data_count);
                        ImPlot::PopStyleVar();

                        //line bên trên, đổi màu sang green
                        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 1.0f);

                        ImVec4 fill_color = { 0.247f, 0.624f, 0.173f, 1.000f};
                        ImPlot::SetNextFillStyle(fill_color);
                        ImPlot::PlotShaded("##plot5_2", data_col6_x[1], data_col6_y[1], data_col6_x[0], rand_data_count, ImPlotShadedFlags_Vertical);
                        ImPlot::PlotLine("##plot5_2", data_col6_x[1], data_col6_y[1], rand_data_count);

                        ImPlot::PopStyleVar();
                        ImPlot::EndPlot();
                    }
                    ImGui::TableSetColumnIndex(1);
                    // bảng này bên phải
                    if (ImPlot::BeginPlot("##POROSITY SATURATION2", ImVec2(-1, -1))) {
                        // Set opactity of shade to 25%

                        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                        ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_Invert, ImPlotAxisFlags_NoDecorations);
                        ImPlot::PlotShaded("##plot5_3", data_col6_x[2], data_col6_y[2], rand_data_count, 0, ImPlotShadedFlags_Vertical);
                        ImPlot::PlotLine("##plot5_3", data_col6_x[2], data_col6_y[2], rand_data_count);

                        ImPlot::PopStyleVar();
                        ImPlot::EndPlot();
                    }
                    ImGui::EndTable();
                }
                ImGui::TableSetColumnIndex(6);
                if (ImPlot::BeginPlot("##OIL IN PLACE", ImVec2(-1, -1))) {
                    // Set opactity of shade to 25%

                    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                    ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_Invert| ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoDecorations);
                    ImPlot::SetupAxis(ImAxis_X2, "X-Axis 2", ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels);
                    ImPlot::SetupAxisLimits(ImAxis_X2, 0, 3000);
                    ImVec4 line_color = { 0.000f, 0.0f, 0.000f, 1.000f };
                    ImPlot::SetNextLineStyle(line_color);
                    ImPlot::PlotLine("##line1", data_col7_x[0], data_col7_y[0], rand_data_count);

                    ImVec4 fill_color = { 0.000f, 0.102f, 0.000f, 1.000f };
                    ImPlot::SetNextFillStyle(fill_color);
                    ImPlot::PlotShaded("##filled1", data_col7_x[0], data_col7_y[0], rand_data_count, 0, ImPlotShadedFlags_Vertical);

                    ImPlot::SetAxes(ImAxis_X2, ImAxis_Y1);


                    // cái này là bên trái 
                    line_color = { 0.000f, 0.392f, 0.000f, 1.000f };
                    
                    ImPlot::SetNextLineStyle(line_color);
                    ImPlot::PlotLine("##filled2", data_col7_x[1], data_col7_y[1], rand_data_count);

                    fill_color = { 0.000f, 0.392f, 0.000f, 1.000f };
                    ImPlot::SetNextFillStyle(fill_color);
                    ImPlot::PlotShaded("##line2", data_col7_x[1], data_col7_y[1], rand_data_count, 0, ImPlotShadedFlags_Vertical);
                    ImPlot::PopStyleVar();
                    ImPlot::EndPlot();
                }
                ImGui::TableSetColumnIndex(7);
                if (ImPlot::BeginPlot("##ELECTROFACIES", ImVec2(-1, -1))) {
                    // Set opactity of shade to 25%

                    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                    ImPlot::SetupAxes("X", "Y", ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_Invert | ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_NoDecorations);
                    ImPlot::SetupAxis(ImAxis_X2, "X-Axis 2", ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels);
                    ImPlot::SetupAxisLimits(ImAxis_X2, 0, 3000);
                    ImVec4 line_color = { 0.000f, 0.0f, 0.000f, 1.000f };
                    ImPlot::SetNextLineStyle(line_color);
                    ImPlot::PlotLine("##line1", data_col7_x[0], data_col7_y[0], rand_data_count);

                    ImVec4 fill_color = { 0.000f, 0.102f, 0.000f, 1.000f };
                    ImPlot::SetNextFillStyle(fill_color);
                    ImPlot::PlotShaded("##filled1", data_col7_x[0], data_col7_y[0], rand_data_count, 0, ImPlotShadedFlags_Vertical);

                    ImPlot::SetAxes(ImAxis_X2, ImAxis_Y1);


                    // cái này là bên trái 
                    line_color = { 0.000f, 0.392f, 0.000f, 1.000f };

                    ImPlot::SetNextLineStyle(line_color);
                    ImPlot::PlotLine("##filled2", data_col7_x[1], data_col7_y[1], rand_data_count);

                    fill_color = { 0.000f, 0.392f, 0.000f, 1.000f };
                    ImPlot::SetNextFillStyle(fill_color);
                    ImPlot::PlotShaded("##line2", data_col7_x[1], data_col7_y[1], rand_data_count, 0, ImPlotShadedFlags_Vertical);
                    ImPlot::PopStyleVar();
                    ImPlot::EndPlot();
                }
                ImGui::EndTable();
            }
           
            ImGui::End();
        }
        
        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
