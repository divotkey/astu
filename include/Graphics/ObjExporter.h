/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Velox/Interpreter/ItemData.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix4.h"

// C++ Standard Library includes
#include <vector>
#include <string>
#include <fstream>

namespace astu {

    class ObjExporter : public velox::ItemData {
    public:

        /**
         * Constructor.
         */
        ObjExporter() = default;

        void AddPolygon2D(const std::vector<astu::Vector2d> &vertices, double z = 0);

        void Export(const std::string& filename) const;

        void ResetTransform();

        /**
         * Translates this pattern by the specified translation vector.
         *
         * @param vt    the translation vector
         */
        void Translate(const Vector3d & vt) {
            Translate(vt.x, vt.y, vt.z);
        }

        /**
         * Translates this pattern by the specified translation vector.
         *
         * @param tx    the x-component of the translation vector
         * @param ty    the y-component of the translation vector
         * @param ty    the z-component of the translation vector
         */
        void Translate(double tx, double ty, double tz);

        /**
         * Scales this pattern by the specified scaling vector.
         *
         * @param vt    the scaling vector
         */
        void Scale(const Vector3d & vs) {
            Scale(vs.x, vs.y, vs.z);
        }

        /**
         * Scales this pattern by the specified scaling vector.
         *
         * @param sx    the x-component of the scaling vector
         * @param sy    the y-component of the scaling vector
         * @param sz    the z-component of the scaling vector
         */
        void Scale(double sx, double sy, double sz);


    private:
        struct Face {
            std::vector<size_t> indices;
        };

        /** The transformation matrix of this pattern. */
        Matrix4d transform;

        /** The list of vertices to export. */
        std::vector<astu::Vector3d> vertices;

        /** The list of faces. */
        std::vector<Face> faces;


        void ExportVertices(std::ofstream &ofs) const;
        void ExportFaces(std::ofstream &ofs) const;
    };

} // end of namespace