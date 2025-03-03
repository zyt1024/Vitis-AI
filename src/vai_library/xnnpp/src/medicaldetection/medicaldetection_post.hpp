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
#pragma once
#include "./ssd_detector.hpp"
#include "vitis/ai/nnpp/medicaldetection.hpp"

namespace vitis {
namespace ai {
namespace medicaldetection {

class MedicalDetectionPost : public vitis::ai::MedicalDetectionPostProcess {
 public:
  MedicalDetectionPost(
      const std::vector<vitis::ai::library::InputTensor>& input_tensors,
      const std::vector<vitis::ai::library::OutputTensor>& output_tensors,
      const vitis::ai::proto::DpuModelParam& config, int& real_batch_size);
  virtual ~MedicalDetectionPost();

  virtual MedicalDetectionResult medicaldetection_post_process(
      unsigned int idx) override;
  virtual std::vector<MedicalDetectionResult> medicaldetection_post_process()
      override;

 private:
  std::vector<std::vector<float>> priors_;
  std::unique_ptr<vitis::ai::medicaldetection::SSDDetector> detector_;

  const std::vector<vitis::ai::library::InputTensor> input_tensors_;
  const std::vector<vitis::ai::library::OutputTensor> output_tensors_;
  int& real_batch_size;
  int ARM_CONF_IDX = 0;
  int ARM_LOC_IDX = 1;
  int ODM_CONF_IDX = 2;
  int ODM_LOC_IDX = 3;
};

}  // namespace medicaldetection
}  // namespace ai
}  // namespace vitis
