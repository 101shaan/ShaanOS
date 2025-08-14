//This is a simple kernel level shell that we're gonna be writing, for the most basic UI.
//This is meant to be used at level 0
#include "dadio_driver.h"
#include "stdbool.h"
#include "keyboard.h"
#include "hal_driver.h"
#include "timer.h"
#include "serial.h"
#include "phymem.h"

#define MAX_COMMAND_SIZE 50
#define MAX_TOKEN_SIZE 25 
#define MAX_NAME_SIZE 20 

//External variables
extern char __VGA_text_memory[];
//Helper functions
static void extract_token(int token_no);  //Takes token number n from command and puts it into buffer
static void parse_command();
static void flush_token_buffer();
static void flush_command_buffer();
static bool string_compare(char* str1, char* str2);
static void string_copy(char* strd,char* strs,int max_len);

//Shell functions
static void command_help();
static void command_fresh();
static void command_timer();
static void command_picture();
static void command_name();
static void command_snake();
static void command_quote();
static void command_echo();
static void command_uptime();
static void command_mem();
// Small helpers
static uint32_t parse_uint(const char* s);

//Global variables 
static char _cmd_buffer[MAX_COMMAND_SIZE];
static char _tkn_buffer[MAX_TOKEN_SIZE];
static char _shell_name[MAX_NAME_SIZE] = "shell";

void kshell()
{
    serial_write("[kshell] ready\n");
	command_fresh();
	//This loop gets the commands from us
	while(1)
	{
		monitor_puts("\n"); monitor_puts(_shell_name); monitor_puts(" >");
		flush_command_buffer();
		for(int i=0;i<MAX_COMMAND_SIZE;i++)
		{
			char input = get_monitor_char();
			if (input == 17)
			{	
				//uint8_t scanned=get_latest_scan_code();
				if(i!=0)
				{	uint32_t pointer = get_cursor();
					i-=2;
					pointer--;
					set_cursor(pointer);
				}
				else i--;
				continue;
			}
			else if (input == 18)
			{
				if(i==0&&_cmd_buffer[i]==0)
					{i--;continue;}	
				uint32_t pointer = get_cursor();
				if(_cmd_buffer[i]!=0)
					{pointer++;}
				else
					i--;
				set_cursor(pointer);
				continue;				
			}
			else if (input == '\b')
				{
					if (i > 0)
					{
						putc('\b');
						if(_cmd_buffer[i]!=0)
						{
							i--;
							int k=i;uint32_t pointer=get_cursor();
							while(_cmd_buffer[k]!=0)
							{
								_cmd_buffer[k]=_cmd_buffer[k+1];
								putc(_cmd_buffer[k]);
								k++;
							}
							set_cursor(pointer);
						}
						else
						{
						i--;
						_cmd_buffer[i] = 0;}
					}
					i--;
					continue;
				}
			else if (input == '\n')
			{
				parse_command();
				break;
			}
			else
			_cmd_buffer[i] = input;
			putc(input);
		}
	}
}

static void parse_command()
{
	extract_token(0);
	if(string_compare(_tkn_buffer,"help")) {command_help();return;}
	if(string_compare(_tkn_buffer,"fresh")){command_fresh();return;}
	if(string_compare(_tkn_buffer,"timer")){command_timer();return;}
    if(string_compare(_tkn_buffer,"picture")){command_picture();return;}
    if(string_compare(_tkn_buffer,"snake")){command_snake();return;}
	if(string_compare(_tkn_buffer,"quote")){command_quote();return;}
    if(string_compare(_tkn_buffer,"name")){command_name();return;}
	if(string_compare(_tkn_buffer,"echo")){command_echo();return;}
	if(string_compare(_tkn_buffer,"uptime")){command_uptime();return;}
	if(string_compare(_tkn_buffer,"mem")){command_mem();return;}

	monitor_puts(" - Command not found: ");
	monitor_puts(_tkn_buffer);
}

