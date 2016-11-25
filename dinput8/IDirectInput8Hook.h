#pragma once

#include "IDirectInputDevice8Hook.h"

class IDirectInput8Hook : public IDirectInput8
{
private:
	IDirectInput8 * m_dinput;

public:
	IDirectInput8Hook(IDirectInput8 * dinput);

	/*** IUnknown methods ***/
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID * ppvObj);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	/*** IDirectInput8A methods ***/
	HRESULT STDMETHODCALLTYPE CreateDevice(REFGUID rguid, LPDIRECTINPUTDEVICE8W * lplpDirectInputDevice, LPUNKNOWN pUknOuter);
	HRESULT STDMETHODCALLTYPE EnumDevices(DWORD dwDevType, LPDIENUMDEVICESCALLBACKW lpCallback, LPVOID pvRef, DWORD dwFlags);
	HRESULT STDMETHODCALLTYPE GetDeviceStatus(REFGUID rguidInstance);
	HRESULT STDMETHODCALLTYPE RunControlPanel(HWND hwndOwner, DWORD dwFlags);
	HRESULT STDMETHODCALLTYPE Initialize(HINSTANCE hinst, DWORD dwVersion);
	HRESULT STDMETHODCALLTYPE FindDevice(REFGUID rguidClass, LPCWSTR ptszName, LPGUID pguidInstance);
	HRESULT STDMETHODCALLTYPE EnumDevicesBySemantics(LPCWSTR ptszUserName, LPDIACTIONFORMATW lpdiActionFormat, LPDIENUMDEVICESBYSEMANTICSCBW lpCallback, LPVOID pvRef, DWORD dwFlags);
	HRESULT STDMETHODCALLTYPE ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK lpdiCallback, LPDICONFIGUREDEVICESPARAMSW lpdiCDParams, DWORD dwFlags, LPVOID pvRefData);
};