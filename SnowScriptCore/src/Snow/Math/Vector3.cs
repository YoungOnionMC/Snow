using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Snow
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        public float X;
        public float Y;
        public float Z;

        public Vector3(float scalar)
        {
            X = Y = Z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vector2 XX
        {
            get { return new Vector2(X, X); }
            set { X = value.X; X = value.Y; }
        }

        public Vector2 XY
        {
            get { return new Vector2(X, Y); }
            set { X = value.X; Y = value.Y; }
        }

        public Vector2 XZ
        {
            get { return new Vector2(X, Z); }
            set { X = value.X; Z = value.Y; }
        }

        public Vector2 YX
        {
            get { return new Vector2(Y, X); }
            set { Y = value.X; X = value.Y; }
        }

        public Vector2 YY
        {
            get { return new Vector2(Y, Y); }
            set { Y = value.X; Y = value.Y; }
        }

        public Vector2 YZ
        {
            get { return new Vector2(Y, Z); }
            set { Y = value.X; Z = value.Y; }
        }

        public Vector2 ZX
        {
            get { return new Vector2(Z, X); }
            set { Z = value.X; X = value.Y; }
        }

        public Vector2 ZY
        {
            get { return new Vector2(Z, Y); }
            set { Z = value.X; Y = value.Y; }
        }

        public Vector2 ZZ
        {
            get { return new Vector2(Z, Z); }
            set { Z = value.X; Z = value.Y; }
        }
    }
}
