#include "ParticleWebLog.h"


ParticleWebLog::ParticleWebLog(String name, LogLevel level,
                               const LogCategoryFilters &filters) : LogHandler(level, filters), m_name(name), m_publishing(0)
{
    LogManager::instance()->addHandler(this);
}



/// Send the log message to Papertrail.
void ParticleWebLog::log(const char *category, String message) {
    //String time = Time.format(Time.now(), TIME_FORMAT_ISO8601_FULL);
    //String packet = String::format("<22>1 %s %s %s - - - %s", time.c_str(), m_system.c_str(), m_app.c_str(), message.c_str());

    // Just in case someone calls Log.foo from inside publish we don't want to recurse
    if(!m_publishing && strcmp(category, "app") == 0) {
        m_publishing++;
        Particle.publish(m_name, message, PRIVATE);
        m_publishing--;
    }
}

ParticleWebLog::~ParticleWebLog() {
    LogManager::instance()->removeHandler(this);
}



// The floowing methods are taken from Particle FW, specifically spark::StreamLogHandler.
// See https://github.com/spark/firmware/blob/develop/wiring/src/spark_wiring_logging.cpp
const char* ParticleWebLog::extractFileName(const char *s) {
    const char *s1 = strrchr(s, '/');
    if (s1) {
        return s1 + 1;
    }
    return s;
}

const char* ParticleWebLog::extractFuncName(const char *s, size_t *size) {
    const char *s1 = s;
    for (; *s; ++s) {
        if (*s == ' ') {
            s1 = s + 1;                                                                                                                         // Skip return type
        } else if (*s == '(') {
            break;                                                                                                                         // Skip argument types
        }
    }
    *size = s - s1;
    return s1;
}

void ParticleWebLog::logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) {
    String s;

    // Source file
    if (attr.has_file) {
        s = extractFileName(attr.file);                                                                                 // Strip directory path
        s.concat(s);                                                                                 // File name
        if (attr.has_line) {
            s.concat(":");
            s.concat(String(attr.line));                                                                                                                         // Line number
        }
        if (attr.has_function) {
            s.concat(", ");
        } else {
            s.concat(": ");
        }
    }

    // Function name
    if (attr.has_function) {
        size_t n = 0;
        s = extractFuncName(attr.function, &n);                                                                                 // Strip argument and return types
        s.concat(s);
        s.concat("(): ");
    }

    // Level
    s.concat(levelName(level));
    s.concat(": ");

    // Message
    if (msg) {
        s.concat(msg);
    }

    // Additional attributes
    if (attr.has_code || attr.has_details) {
        s.concat(" [");
        // Code
        if (attr.has_code) {
            s.concat(String::format("code = %p", (intptr_t)attr.code));
        }
        // Details
        if (attr.has_details) {
            if (attr.has_code) {
                s.concat(", ");
            }
            s.concat("details = ");
            s.concat(attr.details);
        }
        s.concat(']');
    }

    log(category, s);
}
