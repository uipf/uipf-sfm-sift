#ifndef SFM_SIFT_KEYPOINT_HPP
#define SFM_SIFT_KEYPOINT_HPP

#include <uipf/data.hpp>
#include <opencv2/opencv.hpp>

// TODO check if external 'C' is needed here

namespace uipfsfm {
	namespace data {

		UIPF_DATA_TYPE_BEGIN (SiftKeyPoint, "cebe.sfm.data.siftkeypoint", cv::KeyPoint)

		public:
			double row;
			double col;
			double scale;
			double orientation; // in radians from -PI to PI
			double descriptor[128];

			void print(bool verbose = false) {

				cv::KeyPoint& k = getContent();
				std::cout << "KeyPoint: " << k.pt << ", s=" << k.size << ", r=" << k.angle;
				if (verbose) {
					std::cout << ", v=[";
					for (double d: descriptor) {
						std::cout << d << " ";
					}
					std::cout << "]";
				}
				std::cout << std::endl;

			}

		UIPF_DATA_TYPE_END
	}
}


#endif //SFM_SIFT_KEYPOINT_HPP
