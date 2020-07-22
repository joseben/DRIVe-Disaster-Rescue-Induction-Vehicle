void goWaypoint()
{  
Serial.println("Go to Waypoint");
Serial.print("Distance to Home = ");   
Distance_To_Home = TinyGPSPlus::distanceBetween(gps.location.lat(),gps.location.lng(),Destination_LAT, Destination_LON);  //Query Tiny GPS for Distance to Destination    
Serial.println(Distance_To_Home);

 while (true)  
  {   delay(5);                                                             // Start of Go_Home procedure 
       c = client.readStringUntil('\0');
     if(c.length()!=0 && c.length()<5)
       {
        Serial.println("STOP");
         StopCar();
         blueToothVal=5;
       }
  if (blueToothVal == 5){break;}                                   // If a 'Stop' Bluetooth command is received then break from the Loop
  getCompass();                                                    // Update Compass heading                                          
  getGPS();                                                        // Tiny GPS function that retrieves GPS data - update GPS location// delay time changed from 100 to 10
  
  if (millis() > 5000 && gps.charsProcessed() < 10)                // If no Data from GPS within 5 seconds then send error
    Serial.println(F("No GPS data: check wiring"));     
 
  Distance_To_Home = TinyGPSPlus::distanceBetween(gps.location.lat(),gps.location.lng(),Destination_LAT, Destination_LON);  //Query Tiny GPS for Distance to Destination
  GPS_Course = TinyGPSPlus::courseTo(gps.location.lat(),gps.location.lng(),Destination_LAT,Destination_LON);                               //Query Tiny GPS for Course to Destination   
   
   /*
    if (Home_LATarray[ac] == 0) {
      Serial1.print("End of Waypoints");
      StopCar();      
      break;
      }      
   */ 
    if (Distance_To_Home == 0)                                   // If the Vehicle has reached it's Destination, then Stop
        {
        StopCar();                                               // Stop the robot after each waypoint is reached
        Serial.println("You have arrived!");                    // Print to Bluetooth device - "You have arrived"          
        break;                                                   // Break from Go_Home procedure and send control back to the Void Loop 
        
        }   
   
   
   if ( abs(GPS_Course - compass_heading) <= 15)                  // If GPS Course and the Compass Heading are within x degrees of each other then go Forward                                                                  
                                                                  // otherwise find the shortest turn radius and turn left or right  
       {
         Forward();                                               // Go Forward
       } else 
         {                                                       
            int x = (GPS_Course - 360);                           // x = the GPS desired heading - 360
            int y = (compass_heading - (x));                      // y = the Compass heading - x
            int z = (y - 360);                                    // z = y - 360
            Serial.print("GPS Course = "); 
            Serial.println(GPS_Course);
            Serial.print("X = "); 
            Serial.println(x);
            Serial.print("Y = "); 
            Serial.println(y);
            Serial.print("Z = "); 
            Serial.println(z); 
            if ((z <= 160) && (z >= 20))                           // if z is less than 180 and not a negative value then turn left otherwise turn right
                  { SlowLeftTurn();  
                    }
             else { SlowRightTurn();
                    }               
        } 
    

  }                                                              // End of While Loop

  
}                                                                // End of Go_Home procedure
