#ifndef __TIMER_H_
#define __TIMER_H_

#include <string>
#include <sstream>

#ifdef _WINDOWS
#include <windows.h>
#else
#include <time.h>
#include <cerrno>
#endif

#include <iostream>


//---------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------
namespace g3d {

class Timer {
public:
    typedef long double time_t;

    static time_t getTime() {
        return prv_get_time();
    }

    static time_t getTimeMicro() {
        return prv_get_time()*1e6;
    }

    static time_t getTimeMili() {
        return prv_get_time()*1e3;
    }

    Timer() : m_currentTime( getTimeMicro() ) 
    {
    }

    time_t getLastTimeMicro() 
    {
        time_t curr = getTimeMicro();
        const time_t diff = curr - m_currentTime;
        m_currentTime = curr;
        return diff;
    }

    time_t getLastTimeMili() 
    {
        time_t curr = getTimeMicro();
        const time_t diff = curr - m_currentTime;
        m_currentTime = curr;
        return diff*1e-3;       //divide
    }

    std::string print() 
    {
        std::stringstream stream;
        const time_t diff = getLastTimeMili();
        stream << "time: " << diff << "[ms]";
        return stream.str();
    }


protected:
#ifdef _WINDOWS
    static time_t prv_get_time() 
    {
        static INT64 frequency = 0;
        static INT64 seconds   = 0;
        static INT64 fraction  = 0;
        static INT64 counter   = 0;

        INT64 prev_counter = counter;
        QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*> (&counter) );

        if (!frequency) {
            prev_counter = counter; // start our timing at first call
            QueryPerformanceFrequency( reinterpret_cast<LARGE_INTEGER*> (&frequency) );
        }

        fraction += counter - prev_counter;
        seconds += ( fraction / frequency );
        fraction = fraction % frequency;

        return double( seconds ) + double( fraction ) / double( frequency );
    }

#else // win32
    typedef struct timespec port_time_t;

    static time_t prv_get_time()
    {
        port_time_t ts;
        clock_gettime( clock_realtime, &ts );
        return 1e-9*(double)ts.tv_nsec + (double)ts.tv_sec;
    }
#endif // win/linux

private:
    time_t m_currentTime;
};

//------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------
class ScopeTimer {
public:
    ScopeTimer( bool pL = true, bool printEndLine = true ) : m_startTime( Timer::getTimeMili() ), m_printLog( pL ), m_printEndLine( printEndLine )
    {
    }

    ~ScopeTimer() 
    {
        if ( m_printLog ) {
            Timer::time_t diff = Timer::getTimeMili() - m_startTime;
            std::cout << diff << " ms";
            if ( m_printEndLine )
                std::cout<<std::endl;
        }
    }

    void setPrintLog( bool val )
    {
        m_printLog = val;
    }

    void setPrintEndLine( bool val )
    {
        m_printEndLine = val;
    }

    std::string printToString() const {
        Timer::time_t diff = Timer::getTimeMili() - m_startTime;
        std::stringstream stream;
        stream << "time: " << diff << "[ms]";
        if ( m_printEndLine )
            stream<<std::endl;
        return stream.str();
    }

    void print() const {
        Timer::time_t diff = Timer::getTimeMili() - m_startTime;
        std::cout << "ScopeTimer::~ScopeTimer(): scope time: " << diff << "[ms]";
        if ( m_printEndLine )
            std::cout<<std::endl;
    }

    std::string printUpdate() {
        Timer::time_t diff = update();
        std::stringstream stream;
        stream << "time: " << diff << "[ms]";
        return stream.str();
    }

    Timer::time_t getTimeMili() const {
        return (Timer::getTimeMili() - m_startTime);
    }

    Timer::time_t update() {
        Timer::time_t currTime = Timer::getTimeMili();
        Timer::time_t diff = currTime - m_startTime;
        m_startTime = currTime;
        return diff;
    }

protected:
    Timer::time_t m_startTime;

    bool m_printLog;
    bool m_printEndLine;

};

}

#endif /* __TIMER_H_ */
