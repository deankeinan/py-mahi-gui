/*
Copyright (c) 2017-2018 Stanislav Pidhorskyi
Copyright (c) 2019-2020 Joel Linn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>
#include <mutex>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "imgui_helper.h"
#include "leaked_ptr.h"

namespace py = pybind11;

void py_init_module_imgui_enums(py::module&);
void py_init_module_imgui_classes(py::module&);
void py_init_module_imgui_funcs(py::module&);

void py_init_module_imgui(py::module& m) {
  py_init_module_imgui_enums(m);
  py_init_module_imgui_classes(m);
  py_init_module_imgui_funcs(m);
}

void py_init_module_imgui_enums(py::module& m) {
  py::enum_<ImGuiCond_>(m, "Condition", py::arithmetic())
      .value("Always", ImGuiCond_Always)
      .value("Once", ImGuiCond_Once)
      .value("FirstUseEver", ImGuiCond_FirstUseEver)
      .value("Appearing", ImGuiCond_Appearing);

  py::enum_<ImGuiWindowFlags_>(m, "WindowFlags", py::arithmetic())
      .value("NoTitleBar", ImGuiWindowFlags_NoTitleBar)
      .value("NoResize", ImGuiWindowFlags_NoResize)
      .value("NoMove", ImGuiWindowFlags_NoMove)
      .value("NoScrollbar", ImGuiWindowFlags_NoScrollbar)
      .value("NoScrollWithMouse", ImGuiWindowFlags_NoScrollWithMouse)
      .value("NoCollapse", ImGuiWindowFlags_NoCollapse)
      .value("AlwaysAutoResize", ImGuiWindowFlags_AlwaysAutoResize)
      .value("NoBackground", ImGuiWindowFlags_NoBackground)
      .value("NoSavedSettings", ImGuiWindowFlags_NoSavedSettings)
      .value("NoMouseInputs", ImGuiWindowFlags_NoMouseInputs)
      .value("MenuBar", ImGuiWindowFlags_MenuBar)
      .value("HorizontalScrollbar", ImGuiWindowFlags_HorizontalScrollbar)
      .value("NoFocusOnAppearing", ImGuiWindowFlags_NoFocusOnAppearing)
      .value("NoBringToFrontOnFocus", ImGuiWindowFlags_NoBringToFrontOnFocus)
      .value("AlwaysVerticalScrollbar",
             ImGuiWindowFlags_AlwaysVerticalScrollbar)
      .value("AlwaysHorizontalScrollbar",
             ImGuiWindowFlags_AlwaysHorizontalScrollbar)
      .value("AlwaysUseWindowPadding", ImGuiWindowFlags_AlwaysUseWindowPadding)
      .value("NoNavInputs", ImGuiWindowFlags_NoNavInputs)
      .value("NoNavFocus", ImGuiWindowFlags_NoNavFocus)
      .value("UnsavedDocument", ImGuiWindowFlags_UnsavedDocument)
      .value("NoNav ", ImGuiWindowFlags_NoNav)
      .value("NoDecoration", ImGuiWindowFlags_NoDecoration)
      .value("NoInputs", ImGuiWindowFlags_NoInputs);

  py::enum_<ImGuiInputTextFlags_>(m, "InputTextFlags", py::arithmetic())
      .value("CharsDecimal", ImGuiInputTextFlags_CharsDecimal)
      .value("CharsHexadecimal", ImGuiInputTextFlags_CharsHexadecimal)
      .value("CharsUppercase", ImGuiInputTextFlags_CharsUppercase)
      .value("CharsNoBlank", ImGuiInputTextFlags_CharsNoBlank)
      .value("AutoSelectAll", ImGuiInputTextFlags_AutoSelectAll)
      .value("EnterReturnsTrue", ImGuiInputTextFlags_EnterReturnsTrue)
      //.value("CallbackCompletion",
      // ImGuiInputTextFlags_CallbackCompletion)
      //.value("CallbackHistory",
      // ImGuiInputTextFlags_CallbackHistory)
      //.value("CallbackAlways",
      // ImGuiInputTextFlags_CallbackAlways)
      //.value("CallbackCharFilter",
      // ImGuiInputTextFlags_CallbackCharFilter)
      .value("AllowTabInput", ImGuiInputTextFlags_AllowTabInput)
      .value("CtrlEnterForNewLine", ImGuiInputTextFlags_CtrlEnterForNewLine)
      .value("NoHorizontalScroll", ImGuiInputTextFlags_NoHorizontalScroll)
      .value("AlwaysInsertMode", ImGuiInputTextFlags_AlwaysInsertMode)
      .value("ReadOnly", ImGuiInputTextFlags_ReadOnly)
      .value("Password", ImGuiInputTextFlags_Password)
      //.value("NoUndoRedo",
      // ImGuiInputTextFlags_NoUndoRedo)
      .value("CallbackEdit", ImGuiInputTextFlags_CallbackEdit)
      .value("Multiline", ImGuiInputTextFlags_Multiline);

  py::enum_<ImGuiTableFlags_>(m, "TableFlags", py::arithmetic())
      // Features
      .value("None", ImGuiTableFlags_None)
      .value("Resizable", ImGuiTableFlags_Resizable, "Allow resizing columns.")
      .value("Reorderable", ImGuiTableFlags_Reorderable,
             "Allow reordering columns (need calling TableSetupColumn() + "
             "TableAutoHeaders() or TableHeaders() to display headers)")
      .value("Hideable", ImGuiTableFlags_Hideable,
             "Allow hiding columns (with right-click on header) (FIXME-TABLE: "
             "allow without headers).")
      .value("Sortable", ImGuiTableFlags_Sortable,
             "Allow sorting on one column (sort_specs_count will always be == "
             "1). Call TableGetSortSpecs() to obtain sort specs.")
      .value("MultiSortable", ImGuiTableFlags_MultiSortable,
             "Allow sorting on multiple columns by holding Shift "
             "(sort_specs_count may be > 1). Call TableGetSortSpecs() to "
             "obtain sort specs.")
      .value("NoSavedSettings", ImGuiTableFlags_NoSavedSettings,
             "Disable persisting columns order, width and sort settings in the "
             ".ini file.")
      // Decoration
      .value("RowBg", ImGuiTableFlags_RowBg,
             "Set each RowBg color with ImGuiCol_TableRowBg or "
             "ImGuiCol_TableRowBgAlt (equivalent to calling TableSetBgColor "
             "with ImGuiTableBgFlags_RowBg0 on each row manually)")
      .value("BordersInnerH", ImGuiTableFlags_BordersInnerH,
             "Draw horizontal borders between rows.")
      .value("BordersOuterH", ImGuiTableFlags_BordersOuterH,
             "Draw horizontal borders at the top and bottom.")
      .value("BordersInnerV", ImGuiTableFlags_BordersInnerV,
             "Draw vertical borders between columns.")
      .value("BordersOuterV", ImGuiTableFlags_BordersOuterV,
             "Draw vertical borders on the left and right sides.")
      .value("BordersH", ImGuiTableFlags_BordersH, "Draw horizontal borders.")
      .value("BordersV", ImGuiTableFlags_BordersV, "Draw vertical borders.")
      .value("BordersInner", ImGuiTableFlags_BordersInner,
             "Draw inner borders.")
      .value("BordersOuter", ImGuiTableFlags_BordersOuter,
             "Draw outer borders.")
      .value("Borders", ImGuiTableFlags_Borders, "Draw all borders.")
      .value("BordersFullHeightV", ImGuiTableFlags_BordersFullHeightV,
             "Borders covers all rows even when Headers are being used. Allow "
             "resizing from any rows.")
      // Padding, Sizing
      .value("NoClipX", ImGuiTableFlags_NoClipX,
             "Disable pushing clipping rectangle for every individual columns "
             "(reduce draw command count, items will be able to overflow)")
      .value("SizingPolicyFixedX", ImGuiTableFlags_SizingPolicyFixedX,
             "Default if ScrollX is on. Columns will default to use "
             "_WidthFixed or _WidthAlwaysAutoResize policy. Read description "
             "above for more details.")
      .value("SizingPolicyStretchX", ImGuiTableFlags_SizingPolicyStretchX,
             "Default if ScrollX is off. Columns will default to use "
             "_WidthStretch policy. Read description above for more details.")
      .value(
          "NoHeadersWidth", ImGuiTableFlags_NoHeadersWidth,
          "Disable header width contribution to automatic width calculation.")
      .value("NoHostExtendY", ImGuiTableFlags_NoHostExtendY,
             "(FIXME-TABLE: Reword as SizingPolicy?) Disable extending past "
             "the limit set by outer_size.y, only meaningful when neither of "
             "ScrollX|ScrollY are set (data below the limit will be clipped "
             "and not visible)")
      .value("NoKeepColumnsVisible", ImGuiTableFlags_NoKeepColumnsVisible,
             "(FIXME-TABLE) Disable code that keeps column always minimally "
             "visible when table width gets too small and horizontal scrolling "
             "is off.")
      // Scrolling
      .value("ScrollX", ImGuiTableFlags_ScrollX,
             "Enable horizontal scrolling. Require 'outer_size' parameter of "
             "begin_table() to specify the container size. Because this create "
             "a child window, ScrollY is currently generally recommended when "
             "using ScrollX.")
      .value("ScrollY", ImGuiTableFlags_ScrollY,
             "Enable vertical scrolling. Require 'outer_size' parameter of "
             "begin_table() to specify the container size.")
      .value("Scroll", ImGuiTableFlags_Scroll)
      .value("ScrollFreezeTopRow", ImGuiTableFlags_ScrollFreezeTopRow,
             "We can lock 1 to 3 rows (starting from the top). Use with "
             "ScrollY enabled.")
      .value("ScrollFreeze2Rows", ImGuiTableFlags_ScrollFreeze2Rows)
      .value("ScrollFreeze3Rows", ImGuiTableFlags_ScrollFreeze3Rows)
      .value("ScrollFreezeLeftColumn", ImGuiTableFlags_ScrollFreezeLeftColumn,
             "We can lock 1 to 3 columns (starting from the left). Use with "
             "ScrollX enabled.")
      .value("ScrollFreeze2Columns", ImGuiTableFlags_ScrollFreeze2Columns)
      .value("ScrollFreeze3Columns", ImGuiTableFlags_ScrollFreeze3Columns);

  py::enum_<ImGuiTableColumnFlags_>(m, "TableColumnFlags", py::arithmetic())
      .value("None", ImGuiTableColumnFlags_None)
      .value("DefaultHide", ImGuiTableColumnFlags_DefaultHide,
             "Default as a hidden column.")
      .value("DefaultSort", ImGuiTableColumnFlags_DefaultSort,
             "Default as a sorting column.")
      .value("WidthFixed", ImGuiTableColumnFlags_WidthFixed,
             "Column will keep a fixed size, preferable with horizontal "
             "scrolling enabled (default if table sizing policy is "
             "SizingPolicyFixedX and table is resizable).")
      .value(
          "WidthStretch", ImGuiTableColumnFlags_WidthStretch,
          "Column will stretch, preferable with horizontal scrolling disabled "
          "(default if table sizing policy is SizingPolicyStretchX).")
      .value("WidthAlwaysAutoResize",
             ImGuiTableColumnFlags_WidthAlwaysAutoResize,
             "Column will keep resizing based on submitted contents (with a "
             "one frame delay) == Fixed with auto resize (default if table "
             "sizing policy is SizingPolicyFixedX and table is not resizable).")
      .value("NoResize", ImGuiTableColumnFlags_NoResize,
             "Disable manual resizing.")
      .value("NoClipX", ImGuiTableColumnFlags_NoClipX,
             "Disable clipping for this column (all NoClipX columns will "
             "render in a same draw command).")
      .value("NoSort", ImGuiTableColumnFlags_NoSort,
             "Disable ability to sort on this field (even if "
             "ImGuiTableFlags_Sortable is set on the table).")
      .value("NoSortAscending", ImGuiTableColumnFlags_NoSortAscending,
             "Disable ability to sort in the ascending direction.")
      .value("NoSortDescending", ImGuiTableColumnFlags_NoSortDescending,
             "Disable ability to sort in the descending direction.")
      .value("NoHide", ImGuiTableColumnFlags_NoHide,
             "Disable hiding this column.")
      .value("NoHeaderWidth", ImGuiTableColumnFlags_NoHeaderWidth,
             "Header width don't contribute to automatic column width.")
      .value("PreferSortAscending", ImGuiTableColumnFlags_PreferSortAscending,
             "Make the initial sort direction Ascending when first sorting on "
             "this column (default).")
      .value("PreferSortDescending", ImGuiTableColumnFlags_PreferSortDescending,
             "Make the initial sort direction Descending when first sorting on "
             "this column.")
      .value("IndentEnable", ImGuiTableColumnFlags_IndentEnable,
             "Use current Indent value when entering cell (default for 1st "
             "column).")
      .value("IndentDisable", ImGuiTableColumnFlags_IndentDisable,
             "Ignore current Indent value when entering cell (default for "
             "columns after the 1st one). Indentation changes _within_ the "
             "cell will still be honored.")
      .value("NoReorder", ImGuiTableColumnFlags_NoReorder,
             "Disable reordering this column, this will also prevent other "
             "columns from crossing over this column.");

  py::enum_<ImGuiTableRowFlags_>(m, "TableRowFlags", py::arithmetic())
      .value("None", ImGuiTableRowFlags_None)
      .value("Headers", ImGuiTableRowFlags_Headers,
             "Identify header row (set default background color + width of its "
             "contents accounted different for auto column width)");

  py::enum_<ImGuiTableBgTarget_>(m, "TableBgTarget")
      .value("None", ImGuiTableBgTarget_None)
      .value("ColumnBg0", ImGuiTableBgTarget_ColumnBg0)
      .value("ColumnBg1", ImGuiTableBgTarget_ColumnBg1)
      .value("RowBg0", ImGuiTableBgTarget_RowBg0,
             "Set row background color 0 (generally used for background, "
             "automatically set when ImGuiTableFlags_RowBg is used)")
      .value(
          "RowBg1", ImGuiTableBgTarget_RowBg1,
          "Set row background color 1 (generally used for selection marking)")
      .value("CellBg", ImGuiTableBgTarget_CellBg,
             "Set cell background color (top-most color)");

  py::enum_<ImGuiDir_>(m, "Direction")
      .value("None", ImGuiDir_None)
      .value("Left", ImGuiDir_Left)
      .value("Right", ImGuiDir_Right)
      .value("Up", ImGuiDir_Up)
      .value("Down", ImGuiDir_Down);

  py::enum_<ImGuiCol_>(m, "Color")
      .value("Text", ImGuiCol_Text)
      .value("TextDisabled", ImGuiCol_TextDisabled)
      .value("WindowBg", ImGuiCol_WindowBg)
      .value("ChildBg", ImGuiCol_ChildBg)
      .value("PopupBg", ImGuiCol_PopupBg)
      .value("Border", ImGuiCol_Border)
      .value("BorderShadow", ImGuiCol_BorderShadow)
      .value("FrameBg", ImGuiCol_FrameBg)
      .value("FrameBgHovered", ImGuiCol_FrameBgHovered)
      .value("FrameBgActive", ImGuiCol_FrameBgActive)
      .value("TitleBg", ImGuiCol_TitleBg)
      .value("TitleBgActive", ImGuiCol_TitleBgActive)
      .value("TitleBgCollapsed", ImGuiCol_TitleBgCollapsed)
      .value("MenuBarBg", ImGuiCol_MenuBarBg)
      .value("ScrollbarBg", ImGuiCol_ScrollbarBg)
      .value("ScrollbarGrab", ImGuiCol_ScrollbarGrab)
      .value("ScrollbarGrabHovered", ImGuiCol_ScrollbarGrabHovered)
      .value("ScrollbarGrabActive", ImGuiCol_ScrollbarGrabActive)
      .value("CheckMark", ImGuiCol_CheckMark)
      .value("SliderGrab", ImGuiCol_SliderGrab)
      .value("SliderGrabActive", ImGuiCol_SliderGrabActive)
      .value("Button", ImGuiCol_Button)
      .value("ButtonHovered", ImGuiCol_ButtonHovered)
      .value("ButtonActive", ImGuiCol_ButtonActive)
      .value("Header", ImGuiCol_Header)
      .value("HeaderHovered", ImGuiCol_HeaderHovered)
      .value("HeaderActive", ImGuiCol_HeaderActive)
      .value("Separator", ImGuiCol_Separator)
      .value("SeparatorHovered", ImGuiCol_SeparatorHovered)
      .value("SeparatorActive", ImGuiCol_SeparatorActive)
      .value("ResizeGrip", ImGuiCol_ResizeGrip)
      .value("ResizeGripHovered", ImGuiCol_ResizeGripHovered)
      .value("ResizeGripActive", ImGuiCol_ResizeGripActive)
      .value("Tab", ImGuiCol_Tab)
      .value("TabHovered", ImGuiCol_TabHovered)
      .value("TabActive", ImGuiCol_TabActive)
      .value("TabUnfocused", ImGuiCol_TabUnfocused)
      .value("TabUnfocusedActive", ImGuiCol_TabUnfocusedActive)
      .value("PlotLines", ImGuiCol_PlotLines)
      .value("PlotLinesHovered", ImGuiCol_PlotLinesHovered)
      .value("PlotHistogram", ImGuiCol_PlotHistogram)
      .value("PlotHistogramHovered", ImGuiCol_PlotHistogramHovered)
      .value("TableHeaderBg", ImGuiCol_TableHeaderBg, "Table header background")
      .value("TableBorderStrong", ImGuiCol_TableBorderStrong,
             "Table outer and header borders (prefer using Alpha=1.0 here)")
      .value("TableBorderLight", ImGuiCol_TableBorderLight,
             "Table inner borders (prefer using Alpha=1.0 here)")
      .value("TableRowBg", ImGuiCol_TableRowBg,
             "Table row background (even rows)")
      .value("TableRowBgAlt", ImGuiCol_TableRowBgAlt,
             "Table row background (odd rows)")
      .value("TextSelectedBg", ImGuiCol_TextSelectedBg)
      .value("DragDropTarget", ImGuiCol_DragDropTarget)
      .value("NavHighlight", ImGuiCol_NavHighlight)
      .value("NavWindowingHighlight", ImGuiCol_NavWindowingHighlight)
      .value("NavWindowingDimBg", ImGuiCol_NavWindowingDimBg)
      .value("ModalWindowDimBg", ImGuiCol_ModalWindowDimBg);

  py::enum_<ImGuiStyleVar_>(m, "StyleVar")
      .value("Alpha", ImGuiStyleVar_Alpha)
      .value("WindowPadding", ImGuiStyleVar_WindowPadding)
      .value("WindowRounding", ImGuiStyleVar_WindowRounding)
      .value("WindowBorderSize", ImGuiStyleVar_WindowBorderSize)
      .value("WindowMinSize", ImGuiStyleVar_WindowMinSize)
      .value("WindowTitleAlign", ImGuiStyleVar_WindowTitleAlign)
      .value("ChildRounding", ImGuiStyleVar_ChildRounding)
      .value("ChildBorderSize", ImGuiStyleVar_ChildBorderSize)
      .value("PopupRounding", ImGuiStyleVar_PopupRounding)
      .value("PopupBorderSize", ImGuiStyleVar_PopupBorderSize)
      .value("FramePadding", ImGuiStyleVar_FramePadding)
      .value("FrameRounding", ImGuiStyleVar_FrameRounding)
      .value("FrameBorderSize", ImGuiStyleVar_FrameBorderSize)
      .value("ItemSpacing", ImGuiStyleVar_ItemSpacing)
      .value("ItemInnerSpacing", ImGuiStyleVar_ItemInnerSpacing)
      .value("IndentSpacing", ImGuiStyleVar_IndentSpacing)
      .value("CellPadding", ImGuiStyleVar_CellPadding)
      .value("ScrollbarSize", ImGuiStyleVar_ScrollbarSize)
      .value("ScrollbarRounding", ImGuiStyleVar_ScrollbarRounding)
      .value("GrabMinSize", ImGuiStyleVar_GrabMinSize)
      .value("GrabRounding", ImGuiStyleVar_GrabRounding)
      .value("TabRounding", ImGuiStyleVar_TabRounding)
      .value("ButtonTextAlign", ImGuiStyleVar_ButtonTextAlign)
      .value("SelectableTextAlign", ImGuiStyleVar_SelectableTextAlign);

  py::enum_<ImGuiFocusedFlags_>(m, "FocusedFlags")
      .value("None", ImGuiFocusedFlags_None)
      .value("ChildWindows", ImGuiFocusedFlags_ChildWindows)
      .value("RootWindow", ImGuiFocusedFlags_RootWindow)
      .value("AnyWindow", ImGuiFocusedFlags_AnyWindow)
      .value("RootAndChildWindows", ImGuiFocusedFlags_RootAndChildWindows);

  py::enum_<ImGuiHoveredFlags_>(m, "HoveredFlags")
      .value("None", ImGuiHoveredFlags_None)
      .value("ChildWindows", ImGuiHoveredFlags_ChildWindows)
      .value("RootWindow", ImGuiHoveredFlags_RootWindow)
      .value("AnyWindow", ImGuiHoveredFlags_AnyWindow)
      .value("AllowWhenBlockedByPopup",
             ImGuiHoveredFlags_AllowWhenBlockedByPopup)
      .value("AllowWhenBlockedByActiveItem",
             ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)
      .value("AllowWhenOverlapped", ImGuiHoveredFlags_AllowWhenOverlapped)
      .value("AllowWhenDisabled", ImGuiHoveredFlags_AllowWhenDisabled)
      .value("RectOnly", ImGuiHoveredFlags_RectOnly);

  py::enum_<ImDrawCornerFlags_>(m, "CornerFlags", py::arithmetic())
      .value("TopLeft", ImDrawCornerFlags_TopLeft)
      .value("TopRight", ImDrawCornerFlags_TopRight)
      .value("BotRight", ImDrawCornerFlags_BotRight)
      .value("BotLeft", ImDrawCornerFlags_BotLeft)
      .value("Top", ImDrawCornerFlags_Top)
      .value("Bot", ImDrawCornerFlags_Bot)
      .value("Left", ImDrawCornerFlags_Left)
      .value("Right", ImDrawCornerFlags_Right)
      .value("All", ImDrawCornerFlags_All);

  py::enum_<ImGuiSliderFlags_>(m, "SliderFlags", py::arithmetic())
      .value("None", ImGuiSliderFlags_None)
      .value("ClampOnInput", ImGuiSliderFlags_ClampOnInput,
             "Clamp value to min/max bounds when input manually with "
             "CTRL+Click. By default CTRL+Click allows going out of bounds.")
      .value("Logarithmic", ImGuiSliderFlags_Logarithmic,
             "Make the widget logarithmic (linear otherwise). Consider using "
             "ImGuiSliderFlags_NoRoundToFormat with this if using a "
             "format-string with small amount of digits.")
      .value(
          "NoRoundToFormat", ImGuiSliderFlags_NoRoundToFormat,
          "Disable rounding underlying value to match precision of the display "
          "format string (e.g. %.3f values are rounded to those 3 digits)")
      .value("NoInput", ImGuiSliderFlags_NoInput,
             "Disable CTRL+Click or Enter key allowing to input text directly "
             "into the widget");
}

void py_init_module_imgui_classes(py::module& m) {
  py::class_<Bool>(m, "Bool")
      .def(py::init())
      .def(py::init<bool>())
      .def_readwrite("value", &Bool::value);

  py::class_<Float>(m, "Float")
      .def(py::init())
      .def(py::init<float>())
      .def_readwrite("value", &Float::value);

  py::class_<Int>(m, "Int")
      .def(py::init())
      .def(py::init<int>())
      .def_readwrite("value", &Int::value);

  py::class_<String>(m, "String")
      .def(py::init())
      .def(py::init<std::string>())
      .def_readwrite("value", &String::value);

  py::class_<ImVec2>(m, "Vec2")
      .def(py::init())
      .def(py::init<float, float>())
      .def_readwrite("x", &ImVec2::x)
      .def_readwrite("y", &ImVec2::y)
      .def(py::self * float())
      .def(py::self / float())
      .def(py::self + py::self)
      .def(py::self - py::self)
      .def(py::self * py::self)
      .def(py::self / py::self)
      .def(py::self += py::self)
      .def(py::self -= py::self)
      .def(py::self *= float())
      .def(py::self /= float())
      .def(
          "__mul__", [](float b, const ImVec2& a) { return a * b; },
          py::is_operator());

  py::class_<ImVec4>(m, "Vec4")
      .def(py::init())
      .def(py::init<float, float, float, float>())
      .def_readwrite("x", &ImVec4::x)
      .def_readwrite("y", &ImVec4::y)
      .def_readwrite("z", &ImVec4::z)
      .def_readwrite("w", &ImVec4::w)
      .def(py::self + py::self)
      .def(py::self - py::self)
      .def(
          "__mul__",
          [](const ImVec4& a, float b) {
            return ImVec4(a.x * b, a.y * b, a.z * b, a.w * b);
          },
          py::is_operator())
      .def(
          "__rmul__",
          [](const ImVec4& a, float b) {
            return ImVec4(a.x * b, a.y * b, a.z * b, a.w * b);
          },
          py::is_operator());

  py::class_<ImGuiStyle>(m, "Style")
      .def(py::init())
      .def_readwrite("alpha", &ImGuiStyle::Alpha)
      .def_readwrite("window_padding", &ImGuiStyle::WindowPadding)
      .def_readwrite("window_rounding", &ImGuiStyle::WindowRounding)
      .def_readwrite("window_border_size", &ImGuiStyle::WindowBorderSize)
      .def_readwrite("window_min_size", &ImGuiStyle::WindowMinSize)
      .def_readwrite("window_title_align", &ImGuiStyle::WindowTitleAlign)
      .def_readwrite("window_menu_button_position",
                     &ImGuiStyle::WindowMenuButtonPosition)
      .def_readwrite("child_rounding", &ImGuiStyle::ChildRounding)
      .def_readwrite("child_border_size", &ImGuiStyle::ChildBorderSize)
      .def_readwrite("popup_rounding", &ImGuiStyle::PopupRounding)
      .def_readwrite("popup_border_size", &ImGuiStyle::PopupBorderSize)
      .def_readwrite("frame_padding", &ImGuiStyle::FramePadding)
      .def_readwrite("frame_rounding", &ImGuiStyle::FrameRounding)
      .def_readwrite("frame_border_size", &ImGuiStyle::FrameBorderSize)
      .def_readwrite("item_spacing", &ImGuiStyle::ItemSpacing)
      .def_readwrite("item_inner_spacing", &ImGuiStyle::ItemInnerSpacing)
      .def_readwrite("touch_extra_spacing", &ImGuiStyle::TouchExtraPadding)
      .def_readwrite("indent_spacing", &ImGuiStyle::IndentSpacing)
      .def_readwrite("columns_min_spacing", &ImGuiStyle::ColumnsMinSpacing)
      .def_readwrite("scroll_bar_size", &ImGuiStyle::ScrollbarSize)
      .def_readwrite("scroll_bar_rounding", &ImGuiStyle::ScrollbarRounding)
      .def_readwrite("grab_min_size", &ImGuiStyle::GrabMinSize)
      .def_readwrite("grab_rounding", &ImGuiStyle::GrabRounding)
      .def_readwrite("tab_rounding", &ImGuiStyle::TabRounding)
      .def_readwrite("tab_border_size", &ImGuiStyle::TabBorderSize)
      .def_readwrite("color_button_position", &ImGuiStyle::ColorButtonPosition)
      .def_readwrite("button_text_align", &ImGuiStyle::ButtonTextAlign)
      .def_readwrite("selectable_text_align", &ImGuiStyle::SelectableTextAlign)
      .def_readwrite("display_window_padding",
                     &ImGuiStyle::DisplayWindowPadding)
      .def_readwrite("display_safe_area_padding",
                     &ImGuiStyle::DisplaySafeAreaPadding)
      .def_readwrite("mouse_cursor_scale", &ImGuiStyle::MouseCursorScale)
      .def_readwrite("anti_aliased_lines", &ImGuiStyle::AntiAliasedLines)
      .def_readwrite("anti_aliased_fill", &ImGuiStyle::AntiAliasedFill)
      .def_readwrite("curve_tessellation_tol",
                     &ImGuiStyle::CurveTessellationTol)
      .def("get_color",
           [](ImGuiStyle& self, ImGuiCol_ a) { return self.Colors[(int)a]; })
      .def("set_color", [](ImGuiStyle& self, ImGuiCol_ a, ImVec4 c) {
        self.Colors[(int)a] = c;
      });

#define DEF_IO_PROPERTY_PCHAR_BUT_NULL(__py_name__, __c_name__)                \
  .def_property(                                                               \
      #__py_name__,                                                            \
      [](const ImGuiIO& self) -> const char* { return self.__c_name__; },      \
      [](ImGuiIO& self, const char* s) {                                       \
        if (s) {                                                               \
          throw std::invalid_argument("Setting this value to anything but "    \
                                      "None is currently unsupported.");       \
        }                                                                      \
        /* disable if desired */                                               \
        self.__c_name__ = nullptr;                                             \
      })
  py::class_<ImGuiIO, leaked_ptr<ImGuiIO>>(m, "IO")
      .def_readwrite("config_flags", &ImGuiIO::ConfigFlags)
      .def_readwrite("backend_flags", &ImGuiIO::BackendFlags)
      .def_readwrite("display_size", &ImGuiIO::DisplaySize)
      .def_readwrite("delta_time", &ImGuiIO::DeltaTime)
      .def_readwrite("ini_saving_rate", &ImGuiIO::IniSavingRate)
          DEF_IO_PROPERTY_PCHAR_BUT_NULL(ini_filename, IniFilename)
              DEF_IO_PROPERTY_PCHAR_BUT_NULL(log_filename, LogFilename)
      .def_readwrite("mouse_double_click_time", &ImGuiIO::MouseDoubleClickTime)
      .def_readwrite("mouse_double_click_max_dist",
                     &ImGuiIO::MouseDoubleClickMaxDist)
      .def_readwrite("mouse_drag_threshold", &ImGuiIO::MouseDragThreshold)
      //.def_readwrite("key_map", &ImGuiIO::KeyMap[ImGuiKey_COUNT])
      .def_readwrite("key_repeat_delay", &ImGuiIO::KeyRepeatDelay)
      .def_readwrite("key_repeat_rate", &ImGuiIO::KeyRepeatRate)
      .def_readwrite("user_data", &ImGuiIO::UserData)
      // ======================================================================
      //.def_readwrite("fonts" &ImGuiIO::Fonts)
      .def_readwrite("font_global_scale", &ImGuiIO::FontGlobalScale)
      .def_readwrite("font_allow_user_scaling", &ImGuiIO::FontAllowUserScaling)
      //.def_readwrite("font_default", &ImGuiIO::FontDefault)
      .def_readwrite("display_framebuffer_scale",
                     &ImGuiIO::DisplayFramebufferScale)
      // ======================================================================
      .def_readwrite("config_docking_no_split", &ImGuiIO::ConfigDockingNoSplit)
      .def_readwrite("config_docking_with_shift",
                     &ImGuiIO::ConfigDockingWithShift)
      .def_readwrite("config_docking_always_tab_bar",
                     &ImGuiIO::ConfigDockingAlwaysTabBar)
      .def_readwrite("config_docking_transparent_payload",
                     &ImGuiIO::ConfigDockingTransparentPayload)
      // ======================================================================
      .def_readwrite("config_viewports_no_auto_merge",
                     &ImGuiIO::ConfigViewportsNoAutoMerge)
      .def_readwrite("config_viewports_no_task_bar_icon",
                     &ImGuiIO::ConfigViewportsNoTaskBarIcon)
      .def_readwrite("config_viewports_no_decoration",
                     &ImGuiIO::ConfigViewportsNoDecoration)
      .def_readwrite("config_viewports_no_default_parent",
                     &ImGuiIO::ConfigViewportsNoDefaultParent)
      // ======================================================================
      .def_readwrite("mouse_draw_cursor", &ImGuiIO::MouseDrawCursor)
      .def_readwrite("config_macosx_behaviours",
                     &ImGuiIO::ConfigMacOSXBehaviors)
      .def_readwrite("config_input_text_cursor_blink",
                     &ImGuiIO::ConfigInputTextCursorBlink)
      .def_readwrite("config_windows_resize_from_edges",
                     &ImGuiIO::ConfigWindowsResizeFromEdges)
      .def_readwrite("config_windows_move_from_title_bar_only",
                     &ImGuiIO::ConfigWindowsMoveFromTitleBarOnly)
      .def_readwrite("config_windows_memory_compact_timer",
                     &ImGuiIO::ConfigWindowsMemoryCompactTimer)
      // ======================================================================
      .def_readonly("backend_platform_name", &ImGuiIO::BackendPlatformName)
      .def_readonly("backend_renderer_name", &ImGuiIO::BackendRendererName)
      //.def_readwrite("backend_platform_user_data",
      //&ImGuiIO::BackendPlatformUserData)
      //.def_readwrite("backend_renderer_user_data",
      //&ImGuiIO::BackendRendererUserData)
      //.def_readwrite("backend_language_user_data",
      //&ImGuiIO::BackendLanguageUserData)
      // ======================================================================
      // GetClipboardTextFn
      // SetClipboardTextFn
      // ClipboardUserData
      // ======================================================================
      .def_readwrite("mouse_pos", &ImGuiIO::MousePos)
      //.def_readwrite("", &ImGuiIO::MouseDown[5])
      .def_readwrite("mouse_wheel", &ImGuiIO::MouseWheel)
      .def_readwrite("mouse_wheel_h", &ImGuiIO::MouseWheelH)
      .def_readwrite("mouse_hovered_viewport", &ImGuiIO::MouseHoveredViewport)
      .def_readwrite("key_ctrl", &ImGuiIO::KeyCtrl)
      .def_readwrite("key_shift", &ImGuiIO::KeyShift)
      .def_readwrite("key_alt", &ImGuiIO::KeyAlt)
      .def_readwrite("key_super", &ImGuiIO::KeySuper)
      //.def_readwrite("keys_down", &ImGuiIO::KeysDown[512])
      //.def_readwrite("nav_inputs", &ImGuiIO::NavInputs[ImGuiNavInput_COUNT])
      // ======================================================================
      .def("add_input_character", &ImGuiIO::AddInputCharacter, py::arg("c"))
      .def("add_input_character_utf8", &ImGuiIO::AddInputCharactersUTF8,
           py::arg("str"))
      .def("clear_input_characters", &ImGuiIO::ClearInputCharacters)
      // ======================================================================
      .def_readwrite("want_capture_mouse", &ImGuiIO::WantCaptureMouse)
      .def_readwrite("want_capture_keyboard", &ImGuiIO::WantCaptureKeyboard)
      .def_readwrite("want_text_input", &ImGuiIO::WantTextInput)
      .def_readwrite("want_set_mouse_pos", &ImGuiIO::WantSetMousePos)
      .def_readwrite("want_save_ini_settings", &ImGuiIO::WantSaveIniSettings)
      .def_readwrite("nav_active", &ImGuiIO::NavActive)
      .def_readwrite("nav_visible", &ImGuiIO::NavVisible)
      .def_readwrite("framerate", &ImGuiIO::Framerate)
      .def_readwrite("metrics_render_vertices", &ImGuiIO::MetricsRenderVertices)
      .def_readwrite("metrics_render_indices", &ImGuiIO::MetricsRenderIndices)
      .def_readwrite("metrics_render_windows", &ImGuiIO::MetricsRenderWindows)
      .def_readwrite("metrics_active_windows", &ImGuiIO::MetricsActiveWindows)
      .def_readwrite("metrics_active_allocations",
                     &ImGuiIO::MetricsActiveAllocations)
      .def_readwrite("mouse_delta", &ImGuiIO::MouseDelta)
      // ======================================================================
      // INTERNAL
      ;
}

