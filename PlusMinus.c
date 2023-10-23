#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_X_COPY 10
#define MAX_Y_COPY 33

int X_Copy = 4;             // Count Horizontal Calculations

int Y_Copy = 2;             // Count Vertical Calculations

int CALC_MODE = 2;          // 0 = Multiplications
                            // 1 = Subtractions
                            // 2 = Mul. & Sub. mixed

int MISSING = 2;            // 0 = Show All
                            // 1 = remove result
                            // 2 = remove random value

int CONV_LINE = 1;          // If converting help-line is printed
int CONV_SIGN = 1;          // IF sign of converting line is printed

int HELP_LINE = 1;          // If regular help-line is printed
int HELP_SIGN = 1;          // If sign of help-line is printed

char misChar[MAX_X_COPY];    // Save while FillBoxLine() the needed +/-

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
void CursorUp(int y) {
	printf("\x1B[%dA", y);
}
void CursorMoveX(int x){
	if (x < 0){
		// Left
		CursorLeft(x * -1);
	}
	else if (x > 0){
		// Right
		CursorRight(x);
	}
}
void CursorMoveY(int y){
	if (y < 0){
		// Down
		CursorDown(y * -1);
	}
	else if (y > 0){
		// Up
		CursorUp(y);
	}
}
#define CursorMove(x, y) CursorMoveX(x); CursorMoveY(y)
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
        return start + arc4random() % (stop + 1  - start);
    #else
        return start + rand() % (stop + 1 - start);
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

void PrintMidLine(char symbol, int printSymbol){
    for (size_t i = 0; i < X_Copy; i++){
        printf("│ │ │");
        TxtBold(1);

        if(misChar[i]) symbol = misChar[i];
        
        if (printSymbol){
            printf("%c", symbol);
        }
        else{
            printf(" ");
        }

        TxtBold(0);
        printf("│ │ │");
        TxtBold(1);
        printf("=");
        TxtBold(0);
        printf("│ │ │");
        if(i < X_Copy-1) printf("  ");
    }    
    CursorRestore();
}

void PrintTriple(char *strIN, int cnt){
    if(!cnt) cnt = X_Copy;
    for (size_t i = 0; i < cnt; i++){
        for (size_t j = 0; j < 3; j++){
            printf("%s",strIN);
            if(i < X_Copy-1 || j < 3) printf(" ");
        }
        if(i < X_Copy-1) printf(" ");
    }    
}

void PrintBotLine(){
    PrintTriple("└─┴─┘",0);
}

void PrintTopBlock(char symbol, int printSymbol){
    CursorSave();
    PrintTriple("┌─┬─┐",0);
    CursorRestore();
    CursorDown(1);
    PrintMidLine(symbol, printSymbol);
    CursorDown(2);
}

void FillBoxLine(char symbol){
    
    CursorSave();
    TxtBold(1);
    int hasConverterLine = 0;

    for (size_t i = 0; i < X_Copy; i++){
        
        // 1st +/- 2nd = 3rd
        
        int num1st = 0;
        int num2nd = 0;
        int r = 0;
        char char1st[3] = "  ";
        char char2nd[3] = "  ";
        char char3rd[3] = "  ";

        int dontShow = GetRand(1, 3);       // Which value to remove, if randomized
        if(!MISSING) dontShow = 0;          // Show all
        if(MISSING == 1) dontShow = 3;      // remove result

        // Get Calculation pair and result
        if (symbol == '+'){
            r = GetPosPair(&num1st, &num2nd);
        }
        else{
            r = GetNegPair(&num1st, &num2nd);
        }

        // Convert calculation values to strings
        sprintf(char1st,"%d", num1st);
        sprintf(char2nd,"%d", num2nd);
        sprintf(char3rd,"%d", r);
        
        if (dontShow != 1){
            // Print 1st value
            printf("%c", char1st[0]);
            CursorRight(1);
            printf("%c", char1st[1]);
        }
        else{
            CursorRight(3);
        }
        
        CursorRight(3);

        if (dontShow != 2){
            // Print 2nd Value        
            printf("%c", char2nd[0]);
            CursorRight(1);
            printf("%c", char2nd[1]);
        }
        else{
            CursorRight(3);
        }

        CursorRight(3);
    
        if (dontShow != 3){
            // Print result (may have just 1 digit...)
            if (r > 9){
                printf("%c", char3rd[0]);
                CursorRight(1);
                printf("%c", char3rd[1]);
            }
            else{
                CursorRight(2);
                printf("%c", char3rd[0]);
            }
        }
        else{
            CursorRight(3);
        }

        char convSign = 0;
    
        if (symbol == '+'){
            if (dontShow && dontShow < 3){
                // "converter" line with "minus" needed
                convSign = '-';
            }
        }
        else{
            if (dontShow == 1){
                // "converter" line with "plus" needed
                convSign = '+';
            }
            else if (dontShow == 2){
                // "converter" line with "minus" needed
                convSign = '-';
            }            
        }
        misChar[i] = convSign;
        if(!convSign) misChar[i] = symbol;
        if (convSign && CONV_LINE){
            // Add "converter" line
            TxtBold(0);
            hasConverterLine = 1;
            CursorMove(-16, -1);
            PrintTriple("├─┼─┤", 1);
            CursorMove(-19, -1);
            PrintTriple("└─┴─┘", 1);
            CursorMoveX(-14);
            TxtBold(1);
            if (CONV_SIGN){
                printf("%c", convSign);
            }
            else{
                printf(" ");
            }
            CursorMoveX(5);
            printf("=");
            CursorMove(4 ,2);
        }
        
        if (i < X_Copy-1) CursorRight(4);
    }
    
    CursorRestore();
    if (hasConverterLine){
        CursorDown(1);
    }
    CursorDown(2);
    CursorLeft(1);      // Top-edge of HelpLine
    TxtBold(0);

}

