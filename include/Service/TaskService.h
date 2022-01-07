/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes.
#include "Service/UpdateService.h"
#include "TimeService.h"

// C++ Standard Library includes.
#include <algorithm>
#include <memory>
#include <vector>

namespace astu {

    /**
     * Base class for tasks.
     * 
     * @ingroup srv_group     
     */
    class Task {
    public:

        /**
         * Constructor.
         * 
         * @param taskName  the name of this task
         */
        Task(const std::string & taskName = "") 
            : name(taskName) 
            , terminated(false) 
        {
            // Intentionally left empty.            
        }

        /**
         * Returns the name of this task.
         * 
         * @return the name of this task
         */
        const std::string& GetName() const {
            return name;
        }

        /** Virtual destructor. */
        virtual ~Task() {}

		/**
		 * Returns whether this actions has terminated.
		 *
		 * @return `true` if this action has terminated
		 */
		bool IsTerminated() const {
			return terminated;
		}

        /**
         * Terminates this Task.
         */
        void Terminate() {
            OnTerminate();
            terminated = true;
        }

        /**
         * Resets this task to its initial condition.
         */
        virtual void Reset() {
            terminated = false;
        }

        /**
         * Updates this task.
         * 
         * @param dt    the elapsed time since tha last update in seconds
         */
        virtual void Update(double dt) = 0;

    protected:

        /**
         * Called by this base class before termination.
         */
        virtual void OnTerminate() {}

    private:
        /** The name of this task. */
        std::string name;

        /** Whether this task has terminated and can be recycled. */
        bool terminated;

        template <typename T> friend class TaskBuilder;
    };


    /**
     * Base class for task builders.
     * 
     * @ingroup srv_group     
     */
    template <typename T>
    class TaskBuilder {
    public:

        /**
         * Specifies the name of the task to build.
         * 
         * @param name  the name of the task
         * @return a reference to this builder for method chaining
         */
        T& Name(const std::string & name) {
            taskName = name;
            return static_cast<T&>(*this);
        }

        /**
         * Resets this builder to its initial condition.
         * 
         * @return a reference to this builder for method chaining
         */
        T& Reset() {
            taskName = "";
            return static_cast<T&>(*this);
        }

    protected:

        /**
         * Called by derived classes
         */
        void Build(Task &task) {
            task.name = taskName;
        }

    private:
        /** The name of the task to build. */
        std::string taskName;
    };

    /**
     * This service executes is the main facility for Tasks.
     * 
     * @ingroup srv_group     
     */
    class TaskService 
        : virtual public Service
        , private Updatable
        , private TimeClient
    {
    public:

        /**
         * Constructor.
         * 
         * @param updatePriority    the priority to update this service
         */
        TaskService(int updatePriority = Priority::Normal);

        /**
         * Adds a task for execution.
         * 
         * @param task  the task to add
         */
        void AddTask(std::unique_ptr<Task> task);

        /**
         * Removes all tasks with the specified name.
         * This method must not be called by a task, executed by this service.
         * 
         * @param name  the name
         * @throws std::logic_error if this service is currently updating
         */
        void RemoveAll(const std::string &name);

    private:
        /** The currently executed tasks. */
        std::vector<std::unique_ptr<Task>> tasks;

        /** Whether this service is currently updating. */
        bool updating;

        // Inherited via Updatable
        virtual void OnStartup() override;
        virtual void OnShutdown() override;
        virtual void OnUpdate() override;
    };

} // end of namespace