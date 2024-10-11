// This source file is part of the Swift.org open source project
//
// Copyright (c) 2024 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//

#include "CFTimeZone_WindowsMapping.h"
#include "CFInternal.h"
#include <_foundation_unicode/ucal.h>

#if TARGET_OS_WINDOWS

CFStringRef _CFTimeZoneCopyWindowsNameForOlsonName(CFStringRef olson) {
    CFIndex olsonLen = CFStringGetLength(olson);
    SAFE_STACK_BUFFER_DECL(UChar, olsonBuffer, olsonLen, 256);
    UChar *olsonUStr = (UChar *)CFStringGetCharactersPtr(olson);
    if (NULL == olsonUStr) {
        CFStringGetCharacters(olson, CFRangeMake(0, olsonLen), (UniChar *)olsonBuffer);
        olsonUStr = olsonBuffer;
    }
    
    STACK_BUFFER_DECL(UChar, outBuffer, 256);
    
    CFStringRef result = NULL;
    UErrorCode status = U_ZERO_ERROR;
    int32_t outLength = ucal_getWindowsTimeZoneID(olsonUStr, olsonLen, outBuffer, 256, &status);
    if (U_SUCCESS(status)) {
        result = CFStringCreateWithCharacters(kCFAllocatorSystemDefault, outBuffer, outLength);
    } else if (status == U_BUFFER_OVERFLOW_ERROR) {
        status = U_ZERO_ERROR;
        UChar *largerBuffer = calloc(outLength + 1, sizeof(UChar));
        outLength = ucal_getWindowsTimeZoneID(olsonUStr, olsonLen, largerBuffer, outLength + 1, &status);
        if (U_SUCCESS(status)) {
            result = CFStringCreateWithCharacters(kCFAllocatorSystemDefault, largerBuffer, outLength);
        }
        free(largerBuffer);
    }
    
    SAFE_STACK_BUFFER_CLEANUP(olsonBuffer);
    return result;
}

CFStringRef _CFTimeZoneCopyOlsonNameForWindowsName(CFStringRef windows) {
    CFIndex windowsLen = CFStringGetLength(windows);
    SAFE_STACK_BUFFER_DECL(UChar, windowsBuffer, windowsLen, 256);
    UChar *windowsUStr = (UChar *)CFStringGetCharactersPtr(windows);
    if (NULL == windowsUStr) {
        CFStringGetCharacters(windows, CFRangeMake(0, windowsLen), (UniChar *)windowsBuffer);
        windowsUStr = windowsBuffer;
    }
    
    STACK_BUFFER_DECL(UChar, outBuffer, 256);
    
    CFStringRef result = NULL;
    UErrorCode status = U_ZERO_ERROR;
    int32_t outLength = ucal_getTimeZoneIDForWindowsID(windowsUStr, windowsLen, NULL, outBuffer, 256, &status);
    if (U_SUCCESS(status)) {
        result = CFStringCreateWithCharacters(kCFAllocatorSystemDefault, outBuffer, outLength);
    } else if (status == U_BUFFER_OVERFLOW_ERROR) {
        status = U_ZERO_ERROR;
        UChar *largerBuffer = calloc(outLength + 1, sizeof(UChar));
        outLength = ucal_getTimeZoneIDForWindowsID(windowsUStr, windowsLen, NULL, largerBuffer, outLength + 1, &status);
        if (U_SUCCESS(status)) {
            result = CFStringCreateWithCharacters(kCFAllocatorSystemDefault, largerBuffer, outLength);
        }
        free(largerBuffer);
    }
    
    SAFE_STACK_BUFFER_CLEANUP(windowsBuffer);
    return result;
}

#endif
