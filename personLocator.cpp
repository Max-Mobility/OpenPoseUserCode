// 3rdparty dependencies
// GFlags: DEFINE_bool, _int32, _int64, _uint64, _double, _string
#include <gflags/gflags.h>

// Allow Google Flags in Ubuntu 14
#ifndef GFLAGS_GFLAGS_H_
namespace gflags = google;
#endif
// OpenPose dependencies
#include <openpose/core/headers.hpp>
#include <openpose/filestream/headers.hpp>
#include <openpose/gui/headers.hpp>
#include <openpose/pose/headers.hpp>
#include <openpose/utilities/headers.hpp>

// include the librealsense C++ header file
#include <librealsense2/rs.hpp>
// include OpenCV header file
#include <opencv2/opencv.hpp>

// See all the available parameter options withe the `--help` flag. E.g. `build/examples/openpose/openpose.bin --help`
// Note: This command will show you flags for other unnecessary 3rdparty files. Check only the flags for the OpenPose
// executable. E.g. for `openpose.bin`, look for `Flags from examples/openpose/openpose.cpp:`.
// Debugging/Other
DEFINE_int32(logging_level,             3,              "The logging level. Integer in the range [0, 255]. 0 will output any log() message, while"
             " 255 will not output any. Current OpenPose library messages are in the range 0-4: 1 for"
             " low priority messages and 4 for important ones.");
// Producer
DEFINE_string(image_path,               "examples/media/COCO_val2014_000000000192.jpg",     "Process the desired image.");
// OpenPose
DEFINE_string(model_pose,               "BODY_25",      "Model to be used. E.g. `COCO` (18 keypoints), `MPI` (15 keypoints, ~10% faster), "
              "`MPI_4_layers` (15 keypoints, even faster but less accurate).");
DEFINE_string(model_folder,             "models/",      "Folder path (absolute or relative) where the models (pose, face, ...) are located.");
DEFINE_string(net_resolution,           "-1x368",       "Multiples of 16. If it is increased, the accuracy potentially increases. If it is"
              " decreased, the speed increases. For maximum speed-accuracy balance, it should keep the"
              " closest aspect ratio possible to the images or videos to be processed. Using `-1` in"
              " any of the dimensions, OP will choose the optimal aspect ratio depending on the user's"
              " input value. E.g. the default `-1x368` is equivalent to `656x368` in 16:9 resolutions,"
              " e.g. full HD (1980x1080) and HD (1280x720) resolutions.");
DEFINE_string(output_resolution,        "-1x-1",        "The image resolution (display and output). Use \"-1x-1\" to force the program to use the"
              " input image resolution.");
DEFINE_int32(num_gpu_start,             0,              "GPU device start number.");
DEFINE_double(scale_gap,                0.3,            "Scale gap between scales. No effect unless scale_number > 1. Initial scale is always 1."
              " If you want to change the initial scale, you actually want to multiply the"
              " `net_resolution` by your desired initial scale.");
DEFINE_int32(scale_number,              1,              "Number of scales to average.");
// OpenPose Rendering
DEFINE_bool(disable_blending,           false,          "If enabled, it will render the results (keypoint skeletons or heatmaps) on a black"
            " background, instead of being rendered into the original image. Related: `part_to_show`,"
            " `alpha_pose`, and `alpha_pose`.");
DEFINE_double(render_threshold,         0.05,           "Only estimated keypoints whose score confidences are higher than this threshold will be"
              " rendered. Generally, a high threshold (> 0.5) will only render very clear body parts;"
              " while small thresholds (~0.1) will also output guessed and occluded keypoints, but also"
              " more false positives (i.e. wrong detections).");
DEFINE_double(alpha_pose,               0.6,            "Blending factor (range 0-1) for the body part rendering. 1 will show it completely, 0 will"
              " hide it. Only valid for GPU rendering.");

