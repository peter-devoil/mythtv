#ifndef AUDIOPULSEHANDLER_H
#define AUDIOPULSEHANDLER_H

#include <pulse/pulseaudio.h>

class QThread;

class PulseHandler
{
  public:
    enum PulseAction
    {
        kPulseSuspend = 0,
        kPulseResume,
        kPulseCleanup,
    };

    static bool Suspend(enum PulseAction action);
    static PulseHandler *g_pulseHandler;
    static bool          g_pulseHandlerActive;

   ~PulseHandler(void);
    bool Valid(void);

    pa_context_state m_ctx_state          {PA_CONTEXT_UNCONNECTED};
    pa_context      *m_ctx                {nullptr};
    int              m_pending_operations {0};

  private:
    PulseHandler(void) = default;
    bool Init(void);
    bool SuspendInternal(bool suspend);

    pa_mainloop     *m_loop               {nullptr};
    bool             m_initialised        {false};
    bool             m_valid              {false};
    QThread         *m_thread             {nullptr};
};

#endif // AUDIOPULSEHANDLER_H
