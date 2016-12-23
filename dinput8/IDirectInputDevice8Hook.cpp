#include"stdafx.h"
#include "IDirectInputDevice8Hook.h"

static bool mSpaceKeyState = false;
static bool mLastSpaceKeyState = false;

static GUID GUID_GamepadXboxOneWired = {
	0x02FF045E, 0x0000, 0x0000,{ 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 }
};

static GUID GUID_GamepadXbox360WirelessSteam = {
	0x11FF28DE, 0x28DE, 0x0001,{ 0x00, 0x00, 0x50, 0x49, 0x44, 0x56, 0x49, 0x44 }
};

static GUID GUID_GamepadDragonRiseTwinShock = {
	0x9C3F48C0, 0xAF01, 0x11E6,{ 0x80, 0x01, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 }
};

static GUID GUID_GamepadNVidiaShieldGameStream = {
	0xECBB3D3D, 0xC2EA, 0x4861,{ 0x98, 0x3F, 0xB3, 0xE1, 0x5B, 0xDC, 0x6C, 0x52 }
};

// TODO Add your own known gamepad GUIDs here!

IDirectInputDevice8Hook::IDirectInputDevice8Hook(IDirectInput8 * dinput, IDirectInputDevice8 * dinputdevice, REFGUID guid)
{
	m_pDI = dinput;
	m_pDIDevice = dinputdevice;
	m_GUID = guid;
}

/*** IUnknown methods ***/
HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
	return m_pDIDevice->QueryInterface(riid, ppvObj);
}

ULONG STDMETHODCALLTYPE IDirectInputDevice8Hook::AddRef()
{
	return m_pDIDevice->AddRef();
}

ULONG STDMETHODCALLTYPE IDirectInputDevice8Hook::Release()
{
	ULONG uRet = m_pDIDevice->Release();

	if (uRet == 0)
		// If the reference count is 0 delete ourselves
		delete this;

	return uRet;
}

