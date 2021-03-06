// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __MASTER_ALLOCATOR_SORTER_SORTER_HPP__
#define __MASTER_ALLOCATOR_SORTER_SORTER_HPP__

#include <list>
#include <string>

#include <mesos/resources.hpp>
#include <mesos/type_utils.hpp>

namespace mesos {
namespace internal {
namespace master {
namespace allocator {

// Sorters implement the logic for determining the
// order in which users or frameworks should receive
// resource allocations.
//
// TODO(bmahler): Templatize this on Client, so that callers can
// don't need to do string conversion, e.g. FrameworkID, string role,
// etc.
class Sorter
{
public:
  virtual ~Sorter() {}

  // Adds a client to allocate resources to. A client
  // may be a user or a framework.
  virtual void add(const std::string& client, double weight = 1) = 0;

  // Update weight of a client.
  virtual void update(const std::string& client, double weight) = 0;

  // Removes a client.
  virtual void remove(const std::string& client) = 0;

  // Readds a client to the sort after deactivate.
  virtual void activate(const std::string& client) = 0;

  // Removes a client from the sort, so it won't get allocated to.
  virtual void deactivate(const std::string& client) = 0;

  // Specify that resources have been allocated to the given client.
  virtual void allocated(
      const std::string& client,
      const SlaveID& slaveId,
      const Resources& resources) = 0;

  // Updates a portion of the allocation for the client, in order to
  // augment the resources with additional metadata (e.g., volumes)
  // This means that the new allocation must not affect the static
  // roles, or the overall quantities of resources!
  virtual void update(
      const std::string& client,
      const SlaveID& slaveId,
      const Resources& oldAllocation,
      const Resources& newAllocation) = 0;

  // Specify that resources have been unallocated from the given client.
  virtual void unallocated(
      const std::string& client,
      const SlaveID& slaveId,
      const Resources& resources) = 0;

  // Returns the resources that have been allocated to this client.
  virtual const hashmap<SlaveID, Resources>& allocation(
      const std::string& client) = 0;

  // Returns the sum of the scalar resources that are allocated to this client.
  virtual const Resources& allocationScalars(const std::string& client) = 0;

  // Returns the clients that have allocations on this slave.
  virtual hashmap<std::string, Resources> allocation(
      const SlaveID& slaveId) = 0;

  // Returns the given slave's resources that have been allocated to
  // this client.
  virtual Resources allocation(
      const std::string& client,
      const SlaveID& slaveId) = 0;

  // Returns the total resources that are in this sorter.
  virtual const hashmap<SlaveID, Resources>& total() const = 0;

  // Returns the sum of the total scalar resources that are in this sorter.
  virtual const Resources& totalScalars() const = 0;

  // Add resources to the total pool of resources this
  // Sorter should consider.
  virtual void add(const SlaveID& slaveId, const Resources& resources) = 0;

  // Remove resources from the total pool.
  virtual void remove(const SlaveID& slaveId, const Resources& resources) = 0;

  // Updates the total pool of resources.
  virtual void update(const SlaveID& slaveId, const Resources& resources) = 0;

  // Returns a list of all clients, in the order that they
  // should be allocated to, according to this Sorter's policy.
  virtual std::list<std::string> sort() = 0;

  // Returns true if this Sorter contains the specified client,
  // either active or deactivated.
  virtual bool contains(const std::string& client) = 0;

  // Returns the number of clients this Sorter contains,
  // either active or deactivated.
  virtual int count() = 0;
};

} // namespace allocator {
} // namespace master {
} // namespace internal {
} // namespace mesos {

#endif // __MASTER_ALLOCATOR_SORTER_SORTER_HPP__
