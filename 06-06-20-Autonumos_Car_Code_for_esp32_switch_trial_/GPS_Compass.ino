
 
 // ************************************************************************************************************************************************* 

void getGPS()                                                 // Get Latest GPS coordinates
{  
    while (S2.available() > 0)
{
    gps.encode(S2.read());
     delay(1);  
} }

// *************************************************************************************************************************************************
 
void setWaypoint()                                            // Set up to 5 GPS waypoints
{

  
    Serial.print("GPS Waypoint ");
    Serial.print(" Set ");
    /*lati1= 10.177384;
    logi1= 76.422909;
    lati2= 10.177345;
    logi2= 76.423246;      */                     
    Destination_LAT = lati1 ;                   // store waypoint in an array   
    Destination_LON = logi1 ;             // store waypoint in an array   
                                                                  
    Serial.print("Waypoint #1: ");
    Serial.print(Destination_LAT,6);
    Serial.print(" , ");
    Serial.println(Destination_LON,6);
 
         delay(1);                                                    // increment array counter
        
           
}

// ************************************************************************************************************************************************* 



 // *************************************************************************************************************************************************
 
void getCompass()                                               // get latest compass value
 {   delay(1);  
          int xaxis,yaxis,zaxis;
      qmc.read(&xaxis,&yaxis,&zaxis);
    float heading = atan2(yaxis,xaxis);
    float declinationAngle = (-1.0 + (41.0 / 60.0)) / (180 / M_PI);
    heading += declinationAngle;
    if (heading < 0)
      {
        heading += 2 * PI;
      }
    
  compass_heading = heading ;  //(int)(heading * 180/M_PI);                   // aSerialign compass calculation to variable (compass_heading) and convert to integer to remove decimal places                                                              
Serial.print("Heading == ");
Serial.println(heading);
 }

 // *************************************************************************************************************************************************

void clearWaypoints()
{
   Destination_LAT=0.0;            // clear the array
   Destination_LON=0.0;    // clear the array
   Destination_LAT = gps.location.lat() ;                   // store waypoint in an array   
   Destination_LON= gps.location.lng() ;             // store waypoint in an array   
   
   Serial.println("GPS Waypoints Cleared And Replaced with Home Coordinates");                      // display waypoints cleared
  
}

// *************************************************************************************************************************************************
 
void gpsInfo()                                                  // displays Satellite data to user
  {
        Number_of_SATS = (int)(gps.satellites.value());         //Query Tiny GPS for the number of Satellites Acquired 
        Distance_To_Home = TinyGPSPlus::distanceBetween(gps.location.lat(),gps.location.lng(),Destination_LAT, Destination_LON);  //Query Tiny GPS for Distance to Destination    
        Serial.print("Lat:");
        Serial.print(gps.location.lat(),6);
        Serial.print(" Lon:");
        Serial.print(gps.location.lng(),6);
        Serial.print(" ");
        Serial.print(Number_of_SATS); 
        Serial.print(" SATs ");
        Serial.print(Distance_To_Home);
        Serial.print("m"); 
Serial.print("Distance to Home ");
Serial.println(Distance_To_Home);
  
  }
