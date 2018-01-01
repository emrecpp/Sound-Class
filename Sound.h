#include <iostream>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#pragma comment(lib, "Ole32.lib")

 class SOUND
{
private:
	struct INFO
	{
		bool getmute;
		int value;
		INFO(int val, bool getmut)
		{
			value = val;
			getmute = getmut;
		}
	};
	inline INFO  ControlOfVolumes(int setvalue, bool setmute)
	{
		HRESULT hr = NULL;
		IAudioEndpointVolume *endpointVolume;
		float newVolume = -1;
		if (setvalue != -1) {
			newVolume = (setvalue + 0.1) / 100;//intten floata cevirmek icin ekledik
			newVolume -= 0.1 / 100;//eski hali icin cikardik
		}
		hr = NULL;

		CoInitialize(NULL);
		IMMDeviceEnumerator *deviceEnumerator = NULL;
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
		IMMDevice *defaultDevice = NULL;

		hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
		deviceEnumerator->Release();
		deviceEnumerator = NULL;
		endpointVolume = NULL;
		hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
		defaultDevice->Release();
		defaultDevice = NULL;

		if (setmute && setvalue == -1)//mute degisterimek icinse (-1, true)
		{

			BOOL mute;
			endpointVolume->GetMute(&mute);
			endpointVolume->SetMute(!mute, 0);

			endpointVolume->Release();
			CoUninitialize();
			return  INFO(-1, false);//NULL
		}
		else if (setvalue == -2)//Change mute 2 :what do you want to do
		{
			endpointVolume->SetMute(setmute, 0);

			endpointVolume->Release();
			CoUninitialize();
			return  INFO(-1, false);//NULL			
		}
		else if (setvalue == -1 && !setmute)//not set (get volume) (-1,false)
		{
			float currentVolume = 0;
			hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
			int ses = (currentVolume + 0.00000039) * 100;//0.259999961
			BOOL mute;
			endpointVolume->GetMute(&mute);
			endpointVolume->Release();
			CoUninitialize();

			return   INFO(ses, mute);
		}
		else if (setvalue > -1)//set volume
		{
			hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
			endpointVolume->SetMute(false, 0);//eger volume yapilacaksa otomatik muteden cikarilmali
			endpointVolume->Release();
			CoUninitialize();
			return INFO(-1, false);//NULL
		}
	}

public:	//Functions for programmer:
	inline int GetVolume()
	{
		INFO  inf = ControlOfVolumes(-1, false);
		return inf.value;
	}
	inline int GetMute()
	{
		INFO  inf = ControlOfVolumes(-1, false);
		return inf.getmute;
	}

	inline void SetVolume(int ses)//
	{
		ses = ses > 100 ? 100 : ses;
		ses = ses < 0 ? 0 : ses;
		ControlOfVolumes(ses, false);
	}
	inline void SetMute(bool mute)
	{
		ControlOfVolumes(-2, mute);//-2 is special for set mute what do you want to do
	}
	inline void ChangeMute()
	{
		ControlOfVolumes(-1, true);
	}
	inline void GetVolumeAndMute(bool *mut, int *vol) {
		INFO  inf = ControlOfVolumes(-1, false);
		*mut = inf.getmute;
		*vol = inf.value;
	}
	inline int GetVolume_MouseClick(System::Windows::Forms::MouseEventArgs ^e, System::Windows::Forms::ProgressBar ^progressbar, bool vertical)// call that function on "progressBar1_MouseClick" and this function is for Vertical Progressbar
	{
		double value = e->Y * 1.0 / progressbar->Height;
		if (!vertical)
			value = e->X * 1.0 / progressbar->Width;

		int max = progressbar->Maximum;

		double sounddouble = 100 - (progressbar->Minimum + value * (max - progressbar->Minimum));
		if (!vertical)
			sounddouble = progressbar->Minimum + value * (max - progressbar->Minimum);
		int sound = (int)sounddouble;
		sound = sound > 100 ? 100 : sound;
		return (sound = sound < 0 ? 0 : sound);
	}
	
	inline int GetVolume_MouseClick(System::Windows::Forms::MouseEventArgs ^e, int ProgressbarHeight, int ProgressbarWidth, int ProgressbarMinimum, int ProgressbarMaximum, bool vertical)// call that function on "progressBar1_MouseClick" and this function is for Height Progressbar
	{
		double value = e->Y * 1.0 / ProgressbarHeight;
		if (!vertical)
			value = e->X * 1.0 / ProgressbarWidth;
		int max = ProgressbarMaximum;

		double sounddouble = 100 - (ProgressbarMinimum + value * (max - ProgressbarMinimum));
		if (!vertical)
			sounddouble = ProgressbarMinimum + value * (max - ProgressbarMinimum);
		int sound = (int)sounddouble;
		sound = sound > 100 ? 100 : sound;
		return (sound = sound < 0 ? 0 : sound);
	}
};