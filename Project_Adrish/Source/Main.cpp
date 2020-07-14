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
	int rsp = 0;
	const char* wl_msg = "Project Adrish\n\nA program that will turn you into a magician, and make you invisible with the help of a invisible cloak.";
	const char* ins_msg = "The color of the invisible cloak in real world is set to orange for now, but you can change to your favourite color.";

	std::cout << wl_msg << std::endl;
	std::cout << std::endl << ins_msg << std::endl;
	std::cout << "\nPress any key to continue...\n";
	std::cin.get();
	std::cout << "Starting program...\n";
	cv::VideoCapture cam(0);
	//cv::VideoCapture cap_bg("f:/videos/3.avi");

	if (!cam.isOpened()) {
		std::cout << "Unable to open your webcam...";
		return 0;
	}
	std::cout << "\nWebcam access OK.\nStarting program in 3s.\n";
	int frame_width = static_cast<int>(cam.get(cv::CAP_PROP_FRAME_WIDTH)); //get the width of frames of the video
	int frame_height = static_cast<int>(cam.get(cv::CAP_PROP_FRAME_HEIGHT)); //get the height of frames of the video

	cv::Size frame_size(frame_width, frame_height);
	double frames_per_second = cam.get(cv::CAP_PROP_FPS);
	
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
	cv::Mat bg_img, ed_img;
	uchar *ptr, *ptr1;

	//!Change these values to try different colors of cloak.
	cv::Vec3b col_low(10, 20, 200), col_upper(130, 160, 255), cl_cr, cl_cr1;

	std::cout << "\nClear the background.\nCapturing background image. Wait for atleast 5s.\n";

	cv::waitKey(2500);
	cam >> bg_img;
	
	std::cout << "\nBackground image captured\nNow you may step in.\n\nStarting magic in 3s.\n";
	
	
	cv::resize(bg_img, bg_img, cv::Size(550, 600));
	
	ptr = bg_img.data;
	const int lm = bg_img.rows * bg_img.cols * bg_img.channels();
	cv::waitKey(2500);
	{
		m_tym timer("video");
		while (1) {

			cam >> ed_img;

			if (ed_img.empty())
				break;

			cv::resize(ed_img, ed_img, cv::Size(550, 600));
			ptr1 = ed_img.data;
			{
				std::thread thr1(edInThr, ptr, ptr1, 0, 247500, col_low, col_upper);
				std::thread thr2(edInThr, ptr, ptr1, 247500, 495000, col_low, col_upper);
				std::thread thr3(edInThr, ptr, ptr1, 495000, 742500, col_low, col_upper);
				std::thread thr4(edInThr, ptr, ptr1, 742500, 990000, col_low, col_upper);
				thr1.join();
				thr2.join();
				thr3.join();
				thr4.join();
			}
			cv::imshow("Adrish", ed_img);

			//cv::resize(img1, img1, cv::Size(frame_width, frame_height));
			//vw.write(img1);

			if (cv::waitKey(1) == 27) //Press escape to exit..
				break;
		}
	}
	//cap_bg.release();
	cam.release();
	//vw.release();
	cv::destroyAllWindows();
}