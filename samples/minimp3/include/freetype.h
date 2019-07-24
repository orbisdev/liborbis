
#include <ps4link.h>
#include <math.h>       // sinf, cosf
#include <orbis2d.h>
#include <orbisFile.h>
// each orbisFileGetFileContent() call will update filesize!
extern size_t _orbisFile_lastopenFile_size;

/* FreeType font support */
#include <ft2build.h>
#include FT_FREETYPE_H


void FT_init(void);

void FT_update_gradient(const uint32_t *a, const uint32_t *b);
void FT_set_text_size(int pt, int dpi);
int FT_get_text_lenght(uint dst_x, uint dst_y, const char *text);
void FT_print_text(uint dst_x, uint dst_y, const char *text);

void FT_end(void);
