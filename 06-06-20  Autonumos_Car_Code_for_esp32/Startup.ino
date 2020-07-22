void Startup()
{  
     Serial.println("Pause for Startup... ");
             
     for (int i=10; i >= 1; i--)                       // Count down for X seconds
      {         
        Serial.print("Pause for Startup... "); 
        Serial.println(i);
        delay(1000);                                   // Delay for X seconds
      }    
      
      
  Serial.println("Searching for Satellites "); 
  Serial.println("Searching for Satellites "); 
    
  while (Number_of_SATS <= 4)                         // Wait until x number of satellites are acquired before starting main loop
  {                                  
    getGPS();                                         // Update gps data
    Number_of_SATS = (int)(gps.satellites.value());   // Query Tiny GPS for the number of Satellites Acquired       
  } 

    

  Serial.print(Number_of_SATS);
  Serial.println(" Satellites Acquired");    
  //goWaypoint();
  gpsInfo();
 // clearWaypoints();
  //setWaypoint();                                      // set intial waypoint to current location

    
}   
