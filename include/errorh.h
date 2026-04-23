#ifndef ERRORH_H
#define ERRORH_H

#include <string.h>

// small structure that holds error state context
typedef struct _EHErrorContext {
   bool        fInError;
   const char* pszFile;
   int         iLine;
   long        lData;
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

// emit a formatted error trace to Serial
#define EHEmitMsg                                                                                  \
   do {                                                                                            \
      Serial.print("ERROR--> ");                                                                   \
      Serial.print(ehEc.pszFile);                                                                  \
      Serial.print(":");                                                                           \
      Serial.print(ehEc.iLine);                                                                    \
      Serial.print(" 0x");                                                                         \
      Serial.print(ehEc.lData, HEX);                                                               \
   } while (0);
#endif
