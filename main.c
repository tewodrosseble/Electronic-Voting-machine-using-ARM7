#include <NXP/iolpc2124.h>
#include <stdio.h>

void initializeLPC(void);
void initializeLCD1(void);
void initializeLCD2(void);
void initializeLCD3(void);
void writeCommandC1(char cmd);
void writeCommandC2(char cmd);
void writeCommandC3(char cmd);
void writeData1(char data);
void writeData2(char data);
void writeData3(char data);
void sendStringToLCDC1(char *str);
void sendStringToLCDC2(char *str);
void sendStringToLCDC3(char *str);
void displayNumberOnLCDC1(int number);
void displayNumberOnLCDC2(int number);
void displayNumberOnLCDC3(int number);
void calculateAndDisplayDifference(int number1, int number2);
void displayWinnerOnLCDC3(char* winner);

int main() {
    initializeLPC();
    initializeLCD1();
    initializeLCD2();
    initializeLCD3();
    
    char candidate1[] = "Candidate 1";
    char candidate2[] = "Candidate 2";
    sendStringToLCDC1(candidate1);
    sendStringToLCDC2(candidate2);
    writeCommandC1(0x80 | 0x40); // Move cursor to beginning of second line
    writeCommandC2(0x80 | 0x40);
    sendStringToLCDC1("No of Votes: ");
    sendStringToLCDC2("No of Votes: ");
    
    int buttonCountC1 = 0;
    int buttonCountC2 = 0;
    int buttonReleased1 = 1;
    int buttonReleased2 = 1;
    
    char winner[16];
    
    while (1) {
        int currentButtonStateC1 = IO0PIN_bit.P0_11;
        int currentButtonStateC2 = IO0PIN_bit.P0_12;
        
        if (currentButtonStateC1 == 1 && buttonReleased1 == 0) {
            buttonReleased1 = 1;
        }
        if (currentButtonStateC2 == 1 && buttonReleased2 == 0) {
            buttonReleased2 = 1;
        }
        
        if (currentButtonStateC1 == 0 && buttonReleased1 == 1) {
            buttonReleased1 = 0;
            buttonCountC1++;
            if (buttonCountC1 > 99) {
                buttonCountC1 = 0; // Reset back to 00
            }
            
            writeCommandC1(0x80 | 0x40 + 14); // Move cursor to beginning of second line and the position of the number
            displayNumberOnLCDC1(buttonCountC1);
            calculateAndDisplayDifference(buttonCountC1, buttonCountC2);
        }
        
        if (currentButtonStateC2 == 0 && buttonReleased2 == 1) {
            buttonReleased2 = 0;
            buttonCountC2++;
            if (buttonCountC2 > 99) {
                buttonCountC2 = 0;
            }
            writeCommandC2(0x80 | 0x40 + 14); // Move cursor to beginning of second line and the position of the number
            displayNumberOnLCDC2(buttonCountC2);
            calculateAndDisplayDifference(buttonCountC1, buttonCountC2);
        }
    } 
}

void initializeLPC() {
    PINSEL0 = 0;
    PINSEL1 = 0;
    IO0DIR = 0x00FFFFFF; // Set P0.0 - P0.23 as outputs
    IO0DIR_bit.P0_11 = 0; // Set P0.11 as input
    IO0DIR_bit.P0_12 = 0; // Set P0.12 as input
    T0TCR_bit.CE = 0;
    T0PR = 47999;
    T0TC = 0;
}

void initializeLCD1() {
    writeCommandC1(0x38);
    writeCommandC1(0x06);
    writeCommandC1(0x0F);
    writeCommandC1(0x01);
}

void initializeLCD2() {
    writeCommandC2(0x38);
    writeCommandC2(0x06);
    writeCommandC2(0x0F);
    writeCommandC2(0x01);
}

void initializeLCD3() {
    writeCommandC3(0x38);
    writeCommandC3(0x06);
    writeCommandC3(0x0F);
    writeCommandC3(0x01);
}

