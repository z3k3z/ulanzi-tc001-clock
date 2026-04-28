#ifndef ERRORH_H
#define ERRORH_H

#include <Arduino.h>
#include <string.h>

// small structure that holds error state context
typedef struct _EHErrorContext {
   bool        fInError;
   const char* pszFile;
   int         iLine;
   long        lData;
   const char* pszAddlText;
} EHErrorContext;

// initialize an instance of the error context
#define EHInitialize                                                                               \
   EHErrorContext ehEc;                                                                            \
   memset(&ehEc, 0, sizeof(ehEc));

// unconditional error
// using do/while to avoid unanticapted behavior when used in
// if-then-else statements that don't use {}
#define EHRaiseError(lDataToLog)                                                                   \
   do {                                                                                            \
      ehEc.fInError = true;                                                                        \
      ehEc.pszFile  = __FILE__;                                                                    \
      ehEc.iLine    = __LINE__;                                                                    \
      ehEc.lData    = (long)(lDataToLog);                                                          \
      goto End;                                                                                    \
   } while (0);

// error predicated on c being true
#define EHRaiseErrorWhen(f, lDataToLog)                                                            \
   do {                                                                                            \
      if (f) {                                                                                     \
         EHRaiseError(lDataToLog);                                                                 \
      }                                                                                            \
   } while (0);

// error predicated on c being false (useful when success is indicated by a boolean true)
#define EHRaiseErrorWhenNotSuccess(f, lDataToLog) EHRaiseErrorWhen(!(f), lDataToLog);

// evaluates to true if an error has been detected
#define EHErrorRaised (true == ehEc.fInError)

// evaluates to true if no error has been detected
#define EHIsSuccess !(EHErrorRaised)

// attach an additional context string to be emitted.
#define EHSetAdditionalContext(pszContextToLog)                                                    \
   do {                                                                                            \
      ehEc.pszAddlText = (pszContextToLog);                                                        \
   } while (0);

// unconditional emit a formatted error trace to Serial
#define EHEmitMsg                                                                                  \
   do {                                                                                            \
      Serial.print("ERROR--> ");                                                                   \
      Serial.print(ehEc.pszFile);                                                                  \
      Serial.print(":");                                                                           \
      Serial.print(ehEc.iLine);                                                                    \
      {                                                                                            \
         uint16_t uiHighWord = (uint16_t)((ehEc.lData >> 16) & 0xFFFF);                            \
         uint16_t uiLowWord  = (uint16_t)(ehEc.lData & 0xFFFF);                                    \
                                                                                                   \
         Serial.print(" Data: 0x ");                                                               \
         if (uiHighWord < 0x1000)                                                                  \
            Serial.print("0");                                                                     \
         if (uiHighWord < 0x0100)                                                                  \
            Serial.print("0");                                                                     \
         if (uiHighWord < 0x0010)                                                                  \
            Serial.print("0");                                                                     \
         Serial.print(uiHighWord, HEX);                                                            \
                                                                                                   \
         Serial.print(" ");                                                                        \
                                                                                                   \
         if (uiLowWord < 0x1000)                                                                   \
            Serial.print("0");                                                                     \
         if (uiLowWord < 0x0100)                                                                   \
            Serial.print("0");                                                                     \
         if (uiLowWord < 0x0010)                                                                   \
            Serial.print("0");                                                                     \
         Serial.print(uiLowWord, HEX);                                                             \
      }                                                                                            \
      if (nullptr != ehEc.pszAddlText) {                                                           \
         Serial.print("\t");                                                                       \
         Serial.print(ehEc.pszAddlText);                                                           \
      }                                                                                            \
      Serial.print("\n");                                                                          \
   } while (0);

// See errorh.cpp.  Set to true, there, to emit debug messages
extern bool g_fEHDebugEnabled;

// emit a formatted error trace to Serial when debug is enabled
#define EHEmitMsgDebug                                                                             \
   do {                                                                                            \
      if (g_fEHDebugEnabled) {                                                                     \
         EHEmitMsg;                                                                                \
      }                                                                                            \
   } while (0);

#define EH_PACK_INT16_TO_LONG(iHigh, iLow)                                                         \
   ((((long)((unsigned short)(iHigh))) << 16) | ((unsigned short)(iLow)))

#endif
