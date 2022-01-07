/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes.
#include "Util/Pooled.h"
#include "TaskService.h"

// C++ Standard Library includes.
#include <functional>
#include <memory>

namespace astu {

    /////////////////////////////////////////////////
    /////// TimedTask
    /////////////////////////////////////////////////

    /**
     * Base class for timed tasks.
     * 
     * Timed tasks are tasks that last only a certain amount of time.
     * 
     * @ingroup srv_group     
     */
    class TimedTask : public Task {
    public:

        // Inherited via Task
		virtual void Update(double dt) override;
		virtual void Reset() override;

    protected:

        /**
         * Constructor.
         */
        TimedTask();

        /**
         * Sets the duration of this timed task.
         * 
         * @param duration  the duration in seconds
         * @throws std::domain error in case the duration is less than zero
         */
        void SetDuration(double duration);

        /**
         * Returns the duration of this timed task.
         * 
         * @return the duration in seconds
         */
        double GetDuration() const {
            return duration;
        }

		/**
		 * Called by this base class.
		 *
		 * @param t percentage time value in the range [0, 1]
		 */
        virtual void Process(double t) = 0;

    private:
        /** The current time for this task. */
        double time;

        /** The duration of this timed tasked. */
        double duration;
    };

    /////////////////////////////////////////////////
    /////// DelegateTask
    /////////////////////////////////////////////////

    /**
     * Calls a delegate after a specific amount of time.
     */
    class DelegateTask : public Task, public Pooled<DelegateTask> {
    public:

        /**
         * Type alias for delegate functions.
         */
        using Delegate = std::function<void (void)>;

        /**
         * Returns a new delegate instnace.
         * 
         * @param delegate  the delegate to call
         * @param delay     the time to wait in seconds
         */
        static std::unique_ptr<DelegateTask> Create(Delegate delegate, double delay = 1);

        /** Virtual destructor. */
        virtual ~DelegateTask();

		// Disable copy and assignment.
		DelegateTask(const DelegateTask&) = delete;
		DelegateTask & operator=(const DelegateTask&) = delete;

        // Inherited via Task
        virtual void Update(double dt) override;
        virtual void Reset() override;

    public:
        /** The duration to wait before to call the delegate. */
        double delay;

        /** The elapsed time since the tasks has been reseted. */
        double elapsedTime;

		/** The delegate function to be called. */
		Delegate delegateFunc;

    private:

        /***
         * Constructor.
         * 
         * @param delegate  the delegate function to call
         * @param delay     the delay to wait in seconds
         */
        DelegateTask(Delegate delegate, double delay);
    };

    /**
     * Builder for DelegateTask instances.
     * 
     * @ingroup srv_group     
     */
    class DelegateTaskBuilder final : public TaskBuilder<DelegateTaskBuilder> {
    public:

        /**
         * Constructor.
         */
        DelegateTaskBuilder();

        /**
         * Specifies the delegate to be called by the task to build.
         * 
         * @param delegate  the delegate function
         * @return reference to this builder for method chaining
         */
        DelegateTaskBuilder& Delegate(DelegateTask::Delegate delegate);

        /**
         * Returns the delegate used for the task to build.
         * *
         * @return the delegate
         */
        DelegateTask::Delegate GetDelegate() const {
            return delegateFunc;
        }

        /**
         * Specifies the delay for the delegate task to build.
         * 
         * @param delay the delay in seconds
         * @return reference to this builder for method chaining
         */
        DelegateTaskBuilder& Delay(double delay);

        /**
         * Returns the delay used for the task to build.
         * *
         * @return the delay in seconds
         */
        double GetDelay() const {
            return delay;
        }

        /**
         * Resets this builder to is initial configuration.
         * 
         * @return reference to this builder for method chaining
         */
        DelegateTaskBuilder& Reset();

        /**
         * Builds a delegate task according to the current configuration.
         * 
         * @return the newly created delegate task
         */
        std::unique_ptr<DelegateTask> Build();

    private:
        /** The delay of the delegate task to build. */
        double delay;

        /** The delegate of the delegate task. */
        DelegateTask::Delegate delegateFunc;
    };

} // end of namespace