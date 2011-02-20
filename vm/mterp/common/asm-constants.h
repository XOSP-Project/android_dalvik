/*
 * Copyright 2008 The Android Open Source Project
 *
 * Constants used by the assembler and verified by the C compiler.
 */

#if defined(ASM_DEF_VERIFY)
  /*
   * Generate C fragments that verify values; assumes "bool failed" exists.
   * These are all constant expressions, so on success these will compile
   * down to nothing.
   */
# define MTERP_OFFSET(_name, _type, _field, _offset)                        \
    if (offsetof(_type, _field) != _offset) {                               \
        LOGE("Bad asm offset %s (%d), should be %d\n",                      \
            #_name, _offset, offsetof(_type, _field));                      \
        failed = true;                                                      \
    }
# define MTERP_SIZEOF(_name, _type, _size)                                  \
    if (sizeof(_type) != (_size)) {                                         \
        LOGE("Bad asm sizeof %s (%d), should be %d\n",                      \
            #_name, (_size), sizeof(_type));                                \
        failed = true;                                                      \
    }
# define MTERP_CONSTANT(_name, _value)                                      \
    if ((_name) != (_value)) {                                              \
        LOGE("Bad asm constant %s (%d), should be %d\n",                    \
            #_name, (_value), (_name));                                     \
        failed = true;                                                      \
    }
#else
  /* generate constant labels for the assembly output */
# define MTERP_OFFSET(name, type, field, offset)    name = offset
# define MTERP_SIZEOF(name, type, size)             name = size
# define MTERP_CONSTANT(name, value)                name = value
#endif

/*
 * Platform dependencies.  Some platforms require 64-bit alignment of 64-bit
 * data structures.  Some versions of gcc will hold small enumerated types
 * in a char instead of an int.
 */
#if defined(__ARM_EABI__)
# define MTERP_NO_UNALIGN_64
#endif
#if defined(HAVE_SHORT_ENUMS)
# define MTERP_SMALL_ENUM   1
#else
# define MTERP_SMALL_ENUM   4
#endif

/*
 * This file must only contain the following kinds of statements:
 *
 *  MTERP_OFFSET(name, StructType, fieldname, offset)
 *
 *   Declares that the expected offset of StructType.fieldname is "offset".
 *   This will break whenever the contents of StructType are rearranged.
 *
 *  MTERP_SIZEOF(name, Type, size)
 *
 *   Declares that the expected size of Type is "size".
 *
 *  MTERP_CONSTANT(name, value)
 *
 *   Declares that the expected value of "name" is "value".  Useful for
 *   enumerations and defined constants that are inaccessible to the
 *   assembly source.  (Note this assumes you will use the same name in
 *   both C and assembly, which is good practice.)
 *
 * In all cases the "name" field is the label you will use in the assembler.
 *
 * The "value" field must always be an actual number, not a symbol, unless
 * you are sure that the symbol's value will be visible to both C and
 * assembly sources.  There may be restrictions on the possible range of
 * values (which are usually provided as immediate operands), so it's best
 * to restrict numbers assuming a signed 8-bit field.
 *
 * On the assembly side, these just become "name=value" constants.  On the
 * C side, these turn into assertions that cause the VM to abort if the
 * values are incorrect.
 */

/* DvmDex fields */
MTERP_OFFSET(offDvmDex_pResStrings,     DvmDex, pResStrings, 8)
MTERP_OFFSET(offDvmDex_pResClasses,     DvmDex, pResClasses, 12)
MTERP_OFFSET(offDvmDex_pResMethods,     DvmDex, pResMethods, 16)
MTERP_OFFSET(offDvmDex_pResFields,      DvmDex, pResFields, 20)
MTERP_OFFSET(offDvmDex_pInterfaceCache, DvmDex, pInterfaceCache, 24)

/* StackSaveArea fields */
#ifdef EASY_GDB
MTERP_OFFSET(offStackSaveArea_prevSave, StackSaveArea, prevSave, 0)
MTERP_OFFSET(offStackSaveArea_prevFrame, StackSaveArea, prevFrame, 4)
MTERP_OFFSET(offStackSaveArea_savedPc,  StackSaveArea, savedPc, 8)
MTERP_OFFSET(offStackSaveArea_method,   StackSaveArea, method, 12)
MTERP_OFFSET(offStackSaveArea_currentPc, StackSaveArea, xtra.currentPc, 16)
MTERP_OFFSET(offStackSaveArea_localRefCookie, \
                                        StackSaveArea, xtra.localRefCookie, 16)
MTERP_OFFSET(offStackSaveArea_returnAddr, StackSaveArea, returnAddr, 20)
MTERP_SIZEOF(sizeofStackSaveArea,       StackSaveArea, 24)
#else
MTERP_OFFSET(offStackSaveArea_prevFrame, StackSaveArea, prevFrame, 0)
MTERP_OFFSET(offStackSaveArea_savedPc,  StackSaveArea, savedPc, 4)
MTERP_OFFSET(offStackSaveArea_method,   StackSaveArea, method, 8)
MTERP_OFFSET(offStackSaveArea_currentPc, StackSaveArea, xtra.currentPc, 12)
MTERP_OFFSET(offStackSaveArea_localRefCookie, \
                                        StackSaveArea, xtra.localRefCookie, 12)
MTERP_OFFSET(offStackSaveArea_returnAddr, StackSaveArea, returnAddr, 16)
MTERP_SIZEOF(sizeofStackSaveArea,       StackSaveArea, 20)
#endif

  /* ShadowSpace fields */
#if defined(WITH_JIT) && defined(WITH_SELF_VERIFICATION)
MTERP_OFFSET(offShadowSpace_startPC,     ShadowSpace, startPC, 0)
MTERP_OFFSET(offShadowSpace_fp,          ShadowSpace, fp, 4)
MTERP_OFFSET(offShadowSpace_method,      ShadowSpace, method, 8)
MTERP_OFFSET(offShadowSpace_methodClassDex, ShadowSpace, methodClassDex, 12)
MTERP_OFFSET(offShadowSpace_retval,      ShadowSpace, retval, 16)
MTERP_OFFSET(offShadowSpace_interpStackEnd, ShadowSpace, interpStackEnd, 24)
MTERP_OFFSET(offShadowSpace_jitExitState,ShadowSpace, jitExitState, 28)
MTERP_OFFSET(offShadowSpace_svState,     ShadowSpace, selfVerificationState, 32)
MTERP_OFFSET(offShadowSpace_shadowFP,    ShadowSpace, shadowFP, 40)
#endif

/* InstField fields */
#ifdef PROFILE_FIELD_ACCESS
MTERP_OFFSET(offInstField_byteOffset,   InstField, byteOffset, 24)
#else
MTERP_OFFSET(offInstField_byteOffset,   InstField, byteOffset, 16)
#endif

/* Field fields */
MTERP_OFFSET(offField_clazz,            Field, clazz, 0)

/* StaticField fields */
#ifdef PROFILE_FIELD_ACCESS
MTERP_OFFSET(offStaticField_value,      StaticField, value, 24)
#else
MTERP_OFFSET(offStaticField_value,      StaticField, value, 16)
#endif

/* Method fields */
MTERP_OFFSET(offMethod_clazz,           Method, clazz, 0)
MTERP_OFFSET(offMethod_accessFlags,     Method, accessFlags, 4)
MTERP_OFFSET(offMethod_methodIndex,     Method, methodIndex, 8)
MTERP_OFFSET(offMethod_registersSize,   Method, registersSize, 10)
MTERP_OFFSET(offMethod_outsSize,        Method, outsSize, 12)
MTERP_OFFSET(offMethod_name,            Method, name, 16)
MTERP_OFFSET(offMethod_insns,           Method, insns, 32)
MTERP_OFFSET(offMethod_nativeFunc,      Method, nativeFunc, 40)

/* InlineOperation fields -- code assumes "func" offset is zero, do not alter */
MTERP_OFFSET(offInlineOperation_func,   InlineOperation, func, 0)

/* Thread fields */
MTERP_OFFSET(offThread_pc,                InterpSaveState, pc, 0)
MTERP_OFFSET(offThread_fp,                InterpSaveState, fp, 4)
MTERP_OFFSET(offThread_method,            InterpSaveState, method, 8)
MTERP_OFFSET(offThread_methodClassDex,    InterpSaveState, methodClassDex, 12)
MTERP_OFFSET(offThread_bailPtr,           InterpSaveState, bailPtr, 16)
MTERP_OFFSET(offThread_pInterpBreak,      InterpSaveState, pInterpBreak, 20)

/* make sure all JValue union members are stored at the same offset */
MTERP_OFFSET(offThread_retval,            Thread, retval, 32)
MTERP_OFFSET(offThread_retval_z,          Thread, retval.z, 32)
MTERP_OFFSET(offThread_retval_i,          Thread, retval.i, 32)
MTERP_OFFSET(offThread_retval_j,          Thread, retval.j, 32)
MTERP_OFFSET(offThread_retval_l,          Thread, retval.l, 32)
MTERP_OFFSET(offThread_suspendCount,      Thread, suspendCount, 40)
MTERP_OFFSET(offThread_dbgSuspendCount,   Thread, dbgSuspendCount, 44)
MTERP_OFFSET(offThread_cardTable,         Thread, cardTable, 48)
MTERP_OFFSET(offThread_interpStackEnd,    Thread, interpStackEnd, 52)
MTERP_OFFSET(offThread_curFrame,          Thread, curFrame, 56)
MTERP_OFFSET(offThread_exception,         Thread, exception, 60)
MTERP_OFFSET(offThread_threadId,          Thread, threadId, 64)
MTERP_OFFSET(offThread_debugIsMethodEntry, Thread, debugIsMethodEntry, 68)
MTERP_OFFSET(offThread_interpStackSize,   Thread, interpStackSize, 72)
MTERP_OFFSET(offThread_stackOverflowed,   Thread, stackOverflowed, 76)
MTERP_OFFSET(offThread_entryPoint,        Thread, entryPoint, 80)
#ifdef USE_INDIRECT_REF
MTERP_OFFSET(offThread_jniLocal_topCookie, \
                                Thread, jniLocalRefTable.segmentState.all, 84)
#else
MTERP_OFFSET(offThread_jniLocal_topCookie, \
                                Thread, jniLocalRefTable.nextEntry, 84)
#endif

#ifdef WITH_JIT
MTERP_OFFSET(offThread_jitToInterpEntries,Thread, jitToInterpEntries, 100)
MTERP_OFFSET(offThread_inJitCodeCache,    Thread, inJitCodeCache, 124)
MTERP_OFFSET(offThread_pJitProfTable,     Thread, pJitProfTable, 128)
MTERP_OFFSET(offThread_ppJitProfTable,    Thread, ppJitProfTable, 132)
MTERP_OFFSET(offThread_jitThreshold,      Thread, jitThreshold, 136)
MTERP_OFFSET(offThread_jitResumeNPC,      Thread, jitResumeNPC, 140)
MTERP_OFFSET(offThread_jitResumeDPC,      Thread, jitResumeDPC, 144)
MTERP_OFFSET(offThread_jitState,          Thread, jitState, 148)
MTERP_OFFSET(offThread_icRechainCount,    Thread, icRechainCount, 152)
MTERP_OFFSET(offThread_pProfileCountdown, Thread, pProfileCountdown, 156)
#if defined(WITH_SELF_VERIFICATION)
MTERP_OFFSET(offThread_shadowSpace,       Thread, shadowSpace, 160)
#endif
#endif

/* Object fields */
MTERP_OFFSET(offObject_clazz,           Object, clazz, 0)
MTERP_OFFSET(offObject_lock,            Object, lock, 4)

/* Lock shape */
MTERP_CONSTANT(LW_LOCK_OWNER_SHIFT, 3)
MTERP_CONSTANT(LW_HASH_STATE_SHIFT, 1)

/* ArrayObject fields */
MTERP_OFFSET(offArrayObject_length,     ArrayObject, length, 8)
#ifdef MTERP_NO_UNALIGN_64
MTERP_OFFSET(offArrayObject_contents,   ArrayObject, contents, 16)
#else
MTERP_OFFSET(offArrayObject_contents,   ArrayObject, contents, 12)
#endif

/* String fields */
MTERP_CONSTANT(STRING_FIELDOFF_VALUE,     8)
MTERP_CONSTANT(STRING_FIELDOFF_HASHCODE, 12)
MTERP_CONSTANT(STRING_FIELDOFF_OFFSET,   16)
MTERP_CONSTANT(STRING_FIELDOFF_COUNT,    20)

#if defined(WITH_JIT)
/*
 * Reasons for the non-chaining interpreter entry points
 * Enums defined in vm/Globals.h
 */
MTERP_CONSTANT(kInlineCacheMiss,        0)
MTERP_CONSTANT(kCallsiteInterpreted,    1)
MTERP_CONSTANT(kSwitchOverflow,         2)
MTERP_CONSTANT(kHeavyweightMonitor,     3)

/* Size of callee save area */
MTERP_CONSTANT(JIT_CALLEE_SAVE_DOUBLE_COUNT,   8)
#endif

/* ClassObject fields */
MTERP_OFFSET(offClassObject_descriptor, ClassObject, descriptor, 24)
MTERP_OFFSET(offClassObject_accessFlags, ClassObject, accessFlags, 32)
MTERP_OFFSET(offClassObject_pDvmDex,    ClassObject, pDvmDex, 40)
MTERP_OFFSET(offClassObject_status,     ClassObject, status, 44)
MTERP_OFFSET(offClassObject_super,      ClassObject, super, 72)
MTERP_OFFSET(offClassObject_vtableCount, ClassObject, vtableCount, 112)
MTERP_OFFSET(offClassObject_vtable,     ClassObject, vtable, 116)

/* InterpEntry enumeration */
MTERP_SIZEOF(sizeofClassStatus,         InterpEntry, MTERP_SMALL_ENUM)
MTERP_CONSTANT(kInterpEntryInstr,   0)
MTERP_CONSTANT(kInterpEntryReturn,  1)
MTERP_CONSTANT(kInterpEntryThrow,   2)
#if defined(WITH_JIT)
MTERP_CONSTANT(kInterpEntryResume,  3)
#endif

#if defined(WITH_JIT)
MTERP_CONSTANT(kJitNot,                 0)
MTERP_CONSTANT(kJitTSelectRequest,      1)
MTERP_CONSTANT(kJitTSelectRequestHot,   2)
MTERP_CONSTANT(kJitSelfVerification,    3)
MTERP_CONSTANT(kJitTSelect,             4)
MTERP_CONSTANT(kJitTSelectEnd,          5)
MTERP_CONSTANT(kJitSingleStep,          6)
MTERP_CONSTANT(kJitSingleStepEnd,       7)
MTERP_CONSTANT(kJitDone,                8)

#if defined(WITH_SELF_VERIFICATION)
MTERP_CONSTANT(kSVSIdle, 0)
MTERP_CONSTANT(kSVSStart, 1)
MTERP_CONSTANT(kSVSPunt, 2)
MTERP_CONSTANT(kSVSSingleStep, 3)
MTERP_CONSTANT(kSVSNoProfile, 4)
MTERP_CONSTANT(kSVSTraceSelect, 5)
MTERP_CONSTANT(kSVSNormal, 6)
MTERP_CONSTANT(kSVSNoChain, 7)
MTERP_CONSTANT(kSVSBackwardBranch, 8)
MTERP_CONSTANT(kSVSDebugInterp, 9)
#endif
#endif

/* ClassStatus enumeration */
MTERP_SIZEOF(sizeofClassStatus,         ClassStatus, MTERP_SMALL_ENUM)
MTERP_CONSTANT(CLASS_INITIALIZED,   7)

/* MethodType enumeration */
MTERP_SIZEOF(sizeofMethodType,          MethodType, MTERP_SMALL_ENUM)
MTERP_CONSTANT(METHOD_DIRECT,       1)
MTERP_CONSTANT(METHOD_STATIC,       2)
MTERP_CONSTANT(METHOD_VIRTUAL,      3)
MTERP_CONSTANT(METHOD_INTERFACE,    4)

/* ClassObject constants */
MTERP_CONSTANT(ACC_PRIVATE,         0x0002)
MTERP_CONSTANT(ACC_STATIC,          0x0008)
MTERP_CONSTANT(ACC_NATIVE,          0x0100)
MTERP_CONSTANT(ACC_INTERFACE,       0x0200)
MTERP_CONSTANT(ACC_ABSTRACT,        0x0400)
MTERP_CONSTANT(CLASS_ISFINALIZABLE, 1<<31)

/* flags for dvmMalloc */
MTERP_CONSTANT(ALLOC_DONT_TRACK,    0x01)

/* for GC */
MTERP_CONSTANT(GC_CARD_SHIFT, 7)

/* opcode number */
MTERP_CONSTANT(OP_MOVE_EXCEPTION,   0x0d)

/* flags for interpBreak */
MTERP_CONSTANT(kSubModeNormal,         0x0000)
MTERP_CONSTANT(kSubModeMethodTrace,    0x0001)
MTERP_CONSTANT(kSubModeEmulatorTrace,  0x0002)
MTERP_CONSTANT(kSubModeInstCounting,   0x0004)
MTERP_CONSTANT(kSubModeDebuggerActive, 0x0008)
MTERP_CONSTANT(kSubModeSuspendRequest, 0x0010)
