#include <iostream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

#define _CRT_SECURE_NO_WARNINGS
static int *sesptr = new int(0);
static bool *muteptr = new bool(false);
#define GET_MUTE 0
#define GET_VOLUME 1
#define GET_VOLUMEMUTE 2

#define SET_MUTE_AUTO 4
#define SET_VOLUME 5
#define SET_MUTE_WANT 6
namespace eSOUND
{
	inline void Center(byte type)
	{
		try
		{
			IMMDeviceEnumerator *enumerator = NULL;
			IMMDevice *device = NULL;
			IAudioEndpointVolume *endpointVolume = NULL;
			CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (void**)&enumerator);
			enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
			device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);

			if (type == GET_VOLUME)//1
			{
				float currentVolume = 0;
				endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
				int ses = (currentVolume + 0.00000039) * 100;//0.259999961
				endpointVolume->Release();
				device->Release();
				enumerator->Release();
				*sesptr = ses;				
				return; 
			}
			if (type == GET_VOLUMEMUTE)//2
			{
				float currentVolume = 0;
				endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
				int ses = (currentVolume + 0.00000039) * 100;//0.259999961
				BOOL mute;
				endpointVolume->GetMute(&mute);
				endpointVolume->Release();
				device->Release();
				enumerator->Release();
				*sesptr = ses;
				*muteptr = mute;
				return;
			}
			if (type == SET_MUTE_AUTO)//4
			{
				BOOL mute;
				endpointVolume->GetMute(&mute);
				endpointVolume->SetMute(!mute, 0);
				endpointVolume->Release();
				device->Release();
				enumerator->Release();
				return;
			}
			if (type == GET_MUTE)//0
			{
				BOOL mute;
				endpointVolume->GetMute(&mute);
				endpointVolume->Release();
				device->Release();
				enumerator->Release();
				*muteptr = mute;
				return;
			}
			if (type == SET_VOLUME)//5
			{
				float newVolume = static_cast<float>(*sesptr) / 100;//1 ile 0 arasinda tutmak icin 100'e bolduk
				endpointVolume->SetMasterVolumeLevelScalar(newVolume, NULL);
				endpointVolume->SetMute(false, 0);//eger volume yapilacaksa otomatik muteden cikarilmali
				endpointVolume->Release();
				device->Release();
				enumerator->Release();
				return;
			}
			if (type == SET_MUTE_WANT)//6
			{
				endpointVolume->SetMute(*muteptr, 0);
				endpointVolume->Release();
				device->Release();
				enumerator->Release();
				return;
			}
		}
		catch(std::exception){
		}
	}
	
	inline int GetVolume()
	{
		Center(GET_VOLUME);
		return *sesptr;
	}
	
	inline int GetMute()
	{
		Center(GET_MUTE);
		return *muteptr;
	}

	inline void SetVolume(int ses)
	{
		ses = ses > 100 ? 100 : ses;
		*sesptr = ses < 0 ? 0 : ses;
		Center(SET_VOLUME);//Do not forget set *sesptr before call Center(SET_VOLUME);
	}
	inline void SetMute(bool mute)
	{
		*muteptr = mute;
		Center(SET_MUTE_WANT);
	}
	inline void ChangeMute()
	{
		Center(SET_MUTE_AUTO);
	}
	inline void GetVolumeAndMute()
	{
		Center(GET_VOLUMEMUTE);
	}
};
