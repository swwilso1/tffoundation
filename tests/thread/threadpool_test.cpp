/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2022 to 2022 Tectiform Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
SOFTWARE.


******************************************************************************/

#include <iostream>
#include "TFFoundation.hpp"
#include "gtest/gtest.h"

using namespace TF::Foundation;

TEST(ThreadPool, start_stop_pool)
{
    ThreadPool pool{10};
    pool.start();
    pool.stop();
}

TEST(ThreadPool, single_job_pool)
{
    static const int pool_limit{4};
    bool jobs_ran[pool_limit];
    ThreadPool pool{pool_limit};
    pool.start();

    for (int i = 0; i < pool_limit; i++)
    {
        jobs_ran[i] = false;
        ThreadPool::pool_job job;
        job.job_functions.emplace_back([&jobs_ran, i]() {
            jobs_ran[i] = true;
        });
        pool.add_job(job);
    }
    Sleep(std::chrono::seconds(1));
    for (int i = 0; i < pool_limit; i++)
    {
        EXPECT_TRUE(jobs_ran[i]);
    }
    pool.stop();
}

TEST(ThreadPool, multi_job_pool)
{
    static const int pool_limit{8};
    bool jobs_ran[pool_limit];
    ThreadPool pool{pool_limit};
    pool.start();

    for (int i = 0; i < pool_limit; i += 2)
    {
        jobs_ran[i] = false;
        jobs_ran[i + 1] = false;
        ThreadPool::pool_job job;
        job.job_functions.emplace_back([&jobs_ran, i]() {
            jobs_ran[i] = true;
        });
        job.job_functions.emplace_back([&jobs_ran, i]() {
            jobs_ran[i + 1] = true;
        });
        pool.add_job(job);
    }
    Sleep(std::chrono::seconds(1));
    for (int i = 0; i < pool_limit; i++)
    {
        EXPECT_TRUE(jobs_ran[i]);
    }
    pool.stop();
}
