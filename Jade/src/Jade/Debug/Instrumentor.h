#pragma once
#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <iomanip>

#include <thread>

namespace Jade
{
    using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

    struct ProfileResult
    {
        std::string Name;

        FloatingPointMicroseconds Start;
        std::chrono::microseconds ElapsedTime;

        std::thread::id ThreadID;
    };

    struct InstrumentationSession
    {
        std::string Name;
    };

    class Instrumentor
    {
    private:
        std::mutex m_Mutex;
        InstrumentationSession* m_CurrentSession;
        std::ofstream m_OutputStream;

        Instrumentor()
            : m_CurrentSession(nullptr)
            , m_OutputStream()
            , m_Mutex()
        {
        }

    public:
        void BeginSession(const std::string& name, const std::string& filepath = "results.json")
        {
            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession)
            {
                // If there is already a current session, then close it before beginning new one.
                // Subsequent profiling output meant for the original session will end up in the
                // newly opened session instead. That's better than having badly formatted profiling output.
                if (Log::GetCoreLogger())
                {
                    // Edge case: BeginSession() might be called before Log::Init()
                    JADE_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' is already active", name, m_CurrentSession->Name);
                }

                InternalEndSession();
            }

#pragma region preprocess filepath
            namespace fs = std::filesystem;

            fs::path inputPath(filepath);
            fs::path outPath;

            // lowercase extension check
            std::string ext = inputPath.extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

            if (ext == ".json")
            {
                std::error_code ec;
                fs::create_directories("profiles", ec);
                outPath = fs::path("profiles") / inputPath.filename();
            }
            else
            {
                // Currently only JSON output is supported
                if (Log::GetCoreLogger())
                {
                    // Edge case: BeginSession() might be called before Log::Init()
                    JADE_CORE_ERROR("Instrumentor::BeginSession('{0}') has invalid file extension '{1}'! Only .json is supported.", name, ext);
                }
            }
#pragma endregion

            m_OutputStream.open(outPath, std::ios::out);
            if (m_OutputStream.is_open())
            {
                m_CurrentSession = new InstrumentationSession{ name };
                WriteHeader();
            }
            else
            {
                if (Log::GetCoreLogger())
                {
                    // Edge case: BeginSession() might be called before Log::Init()
                    JADE_CORE_ERROR("Instrumentor could not open results file: {0}", outPath.string());
                }
            }
        }

        void EndSession()
        {
            if (!m_OutputStream.is_open())
                return;

            std::lock_guard lock(m_Mutex);
            InternalEndSession();
        }

        void WriteProfile(const ProfileResult& result)
        {
            std::stringstream json;

            json << std::setprecision(3) << std::fixed;
            json << "{";
            json << "\"cat\":\"function\",";
            json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
            json << "\"name\":\"" << result.Name << "\",";
            json << "\"ph\":\"X\",";
            json << "\"pid\":0,";
            json << "\"tid\":" << result.ThreadID << ',';
            json << "\"ts\":" << result.Start.count();
            json << "},";

            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                if (m_CurrentSession)
                {
                    m_OutputStream << json.str();
                    m_OutputStream.flush();
                }
            }
        }

        // Meyer¡¯s Singleton
        // Thread-safe in C++11 and later
        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }

    private:
        void WriteHeader()
        {
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
            m_OutputStream.flush();
        }

        void WriteFooter()
        {
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

        // Note: you must already own the lock on m_Mutex before
        // calling InternalEndSession()
        void InternalEndSession()
        {
            if (m_CurrentSession)
            {
                WriteFooter();
                m_OutputStream.close();
                delete m_CurrentSession;
                m_CurrentSession = nullptr;
            }
        }
    };

    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* name)
            : m_Name(name)
            , m_StartTimepoint(std::chrono::steady_clock::now())
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
            auto endTimepoint = std::chrono::steady_clock::now();
            auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };

            auto elapsedTime = 
                std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch()
                - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

            Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

            m_Stopped = true;
        }

    private:
        const char* m_Name;
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
        bool m_Stopped;
    };
}

#define JADE_PROFILE 0

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