void writeCommandC1(char cmd) {
    IO0CLR_bit.P0_8 = 1;
    IO0CLR_bit.P0_9 = 1;
    IO0SET = cmd;
    IO0SET_bit.P0_10 = 1;
    IO0CLR_bit.P0_10 = 1;
    IO0CLR = 0x000000FF;
}

void writeCommandC2(char cmd) {
    IO0CLR_bit.P0_13 = 1;
    IO0CLR_bit.P0_14 = 1;
    IO0SET = cmd;
    IO0SET_bit.P0_15 = 1;
    IO0CLR_bit.P0_15 = 1;
    IO0CLR = 0x000000FF;
}

void writeCommandC3(char cmd) {
    IO0CLR_bit.P0_16 = 1;
    IO0CLR_bit.P0_17 = 1;
    IO0SET = cmd;
    IO0SET_bit.P0_18 = 1;
    IO0CLR_bit.P0_18 = 1;
    IO0CLR = 0x000000FF;
}

void writeData1(char data) {
    IO0SET_bit.P0_8 = 1;
    IO0CLR_bit.P0_9 = 1;
    IO0SET = data;
    IO0SET_bit.P0_10 = 1;
    IO0CLR_bit.P0_10 = 1;
    IO0CLR = 0x000000FF;
}

void writeData2(char data) {
    IO0SET_bit.P0_13 = 1;
    IO0CLR_bit.P0_14 = 1;
    IO0SET = data;
    IO0SET_bit.P0_15 = 1;
    IO0CLR_bit.P0_15 = 1;
    IO0CLR = 0x000000FF;
}

void writeData3(char data) {
    IO0SET_bit.P0_16 = 1;
    IO0CLR_bit.P0_17 = 1;
    IO0SET = data;
    IO0SET_bit.P0_18 = 1;
    IO0CLR_bit.P0_18 = 1;
    IO0CLR = 0x000000FF;
}

void sendStringToLCDC1(char *str) {
    while (*str) {
        writeData1(*str++);
    }
}

void sendStringToLCDC2(char *str) {
    while (*str) {
        writeData2(*str++);
    }
}

void sendStringToLCDC3(char *str) {
    while (*str) {
        writeData3(*str++);
    }
}

void displayNumberOnLCDC1(int number) {
    char buffer[3];
    buffer[2] = '\0'; // Null terminator
    buffer[1] = '0' + (number % 10); // Get ones digit
    number /= 10; // Remove ones digit
    buffer[0] = '0' + (number % 10); // Get tens digit
    sendStringToLCDC1(buffer);
}

void displayNumberOnLCDC2(int number) {
    char buffer[3];
    buffer[2] = '\0'; // Null terminator
    buffer[1] = '0' + (number % 10); // Get ones digit
    number /= 10; // Remove ones digit
    buffer[0] = '0' + (number % 10); // Get tens digit
    sendStringToLCDC2(buffer);
}

void displayNumberOnLCDC3(int number) {
    char buffer[3];
    buffer[2] = '\0'; // Null terminator
    buffer[1] = '0' + (number % 10); // Get ones digit
    number /= 10; // Remove ones digit
    buffer[0] = '0' + (number % 10); // Get tens digit
    sendStringToLCDC3(buffer);
}

void calculateAndDisplayDifference(int number1, int number2) {
    int difference = 0;
    if (number1 > number2) {
        displayWinnerOnLCDC3("Winner: Can. 1");
    }
    else if (number2 > number1) {
        displayWinnerOnLCDC3("Winner: Cand 2");
    }
    else {
        displayWinnerOnLCDC3("Winner: Tie");
    }

    if (number1 > number2) {
        difference = number1 - number2;
    }
    else if (number2 > number1) {
        difference = number2 - number1;
    }
    else {
        difference = 0;
    }
    writeCommandC3(0x80 | 0x40); // Move cursor to beginning of second line
    sendStringToLCDC3("Difference: ");
    displayNumberOnLCDC3(difference);
}

void displayWinnerOnLCDC3(char *winner) {
    writeCommandC3(0x80); // Move cursor to beginning of first line
    sendStringToLCDC3("                "); // Clear the line
    writeCommandC3(0x80); // Move cursor to beginning of first line
    sendStringToLCDC3(winner);
}
