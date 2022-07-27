module;

#include <windows.h>
#include <gdiplus.h>

export module gdiplusinitialize;

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

export class gdiplusinitialize
{
public:
   gdiplusinitialize();
   ~gdiplusinitialize();

private:
   GdiplusStartupInput _gdiplusStartupInput;

   ULONG_PTR           _gdiplusToken;
};

gdiplusinitialize::gdiplusinitialize()
{
   // Initialize GDI+.
   GdiplusStartup(&_gdiplusToken, &_gdiplusStartupInput, NULL);
}

gdiplusinitialize::~gdiplusinitialize()
{
   GdiplusShutdown(_gdiplusToken);
}