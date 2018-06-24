/*
 * liborbis 
 * Copyright (C) 2015,2016,2017 Antonio Jose Ramos Marquez (aka bigboss) @psxdev on twitter
 * Repository https://github.com/orbisdev/liborbis
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <userservice.h>
#include <sysmodule.h>

#include "orbisKeyboard.h"
#include "kb.h"
#include <debugnet.h>

OrbisKeyboardConfig *orbisKeyboardConf=NULL;
int orbiskeyboard_external_conf=-1;
uint8_t orbisKeyboardNames[3][256];
void orbisKeyboardInitKeyNames()
{
	orbisKeyboardNames[0][KEY_A]=0x61; //0x0004  Keyboard a and A
	orbisKeyboardNames[0][KEY_B]=0x62; //0x0005 // Keyboard b and B
	orbisKeyboardNames[0][KEY_C]=0x63; //0x0006 // Keyboard c and C
	orbisKeyboardNames[0][KEY_D]=0x64; //0x0007 // Keyboard d and D
	orbisKeyboardNames[0][KEY_E]=0x65; //0x0008 // Keyboard e and E
	orbisKeyboardNames[0][KEY_F]=0x66; //0x0009 // Keyboard f and F
	orbisKeyboardNames[0][KEY_G]=0x67; //0x000a // Keyboard g and G
	orbisKeyboardNames[0][KEY_H]=0x68; //0x000b // Keyboard h and H
	orbisKeyboardNames[0][KEY_I]=0x69; //0x000c // Keyboard i and I
	orbisKeyboardNames[0][KEY_J]=0x6a; //0x000d // Keyboard j and J
	orbisKeyboardNames[0][KEY_K]=0x6b; //0x000e // Keyboard k and K
	orbisKeyboardNames[0][KEY_L]=0x6c; //0x000f // Keyboard l and L
	orbisKeyboardNames[0][KEY_M]=0x6d; //0x0010 // Keyboard m and M
	orbisKeyboardNames[0][KEY_N]=0x6e; //0x0011 // Keyboard n and N
	orbisKeyboardNames[0][KEY_O]=0x6f; //0x0012 // Keyboard o and O
	orbisKeyboardNames[0][KEY_P]=0x70; //0x0013 // Keyboard p and P
	orbisKeyboardNames[0][KEY_Q]=0x71; //0x0014 // Keyboard q and Q
	orbisKeyboardNames[0][KEY_R]=0x72; //0x0015 // Keyboard r and R
	orbisKeyboardNames[0][KEY_S]=0x73; //0x0016 // Keyboard s and S
	orbisKeyboardNames[0][KEY_T]=0x74; //0x0017 // Keyboard t and T
	orbisKeyboardNames[0][KEY_U]=0x75; //0x0018 // Keyboard u and U
	orbisKeyboardNames[0][KEY_V]=0x76; //0x0019 // Keyboard v and V
	orbisKeyboardNames[0][KEY_W]=0x77; //0x001a // Keyboard w and W
	orbisKeyboardNames[0][KEY_X]=0x78; //0x001b // Keyboard x and X
	orbisKeyboardNames[0][KEY_Y]=0x79; //0x001c // Keyboard y and Y
	orbisKeyboardNames[0][KEY_Z]=0x7a; //0x001d // Keyboard z and Z
	
	orbisKeyboardNames[1][KEY_A]=0x41; //0x0004  Keyboard a and A
	orbisKeyboardNames[1][KEY_B]=0x42; //0x0005 // Keyboard b and B
	orbisKeyboardNames[1][KEY_C]=0x43; //0x0006 // Keyboard c and C
	orbisKeyboardNames[1][KEY_D]=0x44; //0x0007 // Keyboard d and D
	orbisKeyboardNames[1][KEY_E]=0x45; //0x0008 // Keyboard e and E
	orbisKeyboardNames[1][KEY_F]=0x46; //0x0009 // Keyboard f and F
	orbisKeyboardNames[1][KEY_G]=0x47; //0x000a // Keyboard g and G
	orbisKeyboardNames[1][KEY_H]=0x48; //0x000b // Keyboard h and H
	orbisKeyboardNames[1][KEY_I]=0x49; //0x000c // Keyboard i and I
	orbisKeyboardNames[1][KEY_J]=0x4a; //0x000d // Keyboard j and J
	orbisKeyboardNames[1][KEY_K]=0x4b; //0x000e // Keyboard k and K
	orbisKeyboardNames[1][KEY_L]=0x4c; //0x000f // Keyboard l and L
	orbisKeyboardNames[1][KEY_M]=0x4d; //0x0010 // Keyboard m and M
	orbisKeyboardNames[1][KEY_N]=0x4e; //0x0011 // Keyboard n and N
	orbisKeyboardNames[1][KEY_O]=0x4f; //0x0012 // Keyboard o and O
	orbisKeyboardNames[1][KEY_P]=0x50; //0x0013 // Keyboard p and P
	orbisKeyboardNames[1][KEY_Q]=0x51; //0x0014 // Keyboard q and Q
	orbisKeyboardNames[1][KEY_R]=0x52; //0x0015 // Keyboard r and R
	orbisKeyboardNames[1][KEY_S]=0x53; //0x0016 // Keyboard s and S
	orbisKeyboardNames[1][KEY_T]=0x54; //0x0017 // Keyboard t and T
	orbisKeyboardNames[1][KEY_U]=0x55; //0x0018 // Keyboard u and U
	orbisKeyboardNames[1][KEY_V]=0x56; //0x0019 // Keyboard v and V
	orbisKeyboardNames[1][KEY_W]=0x57; //0x001a // Keyboard w and W
	orbisKeyboardNames[1][KEY_X]=0x58; //0x001b // Keyboard x and X
	orbisKeyboardNames[1][KEY_Y]=0x59; //0x001c // Keyboard y and Y
	orbisKeyboardNames[1][KEY_Z]=0x5a; //0x001d // Keyboard z and Z

	orbisKeyboardNames[0][KEY_1]=0x31; //0x001e // Keyboard 1 and !
	orbisKeyboardNames[0][KEY_2]=0x32; //0x001f // Keyboard 2 and @
	orbisKeyboardNames[0][KEY_3]=0x33; //0x0020 // Keyboard 3 and #
	orbisKeyboardNames[0][KEY_4]=0x34; //0x0021 // Keyboard 4 and $
	orbisKeyboardNames[0][KEY_5]=0x35; //0x0022 // Keyboard 5 and %
	orbisKeyboardNames[0][KEY_6]=0x36; //0x0023 // Keyboard 6 and ^
	orbisKeyboardNames[0][KEY_7]=0x37; //0x0024 // Keyboard 7 and &
	orbisKeyboardNames[0][KEY_8]=0x38; //0x0025 // Keyboard 8 and *
	orbisKeyboardNames[0][KEY_9]=0x39; //0x0026 // Keyboard 9 and (
	orbisKeyboardNames[0][KEY_0]=0x30; //0x0027 // Keyboard 0 and )
	
	orbisKeyboardNames[1][KEY_1]=0x21; //0x001e // Keyboard 1 and !
	orbisKeyboardNames[1][KEY_2]=0x22; //0x001f // Keyboard 2 and "
	orbisKeyboardNames[1][KEY_3]=0x23; //0x0020 // Keyboard 3 and #
	orbisKeyboardNames[1][KEY_4]=0x24; //0x0021 // Keyboard 4 and $
	orbisKeyboardNames[1][KEY_5]=0x25; //0x0022 // Keyboard 5 and %
	orbisKeyboardNames[1][KEY_6]=0x26; //0x0023 // Keyboard 6 and &
	orbisKeyboardNames[1][KEY_7]=0x27; //0x0024 // Keyboard 7 and `
	orbisKeyboardNames[1][KEY_8]=0x28; //0x0025 // Keyboard 8 and (
	orbisKeyboardNames[1][KEY_9]=0x29; //0x0026 // Keyboard 9 and )
	orbisKeyboardNames[1][KEY_0]=0; //0x0027 // Keyboard 0 and =
	

	orbisKeyboardNames[0][KEY_ENTER]=0x0d; //0x0028 // Keyboard Return (ENTER)
	orbisKeyboardNames[0][KEY_ESC]=0x1b; //0x0029 // Keyboard ESCAPE
	orbisKeyboardNames[0][KEY_BACKSPACE]=0x08; //0x002a // Keyboard DELETE (Backspace)
	orbisKeyboardNames[0][KEY_TAB]=0x09; //0x002b // Keyboard Tab
	orbisKeyboardNames[0][KEY_SPACE]=0x20; //0x002c // Keyboard Spacebar
	orbisKeyboardNames[0][KEY_MINUS]=0x27; //0x002d // Keyboard - and _ in spanish '
	orbisKeyboardNames[1][KEY_MINUS]=0x3f; //0x002d // Keyboard - and _ in spanish ?
	//orbisKeyboardNames[0][KEY_EQUAL]="¡"; //0x002e // Keyboard = and + in spanjish ¡
	//orbisKeyboardNames[1][KEY_EQUAL]="¿"; //0x002e // Keyboard = and + in spanish ¿
	
	orbisKeyboardNames[0][KEY_LEFTBRACE]=0x60; //0x2f // Keyboard [ and { in spanish `
	orbisKeyboardNames[1][KEY_LEFTBRACE]=0x7e; //0x2f // Keyboard [ and { in spanish 
	//orbisKeyboardNames[0][KEY_RIGHTBRACE]="+"; //0x30 // Keyboard ] and } in spanish +
	orbisKeyboardNames[1][KEY_RIGHTBRACE]=0x2a; //0x30 // Keyboard ] and } in spanish *
	
	//orbisKeyboardNames[0][KEY_BACKSLASH]="\\"; //0x31 // Keyboard \ and | not in spanish
	//orbisKeyboardNames[1][KEY_BACKSLASH]="|"; //0x31 // Keyboard \ and | not in spanish
	
	//orbisKeyboardNames[0][KEY_HASHTILDE]="#"; //0x32 // Keyboard Non-US # and ~ in spanish ç
	//orbisKeyboardNames[1][KEY_HASHTILDE]="~"; //0x32 // Keyboard Non-US # and ~ in spanish ç
	
	//orbisKeyboardNames[0][KEY_SEMICOLON]="ñ"; //0x33 // Keyboard ; and : in spanish ñ
	//orbisKeyboardNames[1][KEY_SEMICOLON]="Ñ"; //0x33 // Keyboard ; and : in spanish Ñ
	
	//orbisKeyboardNames[0][KEY_APOSTROPHE]="´"; //0x34 // Keyboard ' and " in spanish ´
	//orbisKeyboardNames[1][KEY_APOSTROPHE]="¨"; //0x34 // Keyboard ' and " in spanish 
	
	orbisKeyboardNames[0][KEY_GRAVE]=0x3c; //0x35 // Keyboard ` and ~ in spanish <
	orbisKeyboardNames[1][KEY_GRAVE]=0x3e; //0x35 // Keyboard ` and ~ in spanish >
	
	orbisKeyboardNames[0][KEY_COMMA]=0x2c; //0x36 // Keyboard , and < in spanish ,
	orbisKeyboardNames[1][KEY_COMMA]=0x3b; //0x36 // Keyboard , and < in spanish ;
	
	orbisKeyboardNames[0][KEY_DOT]=0x2e; //0x37 // Keyboard . and > in spanish .
	orbisKeyboardNames[1][KEY_DOT]=0x3a; //0x37 // Keyboard . and > in spanish :
	
	orbisKeyboardNames[0][KEY_SLASH]=0x2d; //0x38 // Keyboard / and ? in spanish -
	orbisKeyboardNames[1][KEY_SLASH]=0x5f; //0x38 // Keyboard / and ? in spanish _
	
	orbisKeyboardNames[0][KEY_CAPSLOCK]=0x0a; //0x39 // Keyboard Caps Lock not in normal keycode vita use special key 0x2 in reports.modifiers[1]

	orbisKeyboardNames[0][KEY_F1]=0x12; //0x3a // Keyboard F1
	orbisKeyboardNames[0][KEY_F2]=0x13; //0x3b // Keyboard F2
	orbisKeyboardNames[0][KEY_F3]=0x14; //0x3c // Keyboard F3
	orbisKeyboardNames[0][KEY_F4]=0x15; //0x3d // Keyboard F4
	orbisKeyboardNames[0][KEY_F5]=0x16; //0x3e // Keyboard F5
	orbisKeyboardNames[0][KEY_F6]=0; //0x3f // Keyboard F6
	orbisKeyboardNames[0][KEY_F7]=0; //0x40 // Keyboard F7
	orbisKeyboardNames[0][KEY_F8]=0; //0x41 // Keyboard F8
	orbisKeyboardNames[0][KEY_F9]=0; //0x42 // Keyboard F9
	orbisKeyboardNames[0][KEY_F10]=0; //0x43 // Keyboard F10
	orbisKeyboardNames[0][KEY_F11]=0; //0x44 // Keyboard F11
	orbisKeyboardNames[0][KEY_F12]=0x11; //0x45 // Keyboard F12 MSX STOP
	
	
	orbisKeyboardNames[0][KEY_RIGHT]=0x03; //0x4f // Keyboard Right Arrow
	orbisKeyboardNames[0][KEY_LEFT]=0x01; //0x50 // Keyboard Left Arrow
	orbisKeyboardNames[0][KEY_DOWN]=0x04; //0x51 // Keyboard Down Arrow
	orbisKeyboardNames[0][KEY_UP]=0x02; //0x52 // Keyboard Up Arrow
	
	//orbisKeyboardNames[0][KEY_NONUS_BACKSLASH]="º"; //0x0064 
	//orbisKeyboardNames[1][KEY_NONUS_BACKSLASH]="ª"; //0x0064
	
	orbisKeyboardNames[2][KEY_NONUS_BACKSLASH]=0x5c; //0x0064 with  alt
	orbisKeyboardNames[2][KEY_1]=0x7c; //0x001e with alt 
	orbisKeyboardNames[2][KEY_2]=0x40; //0x001f with alt
	orbisKeyboardNames[2][KEY_3]=0x23; //0x0020 with alt
	orbisKeyboardNames[2][KEY_4]=0x7e; //0x0021 with alt
	//orbisKeyboardNames[2][KEY_6]="¬"; //0x0023 with alt
	orbisKeyboardNames[2][KEY_LEFTBRACE]=0x5b; //0x002f with alt
	orbisKeyboardNames[2][KEY_RIGHTBRACE]=0x5d; //0x0030 with alt
	orbisKeyboardNames[2][KEY_APOSTROPHE]=0x7b; //0x0034 with alt
	orbisKeyboardNames[2][KEY_BACKSLASH]=0x7d; //0x0031 with alt
	
	
}
/*void orbisKeyboardInitKeyNames()
{
	orbisKeyboardNames[0][KEY_A]="a"; //0x0004  Keyboard a and A
	orbisKeyboardNames[0][KEY_B]="b"; //0x0005 // Keyboard b and B
	orbisKeyboardNames[0][KEY_C]="c"; //0x0006 // Keyboard c and C
	orbisKeyboardNames[0][KEY_D]="d"; //0x0007 // Keyboard d and D
	orbisKeyboardNames[0][KEY_E]="e"; //0x0008 // Keyboard e and E
	orbisKeyboardNames[0][KEY_F]="f"; //0x0009 // Keyboard f and F
	orbisKeyboardNames[0][KEY_G]="g"; //0x000a // Keyboard g and G
	orbisKeyboardNames[0][KEY_H]="h"; //0x000b // Keyboard h and H
	orbisKeyboardNames[0][KEY_I]="i"; //0x000c // Keyboard i and I
	orbisKeyboardNames[0][KEY_J]="j"; //0x000d // Keyboard j and J
	orbisKeyboardNames[0][KEY_K]="k"; //0x000e // Keyboard k and K
	orbisKeyboardNames[0][KEY_L]="l"; //0x000f // Keyboard l and L
	orbisKeyboardNames[0][KEY_M]="m"; //0x0010 // Keyboard m and M
	orbisKeyboardNames[0][KEY_N]="n"; //0x0011 // Keyboard n and N
	orbisKeyboardNames[0][KEY_O]="o"; //0x0012 // Keyboard o and O
	orbisKeyboardNames[0][KEY_P]="p"; //0x0013 // Keyboard p and P
	orbisKeyboardNames[0][KEY_Q]="q"; //0x0014 // Keyboard q and Q
	orbisKeyboardNames[0][KEY_R]="r"; //0x0015 // Keyboard r and R
	orbisKeyboardNames[0][KEY_S]="s"; //0x0016 // Keyboard s and S
	orbisKeyboardNames[0][KEY_T]="t"; //0x0017 // Keyboard t and T
	orbisKeyboardNames[0][KEY_U]="u"; //0x0018 // Keyboard u and U
	orbisKeyboardNames[0][KEY_V]="v"; //0x0019 // Keyboard v and V
	orbisKeyboardNames[0][KEY_W]="w"; //0x001a // Keyboard w and W
	orbisKeyboardNames[0][KEY_X]="x"; //0x001b // Keyboard x and X
	orbisKeyboardNames[0][KEY_Y]="y"; //0x001c // Keyboard y and Y
	orbisKeyboardNames[0][KEY_Z]="z"; //0x001d // Keyboard z and Z
	
	orbisKeyboardNames[1][KEY_A]="A"; //0x0004  Keyboard a and A
	orbisKeyboardNames[1][KEY_B]="B"; //0x0005 // Keyboard b and B
	orbisKeyboardNames[1][KEY_C]="C"; //0x0006 // Keyboard c and C
	orbisKeyboardNames[1][KEY_D]="D"; //0x0007 // Keyboard d and D
	orbisKeyboardNames[1][KEY_E]="E"; //0x0008 // Keyboard e and E
	orbisKeyboardNames[1][KEY_F]="F"; //0x0009 // Keyboard f and F
	orbisKeyboardNames[1][KEY_G]="G"; //0x000a // Keyboard g and G
	orbisKeyboardNames[1][KEY_H]="H"; //0x000b // Keyboard h and H
	orbisKeyboardNames[1][KEY_I]="I"; //0x000c // Keyboard i and I
	orbisKeyboardNames[1][KEY_J]="J"; //0x000d // Keyboard j and J
	orbisKeyboardNames[1][KEY_K]="K"; //0x000e // Keyboard k and K
	orbisKeyboardNames[1][KEY_L]="L"; //0x000f // Keyboard l and L
	orbisKeyboardNames[1][KEY_M]="M"; //0x0010 // Keyboard m and M
	orbisKeyboardNames[1][KEY_N]="N"; //0x0011 // Keyboard n and N
	orbisKeyboardNames[1][KEY_O]="O"; //0x0012 // Keyboard o and O
	orbisKeyboardNames[1][KEY_P]="P"; //0x0013 // Keyboard p and P
	orbisKeyboardNames[1][KEY_Q]="Q"; //0x0014 // Keyboard q and Q
	orbisKeyboardNames[1][KEY_R]="R"; //0x0015 // Keyboard r and R
	orbisKeyboardNames[1][KEY_S]="S"; //0x0016 // Keyboard s and S
	orbisKeyboardNames[1][KEY_T]="T"; //0x0017 // Keyboard t and T
	orbisKeyboardNames[1][KEY_U]="U"; //0x0018 // Keyboard u and U
	orbisKeyboardNames[1][KEY_V]="V"; //0x0019 // Keyboard v and V
	orbisKeyboardNames[1][KEY_W]="W"; //0x001a // Keyboard w and W
	orbisKeyboardNames[1][KEY_X]="X"; //0x001b // Keyboard x and X
	orbisKeyboardNames[1][KEY_Y]="Y"; //0x001c // Keyboard y and Y
	orbisKeyboardNames[1][KEY_Z]="Z"; //0x001d // Keyboard z and Z

	orbisKeyboardNames[0][KEY_1]="1"; //0x001e // Keyboard 1 and !
	orbisKeyboardNames[0][KEY_2]="2"; //0x001f // Keyboard 2 and @
	orbisKeyboardNames[0][KEY_3]="3"; //0x0020 // Keyboard 3 and #
	orbisKeyboardNames[0][KEY_4]="4"; //0x0021 // Keyboard 4 and $
	orbisKeyboardNames[0][KEY_5]="5"; //0x0022 // Keyboard 5 and %
	orbisKeyboardNames[0][KEY_6]="6"; //0x0023 // Keyboard 6 and ^
	orbisKeyboardNames[0][KEY_7]="7"; //0x0024 // Keyboard 7 and &
	orbisKeyboardNames[0][KEY_8]="8"; //0x0025 // Keyboard 8 and *
	orbisKeyboardNames[0][KEY_9]="9"; //0x0026 // Keyboard 9 and (
	orbisKeyboardNames[0][KEY_0]="0"; //0x0027 // Keyboard 0 and )
	
	orbisKeyboardNames[1][KEY_1]="!"; //0x001e // Keyboard 1 and !
	orbisKeyboardNames[1][KEY_2]="\""; //0x001f // Keyboard 2 and "
	orbisKeyboardNames[1][KEY_3]="·"; //0x0020 // Keyboard 3 and ·
	orbisKeyboardNames[1][KEY_4]="$"; //0x0021 // Keyboard 4 and $
	orbisKeyboardNames[1][KEY_5]="%"; //0x0022 // Keyboard 5 and %
	orbisKeyboardNames[1][KEY_6]="&"; //0x0023 // Keyboard 6 and &
	orbisKeyboardNames[1][KEY_7]="/"; //0x0024 // Keyboard 7 and /
	orbisKeyboardNames[1][KEY_8]="("; //0x0025 // Keyboard 8 and (
	orbisKeyboardNames[1][KEY_9]=")"; //0x0026 // Keyboard 9 and )
	orbisKeyboardNames[1][KEY_0]="="; //0x0027 // Keyboard 0 and =
	

	orbisKeyboardNames[0][KEY_ENTER]="\n"; //0x0028 // Keyboard Return (ENTER)
	orbisKeyboardNames[0][KEY_ESC]="^["; //0x0029 // Keyboard ESCAPE
	orbisKeyboardNames[0][KEY_BACKSPACE]=""; //0x002a // Keyboard DELETE (Backspace)
	orbisKeyboardNames[0][KEY_TAB]="	"; //0x002b // Keyboard Tab
	orbisKeyboardNames[0][KEY_SPACE]=" "; //0x002c // Keyboard Spacebar
	orbisKeyboardNames[0][KEY_MINUS]="'"; //0x002d // Keyboard - and _ in spanish '
	orbisKeyboardNames[1][KEY_MINUS]="?"; //0x002d // Keyboard - and _ in spanish ?
	orbisKeyboardNames[0][KEY_EQUAL]="¡"; //0x002e // Keyboard = and + in spanjish ¡
	orbisKeyboardNames[1][KEY_EQUAL]="¿"; //0x002e // Keyboard = and + in spanish ¿
	
	orbisKeyboardNames[0][KEY_LEFTBRACE]="`"; //0x2f // Keyboard [ and { in spanish `
	orbisKeyboardNames[1][KEY_LEFTBRACE]="^"; //0x2f // Keyboard [ and { in spanish 
	orbisKeyboardNames[0][KEY_RIGHTBRACE]="+"; //0x30 // Keyboard ] and } in spanish +
	orbisKeyboardNames[1][KEY_RIGHTBRACE]="*"; //0x30 // Keyboard ] and } in spanish *
	
	orbisKeyboardNames[0][KEY_BACKSLASH]="\\"; //0x31 // Keyboard \ and | not in spanish
	orbisKeyboardNames[1][KEY_BACKSLASH]="|"; //0x31 // Keyboard \ and | not in spanish
	
	orbisKeyboardNames[0][KEY_HASHTILDE]="#"; //0x32 // Keyboard Non-US # and ~ in spanish ç
	orbisKeyboardNames[1][KEY_HASHTILDE]="~"; //0x32 // Keyboard Non-US # and ~ in spanish ç
	
	orbisKeyboardNames[0][KEY_SEMICOLON]="ñ"; //0x33 // Keyboard ; and : in spanish ñ
	orbisKeyboardNames[1][KEY_SEMICOLON]="Ñ"; //0x33 // Keyboard ; and : in spanish Ñ
	
	orbisKeyboardNames[0][KEY_APOSTROPHE]="´"; //0x34 // Keyboard ' and " in spanish ´
	orbisKeyboardNames[1][KEY_APOSTROPHE]="¨"; //0x34 // Keyboard ' and " in spanish 
	
	orbisKeyboardNames[0][KEY_GRAVE]="<"; //0x35 // Keyboard ` and ~ in spanish <
	orbisKeyboardNames[1][KEY_GRAVE]=">"; //0x35 // Keyboard ` and ~ in spanish >
	
	orbisKeyboardNames[0][KEY_COMMA]=","; //0x36 // Keyboard , and < in spanish ,
	orbisKeyboardNames[1][KEY_COMMA]=";"; //0x36 // Keyboard , and < in spanish ;
	
	orbisKeyboardNames[0][KEY_DOT]="."; //0x37 // Keyboard . and > in spanish .
	orbisKeyboardNames[1][KEY_DOT]=":"; //0x37 // Keyboard . and > in spanish :
	
	orbisKeyboardNames[0][KEY_SLASH]="-"; //0x38 // Keyboard / and ? in spanish -
	orbisKeyboardNames[1][KEY_SLASH]="_"; //0x38 // Keyboard / and ? in spanish _
	
	orbisKeyboardNames[0][KEY_CAPSLOCK]=""; //0x39 // Keyboard Caps Lock not in normal keycode vita use special key 0x2 in reports.modifiers[1]

	orbisKeyboardNames[0][KEY_F1]=""; //0x3a // Keyboard F1
	orbisKeyboardNames[0][KEY_F2]=""; //0x3b // Keyboard F2
	orbisKeyboardNames[0][KEY_F3]=""; //0x3c // Keyboard F3
	orbisKeyboardNames[0][KEY_F4]=""; //0x3d // Keyboard F4
	orbisKeyboardNames[0][KEY_F5]=""; //0x3e // Keyboard F5
	orbisKeyboardNames[0][KEY_F6]=""; //0x3f // Keyboard F6
	orbisKeyboardNames[0][KEY_F7]=""; //0x40 // Keyboard F7
	orbisKeyboardNames[0][KEY_F8]=""; //0x41 // Keyboard F8
	orbisKeyboardNames[0][KEY_F9]=""; //0x42 // Keyboard F9
	orbisKeyboardNames[0][KEY_F10]=""; //0x43 // Keyboard F10
	orbisKeyboardNames[0][KEY_F11]=""; //0x44 // Keyboard F11
	orbisKeyboardNames[0][KEY_F12]=""; //0x45 // Keyboard F12
	
	orbisKeyboardNames[0][KEY_SYSRQ]=""; //0x46 // Keyboard Print Screen
	orbisKeyboardNames[0][KEY_SCROLLLOCK]=""; //0x47 // Keyboard Scroll Lock
	orbisKeyboardNames[0][KEY_PAUSE]=""; //0x48 // Keyboard Pause
	orbisKeyboardNames[0][KEY_INSERT]=""; //0x49 // Keyboard Insert
	orbisKeyboardNames[0][KEY_HOME]=""; //0x4a // Keyboard Home
	orbisKeyboardNames[0][KEY_PAGEUP]=""; //0x4b // Keyboard Page Up
	orbisKeyboardNames[0][KEY_DELETE]=""; //0x4c // Keyboard Delete Forward
	orbisKeyboardNames[0][KEY_END]=""; //0x4d // Keyboard End
	orbisKeyboardNames[0][KEY_PAGEDOWN]=""; //0x4e // Keyboard Page Down
	
	orbisKeyboardNames[0][KEY_RIGHT]=""; //0x4f // Keyboard Right Arrow
	orbisKeyboardNames[0][KEY_LEFT]=""; //0x50 // Keyboard Left Arrow
	orbisKeyboardNames[0][KEY_DOWN]=""; //0x51 // Keyboard Down Arrow
	orbisKeyboardNames[0][KEY_UP]=""; //0x52 // Keyboard Up Arrow
	
	orbisKeyboardNames[0][KEY_NONUS_BACKSLASH]="º"; //0x0064 
	orbisKeyboardNames[1][KEY_NONUS_BACKSLASH]="ª"; //0x0064
	
	orbisKeyboardNames[2][KEY_NONUS_BACKSLASH]="\\"; //0x0064 with  alt
	orbisKeyboardNames[2][KEY_1]="|"; //0x001e with alt 
	orbisKeyboardNames[2][KEY_2]="@"; //0x001f with alt
	orbisKeyboardNames[2][KEY_3]="#"; //0x0020 with alt
	orbisKeyboardNames[2][KEY_4]="~"; //0x0021 with alt
	orbisKeyboardNames[2][KEY_6]="¬"; //0x0023 with alt
	orbisKeyboardNames[2][KEY_LEFTBRACE]="["; //0x002f with alt
	orbisKeyboardNames[2][KEY_RIGHTBRACE]="]"; //0x0030 with alt
	orbisKeyboardNames[2][KEY_APOSTROPHE]="{"; //0x0034 with alt
	orbisKeyboardNames[2][KEY_BACKSLASH]="}"; //0x0031 with alt
	
	
}*/
OrbisKeyboardConfig * OrbisKeyboardGetConf()
{
	return orbisKeyboardConf;
}  		
uint8_t orbisKeyboardGetMsxKey()
{
	if(orbisKeyboardConf!=NULL)
	{
		return orbisKeyboardConf->key;
		
	}
	return 0;
}

