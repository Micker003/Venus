using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;
using M2MqttUnity;


namespace M2MqttUnity.Examples
{
    /// <summary>
    /// Script for testing M2MQTT with a Unity UI
    /// </summary>
    public class MainScript : M2MqttUnityClient
    {
        [Tooltip("Set this to true to perform a testing cycle automatically on startup")]
        public bool autoTest = false;
        [Header("User Interface")]
        // public InputField consoleInputField;
        // public Toggle encryptedToggle;
        // public InputField addressInputField;
        // public InputField portInputField;
        // public Button connectButton;
        // public Button disconnectButton;
        // public Button testPublishButton;
        // public Button clearButton;
        
        public int botID;
        
        [Header("MQTT config")]
        float factorUnitConvert = 0.5f;
        public int MQTTPreset;

        [Header("Text file (.txt) containing debug messages")]
        public TextAsset debugText;
        public Boolean debugTextEnabled;
        private float debugDelay = 0;
        private int debugLine = 0;
        private float prevTime = 0;

        [Header("Items existing in the scene")]
        public GameObject Bottom;
        
        public GameObject SmallBlackCube;
        public GameObject SmallBlueCube;
        public GameObject SmallGreenCube;
        public GameObject SmallRedCube;
        public GameObject SmallWhiteCube;

        public GameObject BigBlackCube;
        public GameObject BigBlueCube;
        public GameObject BigGreenCube;
        public GameObject BigRedCube;
        public GameObject BigWhiteCube;

        public GameObject Robot0;
        public GameObject Robot1;
        
        public GameObject BoundaryNorth;
        public GameObject BoundarySouth;
        public GameObject BoundaryWest;
        public GameObject BoundaryEast;
        
        [Header("Instantiated items")]
        public GameObject Obstacle;
        public GameObject Cliff;
        // public GameObject Boundary;
        [Header("Arrays holding the instantiated items")]
        public GameObject[] Obstacles;
        public GameObject[] Cliffs;
        // public GameObject[] Boundaries;
        [Header("Arrays holding the location history of the bots")]
        public float[] Bot0XLocations;
        public float[] Bot0YLocations;
        public float[] Bot1XLocations;
        public float[] Bot1YLocations;
        

        private List<string> eventMessages = new List<string>();
        private bool updateUI = false;
        
        private string mqttTopic = "/pynqbridge/14/#";

        public void TestPublish()
        {
            client.Publish("M2MQTT_Unity/test", System.Text.Encoding.UTF8.GetBytes("Test message"), MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, false);
            Debug.Log("Test message published");
            // AddUiMessage("Test message published.");
        }
        
        public void UnitConvert( out float xOut, out float yOut, out float zOut, float x = 0, float y = 0, float z = 0)
        {
            // float factorUnitConvert = 0.01f;
            xOut = x * factorUnitConvert;
            yOut = y * factorUnitConvert;
            zOut = z * factorUnitConvert;
        }

