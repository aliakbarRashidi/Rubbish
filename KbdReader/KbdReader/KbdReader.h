
// KbdReader.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CKbdReaderApp:
// �� Ŭ������ ������ ���ؼ��� KbdReader.cpp�� �����Ͻʽÿ�.
//

class CKbdReaderApp : public CWinApp
{
public:
	CKbdReaderApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CKbdReaderApp theApp;