/*
 * Copyright 2011-2012, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ANDROID_BCINFO_METADATAEXTRACTOR_H__
#define __ANDROID_BCINFO_METADATAEXTRACTOR_H__

#include <cstddef>
#include <stdint.h>

namespace llvm {
  class Module;
  class NamedMDNode;
}

namespace bcinfo {

enum RSFloatPrecision {
  RS_FP_Full = 0,
  RS_FP_Relaxed = 1,
};

class MetadataExtractor {
 private:
  const llvm::Module *mModule;
  const char *mBitcode;
  size_t mBitcodeSize;

  size_t mExportVarCount;
  size_t mExportFuncCount;
  size_t mExportForEachSignatureCount;
  const char **mExportVarNameList;
  const char **mExportFuncNameList;
  const char **mExportForEachNameList;
  const uint32_t *mExportForEachSignatureList;

  size_t mPragmaCount;
  const char **mPragmaKeyList;
  const char **mPragmaValueList;

  size_t mObjectSlotCount;
  const uint32_t *mObjectSlotList;

  uint32_t mCompilerVersion;
  uint32_t mOptimizationLevel;

  enum RSFloatPrecision mRSFloatPrecision;

  // Helper functions for extraction
  bool populateVarNameMetadata(const llvm::NamedMDNode *VarNameMetadata);
  bool populateFuncNameMetadata(const llvm::NamedMDNode *FuncNameMetadata);
  bool populateForEachMetadata(const llvm::NamedMDNode *Names,
                               const llvm::NamedMDNode *Signatures);
  bool populateObjectSlotMetadata(const llvm::NamedMDNode *ObjectSlotMetadata);
  void populatePragmaMetadata(const llvm::NamedMDNode *PragmaMetadata);

 public:
  /**
   * Reads metadata from \p bitcode.
   *
   * \param bitcode - input bitcode string.
   * \param bitcodeSize - length of \p bitcode string (in bytes).
   */
  MetadataExtractor(const char *bitcode, size_t bitcodeSize);

  /**
   * Reads metadata from \p module.
   *
   * \param module - input module.
   */
  MetadataExtractor(const llvm::Module *module);

  ~MetadataExtractor();

  /**
   * Extract the actual metadata from the supplied bitcode.
   *
   * \return true on success and false if an error occurred.
   */
  bool extract();

  /**
   * \return number of exported global variables (slots) in this script/module.
   */
  size_t getExportVarCount() const {
    return mExportVarCount;
  }

  /**
   * \return array of exported variable names.
   */
  const char **getExportVarNameList() const {
    return mExportVarNameList;
  }

  /**
   * \return number of exported global functions (slots) in this script/module.
   */
  size_t getExportFuncCount() const {
    return mExportFuncCount;
  }

  /**
   * \return array of exported function names.
   */
  const char **getExportFuncNameList() const {
    return mExportFuncNameList;
  }

  /**
   * \return number of exported ForEach functions in this script/module.
   */
  size_t getExportForEachSignatureCount() const {
    return mExportForEachSignatureCount;
  }

  /**
   * \return array of exported ForEach function signatures.
   */
  const uint32_t *getExportForEachSignatureList() const {
    return mExportForEachSignatureList;
  }

  /**
   * \return array of exported ForEach function names.
   */
  const char **getExportForEachNameList() const {
    return mExportForEachNameList;
  }

  /**
   * \return number of pragmas contained in pragmaKeyList and pragmaValueList.
   */
  size_t getPragmaCount() const {
    return mPragmaCount;
  }

  /**
   * \return pragma keys (the name for the pragma).
   */
  const char **getPragmaKeyList() const {
    return mPragmaKeyList;
  }

  /**
   * \return pragma values (contents corresponding to a particular pragma key).
   */
  const char **getPragmaValueList() const {
    return mPragmaValueList;
  }

  /**
   * \return number of object slots contained in objectSlotList.
   */
  size_t getObjectSlotCount() const {
    return mObjectSlotCount;
  }

  /**
   * \return array of object slot numbers that must be cleaned up by driver
   *         on script teardown.
   */
  const uint32_t *getObjectSlotList() const {
    return mObjectSlotList;
  }

  /**
   * \return compiler version that generated this bitcode.
   */
  uint32_t getCompilerVersion() const {
    return mCompilerVersion;
  }

  /**
   * \return compiler optimization level for this bitcode.
   */
  uint32_t getOptimizationLevel() const {
    return mOptimizationLevel;
  }

  /**
   * \return minimal floating point precision that the script requires.
   */
  enum RSFloatPrecision getRSFloatPrecision() const {
    return mRSFloatPrecision;
  }

  /**
   * \return whether or not this ForEach function signature has an "In"
   * parameter.
   *
   * \param sig - ForEach function signature to check.
   */
  static bool hasForEachSignatureIn(uint32_t sig) {
    return sig & 0x01;
  }

  /**
   * \return whether or not this ForEach function signature has an "Out"
   * parameter.
   *
   * \param sig - ForEach function signature to check.
   */
  static bool hasForEachSignatureOut(uint32_t sig) {
    return sig & 0x02;
  }

  /**
   * \return whether or not this ForEach function signature has a "UsrData"
   * parameter.
   *
   * \param sig - ForEach function signature to check.
   */
  static bool hasForEachSignatureUsrData(uint32_t sig) {
    return sig & 0x04;
  }

  /**
   * \return whether or not this ForEach function signature has an "X"
   * parameter.
   *
   * \param sig - ForEach function signature to check.
   */
  static bool hasForEachSignatureX(uint32_t sig) {
    return sig & 0x08;
  }

  /**
   * \return whether or not this ForEach function signature has a "Y"
   * parameter.
   *
   * \param sig - ForEach function signature to check.
   */
  static bool hasForEachSignatureY(uint32_t sig) {
    return sig & 0x10;
  }

  /**
   * \return whether or not this ForEach function signature is a
   * pass-by-value "Kernel".
   *
   * \param sig - ForEach function signature to check.
   */
  static bool hasForEachSignatureKernel(uint32_t sig) {
    return sig & 0x20;
  }
};

}  // namespace bcinfo

#endif  // __ANDROID_BCINFO_METADATAEXTRACTOR_H__
