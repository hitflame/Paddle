/* Copyright (c) 2016 Baidu, Inc. All Rights Reserve.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */


#pragma once

#include "Layer.h"
#include "paddle/math/Matrix.h"

namespace paddle {

/**
 * @brief Expand feature map to minibatch matrix.
 * - matrix width is: blockH_ * blockW_ * channels_
 * - matirx height is: outputH_ * outputW_
 * \f[
 * outputH_ = 1 + (2 * paddingH_ + imgSizeH_ - blockH_ + strideH_ - 1) /
 *            strideH_;
 * outputW_ = 1 + (2 * paddingW_ + imgSizeW_ - blockW_ + strideW_ - 1) /
 *            strideW_;
 * \f]
 * The expand method is same with ExpandConvLayer, but saved the transposed
 * value. After expanding, output_.sequenceStartPositions will store timeline.
 * The number of time steps are outputH_ * outputW_ and the dimension of each
 * time step is blockH_ * blockW_ * channels_. This layer can be used after
 * convolution neural network, and before recurrent neural network.
 *
 * The config file api is block_expand_layer.
 */
class BlockExpandLayer : public Layer {
protected:
  /**
   * @brief Calculate outputH_ and outputW_ and return block number which
   * actually is time steps.
   * @return time steps, outoutH_ * outputW_.
   */
  size_t getBlockNum();
  size_t blockH_, blockW_, strideH_, strideW_, paddingH_, paddingW_;
  size_t imgSizeH_, imgSizeW_, outputH_, outputW_, channels_;

  /// auxiliary variable, which saves the transposed output value.
  MatrixPtr outVTrans_;

public:
  explicit BlockExpandLayer(const LayerConfig& config) : Layer(config) {}

  ~BlockExpandLayer() {}

  virtual bool init(const LayerMap& layerMap, const ParameterMap& parameterMap);

  virtual void forward(PassType passType);
  virtual void backward(const UpdateCallback& callback = nullptr);
};

}  // namespace paddle
