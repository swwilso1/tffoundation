/******************************************************************************

Tectiform Open Source License (TOS)

Copyright (c) 2015 to 2022 Tectiform Inc.

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
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


******************************************************************************/

#ifndef TFMULTIQUEUE_HXX
#define TFMULTIQUEUE_HXX

#define NEEDS_CASSERT
#define NEEDS_CSTDINT
#define NEEDS_MEMORY
#define NEEDS_MUTEX
#include "tfheaders.hpp"
#include "tftypes.hpp"
#include "tfallocator.hpp"
#include "tfdefer.hpp"

namespace TF
{

    namespace Foundation
    {

        namespace multiqueue_internal
        {
            /**
             * MultiQueueBlock is the basic entry in the queue linked list.  It
             * contains the object stored in the queue.
             * @tparam T type of the object stored in the block.
             */
            template<typename T>
            class MultiQueueBlock
            {
            public:
                /**
                 * @brief constructor to initialize the block with the new object
                 * @param t the new object
                 */
                explicit MultiQueueBlock(const T & t) : m_object{t} {}

                /**
                 * @brief getter method to get a reference to the object.
                 * @return a referernce to the object.
                 */
                T & get_object()
                {
                    return m_object;
                }

                /**
                 * @brief setter method to update the object in the block
                 * @param t the new object.
                 */
                void set_object(const T & t)
                {
                    m_object = t;
                }

                /**
                 * @brief get the number of references to the block
                 * @return the number of references
                 */
                size_t get_reference_count() const
                {
                    return m_reference_count;
                }

                /**
                 * @brief method to explicitly set the block reference count.
                 * @param s the new reference count
                 */
                void set_reference_count(size_t s)
                {
                    m_reference_count = s;
                }

                /**
                 * @brief method to increment the reference count by 1.
                 */
                void increment_reference()
                {
                    ++m_reference_count;
                }

                /**
                 * @brief method to decrement the reference count by 1.
                 */
                void decrement_reference()
                {
                    --m_reference_count;
                }

                /**
                 * @brief method to get the pointer to the next block in the list
                 * @return the pointer to the next block, may be nullptr.
                 */
                MultiQueueBlock * get_next() const
                {
                    return m_next;
                }

                /**
                 * @brief setter method to set the pointer to a new block.
                 * @param b the new block.
                 */
                void set_next(MultiQueueBlock * b)
                {
                    m_next = b;
                }

            private:
                MultiQueueBlock * m_next{nullptr};
                size_t m_reference_count{0};
                T m_object{};
            };

            /**
             * MultiQueueCore is queue container that is shared between instances
             * of forked queues. It contains a reference to the head of the queue list,
             * and references to the end of the list for fast insertion at the end of
             * the list.
             * @tparam T the type of the object stored in the queue.
             */
            template<typename T>
            class MultiQueueCore
            {
            public:
                /**
                 * @brief default constructor.  Using MultiQueueCore() = default caused
                 * a crash.
                 */
                MultiQueueCore() {}

                /**
                 * @brief destructor
                 *
                 * The destructor should only be called when the reference count to the
                 * core object goes to zero.
                 */
                ~MultiQueueCore()
                {
                    if (m_mutex)
                    {
                        m_mutex->lock();
                    }

                    MultiQueueBlock<T> * tmp = m_head;
                    while (tmp != nullptr)
                    {
                        MultiQueueBlock<T> * next = tmp->get_next();
                        delete tmp;
                        tmp = next;
                    }

                    if (m_mutex)
                    {
                        m_mutex->unlock();
                        delete m_mutex;
                    }
                }

                /**
                 * @brief getter method to get the head of the block list
                 * @return the head of the list, may be nullptr
                 */
                MultiQueueBlock<T> * get_head()
                {
                    return m_head;
                }

                /**
                 * @brief method to check if a block is the at the end of the queue.
                 * @param block the block to compare
                 * @return true if the block is the last block in the list.
                 */
                bool block_is_back_of_queue(MultiQueueBlock<T> * block)
                {
                    return m_back_of_queue == block;
                }

                /**
                 * @brief getter to get the last block in the list.
                 * @return the pointer to the last block in the list.
                 */
                MultiQueueBlock<T> * get_back()
                {
                    return m_back_of_queue;
                }

                /**
                 * @brief method to push a new object on the back of the list
                 * @param t the new object
                 * @return true if the push succeeded and false otherwise
                 */
                bool push_back(const T & t)
                {
                    if (m_head == nullptr)
                    {
                        // Nothing in the queue yet.
                        m_head = new MultiQueueBlock<T>(t);
                        m_back_of_queue = m_head;
                    }
                    else
                    {
                        assert(m_back_of_queue);
                        MultiQueueBlock<T> * new_block = new MultiQueueBlock<T>(t);
                        m_back_of_queue->set_next(new_block);
                        m_back_of_queue = new_block;
                    }
                    m_back_of_queue->set_reference_count(m_core_users);
                    return true;
                }

