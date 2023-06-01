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

 

    private Light pointLightA;
   
    public float minRange = 1f;
    public float maxRange = 100.0F;

    public float max_Range = 100f;

    Transform pointLightATransform;
    public Transform lightTransform;


    private GameObject cube;

    public Material curtain;





    // Start is called before the first frame update
    void Start()
    {

        GameObject PointLightA = new GameObject("pointLightA");
        pointLightATransform = PointLightA.transform;

        pointLightA = PointLightA.AddComponent<Light>();

        pointLightA.type = LightType.Point;


        pointLightA.transform.position = new Vector3(0,10,0);
        
        pointLightA.transform.rotation = Quaternion.Euler(90,0,0);

        pointLightA.transform.localScale = new Vector3(10,10,10);

        pointLightA.intensity = 2;
        pointLightA.spotAngle = 180f;


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
      


        // On "CurrentLEDLevelValue"
        socket.on("CurrentLEDLevelValue", SetCurrentLEDLightValue);


        // On "CurrentStepperValue"
        socket.on("CurrentStepperValue", SetCurrentStepperValue);


        // on "CurrentSensorValue"
        socket.on("CurrentSensorValue", SetCurrentSensorValue);

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
        this.pointLightA.range = range;
    }

    public void UpdateCurtain(int value) {
        
        float positionY = ((1/2800f) *value) + 6.3875f;
        float scaleX = ((-1/1400f)*value) + 7.2143f;

        cube.transform.position = new Vector3(-4f, positionY, 0f);
        cube.transform.localScale = new Vector3(scaleX,0.1f,10f);

    }


    void Update()
    {

        pointLightATransform.LookAt(lightTransform);

        SetRange(float.Parse(currentLEDLightValue));


        UpdateCurtain(int.Parse(currentStepperValue));
        
        
    }
}

