/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/PatternRenderer.h"
#include "Graphics/Image.h"

namespace astu {

    PatternRenderer::PatternRenderer(PatternRenderer::TransformMode mode)
            : mode(mode)
    {
        // Intentionally left empty.
    }

    void PatternRenderer::SetTransformMode(TransformMode inMode)
    {
        mode = inMode;
    }

    PatternRenderer::TransformMode PatternRenderer::GetTransformMode() const
    {
        return mode;
    }

    void PatternRenderer::Render(const Pattern &pattern, Image &result)
    {
        progressSignalService = ASTU_GET_SERVICE_OR_NULL(ProgressSignalService);
        UpdateTransform(result.GetWidth(), result.GetHeight());
        DoRender(pattern, result);
        ReportProgress(1.0);
    }

    void PatternRenderer::UpdateTransform(double imageWidth, double imageHeight)
    {
        switch (mode) {
            case TransformMode::RAW:
                transform.SetToIdentity();
                break;

            case TransformMode::CENTERED:
                transform.SetToTranslate(-imageWidth * 0.5, -imageHeight * 0.5);
                break;

            case TransformMode::H_NORMALIZED:
                transform.SetToTranslate(-imageWidth * 0.5, -imageHeight * 0.5);
                transform.Scale(1.0 / imageWidth, 1.0 / imageWidth);
                break;

            case TransformMode::V_NORMALIZED:
                transform.SetToTranslate(-imageWidth * 0.5, -imageHeight * 0.5);
                transform.Scale(1.0 / imageHeight, 1.0 / imageHeight);
                break;

            case TransformMode::NORMALIZED:
                transform.SetToTranslate(-imageWidth * 0.5, -imageHeight * 0.5);
                transform.Scale(1.0 / imageWidth, 1.0 / imageHeight);
                break;
        }
    }

    void PatternRenderer::ReportProgress(double completed)
    {
        if (!progressSignalService)
            return;

        progressSignalService->QueueSignalTs(ProgressSignal(completed));
    }

} // end of namespace