#include <iostream>
#include <uipf/logging.hpp>
#include <uipf/data/opencv.hpp>

#include <uipf-sfm/data/KeyPointList.hpp>
//#include "data/Image.hpp"
//#include "foerstner/Foerstner.hpp"

#define UIPF_MODULE_NAME "Foerstner Operator (Point Detector)"
#define UIPF_MODULE_ID "cebe.sfm.points.foerstner"
#define UIPF_MODULE_CLASS FoerstnerModule
#define UIPF_MODULE_CATEGORY "sfm"

#define UIPF_MODULE_INPUTS \
		{"image", uipf::DataDescription(uipf::data::OpenCVMat::id(), "the input image.")}

#define UIPF_MODULE_OUTPUTS \
		{"image", uipf::DataDescription(uipfsfm::data::Image::id(), "the image with annotated keypoints.")}

#define UIPF_MODULE_PARAMS \
		{"sigma", uipf::ParamDescription("defaults to 0.5.", true)}

#include <uipf/Module.hpp>
#include <uipf-sfm/data/Image.hpp>

using namespace uipf;
using namespace uipf::data;
using namespace uipfsfm::data;

void FoerstnerModule::run() {

	OpenCVMat::ptr image = getInputData<OpenCVMat>("image");


	UIPF_LOG_INFO("Number of detected interest points:", image->filename, ": ", points->getContent().size());

	Image::ptr sfmImage(new Image(image->filename));
	sfmImage->keypoints = points;
	setOutputData<Image>("image", sfmImage);
}
