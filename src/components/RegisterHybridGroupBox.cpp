#include <vcl.h>
#pragma hdrstop
#include "HybridGroupBox.h"
#pragma package(smart_init)

namespace Hybridgroupbox
{
    void __fastcall PACKAGE Register()
    {
        TComponentClass classes[1] = {__classid(THybridGroupBox)};
        RegisterComponents(L"Custom Controls", classes, 0);
    }
}
