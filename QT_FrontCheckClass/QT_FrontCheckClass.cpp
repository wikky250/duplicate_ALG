
#pragma once
#include "QT_FrontCheckClass.h"
#include "OutShowCheck.h"
extern "C" QT_FrontCheckClass_EXPORT CBaseCheckAlg* APIENTRY CreateExportObj(bool b_init)
{
	return new COutShowCheck(b_init);
}
//���ﲻ��ֱ��delete pExport����Ϊû�а�CBaseCheckAlg��������������Ϊ�麯��
extern "C" QT_FrontCheckClass_EXPORT void  APIENTRY DestroyExportObj(CBaseCheckAlg* pExport)
{
	pExport->Release();
}
