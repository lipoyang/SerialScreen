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
    public partial class FormSetting : Form
    {
        private FormMain parent;

        // setting data structure
        public struct SettingData
        {
            public string portName;
            public int baudRate;
        }

        // setting file name
        private const string SETTING_FIME_NAME = "setting.conf";

        // initialize
        public FormSetting(FormMain parent)
        {
            this.parent = parent;

            InitializeComponent();

            // COM Port list (searching available COM ports)
            comboBoxPort.DropDownStyle = ComboBoxStyle.DropDownList; // read only
            string[] ports = System.IO.Ports.SerialPort.GetPortNames();
            if (ports.Length == 0)
            {
                ports = new string[1];
                ports[0] = "Not Found";
                buttonOK.Enabled = false;
            }
            else
            {
                buttonOK.Enabled = true;
            }
            foreach (string port in ports)
            {
                comboBoxPort.Items.AddRange(new object[] { port });
            }
            comboBoxPort.SelectedIndex = 0;

            // Baud Rate list
            comboBoxBaud.DropDownStyle = ComboBoxStyle.DropDownList; // read only
            comboBoxBaud.Items.Add("9600");
            comboBoxBaud.Items.Add("19200");
            comboBoxBaud.Items.Add("38400");
            comboBoxBaud.Items.Add("57600");
            comboBoxBaud.Items.Add("115200");
            comboBoxBaud.SelectedIndex = 0;

            loadSetting();
        }

        // load settings
        private void loadSetting()
        {
            SettingData setting;
            try
            {
                // load setting file
                System.Xml.Serialization.XmlSerializer serializer =
                    new System.Xml.Serialization.XmlSerializer(typeof(SettingData));
                System.IO.FileStream fs =
                    new System.IO.FileStream(SETTING_FIME_NAME, System.IO.FileMode.Open);
                setting = (SettingData)serializer.Deserialize(fs);
                fs.Close();

                // COM Port Name
                int index = comboBoxPort.FindStringExact(setting.portName);
                if (index >= 0)
                {
                    comboBoxPort.SelectedIndex = index;
                }

                // Baud Rate
                index = comboBoxBaud.FindStringExact(setting.baudRate.ToString());
                if (index >= 0)
                {
                    comboBoxBaud.SelectedIndex = index;
                }
            }
            catch
            {
                comboBoxPort.SelectedIndex = 0;
                comboBoxBaud.SelectedIndex = 0;
            }
        }

        // save setting
        private void saveSetting()
        {
            SettingData setting;

            // COM Port Name
            setting.portName = comboBoxPort.Text;
            // Baud Rate
            setting.baudRate = int.Parse(comboBoxBaud.Text);

            // save setting file
            try
            {
                System.Xml.Serialization.XmlSerializer serializer =
                    new System.Xml.Serialization.XmlSerializer(typeof(SettingData));
                System.IO.FileStream fs =
                    new System.IO.FileStream(SETTING_FIME_NAME, System.IO.FileMode.Create);
                serializer.Serialize(fs, setting);
                fs.Close();
            }
            catch
            {
                MessageBox.Show("Can not save settings!", "ERROR", 
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        // button [OK]
        private void buttonOK_Click(object sender, EventArgs e)
        {
            parent.ssServer.setBaudRate(int.Parse(comboBoxBaud.Text));
            parent.ssServer.setPortName(comboBoxPort.Text);

            saveSetting();

            this.DialogResult = DialogResult.OK;

            this.Close();
        }

        // button [Cancel]
        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;

            this.Close();
        }
    }
}
