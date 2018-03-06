#include "Controller.h"
#define PI 3.14159265

Controller::Controller() : 
th(track, this),
MIN_OBJECT_AREA(40*40),
FRAME_WIDTH(400),
FRAME_HEIGHT(300) {
 	H_MIN = 0;
    H_MAX = 256;
    S_MIN = 0;
    S_MAX = 256;
    V_MIN = 0;
    V_MAX = 256;
    MAX_OBJECT_AREA = FRAME_HEIGHT * FRAME_WIDTH/1.5;
    set_st(false, false ,false, false, false);
 }

 Controller::~Controller() {
	th.join();
}

void Controller::createTrackbars() { 
    cv::namedWindow("Trackbars",0);

	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN");
	sprintf( TrackbarName, "H_MAX");
	sprintf( TrackbarName, "S_MIN");
	sprintf( TrackbarName, "S_MAX");
	sprintf( TrackbarName, "V_MIN");
	sprintf( TrackbarName, "V_MAX");
    
    cv::createTrackbar( "H_MIN", "Trackbars", &H_MIN, H_MAX, controller::on_trackbar );
    cv::createTrackbar( "H_MAX", "Trackbars", &H_MAX, H_MAX, controller::on_trackbar );
    cv::createTrackbar( "S_MIN", "Trackbars", &S_MIN, S_MAX, controller::on_trackbar );
    cv::createTrackbar( "S_MAX", "Trackbars", &S_MAX, S_MAX, controller::on_trackbar );
    cv::createTrackbar( "V_MIN", "Trackbars", &V_MIN, V_MAX, controller::on_trackbar );
    cv::createTrackbar( "V_MAX", "Trackbars", &V_MAX, V_MAX, controller::on_trackbar );
}

void Controller::morphOps(cv::Mat &thresh) {
	cv::Mat erodeElement = cv::getStructuringElement( MORPH_RECT,Size(3,3));
	cv::Mat dilateElement = cv::getStructuringElement( MORPH_RECT,Size(5,5));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);

	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);	
}

void Controller::sortContours(std::vector<std::vector<cv::Point>>& contours) {
    auto contourComparator = [](std::vector<cv::Point> a, std::vector<cv::Point> b) { 
    	return cv::contourArea(a) > cv::contourArea(b); 
    };
    sort(contours.begin(), contours.end(), contourComparator);
}

void Controller::colorReduce(cv::Mat& image, int div=32) {    
    int nl = image.rows;
    int nc = image.cols * image.channels();

    for (int j = 0; j < nl; j++) {
        uchar* data = image.ptr<uchar>(j);

        for (int i = 0; i < nc; i++) {
            data[i] = data[i] / div * div + div / 2;
        }
    }
}

int Controller::angleToCenter(const Point &finger, const Point &center) {
	float y_angle = center.y - finger.y;
	float x_angle = finger.x - center.x;
	float theta = atan(y_angle / x_angle);
	int angleFinger = (int)round(theta * 180 / PI);

	return angleFinger;
}

std::string Controller::doAction(const int totalAngleOfFinger, const int fingerSize) {
	std::string result = "no-command";
	if (fingerSize == 5)
		result = "five fingers";
	else if (fingerSize == 4)
		result = "four fingers";
	else if (fingerSize == 3)
		result = "three fingers";
	else if (fingerSize == 2)
		result = "two fingers";
	else if (fingerSize == 1)
		result = "one finger";

	return result;
}

