/*
 * Copyright 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cstdint>

#include <math/mat4.h>
#include <ui/FenceTime.h>

// TODO(b/129481165): remove the #pragma below and fix conversion issues
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#pragma clang diagnostic ignored "-Wextra"

#include <ui/GraphicTypes.h>

// TODO(b/129481165): remove the #pragma below and fix conversion issues
#pragma clang diagnostic pop // ignored "-Wconversion -Wextra"

#include <compositionengine/ProjectionSpace.h>
#include <ui/Rect.h>
#include <ui/Region.h>
#include <ui/Transform.h>

namespace android {

namespace compositionengine::impl {

struct OutputCompositionState {
    // If false, composition will not be performed for this display
    bool isEnabled{false};

    // If false, this output is not considered secure
    bool isSecure{false};

    // If true, the current frame on this output uses client composition
    bool usesClientComposition{false};

    // If true, the current frame on this output uses device composition
    bool usesDeviceComposition{false};

    // If true, the client target should be flipped when performing client composition
    bool flipClientTarget{false};

    // RK: If true, the client target should be used afbc format
    bool useNoAfbcClientTarget{false};

    // If true, the current frame reused the buffer from a previous client composition
    bool reusedClientComposition{false};

    // If true, this output displays layers that are internal-only
    bool layerStackInternal{false};

    // The layer stack to display on this display
    uint32_t layerStackId{~0u};

    // The common space for all layers in the layer stack. layerStackSpace.content is the Rect
    // which gets projected on the display. The orientation of this space is always ROTATION_0.
    ProjectionSpace layerStackSpace;

    // Oriented physical display space. It will have the same size as displaySpace oriented to
    // match the orientation of layerStackSpace. The orientation of this space is always ROTATION_0.
    ProjectionSpace orientedDisplaySpace;

    // The space of the framebuffer. Its bounds match the size of the framebuffer and its
    // orientation matches the orientation of the display. Typically the framebuffer space will
    // be identical to the physical display space.
    ProjectionSpace framebufferSpace;

    // The space of the physical display. It is as big as the currently active display mode. The
    // content in this space can be rotated.
    ProjectionSpace displaySpace;

    // Transformation from layerStackSpace to displaySpace
    ui::Transform transform;

    // If true, RenderEngine filtering should be enabled
    bool needsFiltering{false};

    // The logical coordinates for the dirty region for the display.
    // dirtyRegion is semi-persistent state. Dirty rectangles are added to it
    // by the FE until composition happens, at which point it is cleared.
    Region dirtyRegion;

    // The logical coordinates for the undefined region for the display.
    // The undefined region is internal to the composition engine. It is
    // updated every time the geometry changes.
    Region undefinedRegion;

    // True if the last composition frame had visible layers
    bool lastCompositionHadVisibleLayers{false};

    // The color transform matrix to apply
    mat4 colorTransformMatrix;

    // Current active color mode
    ui::ColorMode colorMode{ui::ColorMode::NATIVE};

    // Current active render intent
    ui::RenderIntent renderIntent{ui::RenderIntent::COLORIMETRIC};

    // Current active dataspace
    ui::Dataspace dataspace{ui::Dataspace::UNKNOWN};

    // Current target dataspace
    ui::Dataspace targetDataspace{ui::Dataspace::UNKNOWN};

    // The earliest time to send the present command to the HAL
    std::chrono::steady_clock::time_point earliestPresentTime;

    // The previous present fence. Used together with earliestPresentTime
    // to prevent an early presentation of a frame.
    std::shared_ptr<FenceTime> previousPresentFence;

    // Current display brightness
    float displayBrightnessNits{-1.f};

    // SDR white point
    float sdrWhitePointNits{-1.f};

    // Debugging
    void dump(std::string& result) const;
};

} // namespace compositionengine::impl
} // namespace android
