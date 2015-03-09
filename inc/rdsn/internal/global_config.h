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

# include <string>
# include <rdsn/internal/configuration.h>
# include <rdsn/internal/threadpool_code.h>
# include <rdsn/internal/task_code.h>
# include <map>

namespace rdsn {

struct service_app_spec
{
    std::string name;
    std::string type;
    std::string arguments;
    int         port;
    int         delay_seconds;
    bool        run;

    service_app_spec() {}
    service_app_spec(const service_app_spec& r);
    bool init(const char* section, configuration_ptr config);
};

struct service_spec
{
    configuration_ptr            config;

    std::string                  tool;
    std::list<std::string>       toollets;
    int                          port;    
    
    std::map<rpc_channel, std::string> network_factory_names;
    std::string                  aio_factory_name;
    std::string                  env_factory_name;
    std::string                  lock_factory_name;
    std::string                  rwlock_factory_name;
    std::string                  semaphore_factory_name;

    std::list<std::string>       network_aspects; // applied to all network factories
    std::list<std::string>       aio_aspects;
    std::list<std::string>       env_aspects;
    std::list<std::string>       lock_aspects;
    std::list<std::string>       rwlock_aspects;
    std::list<std::string>       semaphore_aspects;

    std::string                  perf_counter_factory_name;
    std::string                  logging_factory_name;
    
    std::vector<threadpool_spec>  threadpool_specs;

    std::vector<service_app_spec>  app_specs;

    service_spec() {}

    bool init(configuration_ptr config);
};

enum syste_exit_type
{
    SYS_EXIT_NORMAL,
    SYS_EXIT_BREAK, // Ctrl-C/Break,Shutdown,LogOff, see SetConsoleCtrlHandler
    SYS_EXIT_EXCEPTION,

    SYS_EXIT_INVALID
};

ENUM_BEGIN(syste_exit_type, SYS_EXIT_INVALID)
    ENUM_REG(SYS_EXIT_NORMAL)
    ENUM_REG(SYS_EXIT_BREAK)
    ENUM_REG(SYS_EXIT_EXCEPTION)
ENUM_END(syste_exit_type)

}

