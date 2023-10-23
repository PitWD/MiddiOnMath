#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void CursorSave(){
  printf("\0337");
  //printf("\x1B[s");
  fflush(stdout);
}
void CursorRestore(){
  printf("\0338");
  //printf("\x1B[u");
  fflush(stdout);
}
void CursorDown(int y) {
	printf("\x1B[%dB", y);
    fflush(stdout);
}
void CursorRight(int x) {
	printf("\x1B[%dC", x);
    fflush(stdout);
}
void CursorLeft(int x) {
	printf("\x1B[%dD", x);
    fflush(stdout);
}
void CLS(void){
    // CLS
    #if __WIN32__ || _MSC_VER || __WIN64__
        system ("cls");
    #else 
        system ("clear");
    #endif
}
void TxtBold(int set) {
	if (set) {
		// Set
		printf("\x1B[1m");
	}
	else {
		// Reset
		printf("\x1B[22m");
	}
}

/*
a b c d e f g h i j k l m n o p q r s t u v w x y z
▒ ␉ ␌ ␍ ␊ ° ± ␤ ␋ ┘ ┐ ┌ └ ┼ ⎺ ⎻ ─ ⎼ ⎽ ├ ┤ ┴ ┬ │ ≤ ≥
*/
void DECboxMode(int set){
	if (set){
		printf("\x1B(0"); // enable BoxDraw Mode
	}
	else{
		printf("\x1B(B"); // disable BoxDraw Mode
	}
    fflush(stdout);
}
#define DECboxON DECboxMode(1)
#define DECboxOFF DECboxMode(0)

int GetRand(int start, int stop){
    // Randomize number start - stop
    #if __APPLE__
        return start + arc4random() % (stop - start);
    #else
        return start + rand() % (stop - start);
    #endif
}
void GetRandPair(int *num1st, int *num2nd){
    *num1st = GetRand(11, 99);
    *num2nd = GetRand(11, 99);
}
int GetNegPair(int *num1st, int *num2nd){
    int r = 0;
    while (!r){
        GetRandPair(num1st, num2nd);
        r = *num1st - *num2nd;
        if(r < 0){
            r = *num1st;
            *num1st = *num2nd;
            *num2nd = r;
            r = *num1st - *num2nd;
        }
    }
    return r;
}
int GetPosPair(int *num1st, int *num2nd){
    int r = 0;
    while (!r){
        GetRandPair(num1st, num2nd);
        r = *num1st + *num2nd;
        if(r > 99) r = 0;
    }
    return r;
}

void PrintMidLine(char symbol){
    for (size_t i = 0; i < 4; i++){
        printf("│ │ │");
        TxtBold(1);
        printf("%c", symbol);
        TxtBold(0);
        printf("│ │ │");
        TxtBold(1);
        printf("=");
        TxtBold(0);
        printf("│ │ │");
        if(i < 3) printf("  ");
    }    
    CursorRestore();
}

void PrintTriple(char *strIN){
    for (size_t i = 0; i < 4; i++){
        for (size_t j = 0; j < 3; j++){
            printf("%s",strIN);
            if(i < 3 || j < 3) printf(" ");
        }
        if(i < 3) printf(" ");
    }    
}

void PrintBotLine(){
    PrintTriple("└─┴─┘");
}

void PrintTopBlock(char symbol){
    CursorSave();
    PrintTriple("┌─┬─┐");
    CursorRestore();
    CursorDown(1);
    PrintMidLine(symbol);
    CursorDown(2);
}

void PrintCalcLine(char symbol){
    PrintTopBlock(symbol);
    PrintBotLine();
    CursorRestore();
    CursorDown(1);
    CursorRight(1);     // 1st number
}

void PrintHelpLine(char symbol){
    PrintTopBlock(symbol);
    PrintTriple("├─┼─┤");
    CursorRestore();
    CursorDown(3);
    PrintMidLine(symbol);
    CursorDown(4);
    PrintBotLine();
    CursorRestore();
    CursorDown(6);      // Next calc
}

void FillBoxLine(char symbol){
    
    CursorSave();
    TxtBold(1);
    for (size_t i = 0; i < 4; i++){
        
        // 1st +/- 2nd = 3rd
        
        int num1st = 0;
        int num2nd = 0;
        char char1st[3] = "  ";
        char char2nd[3] = "  ";
        char char3rd[3] = "  ";

        int r = 0;

        if (symbol == '+'){
            r = GetPosPair(&num1st, &num2nd);
        }
        else{
            r = GetNegPair(&num1st, &num2nd);
        }

        sprintf(char1st,"%d", num1st);
        sprintf(char2nd,"%d", num2nd);
        sprintf(char3rd,"%d", r);
        
        //if (num1st > 9){
            printf("%c", char1st[0]);
            CursorRight(1);
            printf("%c", char1st[1]);
        //}
        //else{
        //    CursorRight(2);
        //    printf("%c", char1st[0]);
        //}
        CursorRight(3);
        
        //if (num2nd > 9){
            printf("%c", char2nd[0]);
            CursorRight(1);
            printf("%c", char2nd[1]);
        //}
        //else{
            //CursorRight(2);
            //printf("%c", char2nd[0]);
        //}

        CursorRight(3);
        if (r > 9){
            printf("%c", char3rd[0]);
            CursorRight(1);
            printf("%c", char3rd[1]);
        }
        else{
            CursorRight(2);
            printf("%c", char3rd[0]);
        }


        if (i < 3) CursorRight(4);
    }
    
    CursorRestore();
    CursorDown(2);
    CursorLeft(1);      // Top-edge of HelpLine
    TxtBold(0);

}

int main(){
    CLS();
    CursorDown(1); CursorRight(2);

    srand(clock());    

    PrintCalcLine('+');
    FillBoxLine('+');
    PrintHelpLine('+');

    PrintCalcLine('-');
    FillBoxLine('-');
    PrintHelpLine('-');

    return 0;
}
