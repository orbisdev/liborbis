/*
    browsing HOST0: files via liborbisFileBrowser
*/

#include "menu.h"
#include "freetype.h"
#include "minimp3_test.h"  // mp3 decoder


char path[256];
OrbisFileBrowserListEntry *currentEntry;

/*void list()
{
    if(browserList)
    {
        OrbisFileBrowserListEntry *entry = currentEntry->head;
        for(int = currentEntry->length; i>0; i--) {
        {
            OrbisFileBrowserListEntry *next = entry->next;
            debugNetPrintf(DEBUG,"%s\n",entry->dir->name);
            entry = next;
    }
   }
}*/

static char selected_entry[256]; // test selection/execution

void select_entry(char *p)
{
    memset(&selected_entry[0], 0, sizeof(selected_entry));
    strcpy(&selected_entry[0], orbisFileBrowserGetListPath());

    int l = orbisFileBrowserGetDirLevel() -1;
    if (l)
        strcat(&selected_entry[0], "/");

    strcat(&selected_entry[0], p);
    //strcpy(&selected_entry[strlen(orbisFileBrowserGetListPath())], p);
}


int stride = 0;  // stride rows, let scroll file selector

extern char refresh;

static void refresh_page(int page)
{
    currentEntry = orbisFileBrowserListGetNthEntry(page * MAX_DRAWN_LINES); // address entry
    select_entry(currentEntry->dir->name);
    debugNetPrintf(INFO,"select_entry(%s)\n", currentEntry->dir->name);
    debugNetPrintf(INFO,"selected_entry(%s)\n", selected_entry);

    debugNetPrintf(INFO,"refresh_page(%d), dirLevel:%d\n", page, orbisFileBrowserGetDirLevel());
    refresh = 1;
}

// return value sets need of orbis2dDumpBuffer() refresh
static int run_selected(v4i *menu_pos)
{
    currentEntry = orbisFileBrowserListGetNthEntry(menu_pos->y);

    if(currentEntry->dir->customtype == FILE_TYPE_FOLDER)
    {
        //debugNetPrintf(INFO,"1. folder, dirLevel:%d\n", orbisFileBrowserGetDirLevel());

        /*if(strcmp(currentEntry->dir->name, "..") == 0
        && orbisFileBrowserGetDirLevel() > 1) { orbisFileBrowserDirLevelDown(); }
        else*/
        if(strcmp(currentEntry->dir->name, "..") != 0
        && strcmp(currentEntry->dir->name, "." ) != 0)
        {
            orbisFileBrowserDirLevelUp(currentEntry->dir->name);
        }

        menu_pos->y = stride = 0;
        refresh_page(0);

        return 1;
    }

    if(strstr(currentEntry->dir->name, ".mp3"))
    {
        debugNetPrintf(INFO,".mp3 -> select_entry(%s)\n", currentEntry->dir->name);
        select_entry(currentEntry->dir->name);

        debugNetPrintf(INFO,"selected_entry(%s)\n", selected_entry);
        minimp3_Loop(selected_entry);
    }
    else
    if(strstr(currentEntry->dir->name, ".shader"))
    {
        debugNetPrintf(INFO,".shader -> select_entry(%s)\n", currentEntry->dir->name);
        select_entry(currentEntry->dir->name);

        debugNetPrintf(INFO,"selected_entry(%s)\n", selected_entry);
        // do something useful
    }

    return 0; // don't need of orbis2dDumpBuffer() refresh
}


/* (v4i) menu position used as: x = coloumn, y = line, z = VIEW, w = PRESSED_BUTTON */

// we pass pointer, we change value on return!
int browserUpdateController(v4i *i)
{
    if(i->z != MAIN) return 0;  // menu closed, do nothing

    int ret = 0,
        s_check = 0;  // different stride_check for UP/DOWN to trigger refresh_page()

    /*debugNetPrintf(DEBUG," before entry down level=%d base=%d rel=%d %d files\n",
               orbisFileBrowserGetDirLevel(),
               orbisFileBrowserGetBasePos(),
               orbisFileBrowserGetRelPos(),
               orbisFileBrowserGetListLength());*/

    switch(i->w) // PRESSED_BUTTON
    {
        case ORBISPAD_UP:      i->y -= 1; s_check = MAX_DRAWN_LINES -1; break;
        case ORBISPAD_DOWN:    i->y += 1; s_check = 0;                  break;
        case ORBISPAD_CROSS:   ret   = run_selected(i);                 break;
        case ORBISPAD_CIRCLE:
                 if(orbisFileBrowserGetDirLevel() > 1)
                 { orbisFileBrowserDirLevelDown(); i->y = 0; ret = 1; } break;
        default:
            break;
    }

    /* bound checking */
    if(i->y < 0) { i->y = orbisFileBrowserGetListLength() -1; ret = 1; }  // less than min loops to max (UP)
    if(i->y > orbisFileBrowserGetListLength() -1) { i->y = 0; ret = 1; }  // more than max loops to min (DOWN)

    stride = i->y %(MAX_DRAWN_LINES);

    /* trigger refresh_page() if needed */
    if(stride == s_check
    || ret)
        { refresh_page(i->y /MAX_DRAWN_LINES); }  // ask for orbis2dDumpBuffer() in 2d ctx
    else
    {
        currentEntry = orbisFileBrowserListGetNthEntry(i->y);
      //currentEntry=orbisFileBrowserListGetNthEntry(orbisFileBrowserGetBasePos()+orbisFileBrowserGetRelPos());
    }

    debugNetPrintf(INFO, "stride: %d, current entry:%d '%s', view:%d, col:%d, line:%d, page:%d\n",
                          stride, i->y, currentEntry->dir->name,
                          i->z, i->x, i->y,
                          i->y /MAX_DRAWN_LINES); // page num

    // update selected file (ready to run)
    //select_file(currentEntry->dir->name);
    // (already selected on CROSS press)

    sceKernelUsleep(10000); // get more time to control pad

    return ret;
}


