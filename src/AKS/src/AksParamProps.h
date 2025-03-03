/*
 * Copyright 2019 Xilinx Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __AKS_PARAM_PROPS_H_
#define __AKS_PARAM_PROPS_H_

#include <string>
#include <map>

using namespace std;

#include "AksCommonDefs.h"

namespace AKS
{
  //TODO: make it templetized to store default value
  struct ParamProps
  {
    KernelParamType  type;
    bool         optional;
    //T          default;
    void         dump(string prefix);
  };

}

#endif