static void extract_token(int token_no)  //Takes token number n from command and puts it into buffer
{
	flush_token_buffer();
	char* pointer = _cmd_buffer;
	while (*pointer == ' ') pointer++;

	for (int i = 0; i < token_no; i++)
	{
		while (*pointer && *pointer != ' ') pointer++;
		while (*pointer == ' ') pointer++;
	}

	int i = 0;
	while (*pointer && *pointer != ' ' && i < (MAX_TOKEN_SIZE - 1))
	{
		_tkn_buffer[i++] = *pointer++;
	}
	_tkn_buffer[i] = 0;
}

static void flush_command_buffer()
{
	for (int i = 0; i < MAX_COMMAND_SIZE; i++)
	{
		_cmd_buffer[i] = 0;
	}
}
static void flush_token_buffer()
{
	for (int i = 0; i < MAX_TOKEN_SIZE; i++)
	{
		_tkn_buffer[i] = 0;
	}
}

static void command_help()
{
	monitor_puts("\nList of commands (use `command` help for usage):");
	monitor_puts("\nhelp");
	monitor_puts(" fresh");
	monitor_puts(" timer");
    monitor_puts(" picture");
    monitor_puts(" snake");
    monitor_puts(" name\n");
	monitor_puts(" echo");
	monitor_puts(" uptime");
	monitor_puts(" mem\n");
}

static void command_fresh()
{
	static int color[2] = {LIGHT_BLUE,BLACK};

	for(int i =0;i<2;i++)
	{
		extract_token(i+1);
		if(string_compare(_tkn_buffer,"black")) color[i] = BLACK;
		if(string_compare(_tkn_buffer,"blue")) color[i] = BLUE;
		if(string_compare(_tkn_buffer,"green")) color[i] = GREEN;
		if(string_compare(_tkn_buffer,"cyan")) color[i] = CYAN;
		if(string_compare(_tkn_buffer,"red")) color[i] = RED;
		if(string_compare(_tkn_buffer,"magenta")) color[i] = MAGENTA;
		if(string_compare(_tkn_buffer,"brown")) color[i] = BROWN;
		if(string_compare(_tkn_buffer,"light_grey")) color[i] = LIGHT_GREY;
		if(string_compare(_tkn_buffer,"dark_grey")) color[i] = DARK_GREY;
		if(string_compare(_tkn_buffer,"light_blue")) color[i] = LIGHT_BLUE;
		if(string_compare(_tkn_buffer,"light_green")) color[i] = LIGHT_GREEN;
		if(string_compare(_tkn_buffer,"light_cyan")) color[i] = LIGHT_CYAN;
		if(string_compare(_tkn_buffer,"light_red")) color[i] = LIGHT_RED;
		if(string_compare(_tkn_buffer,"light_magenta")) color[i] = LIGHT_MAGENTA;
		if(string_compare(_tkn_buffer,"light_brown")) color[i] = LIGHT_BROWN;
		if(string_compare(_tkn_buffer,"white")) color[i] = WHITE;
		if(string_compare(_tkn_buffer,"help"))
		{
			monitor_puts("\tUsage: fresh c1 c2\ncolors:");
			monitor_puts("  black");
			monitor_puts("  blue");
			monitor_puts("  green");
			monitor_puts("  cyan");
			monitor_puts("  red");
			monitor_puts("  magenta");
			monitor_puts("  brown");
			monitor_puts("  light_grey");
			monitor_puts("  dark_grey");
			monitor_puts("  light_blue");
			monitor_puts("  light_green");
			monitor_puts("  light_cyan");
			monitor_puts("  light_red");
			monitor_puts("  light_magenta");
			monitor_puts("  light_brown");
			monitor_puts("  white");
			return;
		}
	}

	set_fg_color(color[0]);
	set_bg_color(color[1]);
	clear(); // This is the background color

	set_fg_color(color[1]);
	set_bg_color(color[0]);
	for(int i=0;i<80;i++) putc(' ');
	set_cursor(20);
	monitor_puts("Shaan's OS KERNEL SHELL 0.01 (help displays commands)");

	set_fg_color(color[0]);
	set_bg_color(color[1]);
}

