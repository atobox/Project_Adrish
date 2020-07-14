#include "pch.h"


bool operator>(cv::Vec3b& c1, cv::Vec3b& c2) {
	if (c1.channels != c2.channels) {
		std::cout << "Both arguments don't have same no. of channels.\n";
		return false;
	}
	
	for (int i = 0; i < c1.channels; i++)
		if (c1[i] < c2[i])
			return false;

	return true;
}

bool operator<(cv::Vec3b& c1, cv::Vec3b& c2) {
	if (c1.channels != c2.channels) {
		std::cout << "Both arguments don't have same no. of channels.\n";
		return false;
	}

	for (int i = 0; i < c1.channels; i++)
		if (c1[i] > c2[i])
			return false;

	return true;
}

void get_vec(uchar* ptr, cv::Vec3b& vec) {
	vec = cv::Vec3b(*(ptr+0), *(ptr+1), *(ptr+2));
}

void put_vec(uchar* ptr, cv::Vec3b& vec) {
	*ptr = vec[0];
	*(ptr + 1) = vec[1];
	*(ptr + 2) = vec[2];
}

void edInThr(uchar* ptr, uchar* ptr1, int st, int end, cv::Vec3b col, cv::Vec3b col1) {
	cv::Vec3b cl_cr, cl_cr1;
	for (int i = st; i < end; i += 3) {
		get_vec(ptr1 + i, cl_cr1);
		if (cl_cr1 > col && cl_cr1 < col1) {
			get_vec(ptr + i, cl_cr);
			put_vec(ptr1 + i, cl_cr);
		}
	}
}

int main() {
	
	cv::VideoCapture cap_fg("f:/videos/1.avi");
	cv::VideoCapture cap_bg("f:/videos/3.avi");

	if (!cap_fg.isOpened() || !cap_bg.isOpened()) {
		std::cout << "Unable to open file...";
		return 0;
	}

	int frame_width = static_cast<int>(cap_fg.get(cv::CAP_PROP_FRAME_WIDTH)); //get the width of frames of the video
	int frame_height = static_cast<int>(cap_fg.get(cv::CAP_PROP_FRAME_HEIGHT)); //get the height of frames of the video

	cv::Size frame_size(frame_width, frame_height);
	double frames_per_second = 10;//cap_fg.get(cv::CAP_PROP_FPS);
	
	//Create and initialize the VideoWriter object 
	/*
	cv::VideoWriter vw("F:/Video.avi", cv::VideoWriter::fourcc('M','J','P','G'), frames_per_second, frame_size, true);
	
	if (vw.isOpened() == false)
	{
		std::cout << "Cannot save the video to a file" << std::endl;
		std::cin.get(); //wait for any key press
		return -1;
	}
	*/
	cv::Mat img, img1;
	uchar *ptr, *ptr1;

	cv::Vec3b col(10, 20, 200), col1(130, 160, 255), cl_cr, cl_cr1;

	cap_bg >> img;
	
	cv::resize(img, img, cv::Size(550, 600));
	
	ptr = img.data;
	const int lm = img.rows * img.cols * img.channels();
	{
		m_tym timer("video");
		while (1) {

			cap_fg >> img1;

			if (img1.empty())
				break;

			cv::resize(img1, img1, cv::Size(550, 600));
			ptr1 = img1.data;
			{
				std::thread thr1(edInThr, ptr, ptr1, 0, 247500, col, col1);
				std::thread thr2(edInThr, ptr, ptr1, 247500, 495000, col, col1);
				std::thread thr3(edInThr, ptr, ptr1, 495000, 742500, col, col1);
				std::thread thr4(edInThr, ptr, ptr1, 742500, 990000, col, col1);
				thr1.join();
				thr2.join();
				thr3.join();
				thr4.join();
			}
			cv::imshow("Adrish", img1);

			//cv::resize(img1, img1, cv::Size(frame_width, frame_height));
			//vw.write(img1);

			if (cv::waitKey(1) == 27) //Press escape to exit..
				break;
		}
	}
	cap_bg.release();
	cap_fg.release();
	//vw.release();
	cv::destroyAllWindows();
}