void Controller::track(Controller *obj) {
    bool useMorphOps = true;
    obj -> is_in_process = true;

	cv::Mat cameraFeed;
	cv::Mat HSV;
	cv::Mat threshold;
	VideoCapture capture;
	capture.open(0);
	capture.set(CV_CAP_PROP_FRAME_WIDTH,obj -> FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,obj -> FRAME_HEIGHT);

	cv::Mat colorReduced;
	while(cv::waitKey(30) != 27) {

		capture.read(cameraFeed);
		cv::flip(cameraFeed, cameraFeed, 1);
		obj -> createTrackbars();
		cv::cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);

		HSV.convertTo(colorReduced, CV_8UC3);
		obj -> colorReduce(colorReduced);
		cv::inRange(colorReduced,cv::Scalar(obj -> H_MIN,obj -> S_MIN,obj -> V_MIN),cv::Scalar(obj -> H_MAX,obj -> S_MAX,obj -> V_MAX),threshold);

		cv::imshow("Frame", colorReduced);
		cv::imshow("Mask", threshold);

		if(! obj -> is_in_process) {
			cv::destroyWindow("Frame");
			cv::destroyWindow("Mask");
			cv::destroyWindow("Trackbars"); 

			return;
		}
	}

	cv::destroyWindow("Frame");
	cv::destroyWindow("Mask");
	cv::destroyWindow("Trackbars"); 

	cv::Mat thresholdColor;

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::Rect boundRect;

	std::vector<cv::Point> convexHullPoint;
	std::vector<cv::Point> fingerPoint;
	cv::Point centerP;

	double area = 0;
	double maxArea = 0;
	bool handFound = false;

	int inAngleMin = 200, inAngleMax = 300, angleMin = 180, angleMax = 359, lengthMin = 10, lengthMax = 80;

	while(obj -> is_in_process) {

		capture.read(cameraFeed);
		cv::flip(cameraFeed, cameraFeed, 1);
		cv::cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);

		cv::Mat colorReduced;
		HSV.convertTo(colorReduced, CV_8UC3);
		obj -> colorReduce(colorReduced);
		cv::inRange(colorReduced,cv::Scalar(obj -> H_MIN,obj -> S_MIN,obj -> V_MIN),cv::Scalar(obj -> H_MAX,obj -> S_MAX,obj -> V_MAX),thresholdColor);

		if (useMorphOps)
			obj -> morphOps(thresholdColor);

		cv::findContours( thresholdColor, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

		obj -> sortContours(contours);

		cv::Mat objectsOnly = cv::Mat(obj->FRAME_HEIGHT, obj->FRAME_WIDTH, CV_8UC3, cv::Scalar(0));

		cv::drawContours(objectsOnly, contours, 0, cv::Scalar(255, 255, 255), CV_FILLED, LINE_8, hierarchy, 0);
		
		if ((cv::contourArea(contours[0]) > obj -> MIN_OBJECT_AREA) && (cv::contourArea(contours[0]) < obj -> MAX_OBJECT_AREA)) {
			handFound = true;
			boundRect = boundingRect(contours[0]);

			cv::Moments moment = cv::moments(cv::Mat(contours[0]), true);
			int centerX = moment.m10 / moment.m00;
			int centerY = moment.m01 / moment.m00;
			cv::Point centerPoint(centerX, centerY);
			centerP = centerPoint;

			cv::circle(objectsOnly, centerPoint, 8, Scalar(255, 0, 0), CV_FILLED);

			std::vector<std::vector<cv::Point> > hull(1);
			cv::convexHull(cv::Mat(contours[0]), hull[0], false);
			cv::drawContours(objectsOnly, hull, 0, cv::Scalar(0, 255, 0), 3);
			std::vector<cv::Point> validPoints;

			if (hull[0].size() > 2) {
				std::vector<int> hullIndexes;
				cv::convexHull(cv::Mat(contours[0]), hullIndexes, true);
				std::vector<cv::Vec4i> convexityDefects;
				cv::convexityDefects(cv::Mat(contours[0]), hullIndexes, convexityDefects);
				cv::Rect boundingBox = cv::boundingRect(hull[0]);
				cv::rectangle(objectsOnly, boundingBox, cv::Scalar(255, 0, 0));

				for (size_t i = 0; i < convexityDefects.size(); i++) {

					cv::Point p1 = contours[0][convexityDefects[i][0]];
					cv::Point p2 = contours[0][convexityDefects[i][1]];
					cv::Point p3 = contours[0][convexityDefects[i][2]];

					double angle = std::atan2(centerPoint.y - p1.y, centerPoint.x - p1.x) * 180 / CV_PI;
					double inAngle = obj -> innerAngle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
					double length = std::sqrt(std::pow(p1.x - p3.x, 2) + std::pow(p1.y - p3.y, 2));

					if (angle > angleMin - 180 && angle < angleMax - 180 && inAngle > inAngleMin - 180 &&
					    inAngle < inAngleMax - 180 && length > lengthMin / 100.0 * boundingBox.height && 
						length < lengthMax / 100.0 * boundingBox.height) {

					  validPoints.push_back(p1);
					}
				}

				for (size_t i = 0; i < validPoints.size(); i++) {
					cv::circle(objectsOnly, validPoints[i], 9, cv::Scalar(0, 255, 0), 2);
				}
			}

			bool in_fire = false;

			if (validPoints.size() < 3) {
				in_fire = true;
			} 
			else {
				in_fire = false;
			}

			obj -> analyze(centerX, centerY, in_fire);
		}

		obj -> drawLines(objectsOnly);
		cv::namedWindow("Objects Only", cv::WINDOW_AUTOSIZE);
		cv::imshow("Objects Only", objectsOnly);

		char keypress = cv::waitKey(20); 
	  	if (keypress == 27) { 
	    	break; 
	  	}
	}
}

float Controller::innerAngle(float px1, float py1, float px2, float py2, float cx1, float cy1) {
	float dist1 = std::sqrt(  (px1-cx1)*(px1-cx1) + (py1-cy1)*(py1-cy1) );
	float dist2 = std::sqrt(  (px2-cx1)*(px2-cx1) + (py2-cy1)*(py2-cy1) );

	float Ax, Ay;
	float Bx, By;
	float Cx, Cy;
 
	Cx = cx1;
	Cy = cy1;

	if (dist1 < dist2) {
		Bx = px1;
		By = py1;
		Ax = px2;
		Ay = py2;
	}
	else {
		Bx = px2;
		By = py2;
		Ax = px1;
		Ay = py1;
	}

	float Q1 = Cx - Ax;
	float Q2 = Cy - Ay;
	float P1 = Bx - Ax;
	float P2 = By - Ay;

	float A = std::acos( (P1*Q1 + P2*Q2) / ( std::sqrt(P1*P1+P2*P2) * std::sqrt(Q1*Q1+Q2*Q2) ) );

	A = A*180/CV_PI;

 	return A;
}

