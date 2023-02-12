/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/ObjExporter.h"

// C++ Standard Library includes
#include <stdexcept>

using namespace std;

namespace astu {

    void ObjExporter::AddPolygon2D(const std::vector<astu::Vector2d> &polygon, double z)
    {
        size_t idx = vertices.size();
        Face face;

        for (const auto &vertex : polygon) {
            vertices.push_back(transform.TransformPoint(Vector3d(vertex.x, vertex.y, z)));
            face.indices.push_back(++idx);
        }

        faces.push_back(face);
    }

    void ObjExporter::Export(const std::string &filename) const
    {
        std::ofstream ofs(filename.c_str(), std::ios::out);

        if (!ofs) {
            throw std::runtime_error("Unable to open '" + filename + "' to export OBJ data");
        }

        ExportVertices(ofs);
        ExportFaces(ofs);
        ofs.close();
    }

    void ObjExporter::ExportVertices(std::ofstream &ofs) const
    {
        ofs << endl;
        ofs << "# List of geometric vertices" << endl;

        for (const auto &vertex : vertices) {
            ofs << "v " << vertex.x << " " << vertex.y << " " << vertex.z << endl;
        }
    }

    void ObjExporter::ExportFaces(ofstream &ofs) const
    {
        ofs << endl;
        ofs << "# Polygonal face elements" << endl;

        for (const auto &face : faces) {
            ofs << "f";
            for (const auto &index : face.indices) {
                ofs << " " << index;
            }
            ofs << endl;
        }
    }

    void ObjExporter::ResetTransform()
    {
        transform.SetToIdentity();
    }

    void ObjExporter::Translate(double tx, double ty, double tz)
    {
        transform.Translate(tx, ty, tz);
    }

    void ObjExporter::Scale(double sx, double sy, double sz)
    {
        transform.Scale(sx, sy, sz);
    }

} // end of namespace