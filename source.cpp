#include <iostream>
#include <string>

#ifdef __linux__
	#pragma print("Platform: Linux")
	#include <fcntl.h>
	#include <sys/ioctl.h>
	#include <linux/videodev2.h>
#elif _WIN32
	#pragma message("Platform: Windows")
	#include <mfapi.h>
	#include <mfidl.h>
	#include <mferror.h>

//	#pragma message(lib, "mfplat.lib")

	struct IMFAttributes;
	struct IMActivate;
#endif

void listup_webcams() {
#ifdef __linux__
	std::string prefix = "/dev/video";
	for(int id = 0; id < 5; id++) {
		std::string filename = prefix + std::to_string(id);
		int fd = -1;
		if((fd = open(filename.c_str(), O_RDONLY)) < 0) {
			std::cout 
				<< "Cannot open device #" 
				<< id << ". " << std::endl;
			continue;
		}

		struct v4l2_capability vcap;
		if(ioctl(fd, VIDIOC_QUERYCAP, &vcap) < 0) {
			std::cout 
				<< "Cannot read descriptor for device #" 
				<< id << ". " << std::endl;
			continue;
		}

		std::cout 
			<< "Device Name: "
			<< vcap.card
			<< std::endl;
	}
#elif _WIN32
	IMFAttributes *p_attr = NULL;
	HRESULT hr = MFCreateAttributes(&p_attr, 1);
	if(FAILED(hr)){
		std::cout 
			<< std::hex
			<< "Failed. (MFCreateAttributes 0x" << hr << ")" 
			<< std::dec
			<< std::endl;
		return;
	}

	hr = p_attr->SetGUID(
		MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, 
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
	if(FAILED(hr)){
		std::cout 
			<< std::hex
			<< "Failed. (SetGUID 0x" << hr << ")" 
			<< std::dec
			<< std::endl;
		return;
	}

//	IMFMediaSources *p_src = NULL;
	IMFActivate **pp_devices = NULL;
	UINT32 count;
	hr = MFEnumDeviceSources(p_attr, &pp_devices, &count);
	if(FAILED(hr)){
		std::cout 
			<< std::hex
			<< "Failed. (GetString 0x" << hr << ")" 
			<< std::dec
			<< std::endl;
		return;
	}
	if(count == 0) {
		std::cout
			<< "There's no webcam device available. "
			<< std::endl;
	}

	for(unsigned int i = 0; i < count; i++) {
		UINT32 length = 0;
		WCHAR *p_str = NULL;
		hr = pp_devices[i]->GetAllocatedString(
			MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, 
			&p_str, &length);
		if(FAILED(hr)){
			std::cout 
				<< std::hex
				<< "Failed. (GetAllocatedString 0x" << hr << ")" 
				<< std::dec
				<< std::endl;
			return;
		}

		std::wcout << "Device Name: " << p_str << std::endl;
	}

	if(pp_devices != NULL) {
		for(unsigned int i = 0; i < count; i++) {
			pp_devices[i]->Release();
		}
		CoTaskMemFree(pp_devices);
	}
#endif
}

int main() {
	listup_webcams();

	return 0;
}
