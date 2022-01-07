/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Graphics/Palette.h"

// C++ Standard Library includes
#include <cassert>
#include <stdexcept>
#include <algorithm>

namespace astu {

    Palette::Palette(const Color4d & start, const Color4d & end)
    {
        AddColor(start, 0);
        AddColor(end, 1);
    }

    Palette& Palette::SetStartColor(const Color4d & c)
    {
        assert(entries.size() >= 1);
        entries[0].color = c;
        return *this;
    }

    Color4d Palette::GetStartColor() const
    {
        assert(!entries.empty());
        return entries[0].color;
    }

    Palette& Palette::SetEndColor(const Color4d & c)
    {
        assert(!entries.empty());
        entries.back().color = c;
        return *this;
    }

    Color4d Palette::GetEndColor() const
    {
        assert(entries.size() >= 1);
        return entries.back().color;
    }

    Palette& Palette::AddColor(const Color4d & c, double p)
    {
        if (p < 0 || p > 1) {
            throw std::out_of_range("The position of a color within a color palette must lie in the interval [0, 1].");
        }

        entries.push_back(Entry(c, p));
		std::sort(entries.begin(), entries.end());

        return *this;
    }

    Color4d Palette::GetColor(double t) const
    {
        t = (t < 0.0) ? 0.0 : (1.0 < t) ? 1.0 : t;

        // t = std::clamp(t, 0.0, 1.0);

        const Entry *e1, *e2;
        FindBoundaries(t, e1, e2);

        assert(e2->pos >= e1->pos);
        t -= e1->pos;
        t /= e2->pos - e1->pos;
        return e1->color.Lerp(e2->color, t);
    }

    size_t Palette::size() const
    {
        return entries.size();
    }

    const Color4d & Palette::at(size_t idx) const
    {
        return entries.at(idx).color;
    }

    const Color4d & Palette::operator[](size_t idx) const
    {
        assert(idx < entries.size());
        return entries[idx].color;
    }

    void Palette::FindBoundaries(double t, const Entry *&e1, const Entry *&e2) const
    {
        // Preconditions for this method.
        assert(entries.size() >= 2);
        assert(entries[0].pos <= t);
        assert(entries.back().pos >= t);

        const Entry *prev = nullptr;
        for (const auto & entry : entries) {
			if (entry.pos > t) {
                assert(prev != nullptr);  // would violate the precondition
                e1 = prev;
                e2 = &entry;
                return;
            }
            prev = &entry;
        }

        // Must a must be exactly the first element.
        // According to precondition, we must have at least two elements.
        if (t == 0) {
            e1 = &entries[0];
            e2 = &entries[1];
        } else {
            e1 = &entries[entries.size() - 2];
            e2 = &entries[entries.size() - 1];
        }
    }

    Palette::Entry::Entry(const Color4d & c, double p)
        : color(c)
        , pos(p)
    {
        // Intentionally left empty.
    }

    bool Palette::Entry::operator<(const Entry & rhs) const
    {
        return pos < rhs.pos;
    }


} // end of namespace
