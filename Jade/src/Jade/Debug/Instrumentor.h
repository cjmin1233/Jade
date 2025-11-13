#pragma once
#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <filesystem>

#include <thread>

namespace Jade
{
    struct ProfileResult
    {
        std::string Name;
        long long Start, End;
        size_t ThreadID;
    };

    struct InstrumentationSession
    {
        std::string Name;
    };

    class Instrumentor
    {
    private:
        InstrumentationSession* m_CurrentSession;
        std::ofstream m_OutputStream;
        int m_ProfileCount;

        Instrumentor()
            : m_CurrentSession(nullptr)
            , m_OutputStream()
            , m_ProfileCount(0)
        {
        }

    public:
        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            namespace fs = std::filesystem;

            fs::path inputPath(filepath);
            fs::path outPath;

            std::string ext = inputPath.extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

            if (ext == ".json")
            {
                std::error_code ec;
                fs::create_directories("profiles", ec); // 실패해도 진행
                outPath = fs::path("profiles") / inputPath.filename();
            }
            else
            {
                outPath = inputPath; // 그대로 사용
            }

            m_OutputStream.open(outPath.string(), std::ios::out);
            if (!m_OutputStream.is_open())
            {
                JADE_CORE_ASSERT(false, "Instrumentor could not open results file: {0}", filepath);
                return;
            }

            WriteHeader();
            m_CurrentSession = new InstrumentationSession{ name };
        }

        void EndSession()
        {
            if (!m_OutputStream.is_open())
                return;

            WriteFooter();
            m_OutputStream.close();
            delete m_CurrentSession;
            m_CurrentSession = nullptr;
            m_ProfileCount = 0;
        }

        void WriteProfile(const ProfileResult& result)
        {
            if (m_ProfileCount++ > 0)
                m_OutputStream << ",";

            std::string name = result.Name;
            std::replace(name.begin(), name.end(), '"', '\'');
            
            m_OutputStream << "{";
            m_OutputStream << "\"cat\":\"function\",";
            m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';

            m_OutputStream << "\"name\":\"" << name << "\",";
            m_OutputStream << "\"ph\":\"X\",";
            m_OutputStream << "\"pid\":0,";
            m_OutputStream << "\"tid\":" << result.ThreadID << ",";
            m_OutputStream << "\"ts\":" << result.Start;
            m_OutputStream << "}";

            m_OutputStream.flush();
        }

        void WriteHeader()
        {
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
            m_OutputStream.flush();
        }

        void WriteFooter()
        {
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }
    };

    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* name)
            : m_Name(name)
            , m_StartTimepoint(std::chrono::high_resolution_clock::now())
            , m_Stopped(false)
        {
        }

        ~InstrumentationTimer()
        {
            if (!m_Stopped)
            {
                Stop();
            }
        }

        void Stop()
        {
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            size_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

            m_Stopped = true;
        }

    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
    };
}

#define JADE_PROFILE 1

#if JADE_PROFILE
    // Resolve which function signature macro will be used. Note that this only
    // is resolved when the (pre)compiler starts, so the syntax highlighting
    // could mark the wrong one in your editor!
    #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__) || defined(__clang__)
        #define JADE_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__DMC__) && (__DMC__ >= 0x810)
        #define JADE_FUNC_SIG __PRETTY_FUNCTION__
    #elif defined(__FUNCSIG__)
        #define JADE_FUNC_SIG __FUNCSIG__
    #elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
        #define JADE_FUNC_SIG __FUNCTION__
    #elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
        #define JADE_FUNC_SIG __FUNC__
    #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
        #define JADE_FUNC_SIG __func__
    #elif defined(__cplusplus) && (__cplusplus >= 201103)
        #define JADE_FUNC_SIG __func__
    #else
        #define JADE_FUNC_SIG "JADE_FUNC_SIG unknown!"
    #endif // Resolve function signature macro

    // Instrumentation macros
    #define JADE_PROFILE_BEGIN_SESSION(name, filepath) ::Jade::Instrumentor::Get().BeginSession(name, filepath)
    #define JADE_PROFILE_END_SESSION() ::Jade::Instrumentor::Get().EndSession()
    #define JADE_PROFILE_SCOPE(name) ::Jade::InstrumentationTimer timer##__LINE__(name)
    #define JADE_PROFILE_FUNCTION() JADE_PROFILE_SCOPE(JADE_FUNC_SIG)
#else
    #define JADE_PROFILE_BEGIN_SESSION(name, filepath)
    #define JADE_PROFILE_END_SESSION()
    #define JADE_PROFILE_SCOPE(name)
    #define JADE_PROFILE_FUNCTION()
#endif // JADE_PROFILE