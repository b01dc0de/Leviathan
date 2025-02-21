#include "PerformanceGraph.h"

namespace Leviathan
{
    namespace PerformanceGraph
    {
        constexpr float Ms08_Baseline = 0.25f;
        constexpr float Ms16_Baseline = 0.5f;
        constexpr float Ms33_Baseline = 0.75f;
        constexpr int NumSamples = 120;
        constexpr int NumGraphLines = 4;

        float FrameTimeSamples[NumSamples];
        int LastSampleIdx = 0;

        void Init();
        void Tick();
        void Draw(BatchDraw2D& Draw2D, v2f Origin, float Scale);
    }

    void PerformanceGraph::Init()
    {
        constexpr float DefaultSample = 1000.0f / 60.0f;
        for (int SampleIdx = 0; SampleIdx < NumSamples; SampleIdx++)
        {
            FrameTimeSamples[SampleIdx] = DefaultSample;
        }
    }

    void PerformanceGraph::Tick()
    {
        LastSampleIdx = (LastSampleIdx + 1) % NumSamples;
        FrameTimeSamples[LastSampleIdx] = Clock::DeltaTime();
    }

    void PerformanceGraph::Draw(BatchDraw2D& Draw2D, v2f Origin, float Scale)
    {
        // Add graph background
        constexpr fColor BackgroundColor{ Norm8Bit(0, 0, 0) };
        constexpr fColor GraphLinesColor{ Norm8Bit(255, 255, 255) };
        constexpr fColor GraphTimingsColor[] =
        {
            Norm8Bit(0, 255, 0),
            Norm8Bit(255, 255, 0),
            Norm8Bit(255, 0, 0),
        };
        constexpr fColor GraphPointsColor{ Norm8Bit(255, 0, 0) };
        float VisualGraphSize = Scale;
        Draw2D.AddRect(RectF{ Origin.X, Origin.Y, VisualGraphSize, VisualGraphSize }, BackgroundColor);
        // Add graph lines
        constexpr float LineThickness = 2.0f;
        constexpr float HalfLineThickness = LineThickness / 2.0f;
        float VisualCellSize = VisualGraphSize / NumGraphLines;
        for (int LineIdx = 1; LineIdx < NumGraphLines; LineIdx++)
        {
            // Row line
            float LineY = Origin.Y + LineIdx * VisualCellSize - HalfLineThickness;
            fColor TimingsLineColor = GraphTimingsColor[LineIdx - 1];
            Draw2D.AddRect(RectF{ Origin.X, LineY, VisualGraphSize, LineThickness }, TimingsLineColor );

            // Col line
            float LineX = Origin.X + LineIdx * VisualCellSize - HalfLineThickness;
            Draw2D.AddRect(RectF{ LineX, Origin.Y, LineThickness, VisualGraphSize }, GraphLinesColor);
        }
        // Add last average line
        // Draw average
        static float SampleAvgY = 0.0f;
        Draw2D.AddRect(RectF{ Origin.X, SampleAvgY, VisualGraphSize, LineThickness }, Norm8Bit(0, 0, 255));
        // Add graph outline
        Draw2D.AddBox(RectF{ Origin.X, Origin.Y, VisualGraphSize, VisualGraphSize }, GraphLinesColor);

        float SamplePointSize = VisualGraphSize / (float)NumSamples;
        float AdjSamplePointSize = SamplePointSize * 1.5f;
        constexpr float Baseline60FPS = 1000.0f / 60.0f;
        float SampleSum = 0.0f;
        for (int SampleIdx = 0; SampleIdx < NumSamples; SampleIdx++)
        {
            int AdjSampleIdx = (LastSampleIdx + NumSamples - SampleIdx) % NumSamples;
            float SampleVal = FrameTimeSamples[AdjSampleIdx] * 1000.0f;
            RectF PointRect
            {
                Origin.X + VisualGraphSize - ((SampleIdx + 1) * SamplePointSize),
                Origin.Y + (SampleVal / Baseline60FPS * VisualGraphSize),
                AdjSamplePointSize, AdjSamplePointSize
            };
            SampleSum += PointRect.PosY;
            Draw2D.AddRect(PointRect, GraphPointsColor);
        }

        SampleAvgY = SampleSum / NumSamples;
    }

    void PerformanceGraph::Draw(BatchDraw2D& Draw2D)
    {
        static bool bInit = false;
        if (!bInit) { Init(); bInit = true; }
        constexpr v2f Origin{ 1100.0f, 720.0f * 0.25f };
        constexpr float Scale = 150.0f;
        Tick();
        Draw(Draw2D, Origin, Scale);
    }
}
