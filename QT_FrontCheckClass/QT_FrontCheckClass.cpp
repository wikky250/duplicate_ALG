
#pragma once
#include "QT_FrontCheckClass.h"
#include "OutShowCheck.h"
extern "C" QT_FrontCheckClass_EXPORT CBaseCheckAlg* APIENTRY CreateExportObj(bool b_init)
{
	return new COutShowCheck(b_init);
}
//这里不能直接delete pExport，因为没有把CBaseCheckAlg的析构函数定义为虚函数
extern "C" QT_FrontCheckClass_EXPORT void  APIENTRY DestroyExportObj(CBaseCheckAlg* pExport)
{
	pExport->Release();
}
