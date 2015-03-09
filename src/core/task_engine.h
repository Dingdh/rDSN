/*
 * The MIT License (MIT)

 * Copyright (c) 2015 Microsoft Corporation

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
# pragma once

# include "service_engine.h"
# include <rdsn/internal/task_queue.h>
# include <rdsn/internal/admission_controller.h>
# include <rdsn/internal/perf_counter.h>
# include <rdsn/internal/task_worker.h>

namespace rdsn {

class task_engine;
class task_worker_pool;
class task_worker;

//
// a task_worker_pool is a set of TaskWorkers share the same configs;
// they may even share the same task_queue when partitioned == true
//
class task_worker_pool
{
public:
    task_worker_pool(const threadpool_spec& opts, task_engine* owner);

    // service management
    void start();    

    // task procecessing
    void enqueue_task(task_ptr& task);
    void on_dequeue(int count);

    // inquery
    const threadpool_spec& spec() const { return _spec; }
    bool shared_same_worker_with_current_task(task* task) const;
    task_engine* engine() const { return _owner; }
    service_node* node() const { return _node; }

private:
    threadpool_spec                    _spec;
    task_engine*                       _owner;
    service_node*                      _node;

    std::vector<task_worker*>          _workers;
    std::vector<task_queue*>           _queues;
    std::vector<admission_controller*> _controllers;

    bool                              _is_running;
    perf_counter_ptr  _pending_task_counter;
};

class task_engine
{
public:
    task_engine(service_node* node);

    //
    // service management routines
    //
    void start(const std::vector<threadpool_spec>& spec);

    //
    // task management routines
    //
    task_worker_pool* get_pool(int code) const { return _pools[code]; }

    bool is_started() const { return _is_running; }

    service_node* node() const { return _node; }
    
private:
    std::vector<task_worker_pool*> _pools;
    volatile bool                _is_running;
    service_node*                 _node;
};

// -------------------- inline implementation ----------------------------

} // end namespace