static void command_timer()
{
	extract_token(1);
	if(string_compare(_tkn_buffer,"help")) {
		monitor_puts("\tUsage: timer fast|medium|slow|hz <n>\n");
		monitor_puts("\t- fast/medium/slow: preset PIT rates\n");
		monitor_puts("\t- hz n: set PIT to n ticks per second (1..65535 effective)\n");
		return;
	}
	if(string_compare(_tkn_buffer,"fast")) { set_timer(0xffff>>2); monitor_puts("\nTimer: fast"); return; }
	if(string_compare(_tkn_buffer,"medium")) { set_timer(0xffff>>1); monitor_puts("\nTimer: medium"); return; }
	if(string_compare(_tkn_buffer,"slow")) { set_timer(0xffff); monitor_puts("\nTimer: slow"); return; }
	if(string_compare(_tkn_buffer,"hz")) {
		extract_token(2);
		uint32_t hz = parse_uint(_tkn_buffer);
		if(hz == 0) { monitor_puts("\nTimer: invalid hz"); return; }
		uint32_t divisor = 1193182u / hz;
		if(divisor == 0) divisor = 1;
		if(divisor > 0xFFFFu) divisor = 0xFFFFu;
		set_timer((uint16_t)divisor);
		monitor_puts("\nTimer set (hz): ");
		printhex(hz);
		return;
	}
	monitor_puts("\nTimer: unknown setting. Try 'timer help'.");
}

static void command_picture()
{
	monitor_puts("\nx;;;.',,,,,,,,;,;ckO000000000OxddddxxxkkkkkkkkOOOOOOO00OOOO00OOOkx");
	monitor_puts("\nx;;'.,,,,,,,,,,,,',oOOOOO0000000OOO00OOOOOOOOOOOOO0OOO00OOOOOOxl::");
	monitor_puts("\nx;,.',,,,,,,,,,,,,,,;ldxO0000000000000OOO0000000000000OOOOOkoc;;,,");
	monitor_puts("\nx;'.,,,''','''''''',''lkO0000000000000OOO00OOOO0000000Okdoc;;;;;,,");
	monitor_puts("\nx,.''''''''','.....'':kOOOOOOO00000000OO00OOOOOkkOOOO0OOkc,;;;;;,,");
	monitor_puts("\nx..'''''''',,'....'',dOOOOddc,oOO0000OOOOOOOOddl':xOOOOOOkc,;;;;;;");
	monitor_puts("\no.'''......'',,,,;;':kOOOkc,..;kOOOOOOOOOOOOOo,'.'dOOOOOOOx;;:cccc");
	monitor_puts("\no.''.........,,,;;,'dOOOO0OdoldOOOkxxkOOOOOOOOxoldOOOOOOOOOl;ccccc");
	monitor_puts("\nd,'..........,,;;;',loxkO00000OOOOxl:lkOOOOOOOOOOOOkxxxkOOOx:ccccc");
	monitor_puts("\nx,,'''',,,,,;;;:;,.,::::dO0000000OOOOOOOOOOOOOOOOOdc::ccdOOkc:cccc");
	monitor_puts("\nx,,,,,,,,,,;:::;;..;ccc:dO000000OkolclodxkOOOOOOOkl:ccc:ckOOx;;:::");
	monitor_puts("\nx,,,,,,,,,,,,,,,,.;c::cokO000000Ol':odddlokOO000OOdccccldOOOOo;:::");
	monitor_puts("\nx,,,,,,,,,,,,,,,''ckkkOO00000000Od:ldxxdookOO0000OOOkkkkOOOOOk::cc");
	monitor_puts("\nx;;;;,,,;;;;;;;;;;,lOO00000000000OkolllllxOOOOOOOOOOOOOOOOOOOOo;::");
	monitor_puts("\nd,,;::cccccccccccc:;oOOO0000OOOO00OOOOOOOOOOOOOOOOOOOOOOOOOOOOx;;;");
	monitor_puts("\noo'',,,;;:::::::;,,,;dkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkx:,,");
}