void py_init_module_imgui_funcs(py::module& m) {
  m.def("get_io", []() -> leaked_ptr<ImGuiIO> {
    // Does NOT copy the object to python scope and
    // does NOT free when out of scope.
    // The ImGuiIO pointer is valid as long as the ImGui context is valid.
    return leaked_ptr<ImGuiIO>(&ImGui::GetIO());
  });

  m.def("get_style", &ImGui::GetStyle);
  m.def("set_style", [](const ImGuiStyle& a) { ImGui::GetStyle() = a; });

  m.def("style_colors_classic", []() { ImGui::StyleColorsClassic(); });
  m.def("style_colors_dark", []() { ImGui::StyleColorsDark(); });
  m.def("style_colors_light", []() { ImGui::StyleColorsLight(); });

  m.def(
      "show_demo_window", []() { ImGui::ShowDemoWindow(); },
      "create demo/test window (previously called ShowTestWindow). demonstrate "
      "most ImGui features.");
  m.def(
      "show_about_window", []() { ImGui::ShowAboutWindow(); },
      "create About window. display Dear ImGui version, credits and "
      "build/system information.");
  m.def(
      "show_metrics_window", []() { ImGui::ShowMetricsWindow(); },
      "create metrics window. display ImGui internals: draw commands (with "
      "individual draw calls and vertices), window list, basic internal state, "
      "etc.");
  m.def(
      "show_style_editor", []() { ImGui::ShowStyleEditor(); },
      "add style editor block (not a window). you can pass in a reference "
      "ImGuiStyle structure to compare to, revert to and save to (else it uses "
      "the default style)");
  m.def(
      "show_style_selector",
      [](const char* label) { ImGui::ShowStyleSelector(label); },
      "add style selector block (not a window), essentially a combo listing "
      "the default styles.");
  m.def(
      "show_font_selector",
      [](const char* label) { ImGui::ShowFontSelector(label); },
      "add font selector block (not a window), essentially a combo listing the "
      "loaded fonts.");
  m.def(
      "show_user_guide", []() { ImGui::ShowUserGuide(); },
      "add basic help/info block (not a window): how to manipulate ImGui as a "
      "end-user (mouse/keyboard controls).");

  m.def(
      "begin",
      [](const std::string& name, Bool& opened,
         ImGuiWindowFlags flags) -> bool {
        return ImGui::Begin(name.c_str(), opened.null ? nullptr : &opened.value,
                            flags);
      },
      "Push a new ImGui window to add widgets to", py::arg("name"),
      py::arg("opened") = null, py::arg("flags") = ImGuiWindowFlags_(0));

  m.def("end", &ImGui::End);

  m.def(
      "begin_child",
      [](const std::string& str_id, const ImVec2& size, bool border,
         ImGuiWindowFlags extra_flags) -> bool {
        return ImGui::BeginChild(str_id.c_str(), size, border);
      },
      "begin a scrolling region. size==0.0f: use remaining window size, "
      "size<0.0f: use remaining window size minus abs(size). size>0.0f: fixed "
      "size. each axis can use a different mode, e.g. ImVec2(0,400).",
      py::arg("str_id"), py::arg("size") = ImVec2(0, 0),
      py::arg("border") = false, py::arg("extra_flags") = ImGuiWindowFlags_(0));

  m.def("end_child", &ImGui::EndChild);

  m.def("begin_main_menu_bar", &ImGui::BeginMainMenuBar,
        "create and append to a full screen menu-bar. only call "
        "EndMainMenuBar() if this returns true!");
  m.def("end_main_menu_bar", &ImGui::EndMainMenuBar);
  m.def("begin_menu_bar", &ImGui::BeginMenuBar,
        "append to menu-bar of current window (requires "
        "ImGuiWindowFlags_MenuBar flag set on parent window). only call "
        "EndMenuBar() if this returns true!");
  m.def("end_menu_bar", &ImGui::EndMenuBar);
  m.def(
      "begin_menu",
      [](const std::string& name, Bool& enabled) -> bool {
        return ImGui::BeginMenu(
            name.c_str(), (bool*)(enabled.null ? nullptr : &enabled.value));
      },
      "create a sub-menu entry. only call EndMenu() if this returns true!",
      py::arg("name"), py::arg("enabled") = Bool(true));
  m.def(
      "menu_item",
      [](const std::string& label, const std::string& shortcut, Bool& selected,
         bool enabled) -> bool {
        return ImGui::MenuItem(label.c_str(), shortcut.c_str(),
                               selected.null ? nullptr : &selected.value,
                               enabled);
      },
      "return true when activated + toggle (*p_selected) if p_selected != NULL",
      py::arg("name"), py::arg("shortcut"), py::arg("selected") = Bool(false),
      py::arg("enabled") = true);
  m.def("end_menu", &ImGui::EndMenu);

  m.def("begin_tooltip", &ImGui::BeginTooltip);
  m.def("end_tooltip", &ImGui::EndTooltip);
  m.def("set_tooltip", [](const char* text) { ImGui::SetTooltip("%s", text); });

  m.def(
      "open_popup",
      [](std::string str_id) { ImGui::OpenPopup(str_id.c_str()); },
      "call to mark popup as open (don't call every frame!). popups are closed "
      "when user click outside, or if CloseCurrentPopup() is called within a "
      "BeginPopup()/EndPopup() block. By default, Selectable()/MenuItem() are "
      "calling CloseCurrentPopup(). Popup identifiers are relative to the "
      "current ID-stack (so OpenPopup and BeginPopup needs to be at the same "
      "level).");
  m.def(
      "open_popup_context_item",
      [](std::string str_id = "", int mouse_button = 1) {
        ImGui::OpenPopupContextItem(str_id.c_str(), mouse_button);
      },
      "helper to open popup when clicked on last item. return true when just  "
      "opened. (note: actually triggers on the mouse _released_ event to be "
      "consistent with popup behaviors)");
  m.def(
      "begin_popup",
      [](std::string str_id, ImGuiWindowFlags flags) -> bool {
        return ImGui::BeginPopup(str_id.c_str(), flags);
      },
      "", py::arg("name"), py::arg("flags") = ImGuiWindowFlags_(0));
  m.def(
      "begin_popup_modal",
      [](std::string name = "") -> bool {
        return ImGui::BeginPopupModal(name.c_str());
      },
      "");

  // add more arguments later:
  m.def(
      "begin_popup_context_item", []() { ImGui::BeginPopupContextItem(); },
      "helper to open and begin popup when clicked on last item. if you can "
      "pass a NULL str_id only if the previous item had an id. If you want to "
      "use that on a non-interactive item such as Text() you need to pass in "
      "an explicit ID here.");
  m.def(
      "begin_popup_context_window", []() { ImGui::BeginPopupContextWindow(); },
      "helper to open and begin popup when clicked on current window.");
  m.def(
      "begin_popup_context_void", []() { ImGui::BeginPopupContextVoid(); },
      "helper to open and begin popup when clicked in void (where there are no "
      "imgui windows).");

  m.def("end_popup", &ImGui::EndPopup);
  m.def(
      "is_popup_open",
      [](std::string str_id = "") -> bool {
        return ImGui::IsPopupOpen(str_id.c_str());
      },
      "");
  m.def("close_current_popup", &ImGui::CloseCurrentPopup);

  m.def("get_content_region_max", &ImGui::GetContentRegionMax);
  m.def("get_content_region_avail", &ImGui::GetContentRegionAvail);
  // OBSOLETED in 1.70 (from May 2019)
  m.def("get_content_region_avail_width", &ImGui::GetContentRegionAvailWidth);
  m.def("get_window_content_region_min", &ImGui::GetWindowContentRegionMin);
  m.def("get_window_content_region_max", &ImGui::GetWindowContentRegionMax);
  m.def("get_window_content_region_width", &ImGui::GetWindowContentRegionWidth);
  m.def("get_font_size", &ImGui::GetFontSize);
  m.def("set_window_font_scale", &ImGui::SetWindowFontScale);
  m.def("get_window_pos", &ImGui::GetWindowPos);
  m.def("get_window_size", &ImGui::GetWindowSize);
  m.def("get_window_width", &ImGui::GetWindowWidth);
  m.def("get_window_height", &ImGui::GetWindowHeight);
  m.def("is_window_collapsed", &ImGui::IsWindowCollapsed);
  m.def("is_window_appearing", &ImGui::IsWindowAppearing);
  m.def("is_window_focused", &ImGui::IsWindowFocused);
  m.def("is_window_hovered", &ImGui::IsWindowHovered);

  m.def("set_window_font_scale", &ImGui::SetWindowFontScale);

  m.def("set_next_window_pos", &ImGui::SetNextWindowPos, py::arg("pos"),
        py::arg("cond") = 0, py::arg("pivot") = ImVec2(0, 0));
  m.def("set_next_window_size", &ImGui::SetNextWindowSize, py::arg("size"),
        py::arg("cond") = 0);
  m.def(
      "set_next_window_size_constraints",
      [](const ImVec2& size_min, const ImVec2& size_max) {
        ImGui::SetNextWindowSizeConstraints(size_min, size_max);
      },
      py::arg("size_min"), py::arg("size_max") = 0);
  m.def("set_next_window_content_size", &ImGui::SetNextWindowContentSize,
        py::arg("size"));
  m.def("set_next_window_collapsed", &ImGui::SetNextWindowCollapsed,
        py::arg("collapsed"), py::arg("cond") = 0);
  m.def("set_next_window_focus", &ImGui::SetNextWindowFocus);
  m.def("set_next_window_bg_alpha", &ImGui::SetNextWindowBgAlpha,
        py::arg("alpha"));
  m.def("set_window_pos",
        py::overload_cast<const ImVec2&, ImGuiCond>(&ImGui::SetWindowPos),
        py::arg("pos"), py::arg("cond") = 0);
  m.def("set_window_size",
        py::overload_cast<const ImVec2&, ImGuiCond>(&ImGui::SetWindowSize),
        py::arg("size"), py::arg("cond") = 0);
  m.def("set_window_collapsed",
        py::overload_cast<bool, ImGuiCond>(&ImGui::SetWindowCollapsed),
        py::arg("collapsed"), py::arg("cond") = 0);
  m.def("set_window_pos",
        py::overload_cast<const char*, const ImVec2&, ImGuiCond>(
            &ImGui::SetWindowPos),
        py::arg("name"), py::arg("pos"), py::arg("cond") = 0);
  m.def("set_window_size",
        py::overload_cast<const char*, const ImVec2&, ImGuiCond>(
            &ImGui::SetWindowSize),
        py::arg("name"), py::arg("size"), py::arg("cond") = 0);
  m.def("set_window_collapsed",
        py::overload_cast<const char*, bool, ImGuiCond>(
            &ImGui::SetWindowCollapsed),
        py::arg("name"), py::arg("collapsed"), py::arg("cond") = 0);
  m.def(
      "set_window_focus", [](const char* name) { ImGui::SetWindowFocus(name); },
      py::arg("name"));

  m.def("get_scroll_x", &ImGui::GetScrollX);
  m.def("get_scroll_y", &ImGui::GetScrollY);
  m.def("get_scroll_max_x", &ImGui::GetScrollMaxX);
  m.def("get_scroll_max_y", &ImGui::GetScrollMaxY);
  m.def(
      "set_scroll_x", [](float scroll_x) { ImGui::SetScrollX(scroll_x); },
      py::arg("scroll_x"));
  m.def(
      "set_scroll_y", [](float scroll_y) { ImGui::SetScrollY(scroll_y); },
      py::arg("scroll_y"));
  // OBSOLETED in 1.66 (from Sep 2018)
  m.def("set_scroll_here", &ImGui::SetScrollHere,
        py::arg("center_y_ratio") = 0.5f);
  m.def("set_scroll_here_x", &ImGui::SetScrollHereX,
        py::arg("center_x_ratio") = 0.5f);
  m.def("set_scroll_here_y", &ImGui::SetScrollHereY,
        py::arg("center_y_ratio") = 0.5f);
  m.def(
      "set_scroll_from_pos_x",
      [](float pos_x, float center_x_ratio) {
        ImGui::SetScrollFromPosX(pos_x, center_x_ratio);
      },
      py::arg("pos_x"), py::arg("center_x_ratio") = 0.5f);
  m.def(
      "set_scroll_from_pos_y",
      [](float pos_y, float center_y_ratio) {
        ImGui::SetScrollFromPosY(pos_y, center_y_ratio);
      },
      py::arg("pos_y"), py::arg("center_y_ratio") = 0.5f);
  m.def("set_keyboard_focus_here", &ImGui::SetKeyboardFocusHere,
        py::arg("offset") = 0.0f);

  m.def("push_style_color", [](ImGuiCol_ idx, const ImVec4& col) {
    ImGui::PushStyleColor((ImGuiCol)idx, col);
  });
  m.def("pop_style_color", &ImGui::PopStyleColor, py::arg("count") = 1);

  m.def("push_style_var", [](ImGuiStyleVar_ idx, float val) {
    ImGui::PushStyleVar((ImGuiStyleVar)idx, val);
  });
  m.def("push_style_var", [](ImGuiStyleVar_ idx, ImVec2 val) {
    ImGui::PushStyleVar((ImGuiStyleVar)idx, val);
  });
  m.def("pop_style_var", &ImGui::PopStyleVar, py::arg("count") = 1);

  m.def("push_item_width", &ImGui::PushItemWidth);
  m.def("pop_item_width", &ImGui::PopItemWidth);
  m.def("set_next_item_width", &ImGui::SetNextItemWidth);
  m.def("calc_item_width", &ImGui::CalcItemWidth);
  m.def("calc_text_size", &ImGui::CalcTextSize, py::arg("text"),
        py::arg("text_end") = nullptr,
        py::arg("hide_text_after_double_hash") = false,
        py::arg("wrap_width") = 0.0f);
  m.def("push_text_wrap_pos", &ImGui::PushTextWrapPos,
        py::arg("wrap_pos_x") = 0.0f);
  m.def("pop_text_wrap_pos", &ImGui::PopTextWrapPos);
  m.def("push_allow_keyboard_focus", &ImGui::PushAllowKeyboardFocus);
  m.def("pop_allow_keyboard_focus", &ImGui::PopAllowKeyboardFocus);
  m.def("push_button_repeat", &ImGui::PushButtonRepeat);
  m.def("pop_button_repeat", &ImGui::PopButtonRepeat);

  m.def("separator", &ImGui::Separator);
  m.def("same_line", &ImGui::SameLine, py::arg("local_pos_x") = 0.0f,
        py::arg("spacing_w") = -1.0f);
  m.def("new_line", &ImGui::NewLine);
  m.def("spacing", &ImGui::Spacing);
  m.def("dummy", &ImGui::Dummy);
  m.def("indent", &ImGui::Indent, py::arg("indent_w") = 0.0f);
  m.def("unindent", &ImGui::Unindent, py::arg("indent_w") = 0.0f);
  m.def("begin_group", &ImGui::BeginGroup);
  m.def("end_group", &ImGui::EndGroup);
  m.def("get_cursor_pos", &ImGui::GetCursorPos);
  m.def("get_cursor_pos_x", &ImGui::GetCursorPosX);
  m.def("get_cursor_pos_y", &ImGui::GetCursorPosY);
  m.def("set_cursor_pos", &ImGui::SetCursorPos);
  m.def("set_cursor_pos_x", &ImGui::SetCursorPosX);
  m.def("set_cursor_pos_y", &ImGui::SetCursorPosY);
  m.def("get_cursor_start_pos", &ImGui::GetCursorStartPos);
  m.def("get_cursor_screen_pos", &ImGui::GetCursorScreenPos);
  m.def("set_cursor_screen_pos", &ImGui::SetCursorScreenPos);
  m.def("align_text_to_frame_padding", &ImGui::AlignTextToFramePadding);
  m.def("get_text_line_height", &ImGui::GetTextLineHeight);
  m.def("get_text_line_height_with_spacing",
        &ImGui::GetTextLineHeightWithSpacing);
  m.def("get_frame_height", &ImGui::GetFrameHeight);
  m.def("get_frame_height_with_spacing", &ImGui::GetFrameHeightWithSpacing);

  m.def("columns", &ImGui::Columns, py::arg("count") = 1,
        py::arg("id") = nullptr, py::arg("border") = true);
  m.def("next_column", &ImGui::NextColumn);
  m.def("get_column_index", &ImGui::GetColumnIndex);
  m.def("get_column_offset", &ImGui::GetColumnOffset,
        py::arg("column_index") = -1);
  m.def("set_column_offset", &ImGui::SetColumnOffset, py::arg("column_index"),
        py::arg("offset_x"));
  m.def("get_column_width", &ImGui::GetColumnWidth,
        py::arg("column_index") = -1);
  m.def("set_column_width", &ImGui::SetColumnWidth, py::arg("column_index"),
        py::arg("column_width"));
  m.def("get_columns_count", &ImGui::GetColumnsCount);

  // Tables
  m.def("begin_table", &ImGui::BeginTable, py::arg("str_id"),
        py::arg("columns_count"), py::arg("flags") = ImGuiTableFlags(0),
        py::arg("outer_size") = ImVec2(0, 0), py::arg("inner_width") = 0.0f);
  m.def("end_table", &ImGui::EndTable,
        "only call end_table() if begin_table() returns true!");
  m.def("table_next_row", &ImGui::TableNextRow,
        py::arg("row_flags") = ImGuiTableRowFlags(0),
        py::arg("min_row_height") = 0.0f,
        "append into the first cell of a new row.");
  m.def("table_next_cell", &ImGui::TableNextCell,
        "append into the next column (next column, or next row if currently in "
        "last column). Return true if column is visible.");
  m.def("table_set_column_index", &ImGui::TableSetColumnIndex,
        py::arg("column_n"),
        "append into the specified column. Return true if column is visible.");
  m.def("table_get_column_index", &ImGui::TableGetColumnIndex,
        "return current column index.");
  m.def(
      "table_get_column_name",
      [](int colummn_n) -> std::string_view {
        return std::string_view{ImGui::TableGetColumnName(colummn_n)};
      },
      py::arg("column_n") = -1,
      "return NULL if column didn't have a name declared by "
      "TableSetupColumn(). Pass -1 to use current column.");
  m.def("table_get_column_is_visible", &ImGui::TableGetColumnIsVisible,
        py::arg("column_n") = -1,
        "return true if column is visible. Same value is also returned by "
        "TableNextCell() and TableSetColumnIndex(). Pass -1 to use current "
        "column.");
  m.def("table_get_column_is_sorted", &ImGui::TableGetColumnIsSorted,
        py::arg("column_n") = -1,
        "return true if column is included in the sort specs. Rarely used, can "
        "be useful to tell if a data change should trigger resort. Equivalent "
        "to test ImGuiTableSortSpecs's ->ColumnsMask & (1 << column_n). Pass "
        "-1 to use current column.");
  m.def("table_get_hovered_column", &ImGui::TableGetHoveredColumn,
        "return hovered column. return -1 when table is not hovered. return "
        "columns_count if the unused space at the right of visible columns is "
        "hovered.");
  m.def("table_set_bg_color", &ImGui::TableSetBgColor, py::arg("bg_target"),
        py::arg("color"), py::arg("column_n") = -1,
        "change the color of a cell, row, or column. See ImGuiTableBgTarget_ "
        "flags for details.");
  m.def(
      "table_setup_column", &ImGui::TableSetupColumn, py::arg("label"),
      py::arg("flags") = ImGuiTableColumnFlags(0),
      py::arg("init_width_or_weight") = -1.0f, py::arg("user_id") = 0,
      "Tables: Headers & Columns declaration\n"
      "- Use TableSetupColumn() to specify label, resizing policy, default "
      "width, id, various other flags etc.\n"
      "- The name passed to TableSetupColumn() is used by TableAutoHeaders() "
      "and by the context-menu\n"
      "- Use TableAutoHeaders() to submit the whole header row, otherwise you "
      "may treat the header row as a regular row, manually call TableHeader() "
      "and other widgets.\n"
      "- Headers are required to perform some interactions: reordering, "
      "sorting, context menu (FIXME-TABLE: context menu should work without!)");
  m.def("table_auto_headers", &ImGui::TableAutoHeaders,
        "submit all headers cells based on data provided to TableSetupColumn() "
        "+ submit context menu");
  m.def("table_header", &ImGui::TableHeader, py::arg("label"),
        "submit one header cell manually.");
  // FIXME: map ImGuiTableSortSpecs
  // m.def("table_get_sort_specs", &ImGui::TableGetSortSpecs, "get latest sort
  // specs for the table (NULL if not sorting).");

  m.def("begin_tab_bar", &ImGui::BeginTabBar, py::arg("str_id"),
        py::arg("flags") = 0);
  m.def("end_tab_bar", &ImGui::EndTabBar);
  m.def(
      "begin_tab_item",
      [](const std::string& label, Bool& opened,
         ImGuiTabItemFlags flags) -> bool {
        return ImGui::BeginTabItem(
            label.c_str(), opened.null ? nullptr : &opened.value, flags);
      },
      "create a Tab. Returns true if the Tab is selected.", py::arg("label"),
      py::arg("opened") = null, py::arg("flags") = 0);
  m.def("end_tab_item", &ImGui::EndTabItem);
  m.def("set_tab_item_closed", &ImGui::SetTabItemClosed,
        py::arg("tab_or_docked_window_label"));

  m.def(
      "push_id_str",
      [](const char* str_id_begin, const char* str_id_end) {
        ImGui::PushID(str_id_begin, str_id_end);
      },
      py::arg("str_id_begin"), py::arg("str_id_end") = nullptr);
  m.def("push_id_int", [](int int_id) { ImGui::PushID(int_id); });
  m.def("pop_id", &ImGui::PopID);
  m.def(
      "get_id_str",
      [](const char* str_id_begin, const char* str_id_end) {
        ImGui::GetID(str_id_begin, str_id_end);
      },
      py::arg("str_id_begin"), py::arg("str_id_end") = nullptr);

  m.def("text", [](const char* text) { ImGui::Text("%s", text); });
  m.def("text_colored", [](const ImVec4& col, const char* text) {
    ImGui::TextColored(col, "%s", text);
  });
  m.def("text_disabled",
        [](const char* text) { ImGui::TextDisabled("%s", text); });
  m.def("text_wrapped",
        [](const char* text) { ImGui::TextWrapped("%s", text); });
  m.def("label_text", [](const char* label, const char* text) {
    ImGui::LabelText(label, "%s", text);
  });
  m.def("bullet_text", [](const char* text) { ImGui::BulletText("%s", text); });
  m.def("bullet", &ImGui::Bullet);

  m.def("button", &ImGui::Button, py::arg("label"),
        py::arg("size") = ImVec2(0, 0));
  m.def("small_button", &ImGui::SmallButton);
  m.def("invisible_button", &ImGui::InvisibleButton);
  m.def(
      "tree_node", [](const char* label) { return ImGui::TreeNode(label); },
      py::arg("label"));
  m.def("tree_pop", &ImGui::TreePop);
  // OBSOLETED in 1.71 (from June 2019)
  m.def("set_next_tree_node_open", &ImGui::SetNextTreeNodeOpen,
        py::arg("is_open"), py::arg("cond") = 0);
  m.def("set_next_item_open", &ImGui::SetNextItemOpen, py::arg("is_open"),
        py::arg("cond") = 0);
  m.def(
      "collapsing_header",
      [](const char* label, ImGuiTreeNodeFlags flags) {
        return ImGui::CollapsingHeader(label, flags);
      },
      py::arg("label"), py::arg("flags") = 0);
  m.def("checkbox", [](const char* label, Bool& v) {
    return ImGui::Checkbox(label, &v.value);
  });
  m.def("radio_button", [](const char* label, bool active) {
    return ImGui::RadioButton(label, active);
  });

  m.def("begin_combo", &ImGui::BeginCombo, py::arg("label"),
        py::arg("preview_value"), py::arg("flags") = 0);
  m.def("end_combo", &ImGui::EndCombo,
        "only call EndCombo() if BeginCombo() returns true!");
  m.def("combo", [](const char* label, Int& current_item,
                    const std::vector<std::string>& items) {
    if (items.size() < 10) {
      const char* items_[10];
      for (int i = 0; i < (int)items.size(); ++i) {
        items_[i] = items[i].c_str();
      }
      return ImGui::Combo(label, &current_item.value, items_,
                          (int)items.size());
    } else {
      const char** items_ = new const char*[items.size()];
      for (int i = 0; i < (int)items.size(); ++i) {
        items_[i] = items[i].c_str();
      }
      bool result =
          ImGui::Combo(label, &current_item.value, items_, (int)items.size());
      delete[] items_;
      return result;
    }
  });

  m.def(
      "input_text",
      [](const char* label, String& text, size_t buf_size,
         ImGuiInputTextFlags flags) {
        bool result = false;
        if (buf_size > 255) {
          char* buff = new char[buf_size + 1];
          strncpy(buff, text.value.c_str(), buf_size);
          result = ImGui::InputText(label, buff, buf_size, flags);
          if (result) {
            text.value = buff;
          }
          delete[] buff;
        } else {
          char buff[256];
          strncpy(buff, text.value.c_str(), 255);
          result = ImGui::InputText(label, buff, buf_size, flags);
          if (result) {
            text.value = buff;
          }
        }
        return result;
      },
      py::arg("label"), py::arg("text"), py::arg("buf_size"),
      py::arg("flags") = 0);
  m.def(
      "input_text_multiline",
      [](const char* label, String& text, size_t buf_size, const ImVec2& size,
         ImGuiInputTextFlags flags) {
        bool result = false;
        if (buf_size > 255) {
          char* buff = new char[buf_size + 1];
          strncpy(buff, text.value.c_str(), buf_size);
          result =
              ImGui::InputTextMultiline(label, buff, buf_size, size, flags);
          if (result) {
            text.value = buff;
          }
          delete[] buff;
        } else {
          char buff[256];
          strncpy(buff, text.value.c_str(), 255);
          result =
              ImGui::InputTextMultiline(label, buff, buf_size, size, flags);
          if (result) {
            text.value = buff;
          }
        }
        return result;
      },
      py::arg("label"), py::arg("text"), py::arg("buf_size"),
      py::arg("size") = ImVec2(0, 0), py::arg("flags") = 0);
  m.def(
      "input_text_with_hint",
      [](const char* label, const char* hint, String& text, size_t buf_size,
         ImGuiInputTextFlags flags) {
        bool result = false;
        if (buf_size > 255) {
          char* buff = new char[buf_size];
          strncpy(buff, text.value.c_str(), buf_size);
          result = ImGui::InputTextWithHint(label, hint, buff, buf_size, flags);
          if (result) {
            text.value = buff;
          }
          delete[] buff;
        } else {
          char buff[256];
          strncpy(buff, text.value.c_str(), 256);
          result = ImGui::InputTextWithHint(label, hint, buff, buf_size, flags);
          if (result) {
            text.value = buff;
          }
        }
        return result;
      },
      py::arg("label"), py::arg("hint"), py::arg("text"), py::arg("buf_size"),
      py::arg("flags") = 0);
  m.def(
      "input_float",
      [](const char* label, Float& v, float step, float step_fast,
         const char* display_format, ImGuiInputTextFlags flags) {
        return ImGui::InputFloat(label, &v.value, step, step_fast,
                                 display_format, flags);
      },
      py::arg("label"), py::arg("v"), py::arg("step") = 0.0f,
      py::arg("step_fast") = 0.0f, py::arg("display_format") = "%.3f",
      py::arg("flags") = 0);
  m.def(
      "input_float2",
      [](const char* label, Float& v1, Float& v2, const char* display_format,
         ImGuiInputTextFlags flags) {
        float v[2] = {v1.value, v2.value};
        bool result = ImGui::InputFloat2(label, v, display_format, flags);
        v1.value = v[0];
        v2.value = v[1];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"),
      py::arg("display_format") = "%.3f", py::arg("flags") = 0);
  m.def(
      "input_float3",
      [](const char* label, Float& v1, Float& v2, Float& v3,
         const char* display_format, ImGuiInputTextFlags flags) {
        float v[3] = {v1.value, v2.value, v3.value};
        bool result = ImGui::InputFloat3(label, v, display_format, flags);
        v1.value = v[0];
        v2.value = v[1];
        v3.value = v[2];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v3"),
      py::arg("display_format") = "%.3f", py::arg("flags") = 0);
  m.def(
      "input_float4",
      [](const char* label, Float& v1, Float& v2, Float& v3, Float& v4,
         const char* display_format, ImGuiInputTextFlags flags) {
        float v[4] = {v1.value, v2.value, v3.value, v4.value};
        bool result = ImGui::InputFloat4(label, v, display_format, flags);
        v1.value = v[0];
        v2.value = v[1];
        v3.value = v[2];
        v4.value = v[3];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v3"),
      py::arg("v4"), py::arg("display_format") = "%.3f", py::arg("flags") = 0);
  m.def(
      "input_int",
      [](const char* label, Int& v, int step, int step_fast,
         ImGuiInputTextFlags flags) {
        return ImGui::InputInt(label, &v.value, step, step_fast, flags);
      },
      py::arg("label"), py::arg("v"), py::arg("step") = 1,
      py::arg("step_fast") = 100, py::arg("flags") = 0);
  m.def(
      "input_int2",
      [](const char* label, Int& v1, Int& v2, ImGuiInputTextFlags flags) {
        int v[2] = {v1.value, v2.value};
        bool result = ImGui::InputInt2(label, v, flags);
        v1.value = v[0];
        v2.value = v[1];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("flags") = 0);
  m.def(
      "input_int3",
      [](const char* label, Int& v1, Int& v2, Int& v3,
         ImGuiInputTextFlags flags) {
        int v[3] = {v1.value, v2.value, v3.value};
        bool result = ImGui::InputInt3(label, v, flags);
        v1.value = v[0];
        v2.value = v[1];
        v3.value = v[2];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v3"),
      py::arg("flags") = 0);
  m.def(
      "input_int4",
      [](const char* label, Int& v1, Int& v2, Int& v3, Int& v4,
         ImGuiInputTextFlags flags) {
        int v[4] = {v1.value, v2.value, v3.value, v4.value};
        bool result = ImGui::InputInt4(label, v, flags);
        v1.value = v[0];
        v2.value = v[1];
        v3.value = v[2];
        v4.value = v[3];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v3"),
      py::arg("v4"), py::arg("flags") = 0);

  m.def("color_edit", [](const char* label, ImVec4& col) -> bool {
    return ImGui::ColorEdit4(label, &col.x);
  });
  m.def("color_picker", [](const char* label, ImVec4& col) -> bool {
    return ImGui::ColorPicker4(label, &col.x);
  });

  m.def(
      "drag_float",
      [](const char* label, Float& v, float v_speed, float v_min, float v_max,
         const char* format, ImGuiSliderFlags flags) {
        return ImGui::DragFloat(label, &v.value, v_speed, v_min, v_max, format,
                                flags);
      },
      py::arg("label"), py::arg("v"), py::arg("v_speed") = 1.0f,
      py::arg("v_min") = 0.0f, py::arg("v_max") = 0.0f,
      py::arg("format") = "%.3f", py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "drag_float2",
      [](const char* label, Float& v1, Float& v2, float v_speed, float v_min,
         float v_max, const char* format, ImGuiSliderFlags flags) {
        float v[2] = {v1.value, v2.value};
        bool result =
            ImGui::DragFloat2(label, v, v_speed, v_min, v_max, format, flags);
        v1.value = v[0];
        v2.value = v[1];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v_speed") = 1.0f,
      py::arg("v_min") = 0.0f, py::arg("v_max") = 0.0f,
      py::arg("format") = "%.3f", py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "drag_float3",
      [](const char* label, Float& v1, Float& v2, Float& v3, float v_speed,
         float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
        float v[3] = {v1.value, v2.value, v3.value};
        bool result =
            ImGui::DragFloat3(label, v, v_speed, v_min, v_max, format, flags);
        v1.value = v[0];
        v2.value = v[1];
        v3.value = v[2];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v3"),
      py::arg("v_speed") = 1.0f, py::arg("v_min") = 0.0f,
      py::arg("v_max") = 0.0f, py::arg("format") = "%.3f",
      py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "drag_float4",
      [](const char* label, Float& v1, Float& v2, Float& v3, Float& v4,
         float v_speed, float v_min, float v_max, const char* format,
         ImGuiSliderFlags flags) {
        float v[4] = {v1.value, v2.value, v3.value, v4.value};
        bool result =
            ImGui::DragFloat4(label, v, v_speed, v_min, v_max, format, flags);
        v1.value = v[0];
        v2.value = v[1];
        v3.value = v[2];
        v4.value = v[3];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v3"),
      py::arg("v4"), py::arg("v_speed") = 1.0f, py::arg("v_min") = 0.0f,
      py::arg("v_max") = 0.0f, py::arg("format") = "%.3f",
      py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "drag_float_range2",
      [](const char* label, Float& v_current_min, Float& v_current_max,
         float v_speed, float v_min, float v_max, const char* format,
         const char* format_max, ImGuiSliderFlags flags) {
        return ImGui::DragFloatRange2(label, &v_current_min.value,
                                      &v_current_max.value, v_speed, v_min,
                                      v_max, format, format_max, flags);
      },
      py::arg("label"), py::arg("v_current_min"), py::arg("v_current_max"),
      py::arg("v_speed") = 1.0f, py::arg("v_min") = 0.0f,
      py::arg("v_max") = 0.0f, py::arg("format") = "%.3f",
      py::arg("format_max") = nullptr, py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "drag_int",
      [](const char* label, Int& v, float v_speed, int v_min, int v_max,
         const char* format, ImGuiSliderFlags flags) {
        return ImGui::DragInt(label, &v.value, v_speed, v_min, v_max, format,
                              flags);
      },
      py::arg("label"), py::arg("v"), py::arg("v_speed") = 1.0f,
      py::arg("v_min") = 0.0f, py::arg("v_max") = 0.0f,
      py::arg("format") = "%d", py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "drag_int2",
      [](const char* label, Int& v1, Int& v2, float v_speed, int v_min,
         int v_max, const char* format, ImGuiSliderFlags flags) {
        int v[2] = {v1.value, v2.value};
        bool result =
            ImGui::DragInt2(label, v, v_speed, v_min, v_max, format, flags);
        v1.value = v[0];
        v2.value = v[1];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v_speed") = 1.0f,
      py::arg("v_min") = 0.0f, py::arg("v_max") = 0.0f,
      py::arg("format") = "%d", py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "drag_int3",
      [](const char* label, Int& v1, Int& v2, Int& v3, float v_speed, int v_min,
         int v_max, const char* format, ImGuiSliderFlags flags) {
        int v[3] = {v1.value, v2.value, v3.value};
        bool result =
            ImGui::DragInt3(label, v, v_speed, v_min, v_max, format, flags);
        v1.value = v[0];
        v2.value = v[1];
        v3.value = v[2];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v3"),
      py::arg("v_speed") = 1.0f, py::arg("v_min") = 0.0f,
      py::arg("v_max") = 0.0f, py::arg("format") = "%d",
      py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "drag_int4",
      [](const char* label, Int& v1, Int& v2, Int& v3, Int& v4, float v_speed,
         int v_min, int v_max, const char* format, ImGuiSliderFlags flags) {
        int v[4] = {v1.value, v2.value, v3.value, v4.value};
        bool result =
            ImGui::DragInt4(label, v, v_speed, v_min, v_max, format, flags);
        v1.value = v[0];
        v2.value = v[1];
        v3.value = v[2];
        v4.value = v[3];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v3"),
      py::arg("v4"), py::arg("v_speed") = 1.0f, py::arg("v_min") = 0.0f,
      py::arg("v_max") = 0.0f, py::arg("format") = "%d",
      py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "drag_int_range2",
      [](const char* label, Int& v_current_min, Int& v_current_max,
         float v_speed, float v_min, float v_max, const char* format,
         const char* format_max, ImGuiSliderFlags flags) {
        return ImGui::DragIntRange2(label, &v_current_min.value,
                                    &v_current_max.value, v_speed, v_min, v_max,
                                    format, format_max, flags);
      },
      py::arg("label"), py::arg("v_current_min"), py::arg("v_current_max"),
      py::arg("v_speed") = 1.0f, py::arg("v_min") = 0.0f,
      py::arg("v_max") = 0.0f, py::arg("format") = "%.3f",
      py::arg("format_max") = nullptr, py::arg("flags") = ImGuiSliderFlags(0));

  m.def(
      "slider_float",
      [](const char* label, Float& v, float v_min, float v_max,
         const char* format, ImGuiSliderFlags flags) {
        return ImGui::SliderFloat(label, &v.value, v_min, v_max, format, flags);
      },
      py::arg("label"), py::arg("v"), py::arg("v_min"), py::arg("v_max"),
      py::arg("format") = "%.3f", py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "slider_float2",
      [](const char* label, Float& v1, Float& v2, float v_min, float v_max,
         const char* format, ImGuiSliderFlags flags) {
        float v[2] = {v1.value, v2.value};
        bool result =
            ImGui::SliderFloat2(label, v, v_min, v_max, format, flags);
        v1.value = v[0];
        v2.value = v[1];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v_min"),
      py::arg("v_max"), py::arg("format") = "%.3f",
      py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "slider_float3",
      [](const char* label, Float& v1, Float& v2, Float& v3, float v_min,
         float v_max, const char* format, ImGuiSliderFlags flags) {
        float v[3] = {v1.value, v2.value, v3.value};
        bool result =
            ImGui::SliderFloat3(label, v, v_min, v_max, format, flags);
        v1.value = v[0];
        v2.value = v[1];
        v3.value = v[2];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v3"),
      py::arg("v_min"), py::arg("v_max"), py::arg("format") = "%.3f",
      py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "slider_float4",
      [](const char* label, Float& v1, Float& v2, Float& v3, Float& v4,
         float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
        float v[4] = {v1.value, v2.value, v3.value, v4.value};
        bool result =
            ImGui::SliderFloat4(label, v, v_min, v_max, format, flags);
        v1.value = v[0];
        v2.value = v[1];
        v3.value = v[2];
        v4.value = v[3];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v3"),
      py::arg("v4"), py::arg("v_min"), py::arg("v_max"),
      py::arg("format") = "%.3f", py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "slider_angle",
      [](const char* label, Float& v_rad, float v_degrees_min,
         float v_degrees_max, const char* format, ImGuiSliderFlags flags) {
        return ImGui::SliderAngle(label, &v_rad.value, v_degrees_min,
                                  v_degrees_max, format, flags);
      },
      py::arg("label"), py::arg("v_rad"), py::arg("v_degrees_min") = -360.0f,
      py::arg("v_degrees_max") = +360.0f, py::arg("format") = "%.0f deg",
      py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "slider_int",
      [](const char* label, Int& v, int v_min, int v_max, const char* format,
         ImGuiSliderFlags flags) {
        return ImGui::SliderInt(label, &v.value, v_min, v_max, format, flags);
      },
      py::arg("label"), py::arg("v"), py::arg("v_min"), py::arg("v_max"),
      py::arg("format") = "%d", py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "slider_int2",
      [](const char* label, Int& v1, Int& v2, int v_min, int v_max,
         const char* format, ImGuiSliderFlags flags) {
        int v[2] = {v1.value, v2.value};
        bool result = ImGui::SliderInt2(label, v, v_min, v_max, format, flags);
        v1.value = v[0];
        v2.value = v[1];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v_min"),
      py::arg("v_max"), py::arg("format") = "%d",
      py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "slider_int3",
      [](const char* label, Int& v1, Int& v2, Int& v3, int v_min, int v_max,
         const char* format, ImGuiSliderFlags flags) {
        int v[3] = {v1.value, v2.value, v3.value};
        bool result = ImGui::SliderInt3(label, v, v_min, v_max, format, flags);
        v1.value = v[0];
        v2.value = v[1];
        v3.value = v[2];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v3"),
      py::arg("v_min"), py::arg("v_max"), py::arg("format") = "%d",
      py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "slider_int4",
      [](const char* label, Int& v1, Int& v2, Int& v3, Int& v4, int v_min,
         int v_max, const char* format, ImGuiSliderFlags flags) {
        int v[4] = {v1.value, v2.value, v3.value, v4.value};
        bool result = ImGui::SliderInt4(label, v, v_min, v_max, format, flags);
        v1.value = v[0];
        v2.value = v[1];
        v3.value = v[2];
        v4.value = v[3];
        return result;
      },
      py::arg("label"), py::arg("v1"), py::arg("v2"), py::arg("v3"),
      py::arg("v4"), py::arg("v_min"), py::arg("v_max"),
      py::arg("format") = "%d", py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "v_slider_float",
      [](const char* label, const ImVec2& size, Float& v, float v_min,
         float v_max, const char* format, ImGuiSliderFlags flags) {
        return ImGui::VSliderFloat(label, size, &v.value, v_min, v_max, format,
                                   flags);
      },
      py::arg("label"), py::arg("size"), py::arg("v"), py::arg("v_min"),
      py::arg("v_max"), py::arg("format") = "%.3f",
      py::arg("flags") = ImGuiSliderFlags(0));
  m.def(
      "v_slider_int",
      [](const char* label, const ImVec2& size, Int& v, int v_min, int v_max,
         const char* format, ImGuiSliderFlags flags) {
        return ImGui::VSliderInt(label, size, &v.value, v_min, v_max, format,
                                 flags);
      },
      py::arg("label"), py::arg("size"), py::arg("v"), py::arg("v_min"),
      py::arg("v_max"), py::arg("format") = "%d",
      py::arg("flags") = ImGuiSliderFlags(0));

  m.def(
      "plot_lines",
      [](const char* label, const std::vector<float>& values,
         int values_offset = 0, const char* overlay_text = NULL,
         float scale_min = FLT_MAX, float scale_max = FLT_MAX,
         ImVec2 graph_size = ImVec2(0, 0), int stride = sizeof(float)) {
        ImGui::PlotLines(label, values.data(), (int)values.size(),
                         values_offset, overlay_text, scale_min, scale_max,
                         graph_size, stride);
      },
      py::arg("label"), py::arg("values"), py::arg("values_offset") = 0,
      py::arg("overlay_text") = nullptr, py::arg("scale_min") = FLT_MAX,
      py::arg("scale_max") = FLT_MAX, py::arg("graph_size") = ImVec2(0, 0),
      py::arg("stride") = sizeof(float));

  m.def("progress_bar", &ImGui::ProgressBar, py::arg("fraction"),
        py::arg("size_arg") = ImVec2(-1, 0), py::arg("overlay") = nullptr);

  m.def("color_button", &ImGui::ColorButton, py::arg("desc_id"), py::arg("col"),
        py::arg("flags") = 0, py::arg("size") = ImVec2(0, 0),
        "display a colored square/button, hover for details, return true when "
        "pressed.");

  m.def(
      "selectable",
      [](std::string label, bool selected = false,
         ImGuiSelectableFlags flags = 0, ImVec2 size = ImVec2(0, 0)) -> bool {
        return ImGui::Selectable(label.c_str(), selected, flags, size);
      },
      py::arg("label"), py::arg("selected") = false, py::arg("flags") = 0,
      py::arg("size") = ImVec2(0, 0));

  m.def(
      "selectable",
      [](std::string label, Bool& selected, ImGuiSelectableFlags flags = 0,
         ImVec2 size = ImVec2(0, 0)) -> bool {
        return ImGui::Selectable(label.c_str(), &selected.value, flags, size);
      },
      py::arg("label"), py::arg("selected"), py::arg("flags") = 0,
      py::arg("size") = ImVec2(0, 0));

  m.def(
      "list_box_header",
      [](std::string label, ImVec2 size = ImVec2(0, 0)) {
        ImGui::ListBoxHeader(label.c_str(), size);
      },
      py::arg("label"), py::arg("size"));

  m.def("list_box_footer", &ImGui::ListBoxFooter);

  m.def("set_item_default_focus", &ImGui::SetItemDefaultFocus);
  m.def("set_keyboard_focus_here", &ImGui::SetKeyboardFocusHere);

  m.def("is_item_hovered", &ImGui::IsItemHovered, py::arg("flags") = 0);
  m.def("is_item_active", &ImGui::IsItemActive);
  m.def("is_item_focused", &ImGui::IsItemFocused);
  m.def("is_item_clicked", &ImGui::IsItemClicked);
  m.def("is_item_visible", &ImGui::IsItemVisible);
  m.def("is_item_edited", &ImGui::IsItemEdited);
  m.def("is_item_activated", &ImGui::IsItemActivated);
  m.def("is_item_deactivated", &ImGui::IsItemDeactivated);
  m.def("is_item_deactivated_after_edit", &ImGui::IsItemDeactivatedAfterEdit);
  m.def("is_item_toggled_open", &ImGui::IsItemToggledOpen);
  m.def("is_any_item_hovered", &ImGui::IsAnyItemHovered);
  m.def("is_any_item_active", &ImGui::IsAnyItemActive);
  m.def("is_any_item_focused", &ImGui::IsAnyItemFocused);

  m.def("get_item_rect_min", &ImGui::GetItemRectMin);
  m.def("get_item_rect_max", &ImGui::GetItemRectMax);
  m.def("get_item_rect_size", &ImGui::GetItemRectSize);

  m.def("set_item_allow_overlap", &ImGui::SetItemAllowOverlap);

  m.def("get_time", &ImGui::GetTime);
  m.def("get_frame_count", &ImGui::GetFrameCount);

  m.def("get_key_index", &ImGui::GetKeyIndex);
  m.def("is_key_down", &ImGui::IsKeyDown);
  m.def("is_key_pressed", &ImGui::IsKeyPressed);
  m.def("is_key_released", &ImGui::IsKeyReleased);
  m.def("get_key_pressed_amount", &ImGui::GetKeyPressedAmount);
  m.def("is_mouse_down", &ImGui::IsMouseDown);
  m.def("is_any_mouse_down", &ImGui::IsAnyMouseDown);
  m.def("is_mouse_clicked", &ImGui::IsMouseClicked);
  m.def("is_mouse_double_clicked", &ImGui::IsMouseDoubleClicked);
  m.def("is_mouse_released", &ImGui::IsMouseReleased);
  m.def("is_mouse_dragging", &ImGui::IsMouseDragging);
  m.def("is_mouse_hovering_rect", &ImGui::IsMouseHoveringRect);
  m.def("is_mouse_pos_valid", &ImGui::IsMousePosValid);
  m.def("get_mouse_pos", &ImGui::GetMousePos);
  m.def("get_mouse_pos_on_opening_current_popup",
        &ImGui::GetMousePosOnOpeningCurrentPopup);
  m.def("get_mouse_drag_delta", &ImGui::GetMouseDragDelta);
  m.def("reset_mouse_drag_delta", &ImGui::ResetMouseDragDelta);

  m.def("capture_keyboard_from_app", &ImGui::CaptureKeyboardFromApp);
  m.def("capture_mouse_from_app", &ImGui::CaptureMouseFromApp);

  py::enum_<ImGuiDragDropFlags_>(m, "DragDropFlags")
      .value("SourceNoPreviewTooltip",
             ImGuiDragDropFlags_SourceNoPreviewTooltip)
      .value("SourceNoDisableHover", ImGuiDragDropFlags_SourceNoDisableHover)
      .value("SourceNoHoldToOpenOthers",
             ImGuiDragDropFlags_SourceNoHoldToOpenOthers)
      .value("SourceAllowNullID", ImGuiDragDropFlags_SourceAllowNullID)
      .value("SourceExtern", ImGuiDragDropFlags_SourceExtern)
      .value("SourceAutoExpirePayload",
             ImGuiDragDropFlags_SourceAutoExpirePayload)

      .value("AcceptBeforeDelivery", ImGuiDragDropFlags_AcceptBeforeDelivery)
      .value("AcceptNoDrawDefaultRect",
             ImGuiDragDropFlags_AcceptNoDrawDefaultRect)
      .value("AcceptNoPreviewTooltip",
             ImGuiDragDropFlags_AcceptNoPreviewTooltip)
      .value("AcceptPeekOnly", ImGuiDragDropFlags_AcceptPeekOnly)

      .export_values();

  m.def("begin_drag_drop_source", &ImGui::BeginDragDropSource);
  // todo:
  // m.def("set_drag_drop_payload", &ImGui::SetDragDropPayload);
  m.def("set_drag_drop_payload_string", [](std::string data) {
    ImGui::SetDragDropPayload("string", data.c_str(), data.size());
  });
  m.def("end_drag_drop_source", &ImGui::EndDragDropSource);
  m.def("begin_drag_drop_target", &ImGui::BeginDragDropTarget);
  // todo:
  // m.def("accept_drag_drop_payload", &ImGui::AcceptDragDropPayload);
  m.def("accept_drag_drop_payload_string_preview",
        [](ImGuiDragDropFlags flags = 0) -> std::string {
          auto payload = ImGui::AcceptDragDropPayload("string", flags);
          if (!payload->IsDataType("string") || !payload->Data)
            return "";
          if (payload->IsPreview())
            return std::string(static_cast<char*>(payload->Data),
                               payload->DataSize);
          else
            return "";
        });
  m.def("accept_drag_drop_payload_string",
        [](ImGuiDragDropFlags flags = 0) -> std::string {
          auto payload = ImGui::AcceptDragDropPayload("string", flags);
          if (!payload->IsDataType("string") || !payload->Data)
            return "";
          if (payload->IsDelivery())
            return std::string(static_cast<char*>(payload->Data),
                               payload->DataSize);
          else
            return "";
        });

  m.def("end_drag_drop_target", &ImGui::EndDragDropTarget);
  m.def("get_drag_drop_payload", []() -> std::string {
    auto payload = ImGui::GetDragDropPayload();
    if (!payload->IsDataType("string") || !payload->Data)
      return "";
    if (payload->IsDelivery())
      return std::string(static_cast<char*>(payload->Data), payload->DataSize);
    else
      return "";
  });

  m.def("push_clip_rect", &ImGui::PushClipRect);
  m.def("pop_clip_rect", &ImGui::PopClipRect);

  m.def(
      "add_font_from_file_ttf",
      [](std::string filename, float size_pixels = 32.0f) {
        return ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(),
                                                        size_pixels);
      },
      py::arg("filename"), py::arg("size_pixels"),
      py::return_value_policy::reference);

  m.def("push_font", &ImGui::PushFont);
  m.def("pop_font", &ImGui::PopFont);
  m.def("get_font", &ImGui::GetFont);

  py::class_<ImFont>(m, "Font").def(py::init());

  m.def(
      "set_display_framebuffer_scale",
      [](float scale) {
        ImGui::GetIO().DisplayFramebufferScale = ImVec2(scale, scale);
      },
      py::arg("scale"));
  m.def("get_display_framebuffer_scale",
        []() { return ImGui::GetIO().DisplayFramebufferScale; });

  m.def(
      "set_font_global_scale",
      [](float scale) { ImGui::GetIO().FontGlobalScale = scale; },
      py::arg("scale"));
  m.def("get_font_global_scale",
        []() { return ImGui::GetIO().FontGlobalScale; });
}
