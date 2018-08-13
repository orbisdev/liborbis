#include <stdio.h>
#include <stdlib.h>

#include <orbis2d.h>
#include <orbisXbmFont.h>
#include "menu.h"
#include "settings.h"

//Sub_Menu Setting Conf

	// theme selector
	char *theme_text=	"THEME BY DEFAULT - WOODY_GEEK";
	select_theme=0;

	//Slide Menu
	#define slide_menu_on 	"MENU SLIDE = ON  - Press X to switch"
	#define slide_menu_off	"MENU SLIDE = OFF - Press X to switch"
	char *slide_menu_text=slide_menu_off;
	menu_slide=0;		

	// slide terminal
	#define slide_term_speed 	"TERMINAL SLIDE SPEED = %i"
	#define slide_term_off		"TERMINAL SLIDE SPEED = OFF"
	char *slide_term_text=slide_term_speed;
	slide_term_status=1;

	//switch menu terminal
	#define slide_term_adjust	"SWITCH MENU = ADJUST TERMINAL"
	#define slide_term_new		"SWITCH MENU = NEW TERMINAL"
	char *slide_term_switch_text=slide_term_adjust;
	slide_term_switch=1;


	//submenu setting var
	int SubMenuSettinglevel=Theme_Select;


//draw submenu settingas
void SubMenuSettingsDraw()
{
	
	orbis2dDrawRectColor(sub_hig_x, sub_hig_w, sub_hig_y, sub_hig_h, sub_hig_color);

	c1=0xFF4ce600;
	c2=0xFF4ce600;
	update_gradient(&c1, &c2);

	sprintf(tmp_ln, theme_text);
	tx = get_aligned_x(tmp_ln, CENTER);
	print_text(sub_hig_x+5, sub_text_y, theme_text);
			
	ty=sub_text_y + sub_text_step;
	sprintf(tmp_ln, slide_menu_text);
	tx = get_aligned_x(tmp_ln, CENTER);
	print_text(sub_hig_x+5, ty, slide_menu_text);

	ty= ty + sub_text_step;
	char str[80];
	sprintf(str, slide_term_text, slidespeed);
	sprintf(tmp_ln, str);
	tx = get_aligned_x(tmp_ln, CENTER);
	print_text(sub_hig_x+5, ty, str);

	ty= ty + sub_text_step;
	sprintf(tmp_ln, slide_term_switch_text);
	tx = get_aligned_x(tmp_ln, CENTER);
	print_text(sub_hig_x+5, ty, slide_term_switch_text);

}


int SubMenuSettingsGetLevel()
{
	return SubMenuSettinglevel;
}
void SubMenuSettingsetLevel(int levelSubMenuSetting)
{
	if(levelSubMenuSetting>=0)
	{
		SubMenuSettinglevel=levelSubMenuSetting;
	}	
}
void SubMenuSettingsLevelDown()
{
	if(iconGetLevel()==Icon_Settings)
	{
		switch(SubMenuSettingsGetLevel())
		{
		case Theme_Select:
			SubMenuSettingsetLevel(Slide_Menu);
			sub_hig_y=sub_hig_y+sub_hig_step;
			break;
		case Slide_Menu:
			SubMenuSettingsetLevel(Slide_Term_Speed);
			sub_hig_y=sub_hig_y+sub_hig_step;
			break;
		case Slide_Term_Speed:
			SubMenuSettingsetLevel(Slide_Term_Switch);
			sub_hig_y=sub_hig_y+sub_hig_step;
			break;
		default:
			break;
		}
	}
}
void SubMenuSettingsLevelUp()
{
	if(iconGetLevel()==Icon_Settings)
	{
		switch(SubMenuSettingsGetLevel())
		{
		case Theme_Select:
			break;
		case Slide_Menu:
			SubMenuSettingsetLevel(Theme_Select);
			sub_hig_y=sub_hig_y-sub_hig_step;
			break;
		case Slide_Term_Speed:
			SubMenuSettingsetLevel(Slide_Menu);
			sub_hig_y=sub_hig_y-sub_hig_step;
			break;
		case Slide_Term_Switch:
			SubMenuSettingsetLevel(Slide_Term_Speed);
			sub_hig_y=sub_hig_y-sub_hig_step;
			break;
		default:
			break;
		}
	}
}
void SetSettingsValue()
{
	// active/unactive slide Menu
	if(iconGetLevel()==Icon_Settings && SubMenuSettingsGetLevel()==Slide_Menu)
	{
		if(menu_slide==0)
		{
		slide_menu_text=slide_menu_on;
		menu_slide=1;
		main_menu_x=-280;
		}
		else
		{
		slide_menu_text=slide_menu_off;
		menu_slide=0;
		}
	}
	// active/unactive slide Terminal
	if(iconGetLevel()==Icon_Settings && SubMenuSettingsGetLevel()==Slide_Term_Speed)
	{
		if(slide_term_status==0)
		{
		slide_term_text=slide_term_speed;
		slide_term_status=1;
		}
		else
		{
		slide_term_text=slide_term_off;
		slide_term_status=0;
		}
	}
	// adjust/new term switch
	if(iconGetLevel()==Icon_Settings && SubMenuSettingsGetLevel()==Slide_Term_Switch)
	{
		if(slide_term_switch==0)
		{
		slide_term_switch_text=slide_term_adjust;
		slide_term_switch=1;
		}
		else
		{
		slide_term_switch_text=slide_term_new;
		slide_term_switch=0;
		}
	}
}
