#include "Table.h"
#include <cstdlib>
#include <stdlib.h>
#include <imgui_internal.h>

static ImPlotAxisFlags x_axis = ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoMenus| ImPlotAxisFlags_Foreground| ImPlotAxisFlags_Opposite| ImPlotAxisFlags_NoHighlight;
static ImPlotAxisFlags y_axis =  ImPlotAxisFlags_NoLabel | ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoMenus| ImPlotAxisFlags_NoSideSwitch| ImPlotAxisFlags_NoHighlight;
static const int  rand_data_count = 8000;
static double ys[rand_data_count];
static double xs[3][rand_data_count];

float RandomRange1(float min, float max) {
    float scale = rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}
void SetUpData1(double* x, double* y, double y_increasement, int data_count, double x_min, double x_max) {

    for (int j = 0; j < data_count; j++) {
        if(x[j]==NULL)
        x[j] = RandomRange1(x_min, x_max);
        if (y[j] == NULL)
        y[j] = j * y_increasement;
    }
}

void SetUpData1(double* x, double* y, double y_increasement, double* x_min, int data_count, double x_max) {

    for (int j = 0; j < data_count; j++) {
        if (x[j] == NULL)
        x[j] = RandomRange1(x_min[j], x_max);
        if (y[j] == NULL)
        y[j] = j * y_increasement;
    }
}




void drawWell(char*label , double* x, double* y) {

    ImPlot::PushStyleVar(ImPlotStyleVar_PlotDefaultSize, ImVec2(-1, -1));
    ImPlot::PushStyleVar(ImPlotStyleVar_PlotMinSize, ImVec2(-1, -1));
    if (ImPlot::BeginPlot(label, ImVec2(-1, -1))) {
        // Set opactity of shade to 25%
        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
        ImPlot::SetupAxes("X", "Y", x_axis , y_axis | ImPlotAxisFlags_Invert);
        ImVec4 color = { 0.179f, 0.248f, 0.961f, 1.000f };
        ImPlot::SetNextLineStyle(color);
        ImPlot::PlotLine(label, x, y, rand_data_count);
        ImPlot::PlotShaded(label, x, y, rand_data_count, 0, ImPlotShadedFlags_Vertical);
        ImPlot::PopStyleVar();
        ImPlot::EndPlot();
    }
}

void Table::drawTable(bool* open) {
    if (ImGui::Begin("MyTestTable", open)) {
        int col = 4;
        float minDepth = 1000;
        float maxDepth = 8000;
        if (ImGui::BeginTable("##firstTable", col, 0, ImVec2(-1, -1), -1)) {
            ImGuiContext& g = *GImGui;
            ImGuiTable* table = g.CurrentTable;
            table->CellPaddingX = 0;
            table->CellSpacingX1 = 0;
            table->ColumnsAutoFitWidth = 0;
            table->CellSpacingX2 = 0;
       
            int row = 5;
            char* name[3] = {
                "##label 1",
                "##label 2",
                "##label 3"
            };
            for (int i = 0; i < row; i++) {
                ImGui::TableNextRow();
                for (int j = 0; j < col; j++) {
                    if ((row - 1) == i) {
                        if (j != 0) {
                            SetUpData1(xs[j - 1], ys, 1, rand_data_count, 100, 200);
                            ImGui::TableSetColumnIndex(j);
                            drawWell(name[j-1], xs[j - 1], ys);
                        }
                    }
                }
            }
            ImGui::EndTable();
        }
        ImGui::End();
    }
}
