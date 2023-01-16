/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023 Roman Divotkey. All rights reserved.
 */

#pragma once

// Local includes
#include "ThreadStatus.h"
#include "Service/SignalService.h"
#include "Service/TimeService.h"
#include "Service/SignalServiceTs.h"
#include "Input/InputSignals.h"

// C++ Standard Library includes
#include <functional>
#include <vector>
#include <map>
#include <thread>
#include <mutex>

namespace astu {

    /**
     * This signal represents progress reports from threads.
     *
     * @ingroup srv_group
     */
    class ThreadProgressSignal {
    public:

        /**
         * Constructor.
         *
         * @param threadId  the numerical identifier of the thread
         */
        ThreadProgressSignal(int threadId, double progress) : threadId(threadId), progress(progress) {}

        /** The numerical identifier of the thread. */
        int threadId;

        /** The progress in percent. */
        double progress;
    };


    /**
     * Type definition for signal services that transmit thread progress report signals.
     *
     * @ingroup srv_group
     */
    using ThreadProgressSignalService = SignalServiceTs<ThreadProgressSignal>;


    /**
     * Type definition for signal listeners which receive thread progress report signals.
     *
     * @ingroup srv_group
     */
    using IThreadProgressListener = ISignalListener<ThreadProgressSignal>;


    /**
     * This signal represents status updates from threads.
     *
     * Note: When this signal arrives, it is not guaranteed that the retrieval of the information about the thread
     * will be successful. By the time the signal arrives, the information about the thread may have already been
     * cleared because the thread was terminated and the end of the sweeping cycle was completed at the same time.
     *
     * @ingroup srv_group
     */
    class ThreadStatusSignal {
    public:

        /**
         * Constructor.
         *
         * @param threadId  the numerical identifier of the thread
         * @param status    the thread status
         */
        ThreadStatusSignal(int threadId, ThreadStatus status)
            : threadId(threadId), status(status) {}

        /**
         * Constructor.
         *
         * @param threadId  the numerical identifier of the thread
         * @param status    the thread status
         * @param message   the thread status message
         */
        ThreadStatusSignal(int threadId, ThreadStatus status, const std::string &message)
                : threadId(threadId), status(status), message(message) {}

        /** The numerical identifier of the thread. */
        int threadId;

        /** The status of the thread. */
        ThreadStatus status;

        /** An optional message (used in case of an error, etc. ). */
        std::string message;
    };

    /**
     * Type definition for signal services that transmit thread status signals.
     *
     * @ingroup input_group
     */
    using ThreadStatusSignalService = SignalServiceTs<ThreadStatusSignal>;


    /**
     * Type definition for signal listeners which receive thread status signals.
     *
     * @ingroup srv_group
     */
    using IThreadStatusListener = ISignalListener<ThreadStatusSignal>;


    /**
     * Services can derive from this class to process thread status button signals.
     *
     * @ingroup srv_group
     */
    class ThreadStatusListener
        : virtual public Service
        , private IThreadStatusListener
    {
    public:

        /**
         * Constructor.
         */
        ThreadStatusListener() {
            AddStartupHook([this](){
                ASTU_SERVICE(ThreadStatusSignalService).AddListener(*this);
            });

            AddShutdownHook([this](){
                ASTU_SERVICE(ThreadStatusSignalService).RemoveListener(*this);
            });
        }

    protected:

        /**
         * Called by this base class when a thread status has changed to success.
         *
         * @param threadId  the ID of the thread
         * @return `true` if this signal has been consumed
         */
        virtual bool OnThreadSuccess(int threadId) { return false; }

        /**
         * Called by this base class when a thread status has changed to error.
         *
         * @param inThreadId    the ID of the thread
         * @param message       the error message
         * @return `true` if this signal has been consumed
         */
        virtual bool OnThreadError(int inThreadId, const std::string &message) { return false; }

        /**
         * Called by this base class when a thread status has changed to running.
         *
         * @param inThreadId    the ID of the thread
         * @return `true` if this signal has been consumed
         */
        virtual bool OnThreadRunning(int inThreadId) { return false; }


    private:

        // Inherited via IThreadStatusListener
        virtual bool OnSignal(const ThreadStatusSignal & signal) {
            switch (signal.status) {

                case ThreadStatus::Success:
                    return OnThreadSuccess(signal.threadId);

                case ThreadStatus::Error:
                    return OnThreadError(signal.threadId, signal.message);

                case ThreadStatus::Running:
                    return OnThreadRunning(signal.threadId);

                default:
                    return false;

            }
        }

    };


