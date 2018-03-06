#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <thread>
#include <mutex>
#include <chrono>

using namespace cv;

struct State {
    bool up ;
    bool down;
    bool left;
    bool right;
    bool fire;
};

class Controller {
public :
	Controller();
	~Controller();
	static void track(Controller *obj);
	
	void drawLines(Mat& image);

	void getState();
	void createTrackbars();
	void morphOps(cv::Mat &thresh);
	void sortContours(std::vector<std::vector<cv::Point>>& contours);
	void colorReduce(cv::Mat& image, int div);
	int angleToCenter(const Point &finger, const Point &center);
	std::string doAction(const int totalAngleOfFinger, const int fingerSize);
	void analyze(int current_x, int current_y, bool in_fire);
	void set_st(bool in_up,  bool in_down,  bool in_left, bool in_right, bool in_fire);
	static void on_trackbar( int, void* ){};

	float innerAngle(float px1, float py1, float px2, float py2, float cx1, float cy1);
	
	void switch_off() { is_in_process = false; }

	bool get_st_up() {
		std::lock_guard<std::mutex> lock(st_mutex);
		return st.up;
	}

	bool get_st_down() {
		std::lock_guard<std::mutex> lock(st_mutex);
		return st.down;
	}

	bool get_st_left() {
		std::lock_guard<std::mutex> lock(st_mutex);
		return st.left;
	}

	bool get_st_right() {
		std::lock_guard<std::mutex> lock(st_mutex);
		return st.right;
	}

	bool get_st_fire() {
		std::lock_guard<std::mutex> lock(st_mutex);
		return st.fire;
	}
	
private :
	State st;
	std::mutex st_mutex;
	std::thread th;
	
	int H_MIN;
	int H_MAX;
	int S_MIN;
	int S_MAX;
	int V_MIN;
	int V_MAX;

	const int FRAME_WIDTH;
	const int FRAME_HEIGHT;
	const int MIN_OBJECT_AREA;

	int MAX_OBJECT_AREA;
	bool is_in_process;
};

#endif