int personLocator()
{
  try
    {
      op::log("Starting Person Locator...", op::Priority::High);


      // ------------------------- INITIALIZATION -------------------------
      // - Set logging level
      // - 0 will output all the logging messages
      // - 255 will output nothing
      op::check(0 <= FLAGS_logging_level && FLAGS_logging_level <= 255, "Wrong logging_level value.",
                __LINE__, __FUNCTION__, __FILE__);
      op::ConfigureLog::setPriorityThreshold((op::Priority)FLAGS_logging_level);
      op::log("", op::Priority::Low, __LINE__, __FUNCTION__, __FILE__);
      //  Read Google flags (user defined configuration)
      // outputSize
      const auto outputSize = op::flagsToPoint(FLAGS_output_resolution, "-1x-1");
      // netInputSize
      const auto netInputSize = op::flagsToPoint(FLAGS_net_resolution, "-1x368");
      // poseModel
      const auto poseModel = op::flagsToPoseModel(FLAGS_model_pose);
      // Check no contradictory flags enabled
      if (FLAGS_alpha_pose < 0. || FLAGS_alpha_pose > 1.)
        op::error("Alpha value for blending must be in the range [0,1].", __LINE__, __FUNCTION__, __FILE__);
      if (FLAGS_scale_gap <= 0. && FLAGS_scale_number > 1)
        op::error("Incompatible flag configuration: scale_gap must be greater than 0 or scale_number = 1.",
                  __LINE__, __FUNCTION__, __FILE__);
      // Logging
      op::log("", op::Priority::Low, __LINE__, __FUNCTION__, __FILE__);

      // Initialize all required classes
      op::ScaleAndSizeExtractor scaleAndSizeExtractor(netInputSize, outputSize, FLAGS_scale_number, FLAGS_scale_gap);
      op::CvMatToOpInput cvMatToOpInput{poseModel};
      op::CvMatToOpOutput cvMatToOpOutput;
      op::PoseExtractorCaffe poseExtractorCaffe{poseModel, FLAGS_model_folder, FLAGS_num_gpu_start};
      op::PoseCpuRenderer poseRenderer{poseModel, (float)FLAGS_render_threshold, !FLAGS_disable_blending,
          (float)FLAGS_alpha_pose};
      op::OpOutputToCvMat opOutputToCvMat;
      op::FrameDisplayer frameDisplayer{"Person Locator", outputSize};
      // Initialize resources on desired thread (in this case single thread, i.e. we init resources here)
      poseExtractorCaffe.initializationOnThread();
      poseRenderer.initializationOnThread();


      //initialize realsense pipeline 

      //Contruct a pipeline which abstracts the device
      rs2::pipeline pipe;

      //Create a configuration for configuring the pipeline with a non default profile
      rs2::config cfg;

      //Add desired streams to configuration
      cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);

      //Instruct pipeline to start streaming with the requested configuration
      pipe.start(cfg);

      // Camera warmup - dropping several first frames to let auto-exposure stabilize
      rs2::frameset frames;
      for(int i = 0; i < 30; i++)
        {
          //Wait for all configured streams to produce a frame
          frames = pipe.wait_for_frames();
        }

      // ------------------------- POSE ESTIMATION AND RENDERING -------------------------
      // Read and load image
      while(true){
        const auto timerBegin = std::chrono::high_resolution_clock::now();

        //read in frame from realsense
        frames = pipe.wait_for_frames();
        rs2::frame color = frames.get_color_frame();
        
        //convert to mat
        cv::Mat inputImage(cv::Size(640, 480), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
        const op::Point<int> imageSize{inputImage.cols, inputImage.rows};
        // Get desired scale sizes
        std::vector<double> scaleInputToNetInputs;
        std::vector<op::Point<int>> netInputSizes;
        double scaleInputToOutput;
        op::Point<int> outputResolution;
        std::tie(scaleInputToNetInputs, netInputSizes, scaleInputToOutput, outputResolution) = scaleAndSizeExtractor.extract(imageSize);
        // Format input image to OpenPose input and output formats
        const auto netInputArray = cvMatToOpInput.createArray(inputImage, scaleInputToNetInputs, netInputSizes);
        auto outputArray = cvMatToOpOutput.createArray(inputImage, scaleInputToOutput, outputResolution);
        // Estimate poseKeypoints
        poseExtractorCaffe.forwardPass(netInputArray, imageSize, scaleInputToNetInputs);
        const auto poseKeypoints = poseExtractorCaffe.getPoseKeypoints();
        // Render poseKeypoints
        poseRenderer.renderPose(outputArray, poseKeypoints, scaleInputToOutput);
        // OpenPose output format to cv::Mat
        auto outputImage = opOutputToCvMat.formatToCvMat(outputArray);

        const auto now0 = std::chrono::high_resolution_clock::now();
        const auto totalTimeSec0 = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(now0-timerBegin).count() * 1e-9;
        const auto message0 = "OpenPose keypoint estimation finished. Total time: "
          + std::to_string(totalTimeSec0) + " seconds. \n Maintainable FPS:" + std::to_string(1/totalTimeSec0);
        op::log(message0, op::Priority::High);
        // ------------------------- SHOWING RESULT AND CLOSING -------------------------
        // Show results
        frameDisplayer.displayFrame(outputImage, 0); // Alternative: cv::imshow(outputImage) + cv::waitKey(0)
      }
      return 0;
    }
  catch (const std::exception& e)
    {
      op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
      return -1;
    }
}

int main(int argc, char *argv[])
{
  // Parsing command line flags
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // Running openPoseTutorialPose1
  return personLocator();
}