static void command_name()
{
	extract_token(1);
	if(string_compare(_tkn_buffer,"help"))
		{monitor_puts("\tUsage: name string"); return;}
	if(string_compare(_tkn_buffer,"masih"))
		string_copy(_tkn_buffer,"asshole", MAX_TOKEN_SIZE);
			
	for(int i=0;i<MAX_NAME_SIZE;i++) _shell_name[i] = 0; //Flush
	string_copy(_shell_name,_tkn_buffer, MAX_NAME_SIZE);
}

static void command_snake()
{
    extract_token(1);
    if(string_compare(_tkn_buffer,"help"))
        {monitor_puts("\nSnake: SPACE to start. Controls: WASD or arrows. Q to quit, R to restart. Eat * to grow."); return;}
    clear();
    set_cursor(0);
    char* vga_pointer = (char*) __VGA_text_memory;

    const int screen_w = 80;
    const int screen_h = 25;
    const int max_len = screen_w * screen_h - 1;
    static int sx[2000];
    static int sy[2000];
    int length = 3;
    int dirx = 1, diry = 0;
    int headx = 40, heady = 12;
    int foodx = 20, foody = 10;

    for(int i=0;i<length;i++){ sx[i] = headx - i; sy[i] = heady; }
    // Do not change global timer; step the snake every N ticks instead
    int tick_accum = 0;
    int base_ticks = 4;
    int step_ticks = base_ticks; // higher = slower
    monitor_puts("Press SPACE to start. Controls: WASD or arrows. Q to quit, R to restart.");
    while(1){ char c = get_monitor_char(); if(c==' ') break; if(c=='q'){ command_fresh(); return; } }
    clear(); set_cursor(0);
    vga_pointer[2*foodx + 160*foody] = '*';
    for(int i=0;i<length;i++) vga_pointer[2*sx[i] + 160*sy[i]] = (i==0?'O':'o');
    while(1)
    {
        kernel_wait();
        if(_is_keyboard_interrupt)
        {
            _is_keyboard_interrupt = 0;
            char c = get_latest_char();
            if(c=='q') break;
            if(c=='r') { command_fresh(); return; }
            if((c=='w' || c==19) && diry!=1){ dirx=0; diry=-1; }
            else if((c=='s' || c==20) && diry!=-1){ dirx=0; diry=1; }
            else if((c=='a' || c==17) && dirx!=1){ dirx=-1; diry=0; }
            else if((c=='d' || c==18) && dirx!=-1){ dirx=1; diry=0; }
        }
        if(_is_timer_interrupt)
        {
            _is_timer_interrupt = 0;
            if (++tick_accum < step_ticks) continue;
            tick_accum = 0;
			int newx = headx + dirx;
            int newy = heady + diry;
			if(newx<0||newx>=screen_w||newy<0||newy>=screen_h){
				monitor_puts("\nGame over (wall). Press r to restart or q to quit");
				while(1){ char c=get_monitor_char(); if(c=='q') goto end_snake; if(c=='r'){
					clear(); set_cursor(0);
					length=3; dirx=1; diry=0; headx=40; heady=12; foodx=20; foody=10;
					for(int i=0;i<length;i++){ sx[i]=headx-i; sy[i]=heady; }
					tick_accum = 0; step_ticks = base_ticks;
					vga_pointer[2*foodx + 160*foody] = '*';
					for(int i=0;i<length;i++) vga_pointer[2*sx[i] + 160*sy[i]] = (i==0?'O':'o');
					break; }
				}
				continue;
			}
			for(int i=0;i<length;i++){
				if(sx[i]==newx && sy[i]==newy){
					monitor_puts("\nGame over (self). Press r to restart or q to quit");
					while(1){ char c=get_monitor_char(); if(c=='q') goto end_snake; if(c=='r'){
						clear(); set_cursor(0);
						length=3; dirx=1; diry=0; headx=40; heady=12; foodx=20; foody=10;
						for(int j=0;j<length;j++){ sx[j]=headx-j; sy[j]=heady; }
						tick_accum = 0; step_ticks = base_ticks;
						vga_pointer[2*foodx + 160*foody] = '*';
						for(int j=0;j<length;j++) vga_pointer[2*sx[j] + 160*sy[j]] = (j==0?'O':'o');
						break; }
					}
					goto next_tick;
				}
			}
            int ate = (newx==foodx && newy==foody);
            if(!ate){
                int tx = sx[length-1], ty = sy[length-1];
                vga_pointer[2*tx + 160*ty] = ' ';
            }
            for(int i=length-1;i>0;i--){ sx[i]=sx[i-1]; sy[i]=sy[i-1]; }
            sx[0]=newx; sy[0]=newy; headx=newx; heady=newy;
			if(ate){
				if(length<max_len) length++;
                // increase speed a bit as snake grows
                step_ticks = base_ticks - (length/6);
                if(step_ticks < 2) step_ticks = 2;
				// place new food not on snake (naive scan)
				for(int tries=0; tries<screen_w*screen_h; tries++){
					foodx = (foodx + 7) % screen_w; foody = (foody + 3) % screen_h;
					int on_body = 0;
					for(int k=0;k<length;k++){ if(sx[k]==foodx && sy[k]==foody){ on_body = 1; break; } }
					if(!on_body) break;
				}
			}
            vga_pointer[2*foodx + 160*foody] = '*';
            vga_pointer[2*sx[0] + 160*sy[0]] = 'O';
            if(length>1) vga_pointer[2*sx[1] + 160*sy[1]] = 'o';
			next_tick: ;
        }
    }
end_snake:
    command_fresh();
}