static void getSizeString(char string[16], uint64_t size) 
{
    double double_size = (double)size;

    int i = 0;
    static char *units[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
    while (double_size >= 1024.0) {
        double_size /= 1024.0;
        i++;
    }
    snprintf(string, 16, "%.*f %s", (i == 0) ? 0 : 2, double_size, units[i]);
}


// to store texts
char dateString[20];
char sizeString[16];
uint32_t f1, f2;

static void DrawEntry(OrbisFileBrowserListEntry *entry, int i)
{
    int tx = 0;
    //debugNetPrintf(DEBUG,"highlighted: %d, '%s'\n", i, entry->dir->name);

    if(i >= MAX_DRAWN_LINES) return;

    // draw entry text
    if(0) {
        /* XbmFont, disabled
        print_text(960+8, 90 + i*20 +1, dateString);
        print_text(960-8 - 16*strlen(sizeString), 90 + i*20 +1, sizeString); */
    } else {
        // FT_ 1.
        FT_print_text(20, 90 + i*20 +1, entry->dir->name);  // left aligned

        /* compose date string */
        sprintf(dateString,"%02d/%02d/%04d %02d:%02d %s",
                    entry->dir->mtime.day,
                    entry->dir->mtime.month,
                    entry->dir->mtime.year,
                    entry->dir->mtime.hour > 12 ? entry->dir->mtime.hour -12 : entry->dir->mtime.hour,
                    entry->dir->mtime.minute,
                    entry->dir->mtime.hour >= 12 ? "PM" : "AM");
        // FT_ 2.
        tx = FT_get_text_lenght(ATTR_WIDTH -20, 90 + i*20 +1, &dateString[0]);
             FT_print_text( ATTR_WIDTH -20 -tx, 90 + i*20 +1, &dateString[0]);  // right aligned

        /* detect entry type */
        if(entry->dir->customtype == FILE_TYPE_FOLDER) {
              sprintf(sizeString,"%s","FOLDER");
        }    else {
              getSizeString(sizeString, entry->dir->size);
        }
        // FT_ 3.
        tx = FT_get_text_lenght(960-8, 90 + i*20 +1, &sizeString[0]);
             FT_print_text( 960-8 -tx, 90 + i*20 +1, &sizeString[0]);  // right aligned
    }
}


void browserDrawSelection(v4i *menu_pos)
{
    uint32_t c1 = 0xFFFF0000, c2 = 0xFF0000FF; // custom color
    // update_gradient(&c1, &c2);
    FT_update_gradient(&c1, &c2);

    int i = stride;

    if(i < 0
    || i >= MAX_DRAWN_LINES) return;

    OrbisFileBrowserListEntry *entry = orbisFileBrowserListGetNthEntry(menu_pos->y);  // selected
    DrawEntry(entry, i);

    FT_update_gradient(&f1, &f2);  // restore previous color
}


void browserDraw(v4i *menu_pos)
{
    int i = 0;

    /* address page, round mod(MAX_DRAWN_LINES) */
    OrbisFileBrowserListEntry *entry = orbisFileBrowserListGetNthEntry(menu_pos->y - stride);
    //orbisFileBrowserListGetNthEntry(orbisFileBrowserGetBasePos());

    /* loop over entries to draw */
    while(entry && i < MAX_DRAWN_LINES)
    {
        f1 = 0x80FFFFFF, f2 = 0x80404040; // white to grey
        FT_update_gradient(&f1, &f2);
        DrawEntry(entry, i);

        entry = entry->next;
        i++;
    }

    /* print upper-left title */
    if(0) {
        /*f1 = 0xFF24FFBD, f2 = 0xFF24FFBD; update_gradient(&f1, &f2);
        print_text(30, 52, orbisFileBrowserGetListPath());*/
    } else {
        FT_set_text_size(16, 100);

        FT_print_text(30, 50, orbisFileBrowserGetListPath());
    }
}
