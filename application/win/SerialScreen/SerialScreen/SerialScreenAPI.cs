using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

namespace SerialScreen
{
    class SerialScreenAPI
    {
        // SerialScreen API code
        private const Byte CODE_setScreen     = 0x20;
        private const Byte CODE_clearScreen   = 0x21;
        private const Byte CODE_setColor      = 0x22;
        private const Byte CODE_drawLine      = 0x23;
        private const Byte CODE_drawRectangle = 0x24;
        private const Byte CODE_fillRectangle = 0x25;
        private const Byte CODE_drawPolygon   = 0x26;
        private const Byte CODE_fillPolygon   = 0x27;
        private const Byte CODE_drawCircle    = 0x28;
        private const Byte CODE_fillCircle    = 0x29;
        private const Byte CODE_drawPie       = 0x2A;
        private const Byte CODE_fillPie       = 0x2B;
        private const Byte CODE_setFont       = 0x2C;
        private const Byte CODE_drawString    = 0x2D;
        private const Byte CODE_drawImage     = 0x2E;
        private const Byte CODE_loadImage     = 0x2F;

        // default screen size
        public const int SCREEN_WIDTH = 640;
        public const int SCREEN_HEIGHT = 480;

        // parent MainForm
        private FormMain parent;

        // picture box
        private PictureBox pictureBox;
        //private Graphics canvas;

        // pen, brush, font
        private Pen pen;
        private Brush brush;
        private Font font;

        // images
        private Image[] images;
        public const int IMAGE_MAX = 256;

        // initialize
        public SerialScreenAPI()
        {
            pen = Pens.White;
            brush = Brushes.White;
            font = new Font(FontFamily.GenericMonospace, 12);
            images = new Image[IMAGE_MAX];
        }

        // set my picture box
        public void setPictureBox(FormMain parent)
        {
            this.parent = parent;
            parent.setScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
            this.pictureBox = parent.getScreen();
            //this.canvas = Graphics.FromImage(pictureBox.BackgroundImage);
        }

        // execute a command
        private delegate void CommandDelegate(string command);
        public void executeCommand(Byte[] buffer)
        {
            //string command = System.Text.Encoding.ASCII.GetString(buffer);
            string command = System.Text.Encoding.UTF8.GetString(buffer);
            CommandDelegate d;

            switch(buffer[0])
            {
                case CODE_setScreen:
                    d = new CommandDelegate(setScreen);
                    break;
                case CODE_clearScreen:
                    d = new CommandDelegate(clearScreen);
                    break;
                case CODE_setColor:
                    d = new CommandDelegate(setColor);
                    break;
                case CODE_drawLine:
                    d = new CommandDelegate(drawLine);
                    break;
                case CODE_drawRectangle:
                    d = new CommandDelegate(drawRectangle);
                    break;
                case CODE_fillRectangle:
                    d = new CommandDelegate(fillRectangle);
                    break;
                case CODE_drawPolygon:
                    d = new CommandDelegate(drawPolygon);
                    break;
                case CODE_fillPolygon:
                    d = new CommandDelegate(fillPolygon);
                    break;
                case CODE_drawCircle:
                    d = new CommandDelegate(drawCircle);
                    break;
                case CODE_fillCircle:
                    d = new CommandDelegate(fillCircle);
                    break;
                case CODE_drawPie:
                    d = new CommandDelegate(drawPie);
                    break;
                case CODE_fillPie:
                    d = new CommandDelegate(fillPie);
                    break;
                case CODE_setFont:
                    d = new CommandDelegate(setFont);
                    break;
                case CODE_drawString:
                    d = new CommandDelegate(drawString);
                    break;
                case CODE_drawImage:
                    d = new CommandDelegate(drawImage);
                    break;
                case CODE_loadImage:
                    d = new CommandDelegate(loadImage);
                    break;
                default:
                    d = null;
                    break;
            }
            if (d != null)
            {
                parent.Invoke(d, new Object[] { command });
            }
        }

        /********** APIs **********/

        // set my screen size
        private void setScreen(string command)
        {
            int w = Convert.ToInt32(command.Substring(1, 3), 16);
            int h = Convert.ToInt32(command.Substring(4, 3), 16);

            // set screen size
 //           if (canvas != null)
 //          {
 //               canvas.Dispose();
 //               //canvas.ReleaseHdc();
 //           }
            parent.setScreen(w, h);
            this.pictureBox = parent.getScreen();
            Graphics canvas = Graphics.FromImage(pictureBox.BackgroundImage);
//            this.canvas = Graphics.FromImage(pictureBox.BackgroundImage);
            canvas.Clear(Color.Black);
            canvas.Dispose();

            pictureBox.Invalidate();
        }

