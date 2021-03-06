/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ART_COMPILER_DRIVER_COMPILED_METHOD_STORAGE_H_
#define ART_COMPILER_DRIVER_COMPILED_METHOD_STORAGE_H_

#include <iosfwd>
#include <memory>

#include "base/macros.h"
#include "length_prefixed_array.h"
#include "utils/array_ref.h"
#include "utils/dedupe_set.h"
#include "utils/swap_space.h"

namespace art {

class LinkerPatch;
class SrcMapElem;

class CompiledMethodStorage {
 public:
  explicit CompiledMethodStorage(int swap_fd);
  ~CompiledMethodStorage();

  void DumpMemoryUsage(std::ostream& os, bool extended) const;

  void SetDedupeEnabled(bool dedupe_enabled) {
    dedupe_enabled_ = dedupe_enabled;
  }
  bool DedupeEnabled() const {
    return dedupe_enabled_;
  }

  SwapAllocator<void> GetSwapSpaceAllocator() {
    return SwapAllocator<void>(swap_space_.get());
  }

  const LengthPrefixedArray<uint8_t>* DeduplicateCode(const ArrayRef<const uint8_t>& code);
  void ReleaseCode(const LengthPrefixedArray<uint8_t>* code);

  const LengthPrefixedArray<SrcMapElem>* DeduplicateSrcMappingTable(
      const ArrayRef<const SrcMapElem>& src_map);
  void ReleaseSrcMappingTable(const LengthPrefixedArray<SrcMapElem>* src_map);

  const LengthPrefixedArray<uint8_t>* DeduplicateMappingTable(const ArrayRef<const uint8_t>& table);
  void ReleaseMappingTable(const LengthPrefixedArray<uint8_t>* table);

  const LengthPrefixedArray<uint8_t>* DeduplicateVMapTable(const ArrayRef<const uint8_t>& table);
  void ReleaseVMapTable(const LengthPrefixedArray<uint8_t>* table);

  const LengthPrefixedArray<uint8_t>* DeduplicateGCMap(const ArrayRef<const uint8_t>& gc_map);
  void ReleaseGCMap(const LengthPrefixedArray<uint8_t>* gc_map);

  const LengthPrefixedArray<uint8_t>* DeduplicateCFIInfo(const ArrayRef<const uint8_t>& cfi_info);
  void ReleaseCFIInfo(const LengthPrefixedArray<uint8_t>* cfi_info);

  const LengthPrefixedArray<LinkerPatch>* DeduplicateLinkerPatches(
      const ArrayRef<const LinkerPatch>& linker_patches);
  void ReleaseLinkerPatches(const LengthPrefixedArray<LinkerPatch>* linker_patches);

 private:
  template <typename T, typename DedupeSetType>
  const LengthPrefixedArray<T>* AllocateOrDeduplicateArray(const ArrayRef<const T>& data,
                                                           DedupeSetType* dedupe_set);

  template <typename T>
  void ReleaseArrayIfNotDeduplicated(const LengthPrefixedArray<T>* array);

  // DeDuplication data structures.
  template <typename ContentType>
  class DedupeHashFunc;

  template <typename T>
  class LengthPrefixedArrayAlloc;

  template <typename T>
  using ArrayDedupeSet = DedupeSet<ArrayRef<const T>,
                                   LengthPrefixedArray<T>,
                                   LengthPrefixedArrayAlloc<T>,
                                   size_t,
                                   DedupeHashFunc<const T>,
                                   4>;

  // Swap pool and allocator used for native allocations. May be file-backed. Needs to be first
  // as other fields rely on this.
  std::unique_ptr<SwapSpace> swap_space_;

  bool dedupe_enabled_;

  ArrayDedupeSet<uint8_t> dedupe_code_;
  ArrayDedupeSet<SrcMapElem> dedupe_src_mapping_table_;
  ArrayDedupeSet<uint8_t> dedupe_mapping_table_;
  ArrayDedupeSet<uint8_t> dedupe_vmap_table_;
  ArrayDedupeSet<uint8_t> dedupe_gc_map_;
  ArrayDedupeSet<uint8_t> dedupe_cfi_info_;
  ArrayDedupeSet<LinkerPatch> dedupe_linker_patches_;

  DISALLOW_COPY_AND_ASSIGN(CompiledMethodStorage);
};

}  // namespace art

#endif  // ART_COMPILER_DRIVER_COMPILED_METHOD_STORAGE_H_
