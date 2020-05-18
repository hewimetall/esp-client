#include <ctype.h>
#include "head.h"
static const char *TAG = "json";

size_t json_pars(char *Str, size_t len,int *p) {
	char str_f[8]="";
	int i = 0, coint;
	long int Num = 0;
	ESP_LOGI(TAG,"%s",Str);
	for (int k = 0; k < (len - 1); k++, Str++) {
		coint = 1;
		if (*Str == '-') {
			coint = -1;
			Str++;
		}
		if (isdigit((int) *Str) || *Str == '.') {
			for (i = 0; isdigit((int) *Str) || *Str == '.'; i++) {
				str_f[i] = *Str;
				Str++;
			}
			str_f[i] = '\0';
			Num = coint * atol(str_f);
			*p = Num;
			p++;
		};
	}
	return (size_t) 2;

}
