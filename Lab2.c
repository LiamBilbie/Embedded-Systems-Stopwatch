#include "address_map_arm.h"


volatile int *buttons = (int*)KEY_BASE;
volatile char *ptr = (char*)SW_BASE;
volatile int *timer_ptr = (int *)HPS_TIMER0_BASE;
volatile char *sml = (char*)HEX3_HEX0_BASE;
volatile char *sml2 = (char*)HEX5_HEX4_BASE;
unsigned char lookupTable[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67};
int ms1=0, ms2=0, s1=0, s2=0, m1=0, m2 = 0;
int hs1=0, hs2=0, hl1=0, hl2=0, h1=0, h2 = 0;
int running = 0;




void config_HPS_timer()
{
 // timer base address
*(timer_ptr + 0x2) = 0; // write to control register to stop timer

/* set the timer period */
int counter = 1000000; // period = 1/(100 MHz) x (100 x 10^6) = 1 sec
*(timer_ptr) = counter; // write to timer load register

/* write to control register to start timer, with interrupts */
*(timer_ptr + 2) = 0b011; // int mask = 0, mode = 1, enable = 1
}

int ReadSwitches(void)
{
 
    return *(ptr) %8;

}

void display(int a, int b, int c, int d, int e, int f){
			*sml = lookupTable[a];
			*(sml + 1) = lookupTable[b];
			*(sml + 2) = lookupTable[c];
			*(sml + 3) = lookupTable[d];
			*(sml2) = lookupTable[e];
			*(sml2 + 1) = lookupTable[f];
	
}

void resetP(){
	*sml = lookupTable[0];
	*(sml + 1) = lookupTable[0];
	*(sml + 2) = lookupTable[0];
	*(sml + 3) = lookupTable[0];
	*(sml2) = lookupTable[0];
	*(sml2 + 1) = lookupTable[0];
}


int main(void){
*(timer_ptr + 2) = 0b000;
int runTime = 1000000;
*(timer_ptr + 2) = 0b011;
	while (1)
	{
			
	
			
			
			while (running)
			{
				if(ReadSwitches()==1){
					display(hs1, hs2, hl1, hl2, h1, h2);
				}
				if (*buttons == 0b1000){ms1=0; ms2=0;s1=0;s2=0;m1=0;m2=0; resetP();}
				
				if(*buttons == 0b0100){hs1 = ms1; hs2 = ms2; hl1 = s1; hl2 = s2; h1 = m1; h2 = m2;}
				
				if (*buttons == 0b0010) {running = 0;}
				
				else{
				
				*(timer_ptr + 2) = 0b011;
				while (*(timer_ptr + 4) == 0){;}
				ms1++;
				
				if (ms1 == 10) {ms1 = 0; ms2 += 1;}
				if (ms2 == 10) {ms2 = 0; s1 += 1;}
				if (s1 == 10) {s1 = 0; s2 += 1;}
				if (s2 == 6) {s2 = 0; m1 += 1;}
				if (m1 == 10) {m1 = 0; m2 += 1;}
			
				display(ms1, ms2, s1, s2, m1, m2);
			
				config_HPS_timer();	
				}
		
			}
			
			while(!running){
				if (*buttons == 0b0001){running = 1;}
				if (*buttons == 0b1000){ms1=0; ms2=0;s1=0;s2=0;m1=0;m2=0; resetP();}
				if(*buttons == 0b0100){hs1 = ms1; hs2 = ms2; hl1 = s1; hl2 = s2; h1 = m1; h2 = m2;}
				if(ReadSwitches()==1){
				display(hs1, hs2, hl1, hl2, h1, h2);
				} 
				else{	
				display(ms1, ms2, s1, s2, m1, m2);
				}
			}
			
		
		
		
		
		
	}
}