uint8_t orbisKeyboardGetMsxKeyStatus()
{
	if(orbisKeyboardConf!=NULL)
	{
		return orbisKeyboardConf->key_status;
	}
	return 0;
}
uint8_t orbisKeyboardGetMsxKeyShift()
{
	if(orbisKeyboardConf!=NULL)
	{
		return orbisKeyboardConf->shift_key_left|orbisKeyboardConf->shift_key_right;
	}
	return 0;
}
uint8_t orbisKeyboardGetMsxKeyCtrl()
{
	if(orbisKeyboardConf!=NULL)
	{
		return orbisKeyboardConf->control_key_left|orbisKeyboardConf->control_key_right;
	}
	return 0;
}
int orbisKeyboardCreateConf()
{	
	if(!orbisKeyboardConf)
	{
		orbisKeyboardConf=(OrbisKeyboardConfig *)malloc(sizeof(OrbisKeyboardConfig));
		
		orbisKeyboardConf->userId=0;
		orbisKeyboardConf->status=-1;
		orbisKeyboardConf->caps_key=0;
		orbisKeyboardConf->control_key_left=0;
		orbisKeyboardConf->control_key_right=0;
		orbisKeyboardConf->shift_key_left=0;
		orbisKeyboardConf->shift_key_right=0;
		orbisKeyboardConf->alt_key_left=0;
		orbisKeyboardConf->alt_key_right=0;
		orbisKeyboardConf->cmd_key_left=0;
		orbisKeyboardConf->cmd_key_right=0;
		orbisKeyboardConf->key=0;
		orbisKeyboardConf->key_status=0;
		orbisKeyboardConf->orbiskeyboard_initialized=-1;
		
		return 0;
	}
	
	if(orbisKeyboardConf->orbiskeyboard_initialized==1)
	{
		return orbisKeyboardConf->orbiskeyboard_initialized;
	}
	//something weird happened
	return -1;			
}

