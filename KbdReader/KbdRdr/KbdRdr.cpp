#include "KbdRdr.h"
#include "..\Common\KernelUserCommon.h"
#include "..\KnCommLib\KnCommLib.h"
#include "..\KnKbdLib\KnKbdLib.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ULONG	g_myKnCommId = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EXTERN_C
NTSTATUS
DriverEntry(
	IN	PDRIVER_OBJECT	pDriverObject,
	IN	PUNICODE_STRING	pRegistryPath
	)
{
	UNREFERENCED_PARAMETER(pRegistryPath);

	NTSTATUS status = STATUS_UNSUCCESSFUL;

	do
	{
		//
		// KnComm Lib �ʱ�ȭ
		//

		if (InitializeKnCommLib(KBDRDR_DRIVER_NAME) == FALSE)
		{
			break;
		}


		//
		// KNCOMM Ŭ���̾�Ʈ ����
		//

		KNCOMM_CB_INFO knCommCbinfo;
		knCommCbinfo.pfnOnUserConnect = OnUserConnect;
		knCommCbinfo.pfnOnUserDisconnect = OnUserDisconnect;
		knCommCbinfo.pfnOnUserIoctl = NULL;
		knCommCbinfo.pCallbackContext = NULL;

		if (CreateKnCommClient(&knCommCbinfo, &g_myKnCommId) == FALSE)
		{
			break;
		}


		//
		// Ű���� Lib �ʱ�ȭ
		//

		if (InitializeKnKbdLib() == FALSE)
		{
			break;
		}


		//
		// ����
		//

		pDriverObject->DriverUnload = UnloadRoutine;

		status = STATUS_SUCCESS;

	} while (FALSE);

	if (!NT_SUCCESS(status))
	{
		FinalizeKnKbdLib();
		FinalizeKnCommLib();
	}

	return status;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
UnloadRoutine(
	IN	PDRIVER_OBJECT	pDriverObject
	)
{
	UNREFERENCED_PARAMETER(pDriverObject);

	
	//
	// Ű���� �б⸦ ���� �� Finalize�Ѵ�.
	//

	KnKbdStopRead();
	FinalizeKnKbdLib();


	//
	// KnComm Ŭ���̾�Ʈ�� ���� �� Finalize�Ѵ�.
	//

	DeleteKnCommClient(g_myKnCommId);
	FinalizeKnCommLib();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
NTAPI
OnKeyboardReadCompletion(
	IN	PKNKBD_READDATA			pKnKbdInputData,
	IN	PVOID					pContext
	)
{
	UNREFERENCED_PARAMETER(pContext);


	//
	// Ű �̺�Ʈ�� �߻��ߴ�.
	// ���� ��� ���� ����������.
	//

	DbgPrint("[%s:%d] SC : %02X Flags : 0x%X SessionID : %d\n", _FN_, _LN_, pKnKbdInputData->makeCode, pKnKbdInputData->flags, pKnKbdInputData->sessionId);

	KEYEVENT_DATA keyEventData;
	keyEventData.makeCode = pKnKbdInputData->makeCode;
	keyEventData.flags = pKnKbdInputData->flags;
	
	SendToUserViaKnComm(g_myKnCommId, &keyEventData, sizeof(keyEventData), FALSE, 0, NULL, 0);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
NTAPI
OnUserConnect(
	IN	HANDLE			processId,
	IN	PVOID			pContext
	)
{
	UNREFERENCED_PARAMETER(processId);
	UNREFERENCED_PARAMETER(pContext);


	//
	// ���� ��� ����� �����ߴ�.
	// Ű���� �б⸦ �����Ѵ�.
	//

	KnKbdStartRead(OnKeyboardReadCompletion, NULL);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
NTAPI
OnUserDisconnect(
	IN	HANDLE			processId,
	IN	PVOID			pContext
	)
{
	UNREFERENCED_PARAMETER(processId);
	UNREFERENCED_PARAMETER(pContext);


	//
	// ���� ��� ����� ������ ������.
	// Ű���� �б⸦ �����Ѵ�.
	//

	KnKbdStopRead();
}