        public void HandleCommand(int ID, int botID = 0, float xIn = 0, float yIn = 0, float zIn = 0)
        {
            // Bot 0 is the default bot, 1 is the other one
            float x, y, z;
            UnitConvert(out x, out y, out z, xIn, yIn, zIn);
            
            // REMEMBER: y is the height! 
            float yDefault = 0.3f;
            float yBot = 1f;
            float yBoundary = 0.01f;
            
            switch (ID)
            {
                // Robot locations
                case 0:
                    
                    // Append the bot location to the array
                    if (botID == 0)
                    {
                        Robot0.transform.position = new Vector3(x, yBot, z);
                        Array.Resize(ref Bot0XLocations, Bot0XLocations.Length + 1);
                        Array.Resize(ref Bot0YLocations, Bot0YLocations.Length + 1);
                        Bot0XLocations[Bot0XLocations.Length - 1] = xIn;
                        Bot0YLocations[Bot0YLocations.Length - 1] = yIn;
                    }
                    else if (botID == 1)
                    {
                        Robot1.transform.position = new Vector3(x, yBot, z);
                        Array.Resize(ref Bot1XLocations, Bot1XLocations.Length + 1);
                        Array.Resize(ref Bot1YLocations, Bot1YLocations.Length + 1);
                        Bot1XLocations[Bot1XLocations.Length - 1] = xIn;
                        Bot1YLocations[Bot1YLocations.Length - 1] = yIn;
                    }
                    break;
                
                // Cube locations
                case 1:
                    SmallBlackCube.transform.position = new Vector3(x, yDefault, z);
                    break;
                case 2:
                    SmallBlueCube.transform.position = new Vector3(x, yDefault, z);
                    break;
                case 3:
                    SmallGreenCube.transform.position = new Vector3(x, yDefault, z);
                    break;
                case 4:
                    SmallRedCube.transform.position = new Vector3(x, yDefault, z);
                    break;
                case 5:
                    SmallWhiteCube.transform.position = new Vector3(x, yDefault, z);
                    break;
                case 6:
                    BigBlackCube.transform.position = new Vector3(x, 2*yDefault, z);
                    break;
                case 7:
                    BigBlueCube.transform.position = new Vector3(x, 2*yDefault, z);
                    break;
                case 8:
                    BigGreenCube.transform.position = new Vector3(x, 2*yDefault, z);
                    break;
                case 9:
                    BigRedCube.transform.position = new Vector3(x, 2*yDefault, z);
                    break;
                case 10:
                    BigWhiteCube.transform.position = new Vector3(x, 2*yDefault, z);
                    break;
                
                // Boundary locations
                
                case 15:
                    BoundaryNorth.transform.position = new Vector3(x, yBoundary, z);
                    break;
                case 16:
                    BoundarySouth.transform.position = new Vector3(x, yBoundary, z);
                    break;
                case 17:
                    BoundaryWest.transform.position = new Vector3(x, yBoundary, z);
                    break;
                case 18:
                    BoundaryEast.transform.position = new Vector3(x, yBoundary, z);
                    break;
                
                // Obstacle locations
                case 20:
                    //Logic for merging the obstacles will be implemented later
                    Obstacle = Instantiate(Obstacle, new Vector3(x, yDefault, z), Quaternion.identity);
                    // Append the instantiated object to the array 
                    Array.Resize(ref Obstacles, Obstacles.Length + 1);
                    Obstacles[Obstacles.Length - 1] = Obstacle;
                    break;
                 
                // Cliff locations
                case 25:
                    Cliff = Instantiate(Cliff, new Vector3(x, yDefault, z), Quaternion.identity);
                    // Append the instantiated object to the array
                    Array.Resize(ref Cliffs, Cliffs.Length + 1);
                    Cliffs[Cliffs.Length - 1] = Cliff;
                    break;
                
                case 50:
                    // Implement passing the command through the MQTT to the other bot
                    // Handled elsewhere.
                    break;
                
                default:
                    Debug.Log("Invalid command!");
                    break;
            }
        }



        // public void SetBrokerPort(string brokerPort)
        // {
        //     if (portInputField && !updateUI)
        //     {
        //         int.TryParse(brokerPort, out this.brokerPort);
        //     }
        // }

        // public void SetEncrypted(bool isEncrypted)
        // {
        //     this.isEncrypted = isEncrypted;
        // }

        protected override void OnConnecting()
        {
            base.OnConnecting();
            Debug.Log("Connecting to broker on " + brokerAddress + ":" + brokerPort.ToString() + "...\n");
        }

        protected override void OnConnected()
        {
            base.OnConnected();
            Debug.Log("Connected to broker on " + brokerAddress + "\n");
            
            if (autoTest)
            {
                TestPublish();
            }
            SubscribeTopics();
        }

        protected override void SubscribeTopics()
        {
            client.Subscribe(new string[] { mqttTopic }, new byte[] { MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE });
            Debug.Log("Subscribed to topic: " + mqttTopic);
        }

        protected override void UnsubscribeTopics()
        {
            client.Unsubscribe(new string[] { mqttTopic});
        }

        protected override void OnConnectionFailed(string errorMessage)
        {
            Debug.Log("CONNECTION FAILED! " + errorMessage);
        }

        protected override void OnDisconnected()
        {
            Debug.Log("Disconnected.");
        }

        protected override void OnConnectionLost()
        {
            Debug.Log("CONNECTION LOST!");
        }

        protected override void Start()
        {
            Debug.Log("Ready.");
            updateUI = true;
            
            // Test config
            // this.brokerAddress = "public.mqtthq.com";
            // this.brokerPort = 1883;
            
            // TuE config
            this.brokerAddress = "mqtt.ics.ele.tue.nl";
            this.brokerPort = 1883;
            if (MQTTPreset == 0)
            {
                this.mqttUserName = "Student27";
                this.mqttPassword = "shoo8ahS";
            }
            else if (MQTTPreset == 1)
            {
                this.mqttUserName = "Student28";
                this.mqttPassword = "ohsh5UJa";
            }
            else if (MQTTPreset == 2)
            {
                this.mqttUserName = "Student145";
                this.mqttPassword = "Fai1oiJ9";
                this.mqttTopic = "/pynqbridge/75/#";
            }
            else //if (MQTTPreset == 3)
            {
                this.mqttUserName = "Student146";
                this.mqttPassword = "Phob5kai";
                this.mqttTopic = "/pynqbridge/75/#";
            }
            // else
            // {
            //     this.mqttUserName = "";
            //     this.mqttPassword = "";
            // }
            
            
            
            base.Start();
            Connect();
            
            
        }

