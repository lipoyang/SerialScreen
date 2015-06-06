using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace SerialScreen
{
    public class SerialScreenServer
    {
        // charactor code [start of text] / [end of text]
        private const Byte CODE_STX = 0x02;
        private const Byte CODE_ETX = 0x03; 

        // serial port
        private SerialPort serialPort;

        // error message
        private string errorMessage;
        public string getErrorMessage()
        {
            return errorMessage;
        }

        // state of receiver
        private enum RxState { READY, RECEIVING };
        private RxState rxState;

        // buffer of receiver
        private const int RX_BUFFER_SIZE = 256;
        private Byte[] rxBuffer;
        private int rxBufferIdx;

        // SerialScreen API Dispatcher
        SerialScreenAPI ssAPI;

        // initialize
        public SerialScreenServer(FormMain parent)
        {
            // serial port
            serialPort = new SerialPort();
            serialPort.PortName = "COM1";  // default
            serialPort.BaudRate = 9600;    // default
            serialPort.Parity = System.IO.Ports.Parity.None;
            serialPort.DataBits = 8;
            serialPort.StopBits = System.IO.Ports.StopBits.One;
            serialPort.ReadTimeout = 3000; // need not?
            serialPort.ReceivedBytesThreshold = 1;
            serialPort.DataReceived += new SerialDataReceivedEventHandler(serialPort_DataReceived);

            rxState = RxState.READY;
            rxBuffer = new Byte[RX_BUFFER_SIZE];
            rxBufferIdx = 0;

            ssAPI = new SerialScreenAPI();
            ssAPI.setPictureBox(parent);
        }

        // set baud rate
        public void setBaudRate(int baudRate)
        {
            serialPort.BaudRate = baudRate;
        }

        // set port name
        public void setPortName(string portName)
        {
            serialPort.PortName = portName;
        }
        // get port name
        public string getPortName()
        {
            return serialPort.PortName;
        }

        // open
        public bool open()
        {
            serialPort.Close();

            try
            {
                serialPort.Open();
                serialPort.DiscardInBuffer();
                serialPort.DiscardOutBuffer();

                rxState = RxState.READY;
                rxBufferIdx = 0;
            }
            catch
            {
                errorMessage = "Can not open " + serialPort.PortName;
                serialPort.Close();
                return false;
            }
            return true;
        }

        // close
        public void close()
        {
            serialPort.Close();
        }

        // serial port data received event handler
        private void serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            Byte[] recvData = new Byte[serialPort.BytesToRead];
            serialPort.Read(recvData, 0, recvData.Length);


            for(int i=0;i<recvData.Length;i++)
            {
                Byte c = recvData[i];

                switch(rxState)
                {
                    case RxState.READY:
                        if(c == CODE_STX)
                        {
                            rxState = RxState.RECEIVING;
                            rxBufferIdx = 0;
                        }
                        break;

                    case RxState.RECEIVING:
                        if(c == CODE_STX)
                        {
                            rxBufferIdx = 0;
                        }
                        else if(c == CODE_ETX)
                        {
                            rxBuffer[rxBufferIdx] = 0x00;
                            ssAPI.executeCommand(rxBuffer);
                            rxState = RxState.READY;
                        }
                        else
                        {
                            rxBuffer[rxBufferIdx] = c;
                            rxBufferIdx++;
                            if (rxBufferIdx >= RX_BUFFER_SIZE)
                            {
                                rxState = RxState.READY;
                            }
                        }
                        break;

                    default:
                        rxState = RxState.READY;
                        break;
                }
            }
        }
    }
}
