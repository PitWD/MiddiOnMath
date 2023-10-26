#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_X_COPY 10
#define MAX_Y_COPY 33

int X_Copy = 4;             // Count Horizontal Calculations

int Y_Copy = 2;             // Count Vertical Calculations

int CALC_MODE = 3;          // 1 = Additions
                            // 2 = Subtractions
                            // 4 = Multiplications
                            // 8 = Divisions

char *strCalcMode[] = {"+", "-", "+-"};

int MISSING = 2;            // 0 = Show All
                            // 1 = remove result
                            // 2 = remove random value

int CONV_LINE = 1;          // If converting help-line is printed
int CONV_SIGN = 1;          // IF sign of converting line is printed

int HELP_LINE = 1;          // If regular help-line is printed
int HELP_SIGN = 1;          // If sign of help-line is printed

int LINE_COLOR = 8;

int SHRUGGIE = 1;

int TWO_DIGIT = 1;

char misChar[MAX_X_COPY];    // Save while FillBoxLine() the needed +/-

void PrintHelp(char *strIN){
    printf("How to call:\n");
    printf("  %s <option> <option> <option>\n", strIN);
    printf("Known options (defaults in []):\n");
    printf("  -x=n            n [%d]  = 1-%d columns\n", X_Copy, MAX_X_COPY);
    printf("  -y=n            n [%d]  = 1-%d rows\n",Y_Copy, MAX_Y_COPY);
    printf("  -calc='n'       n [%s]  = 1=+ 2=- 4=* 8=/ calculation(s)\n", strCalcMode[CALC_MODE]);
    printf("  -remove=n       n [%d]  = 0=none or 1=result or 2=random\n", MISSING);
    printf("  -convline=n     n [%d]  = 1=true or 0=false, if converter-line is printed\n", CONV_LINE);
    printf("  -convsign=n     n [%d]  = 1=true or 0=false, if converter-sign is printed\n", CONV_SIGN);
    printf("  -helpline=n     n [%d]  = 1=true or 0=false, if help-line is printed\n", HELP_LINE);
    printf("  -helpsign=n     n [%d]  = 1=true or 0=false, if help-signs are printed\n", HELP_SIGN);
    printf("  -linecolor=n    n [%d]  = 0=default 1-8 = black/red/green/yellow/blue/magenta/cyan/white\n",LINE_COLOR);
    printf("  -shruggie=n     n [%d]  = 1=print ¯\\_(ツ)_/¯ on empty convline positions\n",SHRUGGIE);
    printf("  -twodigit=n     n [%d]  = 0= calcs with one digit or 1= calcs with two digits\n",TWO_DIGIT);
    printf("  -help           print this screen\n\n");
}

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
// Reset To Default Colors
void ResFg(void) {
	printf("\x1B[39m");
}
void ResBg(void) {
	printf("\x1B[49m");
}
void ResFB(void) {
	ResFg();
	ResBg();
}
//Set 16 Colors
void SetFg16(int c){
	// ForeGround	
	if (!((c > 29 && c < 38) || (c > 89 && c < 98))){
		ResFg();
	}
	else{
		printf("\x1B[%dm", c);
	}	
}
void SetBg16(int c) {	
	// BackGround
	if (!((c > 39 && c < 48) || (c > 99 && c < 108))){
		ResBg();
	}
	else{
		printf("\x1B[%dm", c);
	}
}
void SetFB16(int fg, int bg) {
	SetFg16(fg);
	SetBg16(bg);
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
void GetRandPairPlusMinus(int *num1st, int *num2nd){
    if (TWO_DIGIT){
        *num1st = GetRand(11, 99);
        *num2nd = GetRand(11, 99);
    }
    else{
        *num1st = GetRand(2, 9);
        *num2nd = GetRand(2, 9);
    }
}
void GetRandPairMulDiv(int *num1st, int *num2nd){
    *num1st = GetRand(2, 9);
    if (TWO_DIGIT){
        *num2nd = GetRand(11, 54);
    }
    else{
        *num2nd = GetRand(2, 9);
    }    
}
int GetPosPair(int *num1st, int *num2nd){
    int r = 0;
    while (!r){
        GetRandPairPlusMinus(num1st, num2nd);
        r = *num1st + *num2nd;
        if(r > 99) r = 0;
    }
    return r;
}
int GetNegPair(int *num1st, int *num2nd){
    int r = 0;
    *num1st = GetPosPair(&r, num2nd);
    return r;
}
int GetMulPair(int *num1st, int *num2nd){
    int r = 0;
    while (!r){
        GetRandPairMulDiv(num1st, num2nd);
        r = *num1st * *num2nd;
        if(r > 99) r = 0;
    }
    if(GetRand(0,1)){
        // Shake 1st & 2nd
        r = *num1st;
        *num1st = *num2nd;
        *num2nd = r;
        r = *num1st * *num2nd;
    } 
    return r;
}
int GetDivPair(int *num1st, int *num2nd){
    int r = 0;
    *num1st = GetMulPair(&r, num2nd);
    return r;
}
void PrintMidLine(char symbol, int printSymbol){
    for (size_t i = 0; i < X_Copy; i++){
        if(LINE_COLOR) SetFg16(LINE_COLOR);
        printf("│ │ │");
        TxtBold(1);

        if(misChar[i]) symbol = misChar[i];

        if(LINE_COLOR) ResFg();
        if (printSymbol){
            printf("%c", symbol);
        }
        else{
            printf(" ");
        }

        TxtBold(0);
        if(LINE_COLOR) SetFg16(LINE_COLOR);
        printf("│ │ │");
        TxtBold(1);
        if(LINE_COLOR) ResFg();
        printf("=");
        TxtBold(0);
        if(LINE_COLOR) SetFg16(LINE_COLOR);
        printf("│ │ │");
        if(i < X_Copy-1) printf("  ");
        if(LINE_COLOR) ResFg();
    }    
    CursorRestore();
}

void PrintTriple(char *strIN, int cnt){
    if(!cnt) cnt = X_Copy;
    if(LINE_COLOR) SetFg16(LINE_COLOR);
    for (size_t i = 0; i < cnt; i++){
        for (size_t j = 0; j < 3; j++){
            printf("%s",strIN);
            if(i < X_Copy-1 || j < 3) printf(" ");
        }
        if(i < X_Copy-1) printf(" ");
    }    
    if(LINE_COLOR) ResFg();
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

void PrintVal(int val){
    char strVal[] = "  ";
    sprintf(strVal, "%d", val);
    if (val > 9){
        printf("%c", strVal[0]);
        CursorRight(1);
        printf("%c", strVal[1]);
    }
    else{
        CursorRight(2);
        printf("%c", strVal[0]);
    }
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

        int dontShow = GetRand(1, 3);       // Which value to remove, if randomized
        if(!MISSING) dontShow = 0;          // Show all
        if(MISSING == 1) dontShow = 3;      // remove result

        // Get Calculation pair and result
        if (symbol == '+'){
            r = GetPosPair(&num1st, &num2nd);
        }
        else if (symbol == '-'){
            r = GetNegPair(&num1st, &num2nd);
        }
        else if (symbol == '*'){
            r = GetMulPair(&num1st, &num2nd);
        }
        else {
            r = GetDivPair(&num1st, &num2nd);
        }
                
        if (dontShow != 1){
            // Print 1st value
            PrintVal(num1st);
        }
        else{
            CursorRight(3);
        }
        
        CursorRight(3);

        if (dontShow != 2){
            // Print 2nd Value        
            PrintVal(num2nd);
        }
        else{
            CursorRight(3);
        }

        CursorRight(3);
    
        if (dontShow != 3){
            // Print result
            PrintVal(r);
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
        else if (symbol == '-'){
            if (dontShow == 1){
                // "converter" line with "plus" needed
                convSign = '+';
            }
            else if (dontShow == 2){
                // "converter" line with "minus" needed
                convSign = '-';
            }            
        }
        else if (symbol == '*'){
            if (dontShow && dontShow < 3){
                // "converter" line with "div" needed
                convSign = '/';
            }
        }
        else{
            if (dontShow == 1){
                // "converter" line with "mul" needed
                convSign = '*';
            }
            else if (dontShow == 2){
                // "converter" line with "mul" needed
                convSign = '/';
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
        else if (SHRUGGIE && CONV_LINE){
            // Print ¯\_(ツ)_/¯
            TxtBold(0);
            SetFg16(GetRand(31,36));
            CursorMove(-12, -2);
            printf("¯\\_(ツ)_/¯");
            CursorMove(2,2);
            ResFg();
            TxtBold(1);
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
        else if (strncmp(argv[i], "-calc=", 6) == 0){
            CALC_MODE = atoi(argv[i] + 6);
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
        else if (strncmp(argv[i], "-linecolor=", 11) == 0){
            LINE_COLOR = atoi(argv[i] + 11);
        }
        else if (strncmp(argv[i], "-shruggie=", 10) == 0){
            SHRUGGIE = atoi(argv[i] + 10);
        }
        else if (strncmp(argv[i], "-twodigit=", 10) == 0){
            TWO_DIGIT = atoi(argv[i] + 10);
        }
        else if (strncmp(argv[i], "-help", 5) == 0){
            printf("\n\n");
            PrintHelp(argv[0]);
            return 0;
        }
        else{
            printf("\n\nIllegal option:\n");
            printf("  %s\n",argv[i]);
            PrintHelp(argv[0]);
            return -1;
        }
    }
    
    if(LINE_COLOR) LINE_COLOR += 29;

    CLS();
    CursorDown(1); CursorRight(2);

    srand(clock());    

    while (Y_Copy){
        if ((CALC_MODE & 1) == 1){
            PrintCalcLine('+');
            Y_Copy--;
        }
        if ((CALC_MODE & 2) == 2){
            PrintCalcLine('-');
            Y_Copy--;
        }
        if ((CALC_MODE & 4) == 4){
            PrintCalcLine('*');
            Y_Copy--;
        }
        if ((CALC_MODE & 8) == 8){
            PrintCalcLine('/');
            Y_Copy--;
        }
    }

    return 0;
}
