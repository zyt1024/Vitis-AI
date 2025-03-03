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

#include <vitis/ai/benchmark.hpp>
#include "movenet_onnx.hpp"

int main(int argc, char* argv[]) {
  auto model_name = std::string(argv[1]);

  return vitis::ai::main_for_performance(
        argc, 
        argv, 
        [model_name] {
             return OnnxMovenet::create(model_name); 
        }
  );
}