void PrintHelpLine(char symbol){
    PrintTopBlock(symbol, HELP_SIGN);
    PrintTriple("├─┼─┤",0);
    CursorRestore();
    CursorDown(3);
    PrintMidLine(symbol, HELP_SIGN);
    CursorDown(4);
    PrintBotLine();
    CursorRestore();
    CursorDown(6);      // Next calc
}

void PrintCalcLine(char symbol){
    for (size_t i = 0; i < X_Copy; i++){
        // Reset 
        misChar[i] = 0;
    }
    PrintTopBlock(symbol, 1);
    PrintBotLine();
    CursorRestore();
    CursorDown(1);
    CursorRight(1);     // 1st number
    FillBoxLine(symbol);
    if(HELP_LINE) PrintHelpLine(symbol);
}

int main(int argc, char *argv[]){

    for (int i = 1; i < argc; i++){
        if (strncmp(argv[i], "-x=", 3) == 0){
            X_Copy = atoi(argv[i] + 3);
            if(X_Copy > MAX_X_COPY) X_Copy = MAX_X_COPY;
            if(X_Copy < 1) X_Copy = 1;
        }
        else if (strncmp(argv[i], "-y=", 3) == 0){
            Y_Copy = atoi(argv[i] + 3);
            if(Y_Copy > MAX_Y_COPY) Y_Copy = MAX_Y_COPY;
            if(Y_Copy < 1) Y_Copy = 1;
        }
        else if (strncmp(argv[i], "-calc=+-", 8) == 0){
            CALC_MODE = 2;
        }
        else if (strncmp(argv[i], "-calc=+", 7) == 0){
            CALC_MODE = 0;
        }
        else if (strncmp(argv[i], "-calc=-", 7) == 0){
            CALC_MODE = 1;
        }
        else if (strncmp(argv[i], "-remove=", 8) == 0){
            MISSING = atoi(argv[i] + 8);
            if(MISSING > 2) MISSING = 2;
            if(MISSING < 0) MISSING = 0;
        }
        else if (strncmp(argv[i], "-convline=", 10) == 0){
            CONV_LINE = atoi(argv[i] + 10);
        }
        else if (strncmp(argv[i], "-convsign=", 10) == 0){
            CONV_SIGN = atoi(argv[i] + 10);
        }
        else if (strncmp(argv[i], "-helpline=", 10) == 0){
            HELP_LINE = atoi(argv[i] + 10);
        }
        else if (strncmp(argv[i], "-helpsign=", 10) == 0){
            HELP_SIGN = atoi(argv[i] + 10);
        }
        else if (strncmp(argv[i], "-help", 5) == 0){
            printf("\n\n");
            printf("How to call:\n");
            printf("  %s <option-1> <option-2> <option-n>\n", argv[0]);
            printf("Known options:\n");
            printf("  -x=n            n = 1-%d columns\n", MAX_X_COPY);
            printf("  -y=n            n = 1-%d rows\n", MAX_Y_COPY);
            printf("  -calc='n'       n = +- or + or - calculation(s)\n");
            printf("  -remove=n       n = 0=none or 1=result or 2=random\n");
            printf("  -convline=n     n = 1=true or 0=false, if converter-line is printed\n");
            printf("  -convsign=n     n = 1=true or 0=false, if converter-sign is printed\n");
            printf("  -helpline=n     n = 1=true or 0=false, if help-line is printed\n");
            printf("  -helpsign=n     n = 1=true or 0=false, if help-signs are printed\n");
            printf("  -help           print this screen\n\n");
            return 0;
        }
        else{
            printf("\n\nIllegal option:\n");
            printf("         %s\n\n",argv[i]);
            return -1;
        }
    }
    

    CLS();
    CursorDown(1); CursorRight(2);

    srand(clock());    

    while (Y_Copy){
        if (!CALC_MODE || CALC_MODE == 2){
            PrintCalcLine('+');
            Y_Copy--;
        }
        if ((CALC_MODE == 1 || CALC_MODE == 2 && Y_Copy)){
            PrintCalcLine('-');
            Y_Copy--;
        }
    }

    return 0;
}
