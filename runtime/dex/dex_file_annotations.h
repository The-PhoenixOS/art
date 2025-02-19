/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef ART_RUNTIME_DEX_DEX_FILE_ANNOTATIONS_H_
#define ART_RUNTIME_DEX_DEX_FILE_ANNOTATIONS_H_

#include "dex/dex_file.h"
#include "handle.h"
#include "mirror/dex_cache.h"
#include "mirror/object_array.h"
#include "obj_ptr.h"

namespace art HIDDEN {

namespace mirror {
class ClassLoader;
}  // namespace mirror
class ArtField;
class ArtMethod;
class ClassLinker;

namespace annotations {

// Field annotations.
ObjPtr<mirror::Object> GetAnnotationForField(ArtField* field,
                                             Handle<mirror::Class> annotation_class)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::ObjectArray<mirror::Object>> GetAnnotationsForField(ArtField* field)
    REQUIRES_SHARED(Locks::mutator_lock_);
EXPORT ObjPtr<mirror::ObjectArray<mirror::String>> GetSignatureAnnotationForField(ArtField* field)
    REQUIRES_SHARED(Locks::mutator_lock_);
bool IsFieldAnnotationPresent(ArtField* field, Handle<mirror::Class> annotation_class)
    REQUIRES_SHARED(Locks::mutator_lock_);

// Method annotations.
ObjPtr<mirror::Object> GetAnnotationDefaultValue(ArtMethod* method)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::Object> GetAnnotationForMethod(ArtMethod* method,
                                              Handle<mirror::Class> annotation_class)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::ObjectArray<mirror::Object>> GetAnnotationsForMethod(ArtMethod* method)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::ObjectArray<mirror::Class>> GetExceptionTypesForMethod(ArtMethod* method)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::ObjectArray<mirror::Object>> GetParameterAnnotations(ArtMethod* method)
    REQUIRES_SHARED(Locks::mutator_lock_);
uint32_t GetNumberOfAnnotatedMethodParameters(ArtMethod* method)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::Object> GetAnnotationForMethodParameter(ArtMethod* method,
                                                       uint32_t parameter_idx,
                                                       Handle<mirror::Class> annotation_class)
    REQUIRES_SHARED(Locks::mutator_lock_);
bool GetParametersMetadataForMethod(
    ArtMethod* method,
    /*out*/ MutableHandle<mirror::ObjectArray<mirror::String>>* names,
    /*out*/ MutableHandle<mirror::IntArray>* access_flags) REQUIRES_SHARED(Locks::mutator_lock_);
EXPORT ObjPtr<mirror::ObjectArray<mirror::String>> GetSignatureAnnotationForMethod(
    ArtMethod* method) REQUIRES_SHARED(Locks::mutator_lock_);
// Check whether `method` is annotated with `annotation_class`.
// If `lookup_in_resolved_boot_classes` is true, look up any of the
// method's annotations' classes in the bootstrap class loader's
// resolved types; if it is false (default value), resolve them as a
// side effect.
bool IsMethodAnnotationPresent(ArtMethod* method,
                               Handle<mirror::Class> annotation_class,
                               uint32_t visibility = DexFile::kDexVisibilityRuntime)
    REQUIRES_SHARED(Locks::mutator_lock_);

// Check whether a method from the `dex_file` with the given `method_index`
// is annotated with @dalvik.annotation.optimization.FastNative or
// @dalvik.annotation.optimization.CriticalNative with build visibility.
// If yes, return the associated access flags, i.e. kAccFastNative or kAccCriticalNative.
EXPORT uint32_t GetNativeMethodAnnotationAccessFlags(const DexFile& dex_file,
                                                     const dex::ClassDef& class_def,
                                                     uint32_t method_index);
// An overload of `GetNativeMethodAnnotationAccessFlags()` that takes a `MethodAnnotationsItem`.
uint32_t GetNativeMethodAnnotationAccessFlags(const DexFile& dex_file,
                                              const dex::MethodAnnotationsItem& method_annotations);
// Is the method from the `dex_file` with the given `field_index`
// annotated with @dalvik.annotation.optimization.NeverCompile?
EXPORT bool MethodIsNeverCompile(const DexFile& dex_file,
                                 const dex::ClassDef& class_def,
                                 uint32_t method_index);
// An overload of `MethodIsNeverCompile()` that takes a `MethodAnnotationsItem`.
bool MethodIsNeverCompile(const DexFile& dex_file,
                          const dex::MethodAnnotationsItem& method_annotations);
// Is the method from the `dex_file` with the given `field_index`
// annotated with @dalvik.annotation.optimization.NeverInline?
bool MethodIsNeverInline(const DexFile& dex_file,
                         const dex::ClassDef& class_def,
                         uint32_t method_index);
// Is the field from the `dex_file` with the given `field_index`
// annotated with @dalvik.annotation.optimization.ReachabilitySensitive?
bool FieldIsReachabilitySensitive(const DexFile& dex_file,
                                  const dex::ClassDef& class_def,
                                  uint32_t field_index);
// Is the method from the `dex_file` with the given `method_index`
// annotated with @dalvik.annotation.optimization.ReachabilitySensitive?
bool MethodIsReachabilitySensitive(const DexFile& dex_file,
                                   const dex::ClassDef& class_def,
                                   uint32_t method_index);
// Does the method from the `dex_file` with the given `method_index` contain an access to a field
// annotated with @dalvik.annotation.optimization.ReachabilitySensitive, or a call to a method
// with that annotation?
// Class_def is the class defining the method. We consider only accessses to classes or methods
// declared in the static type of the corresponding object. We may overlook accesses to annotated
// fields or methods that are in neither class_def nor a containing (outer) class.
bool MethodContainsRSensitiveAccess(const DexFile& dex_file,
                                    const dex::ClassDef& class_def,
                                    uint32_t method_index);
// Is the given class annotated with @dalvik.annotation.optimization.DeadReferenceSafe?
bool HasDeadReferenceSafeAnnotation(const DexFile& dex_file,
                                    const dex::ClassDef& class_def);

// Class annotations.
ObjPtr<mirror::Object> GetAnnotationForClass(Handle<mirror::Class> klass,
                                      Handle<mirror::Class> annotation_class)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::ObjectArray<mirror::Object>> GetAnnotationsForClass(Handle<mirror::Class> klass)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::ObjectArray<mirror::Class>> GetDeclaredClasses(Handle<mirror::Class> klass)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::Class> GetDeclaringClass(Handle<mirror::Class> klass)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::Class> GetEnclosingClass(Handle<mirror::Class> klass)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::Object> GetEnclosingMethod(Handle<mirror::Class> klass)
    REQUIRES_SHARED(Locks::mutator_lock_);
bool GetInnerClass(Handle<mirror::Class> klass, /*out*/ ObjPtr<mirror::String>* name)
    REQUIRES_SHARED(Locks::mutator_lock_);
bool GetInnerClassFlags(Handle<mirror::Class> klass, uint32_t* flags)
    REQUIRES_SHARED(Locks::mutator_lock_);
EXPORT ObjPtr<mirror::ObjectArray<mirror::String>> GetSignatureAnnotationForClass(
    Handle<mirror::Class> klass) REQUIRES_SHARED(Locks::mutator_lock_);
EXPORT const char* GetSourceDebugExtension(Handle<mirror::Class> klass)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::Class> GetNestHost(Handle<mirror::Class> klass)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::ObjectArray<mirror::Class>> GetNestMembers(Handle<mirror::Class> klass)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::Object> getRecordAnnotationElement(Handle<mirror::Class> klass,
                                                  Handle<mirror::Class> array_class,
                                                  const char* element_name)
    REQUIRES_SHARED(Locks::mutator_lock_);
ObjPtr<mirror::ObjectArray<mirror::Class>> GetPermittedSubclasses(Handle<mirror::Class> klass)
    REQUIRES_SHARED(Locks::mutator_lock_);
bool IsClassAnnotationPresent(Handle<mirror::Class> klass,
                              Handle<mirror::Class> annotation_class)
    REQUIRES_SHARED(Locks::mutator_lock_);

// Map back from a PC to the line number in a method.
int32_t GetLineNumFromPC(const DexFile* dex_file, ArtMethod* method, uint32_t rel_pc)
    REQUIRES_SHARED(Locks::mutator_lock_);

// Annotations iterator.
class RuntimeEncodedStaticFieldValueIterator : public EncodedStaticFieldValueIterator {
 public:
  // A constructor meant to be called from runtime code.
  RuntimeEncodedStaticFieldValueIterator(Handle<mirror::DexCache> dex_cache,
                                         Handle<mirror::ClassLoader> class_loader,
                                         ClassLinker* linker,
                                         const dex::ClassDef& class_def)
      REQUIRES_SHARED(Locks::mutator_lock_)
      : EncodedStaticFieldValueIterator(*dex_cache->GetDexFile(), class_def),
        dex_cache_(dex_cache),
        class_loader_(class_loader),
        linker_(linker) {
  }

