void Coordinates()
{
c.toCharArray(cs, 37);
char latitude1[10];
char longitude1[10];
char latitude2[10];
char longitude2[10];

     strncpy(latitude1, cs, 9);
     strncpy(longitude1, cs+9, 9);
     strncpy(latitude2, cs+18, 9);
     strncpy(longitude2, cs+27, 9);
     
    String la1(latitude1);
    String lo1(longitude1);
    String la2(latitude2);
    String lo2(longitude2);
    
    lati1=(la1.toFloat());
    logi1=(lo1.toFloat());
    lati2=(la2.toFloat());
    logi2=(lo2.toFloat());
    
    Serial.println(lati1,6);
    Serial.println(logi1,6);
    Serial.println(lati2,6);
    Serial.println(logi2,6);

    Destination_LAT = lati1 ;                   // store waypoint in an array   
    Destination_LON = logi1 ;             // store waypoint in an array   
                                                                  
    Serial.print("Waypoint #1: ");
    Serial.print(Destination_LAT,6);
    Serial.print(" , ");
    Serial.println(Destination_LON,6);
 
         delay(1);  
  
Serial.println("Begin Journey");
     //  Startup(); 
      blueToothVal=0;
      goWaypoint();
}