static void command_quote()
{
	int sel = get_tick_count() % 5;
	switch(sel)
	{
		case 0:
			monitor_puts("\tTap that ass and step on the gas");
			break;
		case 1:
			monitor_puts("\tAlways stay grounded to your roots");
			break;
		case 2:
			monitor_puts("\tSometimes change is necessary");
			break;
		default:
			monitor_puts("\tTrust that good will happen");
	}
}
static void string_copy(char* strd,char* strs,int max_len)
{
	int i = 0;
	if (max_len <= 0) return;
	while (strs[i] && i < (max_len - 1))
	{
		strd[i] = strs[i];
		i++;
	}
	strd[i] = 0;
}



static void command_echo()
{
	extract_token(1);
	if(string_compare(_tkn_buffer,"help"))
		{monitor_puts("\tUsage: echo text"); return;}
	monitor_puts("\n");
	// Print the rest of the command buffer after first token
	char* pointer = _cmd_buffer;
	// skip leading spaces
	while(*pointer == ' ') pointer++;
	// skip first token
	while(*pointer && *pointer != ' ') pointer++;
	while(*pointer == ' ') pointer++;
	while(*pointer) { putc(*pointer++); }
}

static void command_uptime()
{
	uint32_t ticks = get_tick_count();
	// PIT default here is arbitrary; we print raw ticks for now
	monitor_puts("\nTicks: ");
	printhex(ticks);
}

static void command_mem()
{
	uint32_t blocks = pmmngr_free_block_count();
	monitor_puts("\nFree blocks (4KiB): ");
	printhex(blocks);
}

static uint32_t parse_uint(const char* s)
{
	uint32_t v = 0;
	int i = 0;
	while (s && s[i]) {
		char c = s[i++];
		if (c < '0' || c > '9') break;
		v = v * 10u + (uint32_t)(c - '0');
	}
	return v;
}

static bool string_compare(char* str1, char* str2)
{
	int i = 0;
	while (str1[i])
	{
		if(str1[i] != str2[i]) return false;
		i++;
	}
	if(str2[i]) return false;
	return true;
}