        // clear my screen
        private void clearScreen(string command)
        {
            int r = Convert.ToInt32(command.Substring(1, 2), 16);
            int g = Convert.ToInt32(command.Substring(3, 2), 16);
            int b = Convert.ToInt32(command.Substring(5, 2), 16);

            Color color = Color.FromArgb(r,g,b);
            Graphics canvas = Graphics.FromImage(pictureBox.BackgroundImage);
            canvas.Clear(color);
            canvas.Dispose();

            pictureBox.Invalidate();
        }

        // set my color
        private void setColor(string command)
        {
            int r = Convert.ToInt32(command.Substring(1, 2), 16);
            int g = Convert.ToInt32(command.Substring(3, 2), 16);
            int b = Convert.ToInt32(command.Substring(5, 2), 16);
            int width = Convert.ToInt32(command.Substring(7, 2), 16);

            Color color = Color.FromArgb(r,g,b);

            pen = new Pen(color, (float)width);
            brush = new SolidBrush(color);
        }

        // draw a line
        private void drawLine(string command)
        {
            int x1 = Convert.ToInt32(command.Substring(1, 3), 16);
            int y1 = Convert.ToInt32(command.Substring(4, 3), 16);
            int x2 = Convert.ToInt32(command.Substring(7, 3), 16);
            int y2 = Convert.ToInt32(command.Substring(10, 3), 16);

            Graphics canvas = Graphics.FromImage(pictureBox.BackgroundImage);
            canvas.DrawLine(pen, x1, y1, x2, y2);
            canvas.Dispose();

            pictureBox.Invalidate();
        }

        // draw a rectangle
        private void drawRectangle(string command)
        {
            int x = Convert.ToInt32(command.Substring(1, 3), 16);
            int y = Convert.ToInt32(command.Substring(4, 3), 16);
            int w = Convert.ToInt32(command.Substring(7, 3), 16);
            int h = Convert.ToInt32(command.Substring(10, 3), 16);

            Graphics canvas = Graphics.FromImage(pictureBox.BackgroundImage);
            canvas.DrawRectangle(pen, x, y, w, h);
            canvas.Dispose();

            pictureBox.Invalidate();
        }

        // fill a rectangle
        private void fillRectangle(string command)
        {
            int x = Convert.ToInt32(command.Substring(1, 3), 16);
            int y = Convert.ToInt32(command.Substring(4, 3), 16);
            int w = Convert.ToInt32(command.Substring(7, 3), 16);
            int h = Convert.ToInt32(command.Substring(10, 3), 16);

            Graphics canvas = Graphics.FromImage(pictureBox.BackgroundImage);
            canvas.FillRectangle(brush, x, y, w, h);
            canvas.Dispose();

            pictureBox.Invalidate();
        }

        // draw a polygon
        private void drawPolygon(string command)
        {
            int n = Convert.ToInt32(command.Substring(1, 1), 16);

            if (3 <= n && n <= 8)
            {
                Point[] points = new Point[n];

                for(int i=0;i<n;i++)
                {
                    points[i].X = Convert.ToInt32(command.Substring(2 + i*6, 3), 16);
                    points[i].Y = Convert.ToInt32(command.Substring(5 + i*6, 3), 16);
                }

                Graphics canvas = Graphics.FromImage(pictureBox.BackgroundImage);
                canvas.DrawPolygon(pen, points);
                canvas.Dispose();

                pictureBox.Invalidate();
            }
        }

        // fill a poligon
        private void fillPolygon(string command)
        {
            int n = Convert.ToInt32(command.Substring(1, 1), 16);

            if (3 <= n && n <= 8)
            {
                Point[] points = new Point[n];

                for (int i = 0; i < n; i++)
                {
                    points[i].X = Convert.ToInt32(command.Substring(2 + i * 6, 3), 16);
                    points[i].Y = Convert.ToInt32(command.Substring(5 + i * 6, 3), 16);
                }

                Graphics canvas = Graphics.FromImage(pictureBox.BackgroundImage);
                canvas.FillPolygon(brush, points);
                canvas.Dispose();

                pictureBox.Invalidate();
            }
        }

