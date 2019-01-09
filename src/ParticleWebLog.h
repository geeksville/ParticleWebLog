#pragma once

/* ParticleWebLog library by kevinh@geeksville.com
 */

// This will load the definition for common Particle variable types
#include "Particle.h"


/// LogHandler that send logs to Papertrail (https://papertrailapp.com/). Before using this class it's best to
/// familiarize yourself with Particle's loggin facility https://docs.particle.io/reference/firmware/photon/#logging.
/// You can use this as any other LogHandler - Initialize this class as a global, then call Log.info() and friends.
class ParticleWebLog : public LogHandler {
    String m_name;
    int m_publishing; // a counting semaphore to prevent recursion

public:
/// Initialize the log handler.
/// \param name Name of the events we publish
/// \para level Default log level.
/// \param filters Category filters.
///
/// Each log entry uses RFC 5424 with the following format:
/// "<22>1 %ISO8601_TIME% %system% %app% - - - [%category%] %log_level%: %text".
    explicit ParticleWebLog(String name = "log",
                            LogLevel level = LOG_LEVEL_INFO, const LogCategoryFilters &filters = {
    });
    virtual ~ParticleWebLog();

private:

    const char* extractFileName(const char *s);
    const char* extractFuncName(const char *s, size_t *size);
    void log(const char *category, String message);

protected:
    virtual void logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) override;
};