  template<bool kTransactionActive>
  void ReadValueToField(ArtField* field) const REQUIRES_SHARED(Locks::mutator_lock_);

 private:
  const Handle<mirror::DexCache> dex_cache_;  // Dex cache to resolve literal objects.
  const Handle<mirror::ClassLoader> class_loader_;  // ClassLoader to resolve types.
  ClassLinker* const linker_;  // Linker to resolve literal objects.
  DISALLOW_IMPLICIT_CONSTRUCTORS(RuntimeEncodedStaticFieldValueIterator);
};

enum class VisitorStatus : uint8_t { kVisitBreak, kVisitNext, kVisitInner };

class AnnotationVisitor {
 public:
  virtual ~AnnotationVisitor() {}
  virtual VisitorStatus VisitAnnotation(const char* annotation_descriptor, uint8_t visibility) = 0;
  virtual VisitorStatus VisitAnnotationElement(const char* element_name,
                                               uint8_t type,
                                               const JValue& value) = 0;
  virtual VisitorStatus VisitArrayElement(uint8_t depth,
                                          uint32_t index,
                                          uint8_t type,
                                          const JValue& value) = 0;

  bool HasError() const { return has_error_; }
  void SetErrorMsg(const std::string& msg) {
    DCHECK(!has_error_) << "Already had an error set. New error: " << msg
                        << ", old error: " << error_msg_;
    has_error_ = true;
    error_msg_ = msg;
  }
  const std::string& GetErrorMsg() const { return error_msg_; }

 protected:
  // Whether we found an error while visiting the annotations. If true, `error_msg_` will contain
  // the information about the error.
  bool has_error_ = false;
  std::string error_msg_;
};

// Visit all annotation elements and array elements without creating
// Arrays or Objects in the managed heap.
void VisitClassAnnotations(Handle<mirror::Class> klass, AnnotationVisitor* visitor)
    REQUIRES_SHARED(Locks::mutator_lock_);

}  // namespace annotations

}  // namespace art

#endif  // ART_RUNTIME_DEX_DEX_FILE_ANNOTATIONS_H_
