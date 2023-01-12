#pragma once
#include "implot.h"
#include "implot_internal.h"
namespace ImPlot {
    IMPLOT_TMP void PlotShadedex(const char* label_id, const T* values, int count, double yref = 0, double xscale = 1, double xstart = 0,  int offset = 0, int stride = sizeof(T));
    IMPLOT_TMP void PlotShadedex(const char* label_id, const T* xs, const T* ys, int count, double yref = 0,  int offset = 0, int stride = sizeof(T));
    IMPLOT_TMP void PlotShadedex(const char* label_id, const T* xs, const T* ys1, const T* ys2, int count, ImPlotShadedFlags flags, int offset, int stride);
}