    /**
     * Manages execution of concurrent threads.
     * 
     * @ingroup srv_group
     */
    class ThreadService
        : virtual public Service
        , public Updatable
        , public TimeClient
        , public SignalEmitterTs<ThreadStatusSignal>
    {
    public:

        /** Constant describing an invalid thread identifier. */
        static const int INVALID_THREAD_ID = 0;

        /**
         * Constructor.
         *
         * @param updatePriority  the update priority
         */
        ThreadService(int updatePriority = Priority::VeryLow);

        /**
         * Type alias for thread functions.
         */
        using Delegate = std::function<void (int)>;

        /**
         * Virtual destructor.
         */
        virtual ~ThreadService() {}

        /**
         * Starts a new thread executing the specified delegate.
         *
         * @param delegate the delegate to execute
         * @return the numerical identifier of the newly started thread
         */
        int StartThread(Delegate delegate);

        bool HasThreadTs(int id) const;
        ThreadStatus GetStatusTs(int id) const;
        const std::string& GetMessageTs(int id) const;

    private:

        struct ThreadInfo {
            int infoId;
            std::thread::id threadId;
            Delegate delegate;
            ThreadStatus status;
            std::string messages;

            ThreadInfo(int infoId, std::thread::id threadId, Delegate delegate)
                    : infoId(infoId), threadId(threadId), delegate(delegate), status(ThreadStatus::Running) {}

            ThreadInfo() : infoId(0) {}
        };

        /** Used to generate unique thread identifiers. */
        int cntThreads;

        /** Contains status and additional information about threads. */
        std::map<int, ThreadInfo> infos;

        /** The threads. */
        std::vector<std::thread> threads;

        /** Used to synchronize access to thread infos map. */
        mutable std::mutex infoMutex;

        /** Defines the number of update cycles, before stale thread infos get removed. */
        int sweepInterval;

        /** The number of cycles of the current sweep interval. */
        int cntCycles;

        void ExecuteThread(int infoId);

        Delegate& GetDelegateTs(int infoId);
        void SetStatusTs(int infoId, ThreadStatus status);
        void SetStatusTs(int infoId, ThreadStatus status, const std::string& message);
        void VerifyInfoId(int infoId) const;
        void RemoveStaleInfos();

        // Inherited via Service
        void OnStartup() override;
        void OnShutdown() override;

        // Inherited via Updatable
        void OnUpdate() override;
    };

    /**
 * Convenient class used for easy and fast access to ThreadService
 * This class also functions as status listener for threads.
 *
 * @ingroup srv_group
 */
    class ThreadClient : virtual public ThreadStatusListener
    {
    public:

        /**
         * Constructor.
         */
        ThreadClient() {
            AddStartupHook([this](){
                threadService = ASTU_GET_SERVICE(ThreadService);
            });

            AddShutdownHook([this](){
                threadService = nullptr;
            });
        }

    protected:

        /**
         * Starts a new thread executing the specified delegate.
         *
         * @param delegate the delegate to execute
         * @return the numerical identifier of the newly started thread
         */
        int StartThread(ThreadService::Delegate delegate) {
            return threadService->StartThread(delegate);
        }

    private:
        /** Used for fast access to thread service. */
        std::shared_ptr<ThreadService> threadService;
    };


} // end of namespace