                /**
                 * @brief method to perform house keeping on the internals
                 * of the core.
                 * @return true if the update succeeded and false otherwise
                 *
                 * The update may delete elements from the list.
                 */
                bool update()
                {
                    MultiQueueBlock<T> * tmp = m_head;
                    MultiQueueBlock<T> * prev = nullptr;

                    while (tmp != nullptr)
                    {
                        if (tmp->get_reference_count() == 0)
                        {
                            if (prev != nullptr)
                            {
                                prev->set_next(tmp->get_next());
                                delete tmp;
                                tmp = prev->get_next();
                            }
                            else
                            {
                                m_head = tmp->get_next();
                                delete tmp;
                                tmp = m_head;
                            }
                        }
                        else
                        {
                            prev = tmp;
                            tmp = tmp->get_next();
                        }
                    }
                    return true;
                }

                /**
                 * @brief method to increment the number of users of the core by 1.
                 */
                void increment_users()
                {
                    ++m_core_users;
                }

                /**
                 * @brief method to decrement the number of users of the core by 1.
                 */
                void decrement_users()
                {
                    --m_core_users;
                }

                /**
                 * @brief method to get the number of users of the core.
                 * @return the number of users.
                 */
                size_t get_users()
                {
                    return m_core_users;
                }

                /**
                 * @brief method to check if the core is empty.
                 * @return true if the core has no data, and
                 * false otherwise.
                 */
                bool empty()
                {
                    if (m_head == nullptr)
                    {
                        return true;
                    }

                    return false;
                }

                /**
                 * @brief method to lock the mutex guarding the core.
                 */
                void lock()
                {
                    if (m_mutex)
                    {
                        m_mutex->lock();
                    }
                }

                /**
                 * @brief method to unlock the mutex guarding the core.
                 */
                void unlock()
                {
                    if (m_mutex)
                    {
                        m_mutex->unlock();
                    }
                }

                /**
                 * @brief method to return the number of objects in the
                 * list.
                 * @return the number of objects in the list.
                 */
                size_t size() const
                {
                    auto tmp = m_head;
                    size_t count{0};
                    while (tmp)
                    {
                        ++count;
                        tmp = tmp->get_next();
                    }
                    return count;
                }

                /**
                 * @brief method to allocate a mutex so that
                 * the core can be shared between multiple queues
                 * or used from multiple threads.
                 * @return the core object.
                 */
                MultiQueueCore * use_locks()
                {
                    if (m_mutex == nullptr)
                    {
                        m_mutex = new std::mutex{};
                    }
                    return this;
                }

            private:
                size_t m_core_users{1};
                MultiQueueBlock<T> * m_head{nullptr};
                MultiQueueBlock<T> * m_back_of_queue{nullptr};
                std::mutex * m_mutex{nullptr};
            };

        } // namespace multiqueue_internal

        /**
         * MultiQueue provides a FIFO queue class that can be shared between multiple threads
         * of execution.  In addition the queue can be 'forked' so that you can have multiple
         * producers and consumers from the queue without duplicating the queue contents.
         * @tparam T the type of object contained in the queue.
         */
        template<typename T>
        class MultiQueue
        {
        public:
            /**
             * @brief default constructor
             */
            MultiQueue()
            {
                m_core = new multiqueue_internal::MultiQueueCore<T>{};
                m_front_of_queue = m_core->get_head();
                m_unlock_defer = [this]() -> void {
                    m_core->unlock();
                };
            }

            /**
             * @brief destructor
             */
            ~MultiQueue()
            {
                m_core->lock();
                m_core->decrement_users();
                if (m_core->get_users() == 0)
                {
                    m_core->unlock();
                    delete m_core;
                    m_core = nullptr;
                }
                if (m_core)
                {
                    m_core->unlock();
                }
            }

            /**
             * @brief method to check if the queue has data to read
             * @return true if the queue has no data to read, and false
             * otherwise.
             */
            bool empty()
            {
                Defer d(m_unlock_defer);
                m_core->lock();

                if (m_front_of_queue == nullptr)
                {
                    return m_core->empty();
                }

                if (m_at_end_of_queue)
                {
                    return m_front_of_queue->get_next() == nullptr;
                }

                return false;
            }

