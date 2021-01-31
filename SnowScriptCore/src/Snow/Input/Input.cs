using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Snow
{
    public class Input
    {
        public static bool IsKeyPressed(KeyCode keycode)
        {
            return IsKeyPressed_Native(keycode);
        }

        public static bool IsMouseButtonPressed(MouseCode mouseCode)
        {
            return IsMouseButtonPressed_Native(mouseCode);
        }

        public static Vector2 GetMousePosition()
        {
            Vector2 mousePos;
            GetMousePosition_Native(out mousePos);
            return mousePos;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsKeyPressed_Native(KeyCode keycode);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsMouseButtonPressed_Native(MouseCode keycode);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetMousePosition_Native(out Vector2 mousePos);
    }
}
