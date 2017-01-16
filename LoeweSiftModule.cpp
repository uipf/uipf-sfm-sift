
#include <stdio.h>
#include <iostream>
#include <uipf/logging.hpp>
#include <uipf/data/opencv.hpp>
#include <uipf/util.hpp>

#include <uipf-sfm/data/KeyPointList.hpp>
#include <uipf-sfm/data/Image.hpp>
//#include "foerstner/Foerstner.hpp"

#define UIPF_MODULE_NAME "Loewe Sift (Point Detector and descriptor)"
#define UIPF_MODULE_ID "cebe.sfm.points.sift_loewe"
#define UIPF_MODULE_CLASS LoeweSiftModule
#define UIPF_MODULE_CATEGORY "sfm"

#define UIPF_MODULE_INPUTS \
		{"image", uipf::DataDescription(uipfsfm::data::Image::id(), "the input image.")}

#define UIPF_MODULE_OUTPUTS \
		{"image", uipf::DataDescription(uipfsfm::data::Image::id(), "the image with annotated keypoints.")}

#define UIPF_MODULE_PARAMS \
		{"sigma", uipf::ParamDescription("defaults to 0.5.", true)}

#include <uipf/Module.hpp>
#include <uipf-sfm/data/Image.hpp>
#include <fstream>

#include "include/SiftKeyPoint.hpp"

// TODO configure this path with cmake
#define SIFT_BINARY "/home/cebe/Dokumente/Uni/master/code/uipf-sfm-sift/cmake-build-debug/sift/sift"

using namespace uipf;
using namespace uipf::data;
using namespace uipfsfm::data;

void LoeweSiftModule::run() {

	using namespace std;

	Image::ptr image = getInputData<Image>("image");

	const string& imagePath = image->getContent();
	// TODO convert image to PGM format if it is not already
	// using image magic
	// /usr/bin/mogrify -format pgm ./CIMG6446.jpg; ../bin/sift < ./CIMG6446.pgm > ./CIMG6446.key; gzip -f ./CIMG6446.key; rm ./CIMG6446.pgm

	// TODO alternative input via opencv and use opencv for conversion

	std::string sift = uipf::util::exec((string(SIFT_BINARY) + string(" < ") + imagePath).c_str());
	istringstream s(sift);

	int point_count = 0;
	s >> point_count;
	int vector_length = 0;
	s >> vector_length;


	vector<SiftKeyPoint::ptr> kpoints;
	for(int i = point_count; i > 0; --i) {
		float row, col, scale, rot;
		s >> row;
		s >> col;
		s >> scale;
		s >> rot;

		SiftKeyPoint::ptr kp(new SiftKeyPoint(cv::KeyPoint(row, col, scale, rot)));
		for(int v = 0; v < vector_length; ++v) {
			s >> kp->descriptor[v];
		}
		// kp->print();
	}

	UIPF_LOG_INFO("Number of detected interest points:", image->getContent(), ": ", point_count);

//	sfmImage->keypoints = points;
	setOutputData<Image>("image", image);
}