void orbisKeybordGetSpecialKeys(const OrbisKeyboardEvent *e)
{
	if(e->param.keycode.status&0x00020000)
	{
		orbisKeyboardConf->caps_key=1;	
	}
	else
	{
		orbisKeyboardConf->caps_key=0;	
	}
	if(e->param.keycode.status&0x00000100)
	{
		orbisKeyboardConf->control_key_left=1;	
	}
	else
	{
		orbisKeyboardConf->control_key_left=0;	
	}
	if(e->param.keycode.status&0x00001000)
	{
		orbisKeyboardConf->control_key_right=1;	
	}
	else
	{
		orbisKeyboardConf->control_key_right=0;	
	}
	if(e->param.keycode.status&0x00000200)
	{
		orbisKeyboardConf->shift_key_left=1;	
	}
	else
	{
		orbisKeyboardConf->shift_key_left=0;	
	}
	if(e->param.keycode.status&0x00002000)
	{
		orbisKeyboardConf->shift_key_right=1;	
	}
	else
	{
		orbisKeyboardConf->shift_key_right=0;	
	}
	if(e->param.keycode.status&0x00000400)
	{
		orbisKeyboardConf->alt_key_left=1;	
	}
	else
	{
		orbisKeyboardConf->alt_key_left=0;	
	}
	if(e->param.keycode.status&0x00004000)
	{
		orbisKeyboardConf->alt_key_right=1;	
	}
	else
	{
		orbisKeyboardConf->alt_key_right=0;	
	}
	if(e->param.keycode.status&0x00000800)
	{
		orbisKeyboardConf->cmd_key_left=1;	
	}
	else
	{
		orbisKeyboardConf->cmd_key_left=0;	
	}
	if(e->param.keycode.status&0x00008000)
	{
		orbisKeyboardConf->cmd_key_right=1;	
	}
	else
	{
		orbisKeyboardConf->cmd_key_right=0;	
	}
}
static void orbisKeyboardEventUpdate(void *arg,const OrbisKeyboardEvent *e)
{
	int i;
	int index;
	switch(e->id)
	{
		case ORBIS_KEYBOARD_EVENT_OPEN:
        	debugNetPrintf(DEBUG,"liborbiskeyboard opened for userId:0x%08x\n",e->param.resourceIdArray.userId);
			for(i=0;i<3;i++)
			{
        		debugNetPrintf(DEBUG,"%d ",e->param.resourceIdArray.resourceId[i]);
			}
    		debugNetPrintf(DEBUG,"\n");			
			break;
		case ORBIS_KEYBOARD_EVENT_KEYCODE_DOWN:
		
			orbisKeybordGetSpecialKeys(e);
			
			if(e->param.keycode.keycode<0x001e || e->param.keycode.keycode==0x0033)
			{
				index=orbisKeyboardConf->caps_key|orbisKeyboardConf->shift_key_left|orbisKeyboardConf->shift_key_right;
			}
			else
			{
				index=orbisKeyboardConf->shift_key_left|orbisKeyboardConf->shift_key_right;
			}
			if(orbisKeyboardConf->alt_key_left|orbisKeyboardConf->alt_key_right)
			{
				index=2;
			}
		debugNetPrintf(DEBUG, "liborbiskeyboard key down for keycode:0x%04X\n",e->param.keycode.keycode);
		debugNetPrintf(DEBUG, "liborbiskeyboard key down for status:%8x\n",e->param.keycode.status);
		//debugNetPrintf(DEBUG, "liborbiskeyboard key down for unknown1:%8x\n",e->param.keycode.unknown1);
		//debugNetPrintf(DEBUG, "liborbiskeyboard key down for userId:%8x\n",e->param.keycode.userId);
		//debugNetPrintf(DEBUG, "liborbiskeyboard key down for resourceId:%8x\n",e->param.keycode.resourceId);
		//debugNetPrintf(DEBUG, "liborbiskeyboard key down for unknown2:%8x\n",e->param.keycode.unknown2);
		
		//debugNetPrintf(DEBUG, "liborbiskeyboard key down for timestamp:%lu\n",e->param.keycode.timestamp);
		debugNetPrintf(DEBUG, "liborbiskeyboard key down for caps %d ctrll %d ctrlr %d shiftl %d shiftr %d altl %d altr %d cmdl %d cmdr %d\n",orbisKeyboardConf->caps_key,orbisKeyboardConf->control_key_left,orbisKeyboardConf->control_key_right,orbisKeyboardConf->shift_key_left,orbisKeyboardConf->shift_key_right,orbisKeyboardConf->alt_key_left,orbisKeyboardConf->alt_key_right,orbisKeyboardConf->cmd_key_left,orbisKeyboardConf->cmd_key_right);
		//		if((e->param.keycode.status&0x00000003)==0x00000003)

		if((e->param.keycode.status&0x00000003))
		{
		debugNetPrintf(DEBUG, "liborbiskeyboard key down :%02X\n",orbisKeyboardNames[index][e->param.keycode.keycode]);
    	orbisKeyboardConf->key=orbisKeyboardNames[index][e->param.keycode.keycode];
		orbisKeyboardConf->key_status=1;
		}
		
			
			
			break;
			
		case ORBIS_KEYBOARD_EVENT_KEYCODE_UP:
			debugNetPrintf(DEBUG,"liborbiskeyboard key up for keycode:0x%04x\n",e->param.keycode.keycode);
			orbisKeybordGetSpecialKeys(e);
			if(e->param.keycode.keycode<0x001e || e->param.keycode.keycode==0x0033)
			{
				index=orbisKeyboardConf->caps_key|orbisKeyboardConf->shift_key_left|orbisKeyboardConf->shift_key_right;
			}
			else
			{
				index=orbisKeyboardConf->shift_key_left|orbisKeyboardConf->shift_key_right;
			}
			if(orbisKeyboardConf->alt_key_left|orbisKeyboardConf->alt_key_right)
			{
				index=2;
			}
			orbisKeyboardConf->key=orbisKeyboardNames[index][e->param.keycode.keycode];
			orbisKeyboardConf->key_status=0;
			
			break;
		case ORBIS_KEYBOARD_EVENT_KEYCODE_REPEAT:
			debugNetPrintf(DEBUG,"liborbiskeyboard key repeat for keycode:0x%04x\n",e->param.keycode.keycode);
			break;
		case ORBIS_KEYBOARD_EVENT_CONNECTION:
    		debugNetPrintf(DEBUG,"liborbiskeyboard connected for userId:0x%08x\n",e->param.resourceIdArray.userId);
			for(i=0;i<3;i++)
			{
    			debugNetPrintf(DEBUG,"%d ",e->param.resourceIdArray.resourceId[i]);
			}
			debugNetPrintf(DEBUG,"\n");	
			break;
		case ORBIS_KEYBOARD_EVENT_DISCONNECTION:
    	debugNetPrintf(DEBUG,"liborbiskeyboard disconnected for userId:0x%08x\n",e->param.resourceIdArray.userId);
		for(i=0;i<3;i++)
		{
    		debugNetPrintf(DEBUG,"%d ",e->param.resourceIdArray.resourceId[i]);
		}
		debugNetPrintf(DEBUG,"\n");	
			break;
		case ORBIS_KEYBOARD_EVENT_ABORT:
    	debugNetPrintf(DEBUG,"liborbiskeyboard aborted\n");
		
			break;
		default:
    	debugNetPrintf(DEBUG,"liborbiskeyboard unknown event\n");
		
			break;
	}
}
void orbisKeyboardUpdate()
{
	if(orbisKeyboardConf!=NULL)
	{
		//if(orbisKeyboardConf->orbiskeyboard_initialized==1 && orbisKeyboardConf->status==0)
		//{
			sceImeUpdate(orbisKeyboardEventUpdate);
			//}
	}
}
void orbisKeyboardClose()
{
	int ret;
	if(orbisKeyboardConf!=NULL)
	{
		if(orbisKeyboardConf->orbiskeyboard_initialized!=1)
		{
			return;
		}
		if(orbisKeyboardConf->status==0 )
		{
			ret=sceImeKeyboardClose(orbisKeyboardConf->userId);
			if(ret<0)
			{
		        debugNetPrintf(DEBUG,"liborbiskeyboard sceImeKeyboardClose return error 0x%8x\n",ret);
				
			}
			orbisKeyboardConf->status=-1;
			
		}
	}
}
int orbisKeyboardOpen()
{
    debugNetPrintf(DEBUG,"liborbiskeyboard sceImeKeyboardOpen in\n");
	sleep(1);
	OrbisKeyboardParam param;
	int ret;
	if(orbisKeyboardConf!=NULL)
	{
	    debugNetPrintf(DEBUG,"liborbiskeyboard orbisKeyboardConf not null \n");
		
		if(orbisKeyboardConf->orbiskeyboard_initialized!=1  )
		{
		    debugNetPrintf(DEBUG,"liborbiskeyboard orbisKeyboardConf %d \n",orbisKeyboardConf->orbiskeyboard_initialized);
			
			return -1;
		}
		
		memset(&param,0,sizeof(OrbisKeyboardParam));
		param.option=0;
		param.handler=orbisKeyboardEventUpdate;
		ret=sceImeKeyboardOpen(orbisKeyboardConf->userId,&param);
		sleep(1);
		if(ret<0)
		{
	        debugNetPrintf(DEBUG,"liborbiskeyboard sceImeKeyboardOpen return error 0x%8x\n",ret);
	        return -1;
		}
		sleep(1);
        debugNetPrintf(DEBUG,"liborbiskeyboard sceImeKeyboardOpen return success %d\n",ret);
		orbisKeyboardConf->status=0; //opened
		return 0;
	}
	else
	{
        debugNetPrintf(DEBUG,"liborbiskeyboard orbisKeyboardOpen error config null\n");
		
		return -1;
	}	
}
void orbisKeyboardFinish()
{
	if(orbiskeyboard_external_conf!=1)
	{
		if(orbisKeyboardConf!=NULL)
		{
			orbisKeyboardClose();
			sceSysmoduleUnloadModule(ORBIS_IME_MODULE);
			orbisKeyboardConf->userId=0;
			orbisKeyboardConf->orbiskeyboard_initialized=-1;
			debugNetPrintf(3,"liborbisKeyboard finished\n");
		
		}
	}
	
}
int orbisKeyboardSetConf(OrbisKeyboardConfig *conf)
{
	if(conf)
	{
		orbisKeyboardConf=conf;
		orbiskeyboard_external_conf=1;
		return orbisKeyboardConf->orbiskeyboard_initialized;
	}
	
	return 0; 
}
int orbisKeyboardInitWithConf(OrbisKeyboardConfig *conf)
{
	int ret=orbisKeyboardSetConf(conf);
	if(ret)
	{
		debugNetPrintf(3,"liborbisKeyboard already initialized using configuration external\n");
		debugNetPrintf(3,"orbiskeyboard_initialized=%d\n",orbisKeyboardConf->orbiskeyboard_initialized);
		debugNetPrintf(3,"ready to have a lot of fun...\n");
		return orbisKeyboardConf->orbiskeyboard_initialized;
	}
	else
	{
		return 0;
	}
}

