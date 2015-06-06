using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Reflection;
using System.Runtime.InteropServices;

namespace SerialScreen
{
    public partial class FormAbout : Form
    {
        // initialize
        public FormAbout()
        {
            InitializeComponent();

            // Title
            this.Text = "About " + Application.ProductName;

            // Application Name
            labelName.Text = Application.ProductName;

            // Version
            labelVersion.Text = "Version " + Application.ProductVersion;

            // Copy Right
            Assembly mainAssembly = Assembly.GetEntryAssembly();
            string appCopyright = "-";
            object[] CopyrightArray =
              mainAssembly.GetCustomAttributes(
                typeof(AssemblyCopyrightAttribute), false);
            if ((CopyrightArray != null) && (CopyrightArray.Length > 0))
            {
                appCopyright =
                  ((AssemblyCopyrightAttribute)CopyrightArray[0]).Copyright;
            }
            labelCopyRight.Text = appCopyright;

        }

        // button [OK]
        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
