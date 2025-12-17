#include "pch.h"
#include "WinAPIUtil.h"

#include <commdlg.h>

bool CppLogic::WinAPI::FileDialog::Show()
{
    OPENFILENAME op = {};

    op.lStructSize = sizeof(OPENFILENAME);
    SelectedPath.resize(MAX_PATH);
    op.lpstrFile = SelectedPath.data();
    op.nMaxFile = SelectedPath.size();
    op.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    op.lpstrFilter = Filter;
    op.lpstrTitle = Title;
    return GetOpenFileName(&op);
}
