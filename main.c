#include "mbed.h"               // invoegen mbed bibliotheek

DigitalOut pin13(PC_13);        // klok schuifregister
DigitalOut pin14(PC_14);        // Enable output schuifregister
DigitalOut pin15(PC_15);        // data schuifregister
DigitalOut pin0(PA_0);          // display1 common annode
DigitalOut pin1(PA_1);          // display2 common annode
Serial pc(PA_2, PA_3);          // TX & RX


// variabelen functie: datauitlezen schuifregister
int Waarden[11]= {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x83,0xF8,0x80,0x98,0xFF}; // waarden 7 seg 0-9 + clear
int seg = 0;                    // teller 7segment 1 of 2
int klokschuifregister = 0;     // klok schuifregister
int uitlezen = 0;               // teller aanzetten output enable schuifregister
int schuif = 0;                 // aantal keer schuiven

int seg1 = 0;                   // waarde segment1
int seg2 = 9;                   // waarde segment2

Timer t;                        // functie timer
int tijd;                       // tijd 
int c = 0;                      
int d = 0;
char karakter;


int print(int data1, int data2) {                       // functie: data uitlezen schuifregister 
               
            klokschuifregister = klokschuifregister^1;  // klokschuifregister aan-uit 
                pin13 = klokschuifregister;             // klokschuifregister aansturen
               
            if(seg==0){                                     // display 1 aansturen
                pin15 = (Waarden[data1] >> (7-schuif)) & 1; // Uit te sturen bit selecteren
                if (klokschuifregister == 1){               // Als klok hoog is 
                    schuif++;                               // data volgende keer meer schuiven
                    uitlezen++;                             // variabele verhogen om uit te lezen
                    }                                
                    if (uitlezen == 8){                     // na 8 klokpulsen data op de 7seg plaatsen 
                       pin0 = 0;                            // display 1 aan 
                       pin1 = 1;                            // display 2 uit 
                       pin14 = 1;                           // enable output schuifregister aanzetten
                       uitlezen = 0;                        // reset variable uitlezen                       
                       schuif = 0;                          // aantal bits schuiven reset
                       seg = 1;                             // veranderen aansturing 7segment 2de segment
                    }
                    pin14 = 0;                              // enable output schuifregister uitzetten
                }
            if(seg==1){                                     // display 2 aansturen
                pin15 = (Waarden[data2] >> (7-schuif)) & 1; // Uit te sturen bit selecteren
               if (klokschuifregister == 1){                // Als klok hoog is
                   schuif++;                                // data volgende keer meer schuiven
                   uitlezen++;                              // variabele verhogen om uit te lezen
                }                              
               if (uitlezen == 8){                          // na 8 klokpulsen uitlezen
                   pin0 = 1;                                // display 1 uit
                   pin1 = 0;                                // display 2 aan
                   pin14 = 1;                               // enable output schuifregister aanzetten
                   uitlezen = 0;                            // reset variabele uitlezen 
                   schuif = 0;                              // aantal bits schuiven reset
                   seg = 0;                                 // veranderen aansturen 7segment 1ste segment
                }
            pin14 = 0;                                      // enable output schuifregister uitzetten
            }
    return 0;                                               // einde van de functie
}

int main(){

    t.start();
    while(true) {
        
        if(pc.readable()){  //kijken of er een karakter gelezen kan worden
            karakter = pc.getc(); //karakter inlezen
            
                if(karakter == 's'){  //If 'a' is pressed then turn LED on 
                
                while( karakter != 'r'){
                    if(pc.readable()){
                    karakter = pc.getc();
                    }
                    if(karakter == 'p' ){  //If 'd' is pressed then turn LED off
                    while(karakter != 's' && karakter != 'r'){
                    if(pc.readable()){
                    karakter = pc.getc();
                    }
                    print(seg2,seg1) ;      
                    }
                    }
                    tijd = t.read_ms();
                    if (tijd > 100){
                        t.reset();
                        c++;         
                        if(c == 10){
                            seg1--;
                            c = 0;
                            if(seg1== -1){
                                seg1 = 9;
                                seg2--;
                                if(seg2 == -1){
                                    seg1 =0;
                                    seg2 = 0;
                                }
                            }
                        
                         }
                    }
    

                print(seg2,seg1) ;  
                }
                }
            }
            
             if(karakter == 'r'){  //If 'd' is pressed then turn LED off
             
                print(10,10) ; 
                seg1 = 0;
                seg2 = 9;     
            }
            
        }
        
        

    }
