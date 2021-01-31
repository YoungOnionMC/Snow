using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Snow;

namespace ExampleScriptApp
{
    public class Controller : Entity
    {
        public float Speed;

        private Entity m_CameraEntity;

        public void OnCreate()
        {
            m_CameraEntity = FindEntityByTag("Player");
            Console.Write("Creating CameraEntity");
        }

        public void OnUpdate(float ts)
        {
            Matrix4 transform = GetTransform();

            Vector3 translation = transform.Translation;
            translation.XY = m_CameraEntity.GetTransform().Translation.XY;

            if (Input.IsKeyPressed(KeyCode.W))
                translation.Y += Speed * ts;
            else if (Input.IsKeyPressed(KeyCode.S))
                translation.Y -= Speed * ts;

            if (Input.IsKeyPressed(KeyCode.D))
                translation.X += Speed * ts;
            else if (Input.IsKeyPressed(KeyCode.A))
                translation.X -= Speed * ts;

            transform.Translation = translation;
            SetTransform(transform);

            Console.WriteLine("Mouse Pos {0}, {1}", Input.GetMousePosition().X, Input.GetMousePosition().Y);
        }
    }
}
