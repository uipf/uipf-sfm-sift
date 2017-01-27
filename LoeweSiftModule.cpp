
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

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
		{"createKeyFile", uipf::ParamDescription("whether to create file with keypoint data (bundler compatible). Defaults to false.", true)}, \
		{"gzipOutput", uipf::ParamDescription("whether to gzip keyfile output (bundler compatible). Defaults to false.", true)}, \
		{"cache", uipf::ParamDescription("whether to cache results, implies createKeyFile. Defaults to false.", true)}


// TODO add option to reuse computed values if files exist

#include <uipf/Module.hpp>


// TODO configure this path with cmake
#define SIFT_BINARY "uipf-sfm-sift"

using namespace uipf;
using namespace uipf::data;
using namespace uipf::util;
using namespace uipfsfm::data;

void LoeweSiftModule::run() {

	using namespace std;
	namespace fs = boost::filesystem;

	Image::ptr image = getInputData<Image>("image");

	bool createKeyFile = getParam<bool>("createKeyFile", false);
	bool gzipOutput = getParam<bool>("gzipOutput", false);
	bool cache = getParam<bool>("cache", false);
	if (cache) {
		createKeyFile = true;
	}

	const string& imagePath = image->getContent();
	fs::path ip(imagePath);
	string keyFileName = (ip.parent_path() / ip.stem()).string() + ".key";
	stringstream s;
	UIPF_LOG_DEBUG("using cache: ", cache);
	if (cache && fs::exists(fs::path(keyFileName))) {
		UIPF_LOG_DEBUG("using cached key file: ", keyFileName);
		ifstream keyFile(keyFileName);
		s << keyFile.rdbuf();
		keyFile.close();
	} else {

		string tmpFile = imagePath;
		bool deleteTmpFile = false;
		// convert image to PGM format if it is not already
		if (!str_ends_with(str_to_lower(imagePath), ".pgm")) {
			tmpFile = imagePath + string(".pgm");
			deleteTmpFile = true;
			UIPF_LOG_DEBUG("creating temporary .pgm file for image ", tmpFile);
			cv::Mat tmpImg = load_image_greyscale(imagePath)->getContent();
			tmpImg.convertTo(tmpImg, CV_8U);
			cv::imwrite(tmpFile, tmpImg);
			// alternative: using image magic
			// /usr/bin/mogrify -format pgm ./CIMG6446.jpg; ../bin/sift < ./CIMG6446.pgm > ./CIMG6446.key; gzip -f ./CIMG6446.key; rm ./CIMG6446.pgm
		}

		std::string sift = uipf::util::exec((string(SIFT_BINARY) + string(" < ") + tmpFile).c_str());

		if (createKeyFile) {
			ofstream keyFile(keyFileName);
			keyFile << sift;
			keyFile.close();
			if (gzipOutput) {
				exec(string(string("gzip ") + keyFileName).c_str()); // TODO proper shell arg escaping
			}
		}
		s << sift;

		if (deleteTmpFile) {
			// TODO error handling
			fs::remove(tmpFile);
		}
	}

	// std::stof() is locale aware, meaning params are not portable between platforms
	// the following is a locale independend stof():
	s.imbue(std::locale("C"));

	int point_count = 0;
	s >> point_count;
	int vector_length = 0;
	s >> vector_length;

	image->keypoints = KeyPointList::ptr(new KeyPointList());
	for(int i = point_count; i > 0; --i) {
		float row, col, scale, rot;
		s >> col;
		s >> row;
		s >> scale;
		s >> rot;
		// convert value from range [-PI,PI[ (sift) to [0,360[ (openCV)
		rot = (int)(rot * 180/M_PI) + 180;

		image->keypoints->getContent().push_back(cv::KeyPoint(row, col, scale, rot));
		cv::Mat* descriptor = new cv::Mat(1, vector_length - 1, CV_8U);
		for(int v = 0; v < vector_length; ++v) {
			int no;
			s >> no;
			descriptor->at<uint8_t>(v) = (uint8_t) no;
		}
		image->keypoints->descriptors.push_back(descriptor);
	}
	image->hasKeyPoints = true;
	//image->keypoints->print(false);

	UIPF_LOG_INFO("Number of detected interest points:", image->getContent(), ": ", point_count);

//	sfmImage->keypoints = points;
	setOutputData<Image>("image", image);
}