void Controller::drawLines(cv::Mat& image) {
	int W = (int)FRAME_WIDTH/2; 
    int H = (int)FRAME_HEIGHT/2; 
    int radius = (int)(0.125 * FRAME_HEIGHT);

	cv::line(image, cv::Point(W - radius, 0), cv::Point(W - radius, FRAME_HEIGHT), Scalar( 110, 220, 0 ), 2, 8);
	cv::line(image, cv::Point(W + radius, 0), cv::Point(W + radius, FRAME_HEIGHT), Scalar( 110, 220, 0 ), 2, 8);
	cv::line(image, cv::Point(0, H - radius), cv::Point(FRAME_WIDTH, H - radius), Scalar( 110, 220, 0 ), 2, 8);
	cv::line(image, cv::Point(0, H + radius), cv::Point(FRAME_WIDTH, H + radius), Scalar( 110, 220, 0 ), 2, 8);
	cv::circle(image, cv::Point(W, H), radius, Scalar( 110, 220, 0 ), 2, 8, 0);
}

void Controller::analyze(int current_x, int current_y, bool in_fire) {
    int W = (int)FRAME_WIDTH/2;
    int H = (int)FRAME_HEIGHT/2;
    int radius = (int)(0.125 * FRAME_HEIGHT);
    st_mutex.lock();
    
    //Круг -- STOP
    if ((current_x >= (FRAME_WIDTH/2 - radius)) && (current_x <= (FRAME_WIDTH/2 + radius)) && (current_y >= (FRAME_HEIGHT/2 - radius)) && (current_y <= (FRAME_HEIGHT/2 + radius))) {
       if(in_fire)
        	set_st(false, false, false, false, true);
        else
        	set_st(false, false, false, false, false);
    }
    
    //up_left
    if ((current_x > 0) && (current_x < (W - radius)) && (current_y > 0) && (current_y < (H - radius))) {
        if(in_fire)
        	set_st(true, false, true, false, true);
        else
        	set_st(true, false, true, false, false);
    }
    
    //up
    if ((current_x > (W - radius)) && (current_x < (W + radius)) && (current_y > 0) && (current_y < (H - radius))) {
    	if(in_fire)
        	set_st(true, false, false, false, true);
        else
        	set_st(true, false, false, false, false);
    }
    
    //up_right
    if ((current_x > (W + radius)) && (current_x < FRAME_WIDTH) && (current_y > 0) && (current_y < (H - radius))) {
    	if(in_fire)
        	set_st(true, false, false, true, true);
       	else
       		set_st(true, false, false, true, false);
    }
    
    //left
    if ((current_x > 0) && (current_x < (W - radius)) && (current_y > (H - radius)) && (current_y < (H + radius))) {
    	if(in_fire)
        	set_st(false, false, true, false, true);
        else
        	set_st(false, false, true, false, false);
    }
    
    //right
    if ((current_x > (W + radius)) && (current_x < FRAME_WIDTH) && (current_y > (H - radius)) && (current_y < (H + radius))) {
    	if(in_fire)
        	set_st(false, false, false, true, true);
        else
        	set_st(false, false, false, true, false);
    }
    
    //down_left
    if ((current_x > 0) && (current_x < (W - radius)) && (current_y > (H + radius)) && (current_y < FRAME_HEIGHT)) {
    	if(in_fire)
        	set_st(false, true, true, false, true);
        else
        	set_st(false, true, true, false, false);
    }
    
    //down
    if ((current_x > (W - radius)) && (current_x < (W + radius)) && (current_y > (H + radius)) && (current_y < FRAME_HEIGHT)) {
    	if(in_fire)
        	set_st(false, true, false, false, true);
        else
        	set_st(false, true, false, false, false);
    }
    
    //down_right
    if ((current_x > (W + radius)) && (current_x < FRAME_WIDTH) && (current_y > (H + radius)) && (current_y) < FRAME_HEIGHT) {
    	if(in_fire)
       		set_st(false, true, false, true, true);
       	else
       		set_st(false, true, false, true, false);
    }
    
    st_mutex.unlock();
}

void Controller::getState() {
	std::lock_guard<std::mutex> lock(st_mutex);
	std::cout << st.up << " " <<  st.down   <<  " " <<  st.left << " " << st.right << std::endl;
}

void Controller::set_st(bool in_up,  bool in_down, bool in_left, bool in_right, bool in_fire) {
        st.up = in_up;
        st.down = in_down;
        st.left = in_left;
        st.right = in_right;
        st.fire = in_fire;
    }

    
