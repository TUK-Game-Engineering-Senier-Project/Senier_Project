// ----- 클라이언트 cpp 파일 -----

#include "Stdafx.h" // 사용자 정의 헤더

using namespace std;

// ----- WinMain -----

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
    PSTR cmdLine, int showCmd)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    try
    {
        BoxApp theApp(hInstance);
        if (!theApp.Initialize())
            return 0;

        // 프로그램 실행
        return theApp.Run();
    }
    catch (DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }
}