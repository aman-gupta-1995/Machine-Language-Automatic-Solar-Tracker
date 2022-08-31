/*
  Arduino Solar Tracking Circuit

  Automated single or dual axis Photo-Voltaic(PV) panel array rotation to achieve maximum 
  surface area exposure to sunlight with automatic nightly orientation reset. This will 
  optimize solar power generation with 40%-60% improvement in efficiency over fixed PV 
  panel installations.
  The circuit:
   

  CODE REVISIONS:
  1. Initial code - modified 21 May 2017 By Ryan Downer
  
  LICENSING:
    GNU General Public License v3.0
    Permissions of this strong copyleft license are conditioned on making available
    complete source code of licensed works and modifications, which include larger 
    works using a licensed work, under the same license. Copyright and license notices
    must be preserved. Contributors provide an express grant of patent rights.

*/


// include library definition files
//#include "SolarTracker.h"



/* I\O PIN MAPPING ARDUINO NANO MODULE - Main Controller Module  */
#define ADC_IN_SENSOR_LEFT  6    // Left Swing Axis Sensor   
#define ADC_IN_SENSOR_RIGHT 7    // Right Swing Axis Sensor  

#define ADC_IN_SENSOR_UP     4     // Up Tilt Axis Sensor
#define ADC_IN_SENSOR_DOWN   5     // Down Tilt Axis Sensor

#define PD_OUT_ACTUATOR_SWING_AXIS_CW   3     // Swing Actuator Rotates Clockwise
#define PD_OUT_ACTUATOR_SWING_AXIS_CCW   5     // Swing Actuator Rotates Counter-Clockwise

#define PWM_OUT_ACTUATOR_TILT_AXIS_CW   6   // Tilt Actuator Rotates Clockwise
#define PWM_OUT_ACTUATOR_TILT_AXIS_CCW  9     // Tilt Actuator Rotates Counter-Clockwise



// CREATE SolarTracker CLASS INSTANCE
//SolarTracker Tracker1;


/* GLOBAL VARIABLES Definition */
int sensor_threshold = 25;      //Variable to determine the 

int left_swing_sensor_val = 0;           // variable to store the value read
int right_swing_sensor_val = 0;           // variable to store the value read

int swing_sensors_val_diff =0;

int up_tilt_sensor_val = 0;           // variable to store the value read
int down_tilt_sensor_val = 0;           // variable to store the value read

int tilt_sensors_val_diff = 0;

int tilt_up_speed = 0;
int tilt_down_speed = 0;
//---------------------------------------------


void setup() {
    pinMode(PD_OUT_ACTUATOR_SWING_AXIS_CW, OUTPUT);      // sets the digital pin as output
    pinMode(PD_OUT_ACTUATOR_SWING_AXIS_CCW, OUTPUT);      // sets the digital pin as output

    pinMode(PWM_OUT_ACTUATOR_TILT_AXIS_CW, OUTPUT);      // sets the digital pin as output
    pinMode(PWM_OUT_ACTUATOR_TILT_AXIS_CCW, OUTPUT);      // sets the digital pin as output
       
    Serial.begin(9600);          //  setup serial
//---------------------------------------------
}



void loop() {
  
  // Read Analog Input for Swing Sensor Array.
  left_swing_sensor_val = analogRead(ADC_IN_SENSOR_LEFT);    // read the input pin
  right_swing_sensor_val = analogRead(ADC_IN_SENSOR_RIGHT);    // read the input pin
  
  swing_sensors_val_diff = right_swing_sensor_val - left_swing_sensor_val;
   
  //Serial.println( "swing_sensors_val_diff = " );             // debug value
  //Serial.println( swing_sensors_val_diff );             // debug value
  
  if ( abs(swing_sensors_val_diff) > sensor_threshold )
  {
    // Reset Actuator Led
    digitalWrite(PD_OUT_ACTUATOR_SWING_AXIS_CW, LOW);   // sets the LED Off      
    digitalWrite(PD_OUT_ACTUATOR_SWING_AXIS_CCW, LOW);   // sets the LED Off    
    
    // Checks if swing sensor array difference is greater than the threshold
    if ( swing_sensors_val_diff > 0 )
    {
      // Difference is positive
      digitalWrite(PD_OUT_ACTUATOR_SWING_AXIS_CW, HIGH);   // sets the LED on
    }
    else if ( swing_sensors_val_diff < 0 )
      {
        // Difference is negative
        digitalWrite(PD_OUT_ACTUATOR_SWING_AXIS_CCW, HIGH);   // sets the LED on
      }
    
  }
  else
    {
      // Reset Actuator Led
      digitalWrite(PD_OUT_ACTUATOR_SWING_AXIS_CW, LOW);   // sets the LED Off      
      digitalWrite(PD_OUT_ACTUATOR_SWING_AXIS_CCW, LOW);   // sets the LED Off    
    }

  
  
  // Read Analog Input for Tilt Sensor Array.
  up_tilt_sensor_val = analogRead(ADC_IN_SENSOR_UP);    // read the up tilt sensor input pin
  down_tilt_sensor_val = analogRead(ADC_IN_SENSOR_DOWN);    // read the down tilt sensor input pin
   
  tilt_sensors_val_diff = up_tilt_sensor_val - down_tilt_sensor_val;
   
  Serial.println( "tilt_sensors_val_diff = " );             // debug value
  Serial.println( tilt_sensors_val_diff );             // debug value

  //Serial.println( "up_tilt_sensor_val = " );             // debug value
  //Serial.println( up_tilt_sensor_val );             // debug value
  
  //analogWrite( PWM_OUT_ACTUATOR_TILT_AXIS_CW, up_tilt_sensor_val/4 );  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255

  analogWrite( PWM_OUT_ACTUATOR_TILT_AXIS_CW, 0 );  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
  analogWrite( PWM_OUT_ACTUATOR_TILT_AXIS_CCW, 0 );  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
    
  if ( abs(tilt_sensors_val_diff) > sensor_threshold )
  {
    
    
    // Difference is greater than the threshold
    if ( tilt_sensors_val_diff > 0 )
    {
      // Difference is positive, so rotate avctuator motor clockwise. 
      tilt_up_speed = map(up_tilt_sensor_val,0, 1023, 0,255);     //map range digital to anolog
      
      analogWrite( PWM_OUT_ACTUATOR_TILT_AXIS_CW, abs(tilt_sensors_val_diff) );  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
    }
    else if ( tilt_sensors_val_diff < 0 )
      {
        // Difference is negative, so rotate avctuator motor counter-clockwise. 
        tilt_down_speed = map(down_tilt_sensor_val, 0, 1023, 0, 255);   //map range digital to anolog
        
        analogWrite( PWM_OUT_ACTUATOR_TILT_AXIS_CCW, abs(tilt_sensors_val_diff) );  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
      }
    
  }      
  
  
  delay(900);
//---------------------------------------------
}