int orbisKeyboardInit()
{
	int ret;
	if(orbisKeyboardCreateConf()==1)
	{
		return orbisKeyboardConf->orbiskeyboard_initialized;
	}
	if (orbisKeyboardConf->orbiskeyboard_initialized==1) 
	{
		debugNetPrintf(DEBUG,"liborbiskeyboard is already initialized!\n");
		return orbisKeyboardConf->orbiskeyboard_initialized;
	}
		
	ret=sceSysmoduleLoadModule(ORBIS_IME_MODULE);
	
	if(ret<0)
	{
        debugNetPrintf(DEBUG,"liborbiskeyboard sceSysmoduleLoadModule return error 0x%8x\n",ret);
        return -1;
	}
	//if we have orbisPad initialized then UserService module is already loaded, so first call orbisPadInit before orbisKeyboardInit
    ret=sceUserServiceGetInitialUser(&orbisKeyboardConf->userId);
	
	if(ret<0)
	{
        debugNetPrintf(DEBUG,"liborbiskeyboard sceUserServiceGetInitialUser return error 0x%8x\n",ret);
        return -1;
		
	}
	orbisKeyboardInitKeyNames();
	
	orbisKeyboardConf->orbiskeyboard_initialized=1;
	
	debugNetPrintf(DEBUG,"liborbiskeyboard is initialized!\n");
	
	return orbisKeyboardConf->orbiskeyboard_initialized;
	
}
