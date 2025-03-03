/*
 * Copyright 2019 Xilinx Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <glog/logging.h>
#include <json-c/json.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vitis/ai/demo_accuracy.hpp>

#include "movenet_onnx.hpp"

using namespace std;
using namespace cv;
string g_output_file;
extern int g_last_frame_id;

namespace vitis {
namespace ai {

struct MovenetAccThread : public AccThread {
  static std::shared_ptr<MovenetAccThread> instance() {
    static std::weak_ptr<MovenetAccThread> the_instance;
    std::shared_ptr<MovenetAccThread> ret;
    if (the_instance.expired()) {
      ret = std::make_shared<MovenetAccThread>();
      the_instance = ret;
    }
    ret = the_instance.lock();
    assert(ret != nullptr);
    return ret;
  }

  virtual int run() override {
    std::ofstream of(g_output_file, std::ofstream::out);
    of << "[";
    bool is_print = false;
    while (1) {
      if (is_print) {
        of << ", ";
      } else
        is_print = true;

      DpuResultInfo dpu_result;
      if (!queue_->pop(dpu_result, std::chrono::milliseconds(5000))) {
        LOG(INFO) << "pop dpu result time out";
        return 0;
      }
      void* r = dpu_result.result_ptr.get();
      auto img_w = dpu_result.w;
      auto img_h = dpu_result.h;
      auto result = (OnnxMovenetResult*)r;
      json_object* str_imageid =
          json_object_new_string(dpu_result.single_name.c_str());
      json_object* value = json_object_new_object();
      json_object_object_add(value, "image_id", str_imageid);
      json_object* pose_array = json_object_new_array();

      vector<Point2f> jo(17);
      vector<Point2f> j = result->poses;
      for (size_t i = 0; i < jo.size(); ++i) {
        // jo[i] = j[a[i]];
        json_object* point_array = json_object_new_array();
        jo[i] = j[i];
        json_object_array_add(point_array,
                              json_object_new_double(jo[i].x / img_w));
        json_object_array_add(point_array,
                              json_object_new_double(jo[i].y / img_h));
        json_object_array_add(pose_array, point_array);
      }

      json_object_object_add(value, "joint_self", pose_array);
      // cout << json_object_to_json_string(value);
      of << json_object_to_json_string(value);
      json_object_put(value);

      LOG_IF(INFO, 0) << "test movenet, queue size: " << queue_->size()
                      << " dpu_result id: " << dpu_result.frame_id;
      if (g_last_frame_id == int(dpu_result.frame_id)) {
        of << "]";
        of.close();
        LOG(INFO) << "test movenet accuracy done! byebye " << queue_->size();
        exit(0);
      }
    }
    return 0;
  }
};

}  // namespace ai
}  // namespace vitis

int main(int argc, char* argv[]) {
  string model = argv[1];
  g_output_file = argv[3];
  return vitis::ai::main_for_accuracy_demo(
      argc, argv, [model] { return OnnxMovenet::create(model); },
      vitis::ai::MovenetAccThread::instance(), 2);
}