            /**
             * @brief method to return the queue entry at the front of the queue.
             * @return the element at the front of the queue.  If the queue is empty
             * or the queue is at the end of the queue, throws std::runtime_error.
             */
            T & front()
            {
                Defer d(m_unlock_defer);
                m_core->lock();

                if (m_core->empty())
                {
                    throw std::runtime_error("Queue is empty");
                }

                if (m_front_of_queue == nullptr)
                {
                    m_front_of_queue = m_core->get_head();
                }

                if (m_at_end_of_queue)
                {
                    auto next = m_front_of_queue->get_next();
                    if (next == nullptr)
                    {
                        throw std::runtime_error("Queue is empty");
                    }

                    m_front_of_queue->decrement_reference();
                    m_core->update();
                    m_front_of_queue = next;
                    m_at_end_of_queue = false;
                }

                return m_front_of_queue->get_object();
            }

            /**
             * @brief method to remove the queue entry at the front of the queue.
             */
            void pop_front()
            {
                Defer d(m_unlock_defer);
                m_core->lock();

                if (m_core->empty())
                {
                    return;
                }

                if (m_front_of_queue == nullptr)
                {
                    m_front_of_queue = m_core->get_head();
                }

                multiqueue_internal::MultiQueueBlock<T> * next = m_front_of_queue->get_next();

                if (next)
                {
                    if (m_at_end_of_queue)
                    {
                        m_at_end_of_queue = false;
                    }

                    m_front_of_queue->decrement_reference();
                    m_front_of_queue = next;
                    m_core->update();
                }
                else
                {
                    m_at_end_of_queue = true;
                }
            }

            /**
             * @brief method to add a new entry to the back of the queue.
             * @param t the new object to add to the queue.
             * @return true if the push operation succeeded and false othewise.
             */
            bool push_back(const T & t)
            {
                Defer d(m_unlock_defer);
                m_core->lock();
                auto push_result = m_core->push_back(t);
                if (m_front_of_queue == nullptr)
                {
                    m_front_of_queue = m_core->get_head();
                }
                return push_result;
            }

            /**
             * @brief method to create a copy of the queue.
             * @return a pointer to the new queue object.
             *
             * The pointer to the new queue object must be freed with
             * delete.  After the fork(), each queue will have independent
             * references to the front of the queue.  Reading from one
             * queue will not affect reading from the other queue.
             */
            MultiQueue * fork()
            {
                auto mq = new MultiQueue(*this);
                return mq;
            }

            /**
             * @brief method to return the number of objects in the queue.
             * @return the number of objects in the queue.
             */
            size_t size() const
            {
                Defer d{m_unlock_defer};
                m_core->lock();

                if (m_core->empty())
                {
                    return 0;
                }

                if (m_at_end_of_queue)
                {
                    if (m_front_of_queue == nullptr)
                    {
                        return m_core->size();
                    }
                    return count_size_from(m_front_of_queue->get_next());
                }

                auto tmp = m_front_of_queue == nullptr ? m_core->get_head() : m_front_of_queue;
                return count_size_from(tmp);
            }

            /**
             * @brief method to enable sharing features.
             *
             * To use the queue in multiple threads where the producer
             * is in one thread and the consumer is in another or when
             * using a fork() call share() to make the queue threadsafe.
             * The fork() method automatically calls share().
             */
            void share()
            {
                m_core->use_locks();
            }

        private:
            multiqueue_internal::MultiQueueCore<T> * m_core{nullptr};
            multiqueue_internal::MultiQueueBlock<T> * m_front_of_queue{nullptr};
            std::function<void()> m_unlock_defer{};
            bool m_at_end_of_queue{false};

            /**
             * @brief private copy constructor used by fork() to make a new
             * copy of a queue.
             * @param queue the queue to copy.
             */
            MultiQueue(MultiQueue & queue)
            {
                Defer d{[this]() -> void {
                    if (m_core)
                    {
                        m_core->unlock();
                    }
                }};
                m_core = queue.m_core->use_locks();
                m_core->lock();
                m_front_of_queue = queue.m_front_of_queue;
                m_at_end_of_queue = queue.m_at_end_of_queue;
                m_unlock_defer = [this]() -> void {
                    m_core->unlock();
                };

                m_core->increment_users();
                auto tmp = m_front_of_queue;
                while (tmp)
                {
                    tmp->increment_reference();
                    tmp = tmp->get_next();
                }
            }

            /**
             * @brief helper method to count the number of objects in the queue
             * from a particular starting block.
             * @param block the starting block.
             * @return The number of objects in the queue stating from @e block.
             */
            size_t count_size_from(multiqueue_internal::MultiQueueBlock<T> * block) const
            {
                auto tmp = block;
                size_t count{0};
                while (tmp)
                {
                    ++count;
                    tmp = tmp->get_next();
                }
                return count;
            }
        };

    } // namespace Foundation

} // namespace TF

#endif // TFMULTIQUEUE_HXX