/*** IDirectInputDevice8A methods ***/
HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetCapabilities(LPDIDEVCAPS p0)
{
	return m_pDIDevice->GetCapabilities(p0);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::EnumObjects(LPDIENUMDEVICEOBJECTSCALLBACKW p0, LPVOID p1, DWORD p2)
{
	return m_pDIDevice->EnumObjects(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetProperty(REFGUID rguid, LPDIPROPHEADER p1)
{
	return m_pDIDevice->GetProperty(rguid, p1);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::SetProperty(REFGUID rguid, LPCDIPROPHEADER p1)
{
	return m_pDIDevice->SetProperty(rguid, p1);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::Acquire()
{
	return m_pDIDevice->Acquire();
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::Unacquire()
{
	return m_pDIDevice->Unacquire();
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetDeviceState(DWORD p0, LPVOID p1)
{
	// Forward call
	HRESULT hResult = m_pDIDevice->GetDeviceState(p0, p1);

	// We've acquired gamepad data
	if (SUCCEEDED(hResult) && (p0 == sizeof(DIJOYSTATE) || p0 == sizeof(DIJOYSTATE2)))
	{
		// Cast device state
		LPDIJOYSTATE state = (LPDIJOYSTATE)p1;

		// Log device state (uncomment these lines if you need to capture the button values before remapping them)
		WCHAR message[2048];
		wsprintf(message,
			L"DIJOYSTATE:\n"
			L"lRx = %u, lRy = %u, lRz = %u\n"
			L"lX = %u, lY = %u, lZ = %u\n"
			L"rgdwPOV[0] = %u, rgdwPOV[1] = %u, rgdwPOV[2] = %u, rgdwPOV[3] = %u\n"
			L"rglSlider[0] = %u, rglSlider[1] = %u\n"
			L"rgbButtons[0] = %u, rgbButtons[1] = %u, rgbButtons[2] = %u, rgbButtons[3] = %u\n"
			L"rgbButtons[4] = %u, rgbButtons[5] = %u, rgbButtons[6] = %u, rgbButtons[7] = %u\n"
			L"rgbButtons[8] = %u, rgbButtons[9] = %u, rgbButtons[10] = %u, rgbButtons[11] = %u\n"
			L"rgbButtons[12] = %u, rgbButtons[13] = %u, rgbButtons[14] = %u, rgbButtons[15] = %u\n"
			L"rgbButtons[16] = %u, rgbButtons[17] = %u, rgbButtons[18] = %u, rgbButtons[19] = %u\n"
			L"rgbButtons[20] = %u, rgbButtons[21] = %u, rgbButtons[22] = %u, rgbButtons[23] = %u\n"
			L"rgbButtons[24] = %u, rgbButtons[25] = %u, rgbButtons[26] = %u, rgbButtons[27] = %u\n"
			L"rgbButtons[28] = %u, rgbButtons[29] = %u, rgbButtons[30] = %u, rgbButtons[31] = %u\n",
			state->lRx, state->lRy, state->lRz,
			state->lX, state->lY, state->lZ,
			state->rgdwPOV[0], state->rgdwPOV[1], state->rgdwPOV[2], state->rgbButtons[3],
			state->rglSlider[0], state->rglSlider[1],
			state->rgbButtons[0], state->rgbButtons[1], state->rgbButtons[2], state->rgbButtons[3],
			state->rgbButtons[4], state->rgbButtons[5], state->rgbButtons[6], state->rgbButtons[7],
			state->rgbButtons[8], state->rgbButtons[9], state->rgbButtons[10], state->rgbButtons[11],
			state->rgbButtons[12], state->rgbButtons[13], state->rgbButtons[14], state->rgbButtons[15],
			state->rgbButtons[16], state->rgbButtons[17], state->rgbButtons[18], state->rgbButtons[19],
			state->rgbButtons[20], state->rgbButtons[21], state->rgbButtons[22], state->rgbButtons[23],
			state->rgbButtons[24], state->rgbButtons[25], state->rgbButtons[26], state->rgbButtons[26],
			state->rgbButtons[28], state->rgbButtons[29], state->rgbButtons[30], state->rgbButtons[31]);
		OutputDebugString(message);

		// Log gamepad GUID (uncomment these lines if you need to capture your gamepad GUID)
		wsprintf(message, L"Gamepad GUID: {%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}\n", m_GUID.Data1, m_GUID.Data2, m_GUID.Data3, m_GUID.Data4[0], m_GUID.Data4[1], m_GUID.Data4[2], m_GUID.Data4[3], m_GUID.Data4[4], m_GUID.Data4[5], m_GUID.Data4[6], m_GUID.Data4[7]);
		OutputDebugString(message);

		// Fix DragonRise controls
		if (IsEqualGUID(m_GUID, GUID_GamepadDragonRiseTwinShock))
		{
			// Temporary byte variable
			BYTE tmp[2];

			// Square from 3 to 0
			tmp[0] = state->rgbButtons[0];
			state->rgbButtons[0] = state->rgbButtons[3];

			// Triangle from 0 to 1
			tmp[1] = state->rgbButtons[1];
			state->rgbButtons[1] = tmp[0];

			// Circle from 1 to 3
			state->rgbButtons[3] = tmp[1];

			// Swap start (9) and R3 (11)
			state->rgbButtons[11] = state->rgbButtons[11] ^ state->rgbButtons[9];
			state->rgbButtons[9] = state->rgbButtons[11] ^ state->rgbButtons[9];
			state->rgbButtons[11] = state->rgbButtons[11] ^ state->rgbButtons[9];

			// Swap select (8) and L3 (10)
			state->rgbButtons[10] = state->rgbButtons[10] ^ state->rgbButtons[8];
			state->rgbButtons[8] = state->rgbButtons[10] ^ state->rgbButtons[8];
			state->rgbButtons[10] = state->rgbButtons[10] ^ state->rgbButtons[8];

			// Swap L1 (6) and L2 (4)
			state->rgbButtons[4] = state->rgbButtons[4] ^ state->rgbButtons[6];
			state->rgbButtons[6] = state->rgbButtons[4] ^ state->rgbButtons[6];
			state->rgbButtons[4] = state->rgbButtons[4] ^ state->rgbButtons[6];

			// Swap R1 (7) and R2 (5)
			state->rgbButtons[5] = state->rgbButtons[5] ^ state->rgbButtons[7];
			state->rgbButtons[7] = state->rgbButtons[5] ^ state->rgbButtons[7];
			state->rgbButtons[5] = state->rgbButtons[5] ^ state->rgbButtons[7];
		}

		// Fix Xbox360 / XBoxOne controls (Steam In-Home Streaming and NVidia GameStream both simulate different versions of this controller!)
		else if (IsEqualGUID(m_GUID, GUID_GamepadXbox360WirelessSteam) || IsEqualGUID(m_GUID, GUID_GamepadNVidiaShieldGameStream) ||IsEqualGUID(m_GUID, GUID_GamepadXboxOneWired))
		{
			// Swap A and X button presses to match the expected button number
			state->rgbButtons[0] = state->rgbButtons[0] ^ state->rgbButtons[2];
			state->rgbButtons[2] = state->rgbButtons[0] ^ state->rgbButtons[2];
			state->rgbButtons[0] = state->rgbButtons[0] ^ state->rgbButtons[2];

			// Swap B and Y button presses to match the expected button number
			state->rgbButtons[1] = state->rgbButtons[1] ^ state->rgbButtons[3];
			state->rgbButtons[3] = state->rgbButtons[1] ^ state->rgbButtons[3];
			state->rgbButtons[1] = state->rgbButtons[1] ^ state->rgbButtons[3];

			// Translate back button presses
			state->rgbButtons[10] = state->rgbButtons[6];

			// Translate start button presses
			state->rgbButtons[11] = state->rgbButtons[7];

			// Translate L2 trigger presses
			state->rgbButtons[6] = state->lZ >= 0 && state->lZ <= 126 ? 128 : 0;

			// Translate R2 trigger presses
			state->rgbButtons[7] = state->lZ >= -128 && state->lZ <= -2 ? 128 : 0;

			// Translate horizontal axis of the right analog stick
			state->lZ = state->lRx;

			// Translate vertical axis of the right analog stick
			state->lRz = state->lRy;
		}

		/*
		// TODO This is where you would add additional controller mappings!
		else if (IsEqualGUID(m_GUID, GUID_GamepadYourNewShinyGamepadThatDoesntWorkRightYet))
		{
			// Remap the buttons here!
		}
		*/

		// Hold the space button for as long as the start button is pressed (this game has no way of skipping through cutscenes via gamepad, it's ridiculous)
		mSpaceKeyState = state->rgbButtons[11] != 0;
	}

	return hResult;
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetDeviceData(DWORD p0, LPDIDEVICEOBJECTDATA p1, LPDWORD p2, DWORD p3)
{
	// Forward call
	HRESULT hResult = m_pDIDevice->GetDeviceData(p0, p1, p2, p3);

	// We've acquired keyboard data
	if (SUCCEEDED(hResult) && IsEqualGUID(m_GUID, GUID_SysKeyboard) && p1 != NULL && p0 == sizeof(DIDEVICEOBJECTDATA) && p2 != NULL)
	{
		// Virtual space key state has been changed
		if (mSpaceKeyState != mLastSpaceKeyState)
		{
			// Keep track of data
			//OutputDebugString(L"Virtual space key state has changed\n");

			// Initialize new direct input device data object
			memset(&p1[*p2], 0, sizeof(DIDEVICEOBJECTDATA));

			// Set key state accordingly
			p1[*p2].dwData = mSpaceKeyState ? 0x80 : 0;

			// Set key to space
			p1[*p2].dwOfs = DIK_SPACE;

			// Hope nothing goes up in flames because we didn't set the sequence number or app object

			// Increment sample count by 1
			*p2 += 1;

			// Update last space key state
			mLastSpaceKeyState = mSpaceKeyState;
		}
	}

	// Return result
	return hResult;
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::SetDataFormat(LPCDIDATAFORMAT p0)
{
	return m_pDIDevice->SetDataFormat(p0);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::SetEventNotification(HANDLE p0)
{
	return m_pDIDevice->SetEventNotification(p0);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::SetCooperativeLevel(HWND p0, DWORD p1)
{
	return m_pDIDevice->SetCooperativeLevel(p0, p1);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetObjectInfo(LPDIDEVICEOBJECTINSTANCEW p0, DWORD p1, DWORD p2)
{
	return m_pDIDevice->GetObjectInfo(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetDeviceInfo(LPDIDEVICEINSTANCEW p0)
{
	return m_pDIDevice->GetDeviceInfo(p0);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::RunControlPanel(HWND p0, DWORD p1)
{
	return m_pDIDevice->RunControlPanel(p0, p1);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::Initialize(HINSTANCE p0, DWORD p1, REFGUID rguid)
{
	return m_pDIDevice->Initialize(p0, p1, rguid);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::CreateEffect(REFGUID rguid, LPCDIEFFECT p1, LPDIRECTINPUTEFFECT * p2, LPUNKNOWN p3)
{
	return m_pDIDevice->CreateEffect(rguid, p1, p2, p3);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::EnumEffects(LPDIENUMEFFECTSCALLBACKW p0, LPVOID p1, DWORD p2)
{
	return m_pDIDevice->EnumEffects(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetEffectInfo(LPDIEFFECTINFOW p0, REFGUID rguid)
{
	return m_pDIDevice->GetEffectInfo(p0, rguid);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetForceFeedbackState(LPDWORD p0)
{
	return m_pDIDevice->GetForceFeedbackState(p0);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::SendForceFeedbackCommand(DWORD p0)
{
	return m_pDIDevice->SendForceFeedbackCommand(p0);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK p0, LPVOID p1, DWORD p2)
{
	return m_pDIDevice->EnumCreatedEffectObjects(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::Escape(LPDIEFFESCAPE p0)
{
	return m_pDIDevice->Escape(p0);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::Poll()
{
	return m_pDIDevice->Poll();
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::SendDeviceData(DWORD p0, LPCDIDEVICEOBJECTDATA p1, LPDWORD p2, DWORD p3)
{
	return m_pDIDevice->SendDeviceData(p0, p1, p2, p3);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::EnumEffectsInFile(LPCWSTR p0, LPDIENUMEFFECTSINFILECALLBACK p1, LPVOID p2, DWORD p3)
{
	return m_pDIDevice->EnumEffectsInFile(p0, p1, p2, p3);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::WriteEffectToFile(LPCWSTR p0, DWORD p2, LPDIFILEEFFECT p3, DWORD p4)
{
	return m_pDIDevice->WriteEffectToFile(p0, p2, p3, p4);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::BuildActionMap(LPDIACTIONFORMATW p0, LPCWSTR p1, DWORD p2)
{
	return m_pDIDevice->BuildActionMap(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::SetActionMap(LPDIACTIONFORMATW p0, LPCWSTR p1, DWORD p2)
{
	return m_pDIDevice->SetActionMap(p0, p1, p2);
}

HRESULT STDMETHODCALLTYPE IDirectInputDevice8Hook::GetImageInfo(LPDIDEVICEIMAGEINFOHEADERW p0)
{
	return m_pDIDevice->GetImageInfo(p0);
}
