/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

#include <functional>
#include <vector>

namespace astu {

    /**
     * This class represents a universal implementation of the 
     * command design pattern using C++ lambda expressions.
     */
    class CommandQueue {
    public:

        using Command = std::function<void (void)>;

        /**
         * Adds a command to the queue.
         * 
         * @param cmd   the command to add
         */
        void Add(Command cmd)
        {
            commands.push_back(cmd);
        }

        /**
         * Clears all pending pendingCommands.
         */
        void Clear() {
            return commands.clear();
        }

        /**
         * Returns the number of queued dommands.
         * 
         * @return the number of pendingCommands in the queue
         */
        size_t Size() {
            return commands.size();
        }

        /**
         * Executes all pending pendingCommands.
         */
        void Execute() {
            for (auto const & cmd : commands) {
                cmd();
            }
            commands.clear();
        }

    private:
        /** The queued pendingCommands. */
        std::vector<Command> commands;
    };

} // end of namespace