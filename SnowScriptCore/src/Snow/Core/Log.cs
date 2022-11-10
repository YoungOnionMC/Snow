using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;

namespace Snow
{
    public static class Log
    {

        internal enum LogLevel
        {
            Trace = (1 << 0),
            Debug = (1 << 1),
            Info = (1 << 2),
            Warn = (1 << 3),
            Error = (1 << 4),
            Critical = (1<<5)
        }

        public static void Trace(string format, params object[] parameters) => LogMessage_Native(LogLevel.Trace, string.Format(format, parameters));
        public static void Debug(string format, params object[] parameters) => LogMessage_Native(LogLevel.Debug, string.Format(format, parameters));
        public static void Info(string format, params object[] parameters) => LogMessage_Native(LogLevel.Info, string.Format(format, parameters));
        public static void Warn(string format, params object[] parameters) => LogMessage_Native(LogLevel.Warn, string.Format(format, parameters));
        public static void Error(string format, params object[] parameters) => LogMessage_Native(LogLevel.Error, string.Format(format, parameters));
        public static void Critical(string format, params object[] parameters) => LogMessage_Native(LogLevel.Critical, string.Format(format, parameters));

        public static void Trace(object value) => LogMessage_Native(LogLevel.Trace, value.ToString());
        public static void Debug(object value) => LogMessage_Native(LogLevel.Debug, value.ToString());
        public static void Info(object value) => LogMessage_Native(LogLevel.Info, value.ToString());
        public static void Warn(object value) => LogMessage_Native(LogLevel.Warn, value.ToString());
        public static void Error(object value) => LogMessage_Native(LogLevel.Error, value.ToString());
        public static void Critical(object value) => LogMessage_Native(LogLevel.Critical, value.ToString());

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void LogMessage_Native(LogLevel level, string formattedmessage);
    }
}
