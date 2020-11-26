/////////////////////////////////////////////////////////////////////////////////////////
//
//Mori by Roye Fang
//Based on StateChangeDetection example code
//Last Updated 11/25/2020
//
//Mori is a memory game that tests an individual's short-term memory. This game
//features an Arduino Uno, push buttons, and an RGB LED. The game flashes a string
//of numbers from one to three (1-3) and gives the player a set amount of time to
//memorize them. The string of numbers will be cleared from the screen and the player
//will attempt to reproduce the aforementioned numbers using the push buttons.
//
//A sample string: "1 3 3 1 3 2 1"
//
//To play, open the serial monitor and adjust the window size, such that the numbers
//are no longer visible when entering the string. Also, make sure autoscroll is on.
//The player can adjust the length of the string, the amount of time they have to
//memorize the string, as well as the number of rounds they would like to play.
//At the end, the serial monitor will display a cumulative score and average input time.
//
//
//This project is dedicated to UCLA Physics 4AL Group 3 (Spring 2020) 
//for making an online lab experience relaxing and enjoyable.
//
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////
///////////////Settings//////////////////
/////////////////////////////////////////

const int stringLength = 7; //The length of the string of numbers that the user will memorize
int memorizingTime = 5000; //The amount of time a user has to memorize
int numberOfRounds = 5; //The number of rounds the user desires

/////////////////////////////////////////
////////////////globals//////////////////
/////////////////////////////////////////

//defining pins
const int  b1 = 2;
const int  b2 = 3;
const int  b3 = 4;  

//keeps track of button state
int currentState1 = 0; // current state of button 1
int prevState1 = 0; // previous state of the button 1
int currentState2 = 0;        
int prevState2 = 0;    
int currentState3 = 0; 
int prevState3 = 0; 

//keeps track of how many times a button has been pressed in each game
//once this number equals stringLength, the next round will begin
int buttonCounter;

//random number
//note: Arduino's random function is not effective. Numbers seqeuences will often repeat.
long randNumber;

//elapsed trial counter to keep track of numbers
int roundCounter = 0;

//ensure correct logic paths
bool rerun;

//for recording button pressing time
unsigned long previousTime;
unsigned long elapsedTime;
unsigned long totalTime;
unsigned long avgTime;

//scoring system
//"randomly" generated and user inputted array of numbers
int randomNumbers[stringLength];
int userNumbers[stringLength];

int intCorrect; //same as float, used for the fractional scoring
int totalIntCorrect = 0;

float correct;
float score;

float totalCorrect = 0;
float totalScore;

//RGB LED
int red = 7;
int green = 6;
int blue = 5;

/////////////////////////////////////////
/////////////////game////////////////////
/////////////////////////////////////////
void game()
{   
    //blue
    RGB(0,0,255);
    if(roundCounter == numberOfRounds)
    {   
        finishGame();
    }
    //reset the button counters for next game
    buttonCounter = 0;
    correct = 0;
    intCorrect = 0;
  
    rerun = true;
    roundCounter++;

  
    // initialize the buttons as input
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(4, INPUT);

    //creates random numbers
    randomSeed(analogRead(0));

    //starts the experiments, giving the user 3 seconds
    //Serial.println("Experiment will begin shortly...");
    Serial.print("Round ");
    Serial.println(roundCounter);
  
    //prints some spaces
   for(int i = 0; i <= 5; i++)
   {
       Serial.println(" ");
   }
  
    delay(500);

    //create randomNumbers array
    for(int i = 0; i < stringLength; i++)
    {
      randNumber = random(1, 4);
      randomNumbers[i] = randNumber;
    }

    //print randomNumbers array
    for(int i = 0; i < stringLength; i++)
    {
        Serial.print(randomNumbers[i]);
        Serial.print(" ");
    }
  
    for(int i = 0; i < 8; i++)
    {
        Serial.println();
    }  

    //gives the user this many milliseconds to memorize the above numbers (3000, 5000)?
    delay(memorizingTime);

   

    //prints some spaces so the above numbers go out of view
    for(int i = 0; i < 14; i++)
    {
        Serial.println(" ");
    }  
    
    RGB(0,255,0);
    previousTime = millis();
}

////////////////////////////////////////////////////////
///////////////////////scoring//////////////////////////
////////////////////////////////////////////////////////
void scoring()
{

    for(int i = 0; i < stringLength; i++)
    {
        if(randomNumbers[i] == userNumbers[i])
        {
            correct++;
            intCorrect++;
        }
    }
    
    totalCorrect += correct;
    totalIntCorrect += intCorrect;
    
    score = (correct/stringLength)*100;

    Serial.print("Score: ");
    Serial.print(intCorrect);
    Serial.print("/");
    Serial.print(stringLength);
    Serial.print(" || ");
    Serial.print(score);
    Serial.print("%");
}

