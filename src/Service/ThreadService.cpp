/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020 - 2022 Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Service/ThreadService.h"

// C++ Standard Library includes
#include <stdexcept>

using namespace std;

/** Defines the used sweep interval. Make configurable, if and when required. */
#define SWEEP_INTERVAL 100

namespace astu {

    ThreadService::ThreadService(int updatePriority)
        : Service("Thread Service"), Updatable(updatePriority), sweepInterval(SWEEP_INTERVAL)
    {
        // Intentionally left empty
    }

    void ThreadService::OnStartup()
    {
        cntThreads = 0;
        cntCycles = 0;
    }

    void ThreadService::OnShutdown()
    {
        // Intentionally left empty
    }

    int ThreadService::StartThread(ThreadService::Delegate delegate)
    {
        std::lock_guard<std::mutex> lock(infoMutex);

        const auto infoId = ++cntThreads;

        threads.push_back(thread(&ThreadService::ExecuteThread, this, infoId));
        infos[infoId] = ThreadInfo(infoId, threads.back().get_id(), delegate);

        return infoId;
    }

    void ThreadService::ExecuteThread(int infoId)
    {
        auto delegate = GetDelegateTs(infoId);

        try {
            delegate(infoId);
            SetStatusTs(infoId, ThreadStatus::Success);
        } catch (const std::exception & e) {
            SetStatusTs(infoId, ThreadStatus::Error, e.what());
        }

    }

    ThreadService::Delegate &ThreadService::GetDelegateTs(int infoId)
    {
        std::lock_guard<std::mutex> lock(infoMutex);
        VerifyInfoId(infoId);
        return infos.at(infoId).delegate;
    }

    void ThreadService::SetStatusTs(int infoId, ThreadStatus status)
    {
        std::lock_guard<std::mutex> lock(infoMutex);
        VerifyInfoId(infoId);
        infos.at(infoId).status = status;

        QueueSignalTs(ThreadStatusSignal(infoId, status));
    }

    void ThreadService::SetStatusTs(int infoId, ThreadStatus status, const string &message)
    {
        std::lock_guard<std::mutex> lock(infoMutex);
        VerifyInfoId(infoId);

        auto &info = infos.at(infoId);
        info.status = status;
        info.messages = message;

        QueueSignalTs(ThreadStatusSignal(infoId, status));
    }

    ThreadStatus ThreadService::GetStatusTs(int infoId) const
    {
        std::lock_guard<std::mutex> lock(infoMutex);
        auto it = infos.find(infoId);
        if (it == infos.end()) {
            return ThreadStatus::Undefined;
        }
        return it->second.status;
    }

    bool ThreadService::HasThreadTs(int infoId) const
    {
        std::lock_guard<std::mutex> lock(infoMutex);
        return infos.find(infoId) != infos.end();
    }

    const std::string &ThreadService::GetMessageTs(int infoId) const
    {
        std::lock_guard<std::mutex> lock(infoMutex);
        VerifyInfoId(infoId);
        return infos.at(infoId).messages;
    }

    void ThreadService::VerifyInfoId(int infoId) const
    {
        auto it = infos.find(infoId);
        if (it == infos.end()) {
            throw std::logic_error("Unknown thread info identifier: " + to_string(infoId));
        }
    }

    void ThreadService::OnUpdate()
    {
        if (++cntCycles >= sweepInterval) {
            RemoveStaleInfos();
            cntCycles = 0;
        }
    }

    void ThreadService::RemoveStaleInfos()
    {
        std::lock_guard<std::mutex> lock(infoMutex);
        for (auto it = infos.begin(); it != infos.end();) {
            if (it->second.status != ThreadStatus::Running) {
                it = infos.erase(it);
            } else {
                ++it;
            }
        }
    }

} // end of namespace