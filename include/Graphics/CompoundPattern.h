/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "Graphics/Pattern.h"

// C++ Standard Library includes
#include <memory>
#include <vector>

namespace astu {

    /**
     * Abstract base class for patterns that consist of multiple sub-patterns.
     */
    class CompoundPattern : public Pattern {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~CompoundPattern() = default;

        /**
         * Adds the specified pattern to this compound.
         *
         * @param child the pattern to add
         */
        void AddPattern(std::shared_ptr<Pattern> child);

        /**
         * Removes the specified pattern from this compound.
         *
         * @param child the child pattern to remove
         */
        void RemovePattern(std::shared_ptr<Pattern> child);

        /**
         * Removes all child patterns from this compound.
         */
        void Clear();

        bool IsEmpty() const {
            return children.empty();
        }

    	std::vector<std::shared_ptr<Pattern>>::iterator begin() {
            return children.begin();
        }

        std::vector<std::shared_ptr<Pattern>>::iterator end() {
            return children.end();
        }

    protected:
        /** The child patterns of this compound pattern. */
        std::vector<std::shared_ptr<Pattern>> children;

        virtual void OnPatternAdded(Pattern & pattern) {}
        virtual void OnPatternRemoved(Pattern & pattern) {}
        virtual void OnClear() {}
    };

} // end of namespace