#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

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

#endif
}

int main() {
	listup_webcams();

	return 0;
}