        private void UpdateUI()
        {
            
        }

        protected override void DecodeMessage(string topic, byte[] message)
        {
            string msg = System.Text.Encoding.UTF8.GetString(message);
            Debug.Log("Received: " + msg);
            StoreMessage(msg);
            if (topic == "M2MQTT_Unity/test")
            {
                if (autoTest)
                {
                    autoTest = false;
                    Disconnect();
                }
            }
        }

        private void StoreMessage(string eventMsg)
        {
            eventMessages.Add(eventMsg);
        }

        private void ProcessMessage(string msg, int botID = 0)
        {
            Debug.Log("Received: " + msg);
            //Split the comma-seperated message into an array
            string[] msgArray = msg.Split(',');
            //Convert the array values to integers. If these are not integers, print to log and pass
            int ID;
            float x;
            float z;
            
            if (!int.TryParse(msgArray[0], out ID))
            {
                Debug.Log("Invalid ID!");
                return;
            }
            //If the ID is 50, handle passthrough code
            if (ID == 50)
            {
                if (botID == 0)
                {
                    Bottom.GetComponent<VarStorage>().botMsg1 = msg; // Invert the botID as the message is passed to the other bot
                }
                else
                {
                    Bottom.GetComponent<VarStorage>().botMsg0 = msg;
                }
                
                return;
            }
            
            if (!float.TryParse(msgArray[1], out x))
            {
                Debug.Log("Invalid x!");
                return;
            }
            if (!float.TryParse(msgArray[2], out z))
            {
                Debug.Log("Invalid z!");
                return;
            }
            //If the message is valid, handle the command
            HandleCommand(ID, botID, x, 0, z);
            
        }

        protected override void Update()
        {
            base.Update(); // call ProcessMqttEvents()
            // Read the variable ToBot1 attached to the Bottom GameObject
            // If the variable is true, pass the message to the other bot
            if (debugTextEnabled)
            {
                if (debugDelay != 0)
                {
                    // Subtract the time that has passed since the last time update has been ran from the delay
                    debugDelay = debugDelay - (Time.time - prevTime);
                    if (debugDelay < 0)
                    {
                        debugDelay = 0;
                    }
                }

                if (debugDelay == 0)
                {
                    // Get the amount of lines in the debug text
                    int lineCount = debugText.text.Split('\n').Length;
                    
                    if (lineCount > debugLine) // Handle the line
                    {
                        string currentLine = debugText.text.Split('\n')[debugLine];
                        debugLine++;
                        //  If the line begins with an @, it is followed by a delay float value seperated by a dot
                        if (currentLine[0] == '@')
                        {
                            float delay;
                            if (!float.TryParse(currentLine.Substring(1), out delay))
                            {
                                Debug.Log("Invalid delay value!");
                                return;
                            }
                            Debug.Log("Delay loaded: " + delay + " seconds.");
                            debugDelay = delay;
                        }
                        else
                        {
                            Debug.Log("Processing message: " + currentLine);
                            ProcessMessage(currentLine);
                        }
                        
                    }
                }
                
                prevTime = Time.time;
            }
            
            if (botID == 0) // If this is bot 0
            {
                if (Bottom.GetComponent<VarStorage>().botMsg0 != "")
                {
                    client.Publish(mqttTopic , System.Text.Encoding.UTF8.GetBytes(Bottom.GetComponent<VarStorage>().botMsg0), MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, false);
                    Bottom.GetComponent<VarStorage>().botMsg0 = "";
                }
            }
            else
            {
                if (Bottom.GetComponent<VarStorage>().botMsg1 != "")
                {
                    client.Publish(mqttTopic , System.Text.Encoding.UTF8.GetBytes(Bottom.GetComponent<VarStorage>().botMsg1), MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, false);
                    Bottom.GetComponent<VarStorage>().botMsg1 = "";
                }
            }
            
            
            
                // Pass the message
            if (eventMessages.Count > 0)
            {
                foreach (string msg in eventMessages)
                {
                    ProcessMessage(msg);
                }
                eventMessages.Clear();
            }
            if (updateUI)
            {
                UpdateUI();
            }
        }

        private void OnDestroy()
        {
            Disconnect();
        }

        private void OnValidate()
        {
            if (autoTest)
            {
                autoConnect = true;
            }
        }
    }
}
