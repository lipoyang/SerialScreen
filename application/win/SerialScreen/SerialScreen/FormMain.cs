using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SerialScreen
{
    public partial class FormMain : Form
    {
        // SerialScreen server
        public SerialScreenServer ssServer;

        // initialize this form
        public FormMain()
        {
            InitializeComponent();
            
            ssServer = new SerialScreenServer(this);

            // [Open] is enable, [Close] is disable
            openOToolStripMenuItem.Enabled = true;
            closeCToolStripMenuItem.Enabled = false;

            this.Text = "SerialScreen - [Not Connected]";
        }

        // set the screen size
        public void setScreen(int w, int h)
        {
            // set screen size
            this.pictureBox.Width = w;
            this.pictureBox.Height = h;
            this.pictureBox.BackgroundImage = new Bitmap(w, h);

            // set client field size
            this.ClientSize = new System.Drawing.Size(w, h + this.menuStrip.Height);
        }

        // get the screen (picture box)
        public PictureBox getScreen()
        {
            return this.pictureBox;
        }

        // menu [Open]
        private void openOToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormSetting frm = new FormSetting(this);
            frm.StartPosition = FormStartPosition.CenterParent;
            if (frm.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                if (ssServer.open())
                {
                    // [Open] is disable, [Close] is enable
                    openOToolStripMenuItem.Enabled = false;
                    closeCToolStripMenuItem.Enabled = true;

                    this.Text = "SerialScreen - [" + ssServer.getPortName() + "]";
                }
                else
                {
                    MessageBox.Show(ssServer.getErrorMessage(), "ERROR",
                        MessageBoxButtons.OK, MessageBoxIcon.Error);
                }

            }
        }

        // menu [Close]
        private void closeCToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ssServer.close();

            Graphics canvas = Graphics.FromImage(pictureBox.BackgroundImage);
            canvas.Clear(Color.Black);
            pictureBox.Invalidate();

            // [Open] is enable, [Close] is disable
            openOToolStripMenuItem.Enabled = true;
            closeCToolStripMenuItem.Enabled = false;

            this.Text = "SerialScreen - [Not Connected]";
        }

        // menu [Exit]
        private void exitEToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        // menu[About]
        private void aboutAToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormAbout frm = new FormAbout();
            frm.StartPosition = FormStartPosition.CenterParent;
            frm.ShowDialog();
        }

        // terminate
        private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            ssServer.close();
        }

    }
}
