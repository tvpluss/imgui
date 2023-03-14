#pragma once
#include <implot.h>
#include <iostream>
#include <vector>

typedef struct tag_y {
    double value;
    
    std::string tag_name;
    ImVec4 color;
    int id;
    bool visible;
    tag_y(const char* tagname, double* value = NULL, ImVec4* color = NULL) {
        this->value = value == NULL ? 0 : *value;
        this->color = color == NULL ? RandomColor() : *color;
        this->visible = true;
        this->tag_name = tagname;
    }
    ~tag_y() {

    }
    inline ImVec4 RandomColor() {
        ImVec4 col;
        col.x = RandomRange(0.0f, 1.0f);
        col.y = RandomRange(0.0f, 1.0f);
        col.z = RandomRange(0.0f, 1.0f);
        col.w = 1.0f;
        return col;
    }
    inline float RandomRange(float min, float max) {
        float scale = rand() / (float)RAND_MAX;
        return min + scale * (max - min);
    }
}tag_y;

class Config {
public:
    
     void ColorGui(ImGuiStyle* ref);
     void controller(ImGuiStyle& style);
     void ColorGui();
     void showTagConfig();

     void colorPicker(ImVec4* color);
     void addTagYConfig();
     void addTagY(const char* inputTagName, double* value = NULL, ImVec4* tagColor = NULL);

     std::vector<tag_y> v_listTagY;
};
