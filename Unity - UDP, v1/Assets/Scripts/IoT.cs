using UnityEngine;
using KyleDulce.SocketIo;

public class IoT : MonoBehaviour
{
    private bool runLocal = true;
    Socket socket;

    private string currentLEDValue = "0";


    private string currentSensorValue = "0";
    private string currentLEDLightValue = "0";
    private string currentStepperValue = "0";

    public Camera firstPersonCamera;
    public Camera overheadCamera;

    private Light spotlightA;
    public float minIntensity = 0f;
    public float maxIntensity = 15f;
    public float minRange = 1f;
    public float maxRange = 100.0F;

    public float max_Range = 100f;

    Transform spotlightATransform;
    public Transform lightTransform;


    private GameObject cube;

    public Material curtain;





    // Start is called before the first frame update
    void Start()
    {

        GameObject SpotLightA = new GameObject("SpotlightA");
        spotlightATransform = SpotLightA.transform;

        spotlightA = SpotLightA.AddComponent<Light>();

        spotlightA.type = LightType.Spot;


        spotlightA.transform.position = new Vector3(0,10,0);
        
        spotlightA.transform.rotation = Quaternion.Euler(90,0,0);

        spotlightA.transform.localScale = new Vector3(10,10,10);

        spotlightA.intensity = 2;
        spotlightA.spotAngle = 180f;


        cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
        cube.transform.position = new Vector3(-4f, 10f, 0f);
        cube.transform.rotation = Quaternion.Euler(0,0,90);
        cube.transform.localScale = new Vector3(1,0.1f,10f);

        cube.GetComponent<MeshRenderer>().material = curtain;



        /*--------------               Connect to Server                                    ----------------------*/
        if (runLocal)
        {
            Debug.Log("Connect to Local Server");
            socket = SocketIo.establishSocketConnection("ws://localhost:3000");
        }
        else
        {
            Debug.Log("Connect to Online Server");
            socket = SocketIo.establishSocketConnection("ws://sdu-e22-iot-v1.eu-4.evennode.com");
        }

        //Connect to server
        socket.connect();
        Debug.Log("Socket IO - Connected");



        /*--------------               Receive Updates                                    ----------------------*/
        //On "CurrentLEDValue"
        socket.on("CurrentLEDValue", SetCurrentLEDValue);

        //On "CurrentPotentiometerValue"

        // On "CurrentLEDLevelValue"
        socket.on("CurrentLEDLevelValue", SetCurrentLEDLightValue);


        // On "CurrentStepperValue"
        socket.on("CurrentStepperValue", SetCurrentStepperValue);


        // on "CurrentSensorValue"
        socket.on("CurrentSensorValue", SetCurrentSensorValue);

    }

    void SetCurrentLEDValue(string data)
    {
        currentLEDValue = data;
        Debug.Log("CurrentLEDValue Received: " + currentLEDValue);
    }

   
    void SetCurrentSensorValue(string data){
        currentSensorValue = data;
        Debug.Log("CurrentSensorValue Received: " + currentSensorValue);
    }


    void SetCurrentStepperValue(string data)
    {
        currentStepperValue = data;
        Debug.Log("CurrentStepperValue Received: " + currentStepperValue);
    }

    void SetCurrentLEDLightValue(string data)
    {
        currentLEDLightValue = data;
        Debug.Log("CurrentLEDLightValue Received: "+ currentLEDLightValue);
    }


    public void SetRange(float value) {
        float input = Mathf.Clamp01(value / 255f);
        float range = Mathf.Lerp(minRange, max_Range, input);
        this.spotlightA.range = range;
    }

    public void UpdateCurtain(int value) {
        
        
        

        float positionY = ((1/2800f) *value) + 6.3875f;
        float scaleX = ((-1/1400f)*value) + 7.2143f;


        Debug.Log("ScaleX: "+scaleX);
        Debug.Log("Position Y: "+ positionY);

        cube.transform.position = new Vector3(-4f, positionY, 0f);
        cube.transform.localScale = new Vector3(scaleX,0.1f,10f);

        /*
        if(value == 0){
            cube.transform.position = new Vector3(-4f, 9.5f, 0f);
            cube.transform.localScale = new Vector3(1f,0.1f,10f);
        } else if(value == 9000){
            cube.transform.position = new Vector3(-4f, 6.5f, 0f);
            cube.transform.localScale = new Vector3(7f,0.1f,10f);
        }
        */
    }


    void Update()
    {

        spotlightATransform.LookAt(lightTransform);

        SetRange(float.Parse(currentLEDLightValue));


        UpdateCurtain(int.Parse(currentStepperValue));
        
        
    }
}