////////////////////////////////////////////////////////
//////////////////////Finish Round//////////////////////
////////////////////////////////////////////////////////
void finishRound()
{   
    //red light
    RGB(255,0,0);
    //record time needed to press buttons
    elapsedTime =  millis() - previousTime; 
    totalTime += elapsedTime;
    
    //makes sure time is printed once
    rerun = false; 

    //prints time
    Serial.println();
    scoring();
    Serial.println();
    Serial.print("Time: ");
    Serial.print(elapsedTime);
    Serial.println(" ms");
    
    for(int i = 0; i < 12 ;i++)
    {
      Serial.println();
    }
    
    //short delay, print some spaces in preperation for next game
    delay(3000);
    
    for(int i = 0; i < 9 ;i++)
    {
      Serial.println();
    }

    //new round
    game();
}

////////////////////////////////////////////////////////
//////////////////////Finish Game///////////////////////
////////////////////////////////////////////////////////
void finishGame()
{   

    for(int i = 0; i <= 3; i++)
    {
        Serial.println();
    } 

    totalScore = (totalCorrect/(numberOfRounds*stringLength))*100;
    avgTime = totalTime/(numberOfRounds);
      
    Serial.println("Fin");
    Serial.print("Final Score: ");
    Serial.print(totalIntCorrect);
    Serial.print("/");
    Serial.print(stringLength*numberOfRounds);
    Serial.print(" || ");
    Serial.print(totalScore);
    Serial.println("%");
    Serial.print("Average Time: ");
    Serial.print(avgTime);
    Serial.println("ms");
    Serial.println();
    
    if(totalScore >= 90.00)
    {
        Serial.println("You have great memory. Nice Job!");
        flashRandomColors();
    }
    
    else if ((totalScore < 90.00) && (totalScore >= 70.00))
    {
        Serial.println("Good Job!");
        RGB(192,192,192);
    }

    Serial.println("To play again, press reset.");
    for(int i = 0; i < 9; i++)
    {
        Serial.println();
    } 
    
    while(1);  
}

////////////////////////////////////////////////////////
//////////////////////RGB LED///////////////////////////
////////////////////////////////////////////////////////
void RGB(int red_value, int green_value, int blue_value)
 {
    analogWrite(red, red_value);
    analogWrite(green, green_value);
    analogWrite(blue, blue_value);
}

////////////////////////////////////////////////////////
////////////////Flash Random Colors/////////////////////
////////////////////////////////////////////////////////
void flashRandomColors()
{   
    randomSeed(analogRead(0));
    int randNumber1;
    int randNumber2;
    int randNumber3;
    for(int i = 0; i < 10; i++)
    {
        randNumber1 = random(0, 255);
        randNumber2 = random(0, 255);
        randNumber3 = random(0, 255);
        RGB(randNumber1,randNumber2,randNumber3);
        delay(150);
    }
    RGB(0,0,0);
}




/////////////////////////////////////////
////////////////start game///////////////
/////////////////////////////////////////
void setup() 
{   
    Serial.begin(9600);
    Serial.println("Welcome, game will begin shortly...");
    flashRandomColors();
    
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(blue, OUTPUT);
   
    delay(2000);
    game();  
}
 
/////////////////////////////////////////
////////////////gameplay/////////////////
/////////////////////////////////////////
void loop() 
{ 

    // read the pushbutton input pin
    currentState1 = digitalRead(2);
    currentState2 = digitalRead(3);
    currentState3 = digitalRead(4);

    //////////////////////////////////////////////////
    ////////////////////button 1//////////////////////
    //////////////////////////////////////////////////

    // compare the buttonState to its previous state
    if (currentState1 != prevState1) 
    {
        // if the state has changed, increment the counter
        if (currentState1 == HIGH) 
        {
            // if the current state is HIGH then the button went from off to on:
            buttonCounter++;
            userNumbers[buttonCounter-1] = 1;
            Serial.print("1 ");
        } 
    
        // Delay a little bit to avoid bouncing
        delay(50);
    }
    // save the current state as the last state, for next time through the loop
    prevState1 = currentState1;

    //////////////////////////////////////////////////
    ////////////////////button 2//////////////////////
    //////////////////////////////////////////////////

    if (currentState2 != prevState2) 
    {
        if (currentState2 == HIGH) 
        {
            buttonCounter++;
            userNumbers[buttonCounter-1] = 2;
            Serial.print("2 ");
        } 
        delay(50);
    }
    prevState2 = currentState2;

    //////////////////////////////////////////////////
    ////////////////////button 3//////////////////////
    //////////////////////////////////////////////////
  
    if (currentState3 != prevState3) 
    {
        if (currentState3 == HIGH) 
        {
            buttonCounter++;
            userNumbers[buttonCounter-1] = 3;
            Serial.print("3 ");
        } 
        delay(50);
    }
    prevState3 = currentState3;

    //To end a round
    if(rerun == true && buttonCounter == stringLength)
    { 
        finishRound();
    }
}
