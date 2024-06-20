using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
// using System.Math;

public class MagneticsScript : MonoBehaviour
{
    // Start is called before the first frame update
    // There are 2 arrows, showcasing the k and E vectors. The E vector is the electric field vector, and k is the wave vector
    public GameObject kArrow;
    public GameObject EArrow;
    
//    void Start()
//    {
//        
//    }

    // Update is called once per frame
    void Update()
    {
        // Get the time
        float time = Time.time;
        // Define the wave vector k
        Vector3 k = new Vector3(15*time, 15*time, 0);
        // Define the electric field vector E using the wave vector k
        
        Vector3 E_0 = new Vector3(15, 15, 0);
        // pi
        float pi = (float)Math.PI;
        Vector3 theta = new Vector3(pi/4, pi/4, 0);
        // Vector3 A = new Vector3(E_0.x * Math.E^theta.x, E_0.y * Math.E^theta.y, E_0.z * Math.E^theta.z);

    }
}
