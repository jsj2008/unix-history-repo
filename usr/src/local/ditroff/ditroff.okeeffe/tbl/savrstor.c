#ifndef lint
static char sccsid[] = "@(#)savrstor.c	1.1 (CWI) 85/10/01";
#endif lint


 /* tf.c: save and restore fill mode around table */

#include "defs.h"
#include "ext.h"

/*
 * remembers various things: fill mode, vs, ps in mac 35(SF)
 */
savefill()
{
	printf(".de %d\n", SF);
	printf(".ps \\n(.s\n");
	printf(".vs \\n(.vu\n");
	printf(".in \\n(.iu\n");
	printf(".if \\n(.u .fi\n");
	printf(".if \\n(.j .ad\n");
	printf(".if \\n(.j=0 .na\n");
	printf("..\n");
	printf(".nf\n");
	/*
	 * set obx offset if useful
	 * 
	printf(".nr #~ 0\n");
	 *
	 * JNA, the offset is used to set the vertical line to the left
	 *
	 * I'll introduce register #| to offset the horizontal lines
	 * a bit up
	 */
	switch(device){
	case HARRIS:
		printf(".nr #~ 0.24n\n");
		printf(".nr #| 0.1n\n");
		printf(".nr Tw 22.5c\n");
		break;
	case CAT:
	case DEVVER:
		printf(".nr #~ 0\n");
		printf(".nr #| 0\n");
		printf(".nr Tw 7.65i\n");
		break;
	}
	
	/*
	 * This is the offset for the T450 boxes see drawvert()
	 */
	printf(".if \\n(.T .if n .nr #~ 0.6n\n");
}

/*
 * Call the macro SF (35) to restore collected data
 */
rstofill()
{
	printf(".%d\n", SF);
}

/*
 * Clean up at end of file?
 */
endoff()
{
	int i;
	/*
	 * make some registers to be 0 ...
	 */
	for(i = 0; i < MAXHEAD; i++)
		if(linestop[i])
			printf(".nr #%c 0\n", 'a' + i);
	/*
	 * and remove used macros, strings (and diversions?)
	 */
	for(i = 0; i < texct; i++)
		printf(".rm %c+\n", texstr[i]);
	printf("%s\n", last);
}

/*
 * Let's check wether we are in a diversion
 */
ifdivert()
{
	/*
	 * #d hold current vertical place if in a diversion
	 */
	printf(".ds #d .d\n");
	/*
	 * if diversion doesn't exist, rememenber current vertical
	 * place (nl) in #d
	 */
	printf(".if \\(ts\\n(.z\\(ts\\(ts .ds #d nl\n");
}

/*
 * save current input line number (of troff), since we are going to add a lot of
 * lines
 *
 * Note that register b. is never set to be auto incremeted, so this
 * garanteed to fail!!! (jna)
 *
 * (should be replaced with a request for new troff)
 */
saveline()
{
	printf(".if \\n+(b.=1 .nr d. \\n(.c-\\n(c.-1\n");
	linstart = iline;
}

/*
 * rstore line count of troff
 */
restline()
{
	printf(".if \\n-(b.=0 .nr c. \\n(.c-\\n(d.-%d\n", iline - linstart);
	linstart = 0;
	/*
	 * support for .lf request of troff (jna)
	 */
	printf(".lf %d\n", iline);
}

/*
 * Turn the field mechanism off
 */
cleanfc()
{
	printf(".fc\n");
}
