/*
 * ASTU - AST Utilities
 * A collection of Utilities for Applied Software Techniques (AST).
 *
 * Copyright (c) 2020-2023. Roman Divotkey. All rights reserved.
 */

// Local includes
#include "Service/TaskService.h"

namespace astu {

    TaskService::TaskService(int updatePriority)
        : Service("Task Service")
        , Updatable(updatePriority)
    {
        // Intentionally left empty.
    }

    void TaskService::AddTask(std::unique_ptr<Task> task)
    {
        tasks.push_back(std::move(task));
    }

    void TaskService::OnStartup() 
    {
        updating = false;
    }

    void TaskService::OnShutdown() 
    {
        tasks.clear();
    }

    void TaskService::RemoveAll(const std::string &name)
    {
        if (updating) {
            throw std::logic_error("Unable to remove tasks while updating");
        }
        
        for (auto it = tasks.begin(); it != tasks.end();) {
            if ((*it)->GetName() == name) {
                it = tasks.erase(it);
            } else {
                ++it;        
            }
        }
    }

    void TaskService::OnUpdate()
    {
        updating = true;
        auto dt = GetElapsedTime();
        for (auto it = tasks.begin(); it != tasks.end();) {
            Task &task = **it;
            if (task.IsTerminated()) {
                it = tasks.erase(it);
            } else {
                task.Update(dt);
                ++it;
            }
        }
        updating = false;
    }

} // end of namespace