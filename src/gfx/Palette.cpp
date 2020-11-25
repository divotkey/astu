/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 * 
 * Copyright (c) 2020 Roman Divotkey, Nora Loimayr. All rights reserved.
 */

#include <cassert>
#include <stdexcept>
#include <algorithm>
#include "Palette.h"

namespace astu {

    Palette::Palette(const Color & start, const Color & end)
    {
        AddColor(start, 0);
        AddColor(end, 1);
    }

    void Palette::SetStartColor(const Color & c)
    {
        assert(entries.size() >= 1);
        entries[0].color = c;
    }

    Color Palette::GetStartColor() const
    {
        assert(!entries.empty());
        return entries[0].color;
    }

    void Palette::SetEndColor(const Color & c)
    {
        assert(!entries.empty());
        entries.back().color = c;
    }

    Color Palette::GetEndColor() const
    {
        assert(entries.size() >= 1);
        return entries.back().color;
    }

    void Palette::AddColor(const Color & c, double p)
    {
        if (p < 0 || p > 1) {
            throw std::out_of_range("The position of a color within a color palette must lie in the inverval [0, 1].");
        }

        entries.push_back(Entry(c, p));
		std::sort(entries.begin(), entries.end());
    }

    Color Palette::GetColor(double t)
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
        // According to precondition, wie must have at least two elements.
        e1 = &entries[0];
        e2 = &entries[1];
    }

    Palette::Entry::Entry(const Color & c, double p)
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