        // draw a circle
        private void drawCircle(string command)
        {
            int x = Convert.ToInt32(command.Substring(1, 3), 16);
            int y = Convert.ToInt32(command.Substring(4, 3), 16);
            int w = Convert.ToInt32(command.Substring(7, 3), 16);
            int h = Convert.ToInt32(command.Substring(10, 3), 16);

            Graphics canvas = Graphics.FromImage(pictureBox.BackgroundImage);
            canvas.DrawEllipse(pen, x, y, w, h);
            canvas.Dispose();

            pictureBox.Invalidate();
        }

        // fill a circle
        private void fillCircle(string command)
        {
            int x = Convert.ToInt32(command.Substring(1, 3), 16);
            int y = Convert.ToInt32(command.Substring(4, 3), 16);
            int w = Convert.ToInt32(command.Substring(7, 3), 16);
            int h = Convert.ToInt32(command.Substring(10, 3), 16);

            Graphics canvas = Graphics.FromImage(pictureBox.BackgroundImage);
            canvas.FillEllipse(brush, x, y, w, h);
            canvas.Dispose();

            pictureBox.Invalidate();
        }

        // draw a pie
        private void drawPie(string command)
        {
            int x = Convert.ToInt32(command.Substring(1, 3), 16);
            int y = Convert.ToInt32(command.Substring(4, 3), 16);
            int w = Convert.ToInt32(command.Substring(7, 3), 16);
            int h = Convert.ToInt32(command.Substring(10, 3), 16);
            int start = Convert.ToInt32(command.Substring(13, 3), 16);
            int sweep = Convert.ToInt32(command.Substring(16, 3), 16);

            Graphics canvas = Graphics.FromImage(pictureBox.BackgroundImage);
            canvas.DrawPie(pen, x, y, w, h, start, sweep);
            canvas.Dispose();

            pictureBox.Invalidate();
        }

        // fill a pie
        private void fillPie(string command)
        {
            int x = Convert.ToInt32(command.Substring(1, 3), 16);
            int y = Convert.ToInt32(command.Substring(4, 3), 16);
            int w = Convert.ToInt32(command.Substring(7, 3), 16);
            int h = Convert.ToInt32(command.Substring(10, 3), 16);
            int start = Convert.ToInt32(command.Substring(13, 3), 16);
            int sweep = Convert.ToInt32(command.Substring(16, 3), 16);

            Graphics canvas = Graphics.FromImage(pictureBox.BackgroundImage);
            canvas.FillPie(brush, x, y, w, h, start, sweep);
            canvas.Dispose();

            pictureBox.Invalidate();
        }

        // set my font
        private void setFont(string command)
        {
            int size = Convert.ToInt32(command.Substring(1, 2), 16);

            font = new Font(FontFamily.GenericMonospace, size);
        }

        // draw a string
        private void drawString(string command)
        {
            int x = Convert.ToInt32(command.Substring(1, 3), 16);
            int y = Convert.ToInt32(command.Substring(4, 3), 16);
            string s = command.Substring(7);
            int iFind0 = s.IndexOf('\0');
            s = s.Substring(0, iFind0);

            Graphics canvas = Graphics.FromImage(pictureBox.BackgroundImage);
            canvas.DrawString(s, font, brush, x, y);
            canvas.Dispose();

            pictureBox.Invalidate();
        }

        // draw a image
        private void drawImage(string command)
        {
            int x = Convert.ToInt32(command.Substring(1, 3), 16);
            int y = Convert.ToInt32(command.Substring(4, 3), 16);
            int index = Convert.ToInt32(command.Substring(7, 2), 16);

            if (0 <= index && index < IMAGE_MAX)
            {
                if (images[index] != null)
                {
                    Graphics canvas = Graphics.FromImage(pictureBox.BackgroundImage);
                    canvas.DrawImage(images[index], x, y);
                    canvas.Dispose();
                }
            }

            pictureBox.Invalidate();
        }

        // load a image
        private void loadImage(string command)
        {
            int index = Convert.ToInt32(command.Substring(1, 2), 16);
            string path = command.Substring(3);
            int iFind0 = path.IndexOf('\0');
            path = path.Substring(0, iFind0);

            if (0 <= index && index < IMAGE_MAX)
            {
                images[index] = Image.FromFile(path);
            }
        }
    }
}
